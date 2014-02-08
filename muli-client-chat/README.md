# A simple multiple client chat sever using sockets

Author : Araj P Raju <arajparaj@gmail.com>
License : GPL V2

INSTRUCIONS

1. compile the server code `gcc Server.c -o server -pthread`
2. compile the client code `gcc Client.c -o client -pthread`
3. Run server `./server <port number>`. Eg: `./server 8089`
4. Run the client in a different terminal. `./client <server> <port>`. Eg: `./client localhost 8089`
5. Run any number of clients in different terminals
6. Give username and Password for logging in. If its the first time the password would be saved for next login
7. Username must be in the format of user-0 ,user-1 , etc
8. A message can be sent from peer to peer in the format 'TO  user-0: Message'


Used multithreaded approach that makes cients simultaniously send and recieve messages




