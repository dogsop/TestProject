/*
 * UDPServerThread.cpp
 *
 *  Created on: Aug 9, 2012
 *      Author: ken
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>  /* memset(), memcpy() */
#include <sys/utsname.h>   /* uname() */
#include <sys/types.h>
#include <sys/socket.h>   /* socket(), bind(),
                             listen(), accept() */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>  /* fork(), write(), close() */

#include "Globals.h"
#include "UDPServerThread.h"

#define BUFLEN 512



void *udpServerThread( void *ptr )
{
	int serverSocket = 0;
	int port = 0;
	int status = 0;

	struct sockaddr_in serverName = { 0 };

	port = udpSocketPort;

	for (;;) {
		serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (-1 == serverSocket) {
			perror("socket()");
			return NULL;
		}

		(void) memset(&serverName, 0, sizeof(serverName));
		//(void) memcpy(&serverName.sin_addr, hostPtr->h_addr, hostPtr->h_length);

		/*
		 * to allow server be contactable on any of
		 * its IP addresses, uncomment the following
		 * line of code:
		 * serverName.sin_addr.s_addr=htonl(INADDR_ANY);
		 */
		serverName.sin_addr.s_addr=htonl(INADDR_ANY);

		serverName.sin_family = AF_INET;
		/* network-order */
		serverName.sin_port = htons(port);

		status = bind(serverSocket, (struct sockaddr *) &serverName,
				sizeof(serverName));
		if (-1 == status) {
			perror("bind()");
			return NULL;
		}

		struct sockaddr_in clientName = { 0 };
		unsigned int clientLength = sizeof(clientName);
		char buf[BUFLEN];

		(void) memset(&clientName, 0, sizeof(clientName));

		while(1)
		{
			int bytesReceived;

			if( (bytesReceived = recvfrom(serverSocket, buf, BUFLEN, 0, (struct sockaddr*)&clientName, &clientLength)) == -1)
			{
				 //err("recvfrom()");
				 break;
			}
			printf("Received packet from %s:%d\nData: %s\n\n",
					 inet_ntoa(clientName.sin_addr), ntohs(clientName.sin_port), buf);

			//sprintf(buf, "ServerSocket=%d|CamSocket=%d", tcpSocketPort, camSocketPort);
			sprintf(buf, "ServerSocket=%d:CamSocket=%d", tcpSocketPort, 8888);
			printf("Sending %s\n", buf);

			sendto(serverSocket,buf,strlen(buf),0,(struct sockaddr *)&clientName,clientLength);
		}

		close(serverSocket);

		printf("udp thread returned\n");
	}

	return NULL;
}
