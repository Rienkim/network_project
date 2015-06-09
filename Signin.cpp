/*
 * Signin.cpp
 *
 *  Created on: 2015. 6. 3.
 *      Author: suck
 */

#include "Event.h"
#include "Signin.h"
#include "Calendar.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using std::string;
using std::vector;
using std::cout;
using std::endl;

//struct to use thread input parameter
typedef struct thread_in_
{
    Calendar* calendar;
    int sock;
} thread_in;

//------------------------------------------------------------------------------
Signin::Signin(const string& name)
    : Command(name)
{
}

//------------------------------------------------------------------------------
Signin::~Signin() throw()
{
}

//------------------------------------------------------------------------------
void Signin::printUsage()
{
  cout << "Usage : signin <IP> <Port>" << endl;
}

//------------------------------------------------------------------------------
/*void* Signin::tx_message(void* arg)
{
  //Variables
  thread_in thread_input = *((thread_in *)arg);
  int server_socket = thread_input.sock;
  char ack_buffer[10];

  //Handshake : Send TX
  write(server_socket, "tx", 3);
  read(server_socket, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : Tx ack" << endl;
    return NULL;
  }
  //Handshake : Send ID
  char IDbuffer[40];
  thread_input.calendar->getID(IDbuffer);
  write(server_socket, IDbuffer, 40);
  read(server_socket, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : Tx ID ack" << endl;
    return NULL;
  }

  cout << "Client tx successfully connected to server" << endl;

  //tx Communication
  int error = 0;
  socklen_t len = sizeof(error);
  string command_out;
  vector<string> data_out;
  while(isAlive(server_socket))
  {
    //TODO : Implement tx communication between serv, clnt
    thread_input.calendar->getSendData(command_out, data_out);
    if(command_out == "sync")
    {
      //Send sync command to the server
      write(server_socket, command_out.c_str(),
          strlen(command_out.c_str()) + 1);
      read(server_socket, ack_buffer, sizeof(ack_buffer));
      if(strcmp(ack_buffer, "ack") != 0)
      {
        cout << "Handshake Error : sync ack" << endl;
        return NULL;
      }

      for(int i = 0; i < data_out.size(); i++)
      {
        write(server_socket, data_out[i].c_str(),
            strlen(data_out[i].c_str()) + 1);
        read(server_socket, ack_buffer, sizeof(ack_buffer));
        if(strcmp(ack_buffer, "ack") != 0)
        {
          cout << "Handshake Error : data write ack" << endl;
          return NULL;
        }
      }
      write(server_socket, "end", 4);
      read(server_socket, ack_buffer, sizeof(ack_buffer));
      if((strcmp(ack_buffer, "yes") != 0) && (strcmp(ack_buffer, "no") != 0))
      {
        cout << "Handshake Error : sync end ack" << endl;
        return NULL;
      }
      thread_input.calendar->setRecvData(string(ack_buffer));
    }
    command_out = "";
    data_out.clear();
    thread_input.calendar->setSendData(command_out, data_out);
  }
  cout << "end of tx connection" << endl;
  return NULL;
}

//------------------------------------------------------------------------------
void* Signin::rx_message(void* arg)
{
  //Variables
  thread_in thread_input = *((thread_in *)arg);
  int server_socket = thread_input.sock;
  char ack_buffer[10];

  //Handshake : Send TX
  write(server_socket, "rx", 3);
  read(server_socket, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : Rx ack" << endl;
    return NULL;
  }
  //Handshake : Send ID
  char IDbuffer[40];
  thread_input.calendar->getID(IDbuffer);
  write(server_socket, IDbuffer, 40);
  read(server_socket, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : Tx ID ack" << endl;
    return NULL;
  }

  cout << "Client rx successfully connected to server" << endl;

  //rx Communication
  int error = 0;
  char buffer[40];
  socklen_t len = sizeof(error);
  while(isAlive(server_socket))
  {
    //TODO : Implement rx communication between serv, clnt
    read(server_socket, buffer, sizeof(buffer));
    if(strcmp("ok", buffer) == 0) // While connected
    {
      write(server_socket, "ack", 4);

      vector<string> eventdata;
      while(isAlive(server_socket)) // While connected
      {
        read(server_socket, buffer, sizeof(buffer));
        if(strcmp(buffer, "end") == 0)
        {
          if(thread_input.calendar->isOverlap(eventdata) == true)
          {
            //overlap
            write(server_socket, "no", 3);
            read(server_socket, buffer, sizeof(buffer));
            //if overlap happens, do nothing
            write(server_socket, "ack", 4);
          }
          else
          {
            //not overlap
            write(server_socket, "yes", 4);
            read(server_socket, buffer, sizeof(buffer));
            if(strcmp(buffer, "add") == 0)
            {
              Event tmpevent;
              tmpevent.stringToEvent(eventdata);
              thread_input.calendar->addEvent( &tmpevent, false);
              write(server_socket, "ack", 4);
            }
            else
              if(strcmp(buffer, "remove") == 0)
              {
                //if other calenders have overlaps, do nothing.
                write(server_socket, "ack", 4);
              }
          }
          break;
        }
        eventdata.push_back(string(buffer));
        write(server_socket, "ack", 4);
      }
    }
  }
  cout << "end of rx connection" << endl;
  return NULL;
}*/


//--------------------------------------------------------------------------
static void* threadFunc(void * object)
{
    return ((Calendar *) object)->serverCom(NULL);
}

//------------------------------------------------------------------------------
int Signin::execute(Calendar& calendar, vector<string> &params)
{
  if(calendar.getConnectionState())
    return ERROR;

  if(params.size() != 3)
  {
    cout << params.size() << endl;
    printUsage();
    return ERROR;
  }

  //--------------------------------------------------
  //---------Save ID
  calendar.setID(params[0].c_str());

  //--------------------------------------------------
  //---------Variables
  int sock;
  struct sockaddr_in serv_addr;

  //--------------------------------------------------
  //---------Creating Socket
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    cout << "Socket() Error" << endl;
    return ERROR;
  }

  //--------------------------------------------------
  //---------Connecting Socket to the SERVER
  memset( &serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(params[2].c_str()));
  serv_addr.sin_addr.s_addr = inet_addr(params[1].c_str());

  if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
  {
    cout << "tx_connect() error" << endl;
    return ERROR;
  }

  char ack_buffer[10];

  //Handshake : Send con
  write(sock, "con", 3);

  read(sock, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : con not ack" << endl;
    return ERROR;
  }
  cout << "Received ack on con" << endl;

  //Handshake : Send ID
  char IDbuffer[40];
  calendar.getID(IDbuffer);
  write(sock, IDbuffer, 40);

  read(sock, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : ID not ack" << endl;
    return ERROR;
  }
  cout << "Received ack on ID" << endl;

  cout << "Client successfully connected to server!" << endl;

  calendar.setServerSock(sock);
  calendar.setConnectionState(true);

  pthread_t server_com;
  pthread_create(&server_com, NULL, threadFunc, &calendar);
  calendar.setServerThread(server_com);

  return SUCCESS;
}

