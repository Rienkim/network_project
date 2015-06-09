/*
 * Servermode.cpp
 *
 *  Created on: 2015. 6. 4.
 *      Author: suck
 */

#include "Servermode.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::string;

int clnt_number = 0;
int clnt_socks[10];
int clnt_state[10];
char clnt_ID[10][40] = { 0 };
//int clnt_command[10] = { 0 };
//int clnt_result[10] = { 0 };
pthread_mutex_t mutx;
//bool isAllClientNotified = false;
//vector<string> eventdata;

void * clientCom(void *)
{
  while(1)
  {
    pthread_mutex_lock( &mutx);
    int clients = clnt_number;
    int socks[10];
    memcpy(socks, clnt_socks, sizeof(int) * clnt_number);
    int state[10];
    memcpy(state, clnt_state, sizeof(int) * clnt_number);
    char cl_id[10][40];
    memcpy(cl_id, clnt_ID, 10 * 40);
    pthread_mutex_unlock( &mutx);

    for(int i = 0; i < clients; i++)
    {
      if(state[i] == NCON)
        continue;

      char buffer[BUFSIZE] = { 0 };
      int ret = recv(socks[i], buffer, BUFSIZE, MSG_PEEK | MSG_DONTWAIT);

      if(ret > 0)
      {
        read(socks[i], buffer, ret);

        if(strcmp(buffer, "close") == 0)
        {
          pthread_mutex_lock( &mutx);
          cout << "Client " << clnt_ID[i] << " disconnected" << endl;
          clnt_state[i] = NCON;
          pthread_mutex_unlock( &mutx);
        }

        if(strcmp(buffer, "sync") == 0)
        {
          cout << "Sync received!" << endl;
          write(socks[i], "ack", 4);

          vector<string> data;

          read(socks[i], buffer, BUFSIZE);
          while(strcmp(buffer, "ended") != 0)
          {
            cout << "Received data: " << buffer << endl;
            data.push_back(string(buffer));
            memset( &buffer[0], 0, sizeof(buffer));
            write(socks[i], "ack", 4);
            read(socks[i], buffer, BUFSIZE);
          }

          cout << "Receive ended" << endl;

          if(data.size() < 5)
            cout << "Communication Error: Not all data received!" << endl;

          bool create = false;
          int num_inform = 0;
          int to_inform[10] = { 0 };

          // check all clients and inform them about sync
          for(unsigned int id = 4; id < data.size(); id++)
          {
            for(int clts = 0; clts < clients; clts++)
            {
              if(strcmp(cl_id[clts], data[id].c_str()) != 0)
                continue;

              // Client found.
              // Start communication.
              char buffer[4] = { 0 };
              write(socks[clts], "sync", 5);
              read(socks[clts], buffer, 4);
              if(strcmp(buffer, "ack") != 0)
              {
                cout << "sync not ack" << endl;
                create = false;
                break;
              }
              cout << "sync is ack" << endl;

              for(int i = 0; i < 4; i++)
              {
                char ack_buffer[4] = { 0 };
                write(socks[clts], data[i].c_str(),
                    strlen(data[i].c_str()) + 1);

                read(socks[clts], ack_buffer, 4);
                if(strcmp(ack_buffer, "ack") != 0)
                {
                  cout << "Handshake Error : data write ack" << endl;
                  create = false;
                  break;
                }
                cout << "data is ack" << endl;
              }

              write(socks[clts], "ended", 6);
              read(socks[clts], buffer, 7);

              if(strcmp(buffer, "delete") == 0)
              {
                create = false;
                break;
              }
              else
                if(strcmp(buffer, "create") == 0)
                {
                  create = true;
                  to_inform[num_inform++ ] = clts;
                  break;
                }
                else
                {
                  cout << "Communication Error!" << endl;
                  create = false;
                  break;
                }

            }

            if( !create)
            {
              for(int j = 0; j < num_inform; j++)
                write(socks[to_inform[j]], "delete", 7);

              write(socks[i], "delete", 7);
            }
          }

          if(create)
          {
            for(int j = 0; j < num_inform; j++)
              write(socks[to_inform[j]], "create", 7);

            write(socks[i], "create", 7);
          }
        }
      }
    }
  }
  return NULL;
}

