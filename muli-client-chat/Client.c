#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

	int sockfd,portno,n;

	struct sockaddr_in serv_addr;
	struct hostent *server;
	pthread_t threads[2];
	char buffer[256];

	void * Read(){
		while(1)
		{
			bzero(buffer,255);
			n = read(sockfd,buffer,255);
			if( n >= 0)
				printf(ANSI_COLOR_RED     "%s"     ANSI_COLOR_RESET "\n",buffer);
		}

		pthread_exit(NULL);
	}

void * Write(){
		
		while(1){
			bzero(buffer,255);
			fgets(buffer,255,stdin);

			if (buffer[0]=='~')
			{
			
				/* code */
				n = write(sockfd,buffer,strlen(buffer));
				close(sockfd);
				exit(0);
			}

			n = write(sockfd,buffer,strlen(buffer));
		}

	}

int main(int argc, char const *argv[])
{
	/* code */

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	server = gethostbyname(argv[1]);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
		error("ERROR connecting");
	}

	
	printf("Please enter the username: ");
	bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));

    bzero(buffer,256);

    n = read(sockfd,buffer,255);


    printf("Please enter the password: ");    
    bzero(buffer,256);
    gets(buffer);
    n = write(sockfd,buffer,strlen(buffer));

    bzero(buffer,256);

    n = read(sockfd,buffer,255);
    
    if(buffer[0]=='N'){
    	printf("wrong password\n");
    	exit(0);
    }
    if(buffer[0]=='Y'){
    	printf("Logged in successfully\n");
    
    }
    if (buffer[0]=='A')
    {
    	printf("Already logged in\n");
    	exit(0);
    }
    


	bzero(buffer,256);
    
    printf("Welcome\n ~   :  Exit the client \n ?   : Show the online clients\n");
	n = pthread_create(&threads[1], NULL,Write,NULL);

	Read();

	return 0;
}