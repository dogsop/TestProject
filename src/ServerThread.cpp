/*
 * ServerThread.cpp
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
#include <errno.h>

#define BUF_SIZE	128

void *serverThread( void *ptr )
{
	printf("Server thread started\n");

	int socket = (int) ptr;

	/* other local variables ------------------------------------------------ */
	char in_buf[BUF_SIZE];           // Input buffer for GET request
	char out_buf[BUF_SIZE];          // Output buffer for HTML response
	unsigned int buf_len;                    // Buffer length for file reads
	int retcode;                    // Return code

	while(1)
	{
//		printf("Sending\n");
//
//		retcode = send(socket, out_buf, 1, MSG_DONTWAIT);
//
//		/* receive the first HTTP request (HTTP GET) ------- */
//		//retcode = recv(socket, in_buf, BUF_SIZE, 0);
//
//		/* if receive error --- */
//		if (retcode < 0) {
//			printf("send error detected ...\n");
//			break;
//		}
//
//		if (retcode < 0) {
//			printf("send error detected ...\n");
//			break;
//		}

		retcode = recv(socket, in_buf, 1, MSG_DONTWAIT | MSG_PEEK);
		/* if receive error --- */
		if (retcode < 0) {
			if(errno == EAGAIN) {
				printf("keep listening\n");
			} else {
				printf("recv error detected ...\n");
				break;
			}
		} else if(retcode == 0) {
			printf("socket closed\n");
			break;
		} else {
			printf("retcode = %d\n", retcode);
					//reading received bytes\n");
			for(int Counter = 0; Counter < retcode; Counter++) {
				recv(socket, in_buf, 1, 0);
			}
		}


		sleep(2);
	}
	close(socket);

	pthread_exit(NULL);
	return NULL;
}
