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




const int _MAX_BUFF_SIZE = 100; //in bytes

//helper function handles IPV4 and IPV6
void *inAddr(struct sockaddr *sckAdd);


int main(int argc, char** argv)
{
	//structure of client
	//getaddrinfor();
	//socket();
	//connect();
	//then send(), recv();
	string sHost, sPort;
	cin>>sHost;
	cin>>sPort;

	int socketFD, status;
	char buffer[_MAX_BUFF_SIZE];
	//declare addrinfo struct
	struct addrinfo aD, *res;

	//set aD to 0
	memset(&aD, 0, sizeof(aD));
	aD.ai_family = AF_UNSPEC; aD.ai_socktype = SOCK_STREAM; //handle IPV4 and IPV6 and tcp type
	status = getaddrinfo(sHost.c_str(), sPort.c_str(), &aD, &res);
	if(status!=0)
	{
		fprintf(stderr,"Error cannot fill out struct\n");
		return 1;
	}
	for (struct addrinfo* p = res; p != NULL; p = p->ai_next)
    {
        socketFD = socket(res->ai_family, res->ai_socktype, 0);
        if (socketFD < 0)
        {
            fprintf(stderr, "Could not create socket");
            return 2;
        }
		//now connect to first socket
		int connSockFD = connect(socketFD, p->ai_addr, p->ai_addrlen);
		if(connSockFD == -1)
		{
			close(socketFD);
			fprintf(stderr,"Cannot connect to server\n");
			continue;
		}
		//display ip connection
		char s[INET6_ADDRSTRLEN];
		inet_ntop(p->ai_family, inAddr((struct sockaddr *)p->ai_addr), s, sizeof(s));
		printf("Connecting to server: %s\n",s);
        break;
    }
	freeaddrinfo(res);
	int nB = recv(socketFD, buffer, _MAX_BUFF_SIZE-1, 0);
	if(nB==-1)
	{
		perror("recv");
		exit(1);
	}
	buffer[nB]='\0';
	printf("Received: [ %s ]\n", buffer);
	close(socketFD);
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