void runServer()
{
//Local Variables
  int serv_sock;
  int clnt_sock;
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  unsigned int clnt_addr_size;

//Mutex Init
  if(pthread_mutex_init( &mutx, NULL))
    error_handling("mutex error");

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  int option = 1;
  setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

//Server Address Setting
  memset( &serv_addr, 0x00, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5050);

  if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() error");

  if(listen(serv_sock, 5) == -1)
    error_handling("listen() error");

//Connects Client
  char message[BUFSIZE];

  pthread_t thread;
  pthread_create( &thread, NULL, clientCom, (void *) NULL);

  while(1)
  {
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr,
        &clnt_addr_size);

    read(clnt_sock, message, BUFSIZE);

    if(strcmp(message, "con") == 0)
    {
      memset( &message[0], 0, sizeof(message));
      cout << "received con request" << endl;
      write(clnt_sock, "ack", 4);

      read(clnt_sock, message, BUFSIZE);
      cout << "ID: " << message << endl;

      bool connected = false;

      pthread_mutex_lock( &mutx);
      for(int i = 0; i < clnt_number; i++)
      {
        // already exists.
        if(strcmp(clnt_ID[i], message) == 0)
        {
          cout << "ID already exists " << endl;
          // is connected.
          if(clnt_state[i] == CON)
          {
            pthread_mutex_unlock( &mutx);
            close(clnt_sock);
            connected = true;
            cout << " and is connected!" << endl;
            break;
          }
          // is not connected.
          else
          {
            clnt_socks[i] = clnt_sock;
            clnt_state[i] = CON;
            pthread_mutex_unlock( &mutx);
            connected = true;
            cout << " but was not connected!" << endl;
            break;
          }
        }
      }

      if( !connected)
      {
        strcpy(clnt_ID[clnt_number], message);
        clnt_socks[clnt_number] = clnt_sock;
        clnt_state[clnt_number++ ] = CON;

        pthread_mutex_unlock( &mutx);

        cout << "New Connection to IP: " << inet_ntoa(clnt_addr.sin_addr)
            << endl;
      }

      memset( &message[0], 0, sizeof(message));

      write(clnt_sock, "ack", 4);
    }
  }
}

