/*
 * SocketHost.cpp
 *
 *  Created on: Jul 16, 2012
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

#include "ServerThread.h"

/*
 * prototypes
 */


/*
 * constants
 */
const char MESSAGE[] = "Hello, World!\n";
const int BACK_LOG = 1;



void *socketHostThread( void *ptr )
{
	int serverSocket = 0;
	int on = 0;
	int port = 0;
	int status = 0;

	struct sockaddr_in serverName = { 0 };

	pthread_t server_thread;

	port = 12345;
	for (;;) {
		serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (-1 == serverSocket) {
			perror("socket()");
			return NULL;
		}

		/*
		 * turn off bind address checking, and allow
		 * port numbers to be reused - otherwise
		 * the TIME_WAIT phenomenon will prevent
		 * binding to these address.port combinations
		 * for (2 * MSL) seconds.
		 */

		on = 1;

		status = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,
				(const char *) &on, sizeof(on));

		if (-1 == status) {
			perror("setsockopt(...,SO_REUSEADDR,...)");
		}

		/*
		 * when connection is closed, there is a need
		 * to linger to ensure all data is
		 * transmitted, so turn this on also
		 */
		{
			struct linger linger = { 0 };

			linger.l_onoff = 1;
			linger.l_linger = 30;
			status = setsockopt(serverSocket, SOL_SOCKET, SO_LINGER,
					(const char *) &linger, sizeof(linger));

			if (-1 == status) {
				perror("setsockopt(...,SO_LINGER,...)");
			}
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

		status = listen(serverSocket, BACK_LOG);
		if (-1 == status) {
			perror("listen()");
			return NULL;
		}

		struct sockaddr_in clientName = { 0 };
		int slaveSocket;
		unsigned int clientLength = sizeof(clientName);

		(void) memset(&clientName, 0, sizeof(clientName));

		printf("calling accept\n");

		slaveSocket = accept(serverSocket, (struct sockaddr *) &clientName, &clientLength);
		if (-1 == slaveSocket) {
			perror("accept()");
			return NULL;
		}

		printf("launching new server thread\n");

		status = pthread_create(&server_thread, NULL, serverThread, (void *)slaveSocket);

		close(serverSocket);

        pthread_join(server_thread, NULL);

		printf("server thread returned\n");
	}

	return NULL;
}
