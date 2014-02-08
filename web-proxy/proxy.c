#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



int main( int argc, char *argv[] )
{
    int sockfd, newsockfd, portno, clilen;
    
    
    struct sockaddr_in serv_addr, cli_addr;
    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }

    /* Now start listening for the clients, here process will
    * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);


acceptor:

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, 
                                &clilen);
    if (newsockfd < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
    
    /* If connection is established then start communicating */
    int pid;
    pid = fork();
    if (pid<0)
    {
        printf("error on forking\n");
        exit(1);
    }
    
    if(pid==0){
        close(sockfd);
        startProxy(newsockfd);
        exit(0);
    }
    else{
        close(newsockfd);
    }
    goto acceptor;


    
    return 0; 
}

void startProxy(int newsockfd){

    int n;
    char methode[10];
    char path[100];
    char version[10];
    char host_ad[100];
    char temp[100];
    int sockfd1;
    char buffer[5000];
    
    bzero(buffer,5000);
    n = read( newsockfd,buffer,4999);
    
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    int http_portno= 80;

    sscanf(buffer,"%s %s %s",methode,path,version);


    if ((strcmp(methode,"GET")==0)&&((strcmp(version,"HTTP/1.1"))||(strcmp(version,"HTTP/1.0"))))
    {   

    char null[100];

    sscanf(path,"http://%s",temp);
    


    int i,j;
    for (i = 0;(i < 100)&&(temp[i]!='/')&&(temp[i]!=':'); ++i)
    {
        host_ad[i]=temp[i];
    }
    if(temp[i]==':')
    {
        host_ad[i]='\0';
        http_portno = atoi(host_ad[i+1]);
    }else{

        host_ad[i] ='\0';   
    }
        bzero(path,100);

    while((temp[i]!='/')&&(temp[i]!='\0')){
        i++;
    }

    for (j=0; (i < 100)&&(temp[i]!='\0'); ++i,j++)
    {
          path[j]=temp[i];
    }

    struct sockaddr_in host_addr;
    struct hostent *server;

    sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(host_ad);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *)&host_addr, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
           (char *)&host_addr.sin_addr.s_addr,
                server->h_length);
    host_addr.sin_port = htons(http_portno);

    if (connect(sockfd1,&host_addr,sizeof(host_addr)) < 0) 
    {
         perror("ERROR connecting");
         exit(1);
    }
    bzero(buffer,5000);

    sprintf(buffer,"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path,host_ad);
    //sprintf(buffer,"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",path,host_ad);
    

    printf("%s\n", buffer);


    n = write(sockfd1,buffer,strlen(buffer));

    if (n < 0) 
    {
         perror("ERROR writing to socket");
         exit(1);
    }

    bzero(buffer,5000);
    
  
    do{
    n = read(sockfd1,buffer,4999);
    if (n<0)
    {
        exit(0);
    }
    n = write(newsockfd,buffer,strlen(buffer));

    bzero(buffer,5000);
    
    }while(n>0);
    }
    else
    {
        bzero(buffer,5000);
        sprintf(buffer,"400 BAD REQUEST");
        n = write(newsockfd,buffer,strlen(buffer));        
        bzero(buffer,5000);
    }
}