/*void * clnt_tx_connection(void * arg)
 {
 int clnt_sock = *((int*)arg);
 char message[BUFSIZE];
 int i;
 int error = 0;
 socklen_t len = sizeof(error);
 while(isAlive(clnt_sock))
 {
 read(clnt_sock, message, sizeof(message));
 if(strcmp(message, "sync") == 0)
 {
 write(clnt_sock, "ack", 4);
 //Read Sync Data from client
 vector < string > recvdata;
 while(isAlive(clnt_sock))
 {
 read(clnt_sock, message, sizeof(message));
 if(strcmp(message, "end") == 0)
 {
 //Check all of client's time is okay
 //TODO: Check all of client's time is okay
 if(recvdata.size() == 4)
 {
 //--Copy ID data(id1,id2,id3...) to tmprcv
 char tmprcv[40], *ptr = NULL;
 strcpy(tmprcv, recvdata[3].c_str());

 //--Copy event data to eventdata vector
 eventdata = recvdata;
 eventdata.pop_back();

 //--Separate id by token
 ptr = strtok(tmprcv, "(),");
 while(ptr != NULL)
 {
 for(i = 0; i < clnt_number; i++)
 {
 if(strcmp(clnt_ID[i], ptr) == 0)
 {
 //Set Client to check their time is okay
 clnt_command[i] = 1;
 break;
 }
 }
 ptr = strtok(NULL, "(),");
 }

 //--Wait until query to every client is done
 for(i = 0; i < clnt_number; i++)
 {
 if(isAllClientNotified == false)
 {
 i = -1;
 continue;
 }
 if(clnt_command[i] == 1)
 i--;
 }

 //--Check all the client is okay
 for(i = 0; i < clnt_number; i++)
 {
 if(clnt_result[i] == -1)
 {
 write(clnt_sock, "no", 3);
 break;
 }
 if(i == (clnt_number - 1))
 {
 write(clnt_sock, "yes", 4);
 break;
 }
 }

 //--Set result data to zero
 for(i = 0; i < clnt_number; i++)
 {
 clnt_result[i] = 0;
 }
 break;
 }
 else
 {
 //if(recvdata.size != 4)
 cout << "Recv Data length error!" << endl;
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
 pthread_mutex_lock( &mutx);
 for(i = 0; i < clnt_number; i++)
 if(clnt_sock == clnt_tx_socks[i])
 {
 close(clnt_rx_socks[i]);
 for(; i < clnt_number - 1; i++)
 {
 clnt_tx_socks[i] = clnt_tx_socks[i + 1];
 clnt_rx_socks[i] = clnt_rx_socks[i + 1];
 strcpy(clnt_ID[i], clnt_ID[i + 1]);
 }
 break;
 }
 clnt_number--;
 pthread_mutex_unlock( &mutx);
 close(clnt_sock);
 return 0;
 }

 void * clnt_rx_connection(void * arg)
 {
 int clnt_sock = *((int*)arg);
 char message[BUFSIZE];
 int i, index = 0;
 int error = 0;
 socklen_t len = sizeof(error);

 //Find index of this socket
 for(i = 0; i < clnt_number; i++)
 {
 if(clnt_rx_socks[i] == clnt_sock)
 index = i;
 }

 while(isAlive(clnt_sock))
 {
 if(clnt_command[index] == 1)
 {
 write(clnt_sock, "ok", 3);
 read(clnt_sock, message, sizeof(message));
 if(strcmp(message, "ack") != 0)
 {
 cout << "ok ack error" << endl;
 break;
 }
 //TODO : implement serverside of client rx

 //Event vector Size Check
 if(eventdata.size() != 4)
 {
 cout << "Event size error" << endl;
 break;
 }

 //Send event data to the client
 bool errorflag = false;
 for(i = 0; i < 4; i++)
 {
 write(clnt_sock, eventdata[0].c_str(), strlen(eventdata[0].c_str()));
 read(clnt_sock, message, sizeof(message));
 if(strcmp(message, "ack") != 0)
 {
 cout << "data ack error" << endl;
 errorflag = true;
 break;
 }
 }
 if(errorflag == true)
 break;
 write(clnt_sock, "end", 4);

 //Read Event are available or not
 read(clnt_sock, message, sizeof(message));
 if(strcmp(message, "yes") == 0)
 {
 cout << "event is possible for " << clnt_ID[index] << endl;
 clnt_result[index] = 1;
 clnt_command[index] = 0;
 }
 else
 if(strcmp(message, "no") == 0)
 {
 cout << "event is impossible for " << clnt_ID[index] << endl;
 clnt_result[index] = -1;
 clnt_command[index] = 0;
 }
 else
 {
 clnt_result[index] = -1;
 clnt_command[index] = 0;
 cout << "wrong response" << endl;
 break;
 }

 //--Wait until all client answered
 for(i = 0; i < clnt_number; i++)
 {
 if(clnt_command[i] == 1)
 i--;
 }

 //--Check all the client is okay
 for(i = 0; i < clnt_number; i++)
 {
 if(clnt_result[i] == -1)
 {
 write(clnt_sock, "remove", 3);
 break;
 }
 if(i == (clnt_number - 1))
 {
 write(clnt_sock, "add", 4);
 break;
 }
 }

 //Read ack
 read(clnt_sock, message, sizeof(message));
 if(strcmp(message, "ack") != 0)
 {
 cout << "ok ack error" << endl;
 break;
 }

 }
 }
 //send_message(message, str_len);

 pthread_mutex_lock( &mutx);
 for(i = 0; i < clnt_number; i++)
 if(clnt_sock == clnt_rx_socks[i])
 {
 close(clnt_tx_socks[i]);
 break;
 }
 pthread_mutex_unlock( &mutx);

 close(clnt_sock);
 return 0;
 }*/

void error_handling(const char* message)
{
  cout << message << endl;
  exit(1);
}

