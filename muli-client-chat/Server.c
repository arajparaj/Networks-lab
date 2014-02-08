#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <netdb.h>

struct client
{
	char password[256];

	int socket;
	
	/*
	0 = not registerd
	1 = registered not loged in
	2 = registered and loged in
	*/

	int status;

}user[100];

int sockfd,newsockfd,portno,clilen; 
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
pthread_t threads[100];




/*thread starts when a client connected*/
void * login(int user_index){

	int sockfd,from,to,n;
	char buffer[256],msg[256];
	char temp[256];

	from = user_index;
	user[from].status=2;
	int i;
	bzero(buffer,256);
	bzero(msg,256);



	n=write(user[from].socket,"Online Users",13);

	for (i=0;i<10;i++)
	{
		

		if(user[i].status==2)
		{
			sprintf(msg,"user-%d  ",i);
			strcat(buffer,msg);
		}
			bzero(msg,256);
	}

	n=write(user[from].socket,buffer,256);

	while(1){

		bzero(msg,256);
		bzero(buffer,256);
		

		n = read(user[from].socket, buffer, 256);

		if(buffer[0]=='~'){
			user[user_index].status=1;
			printf("User-%d logged out\n",user_index);
			close(user[user_index].socket);
			pthread_exit(NULL);
		}

		if(buffer[0]=='?'){

			n=write(user[from].socket,"Online Users",13);
		
			bzero(temp,256);

			for (i=0;i<10;i++)
			{
				if(user[i].status==2)
				{
				sprintf(msg,"user-%d  ",i);
				strcat(temp,msg);
				}
				bzero(msg,256);
			}
		
			n=write(user[from].socket,temp,256);
			continue;
		}

		to = atoi(&buffer[8]);

		
		sprintf(msg,"FROM user-%d %s",from,&buffer[9]);

		n = write(user[to].socket,msg,256);
		


		if(n<0){
			printf("user-%d doesnt exit\n",to);
		}
		
		bzero(msg,256);

		bzero(buffer,256);

	}

}



int main(int argc, char const *argv[])
{


	int i,n;
	for (i = 0; i < 100; i++)
	{
		user[i].status=0;
	}

	int user_index=0;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	
	listen(sockfd,5);


	while(1){

		bzero(buffer,256);
		clilen = sizeof(cli_addr);

		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

		n = read(newsockfd,buffer,255);
		user_index = atoi(&buffer[5]);
		n = write(newsockfd,"LOL",4);

		bzero(buffer,256);

		n = read(newsockfd,buffer,255);
		

		/*not registerd user*/
		if(user[user_index].status==0){

			strcpy(user[user_index].password,buffer);
			user[user_index].status=1;
			fprintf(stdout, "user-%d registerd\n",user_index);
		}


		/*logging in the user*/

			user[user_index].socket=newsockfd;
		



		if(strcmp(user[user_index].password,buffer)!=0){

			n = write(newsockfd,"N",2);
			continue;		
		}else
		{
			if (user[user_index].status==1)
			{
				n = write(newsockfd,"Y",2);
			}else
			{
				n = write(newsockfd,"A",2);
			}
			
			
		}

		/*logged in*/
			

			fprintf(stdout, "user-%d logged in\n",user_index);
			

			n = pthread_create(&threads[1], NULL, login ,user_index);
		
		}

	close(newsockfd);
	close(sockfd);

	return 0;


}