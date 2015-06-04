/*
 * Servermode.cpp
 *
 *  Created on: 2015. 6. 4.
 *      Author: suck
 */

#include "Servermode.h"

 int  clnt_number = 0;
 int clnt_tx_socks[10];
 int clnt_rx_socks[10];
 int clnt_state[10];
 char clnt_ID[10][40];
 pthread_mutex_t mutx;

void runServer()
{
	//Local Variables
	int serv_sock;
	int clnt_sock;
	int clnt_sock2;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	unsigned int clnt_addr_size;
	pthread_t thread;

	//Mutex Init
	if (pthread_mutex_init(&mutx, NULL))
	error_handling("mutex error");

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	int option = 1;
	setsockopt( serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) );

	//Server Address Setting
	memset(&serv_addr, 0x00, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5050);

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
	error_handling("listen() error");

	//Connects Client
	char message[BUFSIZE];
	char ID[40];
	FILE * readfp;

	while (1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr,
				&clnt_addr_size);

		readfp = fdopen(clnt_sock, "r");
		fgets(message, BUFSIZE - 1, readfp);

		//Client TX Connected
		if(strcmp(message,"tx\n")==0)
		{
			cout<<"tx detected"<<endl;
			int count;
			fgets(message, BUFSIZE - 1, readfp);
			cout<<"message : "<<message<<endl;
			for(count=0;count<clnt_number;count++)
			{
				//If there exists same ID
				if(strcmp(clnt_ID[count],message)==0)
				{
					if(clnt_state[count]==RXONLY)
					{
						clnt_tx_socks[count]=clnt_sock;
						clnt_state[count] = TXRXBOTH;
						pthread_create(&thread, NULL, clnt_tx_connection,
								(void *) clnt_sock);
						pthread_create(&thread, NULL, clnt_rx_connection,
								(void *) clnt_sock);
						cout << "New Connection : IP: " << inet_ntoa(clnt_addr.sin_addr)<< endl;
						break;
					}
					else if(clnt_state[count]==TXONLY || clnt_state[count]==TXRXBOTH)
					{
						close(clnt_sock);
						fclose(readfp);
						break;
					}
				}

				if (count == clnt_number)
				{
					clnt_state[clnt_number] = TXONLY;
					strcpy(clnt_ID[clnt_number], message);
					clnt_tx_socks[clnt_number++] = clnt_sock;
				}
			}
		}

		//Client RX connected
		else if(strcmp(message,"rx\n")==0)
		{
			cout<<"rx detected"<<endl;
			int count;
			fgets(message, BUFSIZE - 1, readfp);
			cout<<"message : "<<message<<endl;
			for(count=0;count<clnt_number;count++)
			{
				//If there exists same ID
				if(strcmp(clnt_ID[count],message)==0)
				{
					if(clnt_state[count]==TXONLY)
					{
						clnt_tx_socks[count]=clnt_sock;
						clnt_state[count] = TXRXBOTH;
						pthread_create(&thread, NULL, clnt_tx_connection,
								(void *) clnt_sock);
						pthread_create(&thread, NULL, clnt_rx_connection,
								(void *) clnt_sock);
						cout << "New Connection : IP: " << inet_ntoa(clnt_addr.sin_addr)<< endl;
						break;
					}
					else if(clnt_state[count]==RXONLY || clnt_state[count]==TXRXBOTH)
					{
						close(clnt_sock);
						fclose(readfp);
						break;
					}
				}

				if (count == clnt_number)
				{
					clnt_state[clnt_number] = RXONLY;
					strcpy(clnt_ID[clnt_number], message);
					clnt_tx_socks[clnt_number++] = clnt_sock;
				}
			}
		}

		fclose(readfp);
		pthread_mutex_lock(&mutx);
		pthread_mutex_unlock(&mutx);

	}
}

void * clnt_tx_connection(void * arg)
{
	int clnt_sock = *((int*) arg);
	int str_len = 0;
	char message[BUFSIZE];
	int i;
	FILE * readfp;
	readfp = fdopen(clnt_sock, "r");

	while (!feof(readfp))
	{
		fgets(message, BUFSIZE - 1, readfp);
		send_message(message, strlen(message) + 1);
	}
	//send_message(message, str_len);



	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_number; i++)
		if (clnt_sock == clnt_tx_socks[i])
		{
			for (; i < clnt_number - 1; i++)
			{
				clnt_tx_socks[i] = clnt_tx_socks[i + 1];
				clnt_rx_socks[i] = clnt_rx_socks[i + 1];
			}
			break;
		}
	clnt_number--;
	pthread_mutex_unlock(&mutx);

	close(clnt_sock);
	return 0;
}

void * clnt_rx_connection(void * arg)
{
	int clnt_sock = *((int*) arg);
	int str_len = 0;
	char message[BUFSIZE];
	int i;
	FILE * readfp;

	readfp = fdopen(clnt_sock, "r");

	while (!feof(readfp))
	{
		fgets(message, BUFSIZE - 1, readfp);
		send_message(message, strlen(message) + 1);
	}
	//send_message(message, str_len);

	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_number; i++)
		if (clnt_sock == clnt_rx_socks[i])
		{
			for (; i < clnt_number - 1; i++)
			{
				clnt_tx_socks[i] = clnt_tx_socks[i + 1];
				clnt_rx_socks[i] = clnt_rx_socks[i + 1];
			}
			break;
		}
	clnt_number--;
	pthread_mutex_unlock(&mutx);

	close(clnt_sock);
	return 0;
}

void send_message(char * message, int len)
{
	int i;
	FILE * writefp;
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_number; i++)
	{
		//writefp = fdopen(clnt_socks[i], "w");
		fputs(message, writefp);
		fflush(writefp);
		//write(clnt_socks[i], message, len);
	}
	pthread_mutex_unlock(&mutx);
}

void error_handling(const char* message)
{
	cout << message << endl;
	exit(1);

}

