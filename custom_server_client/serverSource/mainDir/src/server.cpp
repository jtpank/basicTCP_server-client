#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/wait.h>

using namespace std;

const string _IPADDR = "127.0.0.1";
const string _PORT = "1234";
const int _BACKLOG = 5;
const int _MAX_BUFF_SIZE = 100; //in bytes



//helper function handles IPV4 and IPV6
void *inAddr(struct sockaddr *sckAdd);

int main(int argc, char** argv)
{
	//structure of server
	//getaddrinfor();
	//socket();
	//bind();
	//listen();
	//accept();
	//then send(), recv();

	int socketFD, status;
	//declare addrinfo struct
	struct addrinfo addData, *res;
	//for accepted connections:
	struct sockaddr_storage incAddr;
	socklen_t addr_size;
	int incSockFD;


	//set addrData to 0
	memset(&addData, 0, sizeof(addData));
	//set up address info struct
	addData.ai_family = AF_UNSPEC; //IPV4 or IPV6
	addData.ai_socktype = SOCK_STREAM; //TCP type
	addData.ai_flags = AI_PASSIVE; //my IP
	//get ready to connect
	//to convert const string to const char* use: str.c_str();

	//use _IPADDR.c_str() for const str _IPADDR
	status = getaddrinfo(NULL, _PORT.c_str(), &addData, &res);
	//check status and exit if non zero
	if(status != 0)
	{
		fprintf(stderr,"Error setting up ip and port");
		return 1;
	}
	//now setup socket file descriptor
	//loop through and bind to first
	for (struct addrinfo* p = res; p != NULL; p = p->ai_next)
	{
		socketFD = socket(res->ai_family, res->ai_socktype, 0);
		if (socketFD < 0)
		{
			fprintf(stderr, "Could not create socket");
			return 2;
		}
		//now  bind to the port used in getaddrinfo
		if (bind(socketFD, res->ai_addr, res->ai_addrlen) < 0)
		{
			fprintf(stderr, "Could not bind to port");
			return 3;
		}
		break;
	}
	//as a server, time to listen
	if(listen(socketFD, _BACKLOG) == -1)
	{
		perror("listen");
		exit(1);
	}


	printf("Server waiting for connections...\n");


	//now accept incoming connection from backlog
	//socketFD continues to listen to incoming connections
	//incSockFD is one of the queued up connections from backlog

	//main loop
	int count = 0;
	//3 connections before quitting server
	while(count < 3)
	{
		addr_size = sizeof incAddr;
		incSockFD = accept(socketFD, (struct sockaddr *) &incAddr, &addr_size);
		if(incSockFD == -1)
		{
			perror("accept");
			continue;
		}
		//display ip connection
		char s[INET6_ADDRSTRLEN];
		inet_ntop(incAddr.ss_family, inAddr((struct sockaddr *) &incAddr), s, sizeof s);
		printf("Server received connection from %s\n", s);


		if (!fork()) 
		{ // this is the child process
            		close(socketFD); // child doesn't need the listener
            		string sS = "Thanks for connecting to SERVER!\n";
				if (send(incSockFD, sS.c_str(), sizeof sS, 0) == -1)
                			perror("send");
            		close(incSockFD);
            		exit(0);
        	}
        	close(incSockFD);  // parent doesn't need this
		count++;
	}

	//free the linked list
	freeaddrinfo(res);


	return 0;
}

void *inAddr(struct sockaddr *sckAdd)
{
	if(sckAdd->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sckAdd)->sin_addr);
    }
    return &(((struct sockaddr_in6 *)sckAdd)->sin6_addr);
}
