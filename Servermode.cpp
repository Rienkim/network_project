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
 char clnt_ID[10][40] = {0};
 int clnt_command[10] = {0};
 int clnt_result[10] = {0};
 pthread_mutex_t mutx;
 bool isAllClientNotified = false;
 vector<string> eventdata;

void runServer()
{
	//Local Variables
	int serv_sock;
	int clnt_sock;
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

	while (1)
	{
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr,
				&clnt_addr_size);

		read(clnt_sock, message, BUFSIZE);

		//Client TX Connected
		if(strcmp(message,"tx")==0)
		{
			cout<<"tx detected"<<endl;
			write(clnt_sock, "ack", 4);
			int count;
			read(clnt_sock, message, BUFSIZE);
			cout<<"ID : "<<message<<endl;
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
								(void *) &clnt_sock);
						pthread_create(&thread, NULL, clnt_rx_connection,
								(void *) &clnt_sock);
						cout << "New Connection : IP: " << inet_ntoa(clnt_addr.sin_addr)<< endl;
						write(clnt_sock, "ack", 4);
						break;
					}
					else if(clnt_state[count]==TXONLY || clnt_state[count]==TXRXBOTH)
					{
						close(clnt_sock);
						break;
					}
				}
			}

			if (count == clnt_number)
			{
				clnt_state[clnt_number] = TXONLY;
				strcpy(clnt_ID[clnt_number], message);
				clnt_tx_socks[clnt_number++] = clnt_sock;
				write(clnt_sock, "ack", 4);
			}

		}

		//Client RX connected
		else if(strcmp(message,"rx")==0)
		{
			cout<<"rx detected"<<endl;
			write(clnt_sock, "ack", 4);
			int count;
			read(clnt_sock, message, BUFSIZE);
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
								(void *) &clnt_sock);
						pthread_create(&thread, NULL, clnt_rx_connection,
								(void *) &clnt_sock);
						cout << "New Connection : IP: " << inet_ntoa(clnt_addr.sin_addr)<< endl;
						write(clnt_sock, "ack", 4);
						break;
					}
					else if(clnt_state[count]==RXONLY || clnt_state[count]==TXRXBOTH)
					{
						close(clnt_sock);
						break;
					}
				}
			}

			if (count == clnt_number)
			{
				clnt_state[clnt_number] = RXONLY;
				strcpy(clnt_ID[clnt_number], message);
				clnt_tx_socks[clnt_number++] = clnt_sock;
				write(clnt_sock, "ack", 4);
			}
		}

		pthread_mutex_lock(&mutx);
		pthread_mutex_unlock(&mutx);

	}
}

