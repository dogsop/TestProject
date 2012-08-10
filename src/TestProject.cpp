//============================================================================
// Name        : TestProject.cpp
// Author      : KEB
// Version     :
// Copyright   : Copyright 2010, KBrown & Associates
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "SocketHost.h"
#include "UDPServerThread.h"

int udpSocketPort = 12233;
int tcpSocketPort = 12345;
int camSocketPort = 5000;

void *print_message_function( void *ptr );

int main() {
	pthread_t thread1, thread2;
	const char *message1 = "Thread 1";
	const char *message2 = "Thread 2";
	int iret1, iret2;

	/* Create independent threads each of which will execute function */

	iret2 = pthread_create(&thread2, NULL, udpServerThread, NULL);

	iret1 = pthread_create(&thread1, NULL, socketHostThread, NULL );

	/* Wait till threads are complete before main continues. Unless we  */
	/* wait we run the risk of executing an exit which will terminate   */
	/* the process and all threads before the threads have completed.   */

	pthread_join(thread1, NULL);
	printf("Thread 1 returns: %d\n", iret1);

	pthread_join(thread2, NULL);
	printf("Thread 3 returns: %d\n", iret2);

	exit(0);
}