void * clnt_tx_connection(void * arg)
{
	int clnt_sock = *((int*) arg);
	char message[BUFSIZE];
	int i;
	int error = 0;
	socklen_t len = sizeof (error);
	while (getsockopt (clnt_sock, SOL_SOCKET, SO_ERROR, &error, &len )!=0)
	{
		read(clnt_sock, message, sizeof(message));
		if(strcmp(message,"sync")==0)
		{
			write(clnt_sock, "ack", 4);
			//Read Sync Data from client
			vector<string> recvdata;
			while(getsockopt (clnt_sock, SOL_SOCKET, SO_ERROR, &error, &len )!=0)
			{
				read(clnt_sock, message, sizeof(message));
				if(strcmp(message, "end")==0)
				{
					//Check all of client's time is okay
					//TODO: Check all of client's time is okay
					if(recvdata.size()==4)
					{
						//--Copy ID data(id1,id2,id3...) to tmprcv
						char tmprcv[40], *ptr = NULL;
						strcpy(tmprcv, recvdata[3].c_str());

						//--Copy event data to eventdata vector
						eventdata = recvdata;
						eventdata.pop_back();

						//--Separate id by token
						ptr = strtok(tmprcv, "(),");
						while(ptr!=NULL)
						{
							for( i = 0;i<clnt_number;i++)
							{
								if(strcmp(clnt_ID[i], ptr)==0)
								{
									//Set Client to check their time is okay
									clnt_command[i] = 1;
									break;
								}
							}
							ptr = strtok(NULL, "(),");
						}

						//--Wait until query to every client is done
						for(i=0;i<clnt_number;i++)
						{
							if(isAllClientNotified==false)
							{
								i = -1;
								continue;
							}
							if(clnt_command[i]==1)
								i--;
						}

						//--Check all the client is okay
						for(i=0;i<clnt_number;i++)
						{
							if(clnt_result[i]==-1)
							{
								write(clnt_sock, "no", 3);
								break;
							}
							if(i==(clnt_number-1))
							{
								write(clnt_sock, "yes", 4);
								break;
							}
						}

						//--Set result data to zero
						for(i=0;i<clnt_number;i++)
						{
							clnt_result[i] = 0;
						}
						break;
					}
					else
					{
						//if(recvdata.size != 4)
						cout<<"Recv Data length error!"<<endl;
					}
					break;
				}
				//if message!="end"
				recvdata.push_back(string(message));
			}
		}
	}
	//send_message(message, str_len);


	//End of Communication : connection lost
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_number; i++)
		if (clnt_sock == clnt_tx_socks[i])
		{
			close(clnt_rx_socks[i]);
			for (; i < clnt_number - 1; i++)
			{
				clnt_tx_socks[i] = clnt_tx_socks[i + 1];
				clnt_rx_socks[i] = clnt_rx_socks[i + 1];
				strcpy(clnt_ID[i], clnt_ID[i+1]);
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
	char message[BUFSIZE];
	int i, index = 0;
	int error = 0;
	socklen_t len = sizeof (error);

	//Find index of this socket
	for(i=0;i<clnt_number;i++)
	{
		if(clnt_rx_socks[i]==clnt_sock)
			index = i;
	}

	while (getsockopt (clnt_sock, SOL_SOCKET, SO_ERROR, &error, &len )!=0)
	{
		if(clnt_command[index]==1)
		{
			write(clnt_sock, "ok", 3);
			read(clnt_sock, message, sizeof(message));
			if(strcmp(message,"ack")!=0)
			{
				cout<<"ok ack error"<<endl;
				break;
			}
			//TODO : implement serverside of client rx

			//Event vector Size Check
			if(eventdata.size()!=4)
			{
				cout<<"Event size error"<<endl;
				break;
			}

			//Send event data to the client
			bool errorflag = false;
			for(i=0;i<4;i++)
			{
				write(clnt_sock, eventdata[0].c_str(), strlen(eventdata[0].c_str()));
				read(clnt_sock, message, sizeof(message));
				if(strcmp(message,"ack")!=0)
				{
					cout<<"data ack error"<<endl;
					errorflag = true;
					break;
				}
			}
			if(errorflag==true)break;
			write(clnt_sock, "end", 4);

			//Read Event are available or not
			read(clnt_sock, message, sizeof(message));
			if(strcmp(message,"yes")==0)
			{
				cout<<"event is possible for "<<clnt_ID[index]<<endl;
				clnt_result[index] = 1;
				clnt_command[index] = 0;
			}
			else if(strcmp(message,"no")==0)
			{
				cout<<"event is impossible for "<<clnt_ID[index]<<endl;
				clnt_result[index] = -1;
				clnt_command[index] = 0;
			}
			else
			{
				clnt_result[index] = -1;
				clnt_command[index] = 0;
				cout<<"wrong response"<<endl;
				break;
			}

			//--Wait until all client answered
			for(i=0;i<clnt_number;i++)
			{
				if(clnt_command[i]==1)
					i--;
			}

			//--Check all the client is okay
			for(i=0;i<clnt_number;i++)
			{
				if(clnt_result[i]==-1)
				{
					write(clnt_sock, "remove", 3);
					break;
				}
				if(i==(clnt_number-1))
				{
					write(clnt_sock, "add", 4);
					break;
				}
			}

			//Read ack
			read(clnt_sock, message, sizeof(message));
			if(strcmp(message,"ack")!=0)
			{
				cout<<"ok ack error"<<endl;
				break;
			}

		}
	}
	//send_message(message, str_len);

	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_number; i++)
		if (clnt_sock == clnt_rx_socks[i])
		{
			close(clnt_tx_socks[i]);
			break;
		}
	pthread_mutex_unlock(&mutx);

	close(clnt_sock);
	return 0;
}

void error_handling(const char* message)
{
	cout << message << endl;
	exit(1);

}

