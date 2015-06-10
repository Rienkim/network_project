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

//--------------------------------------------------------------------------
static void* threadFunc(void * object)
{
  return ((Calendar *)object)->serverCom(NULL);
}

//------------------------------------------------------------------------------
int Signin::execute(Calendar& calendar, vector<string> &params)
{
  if(calendar.getConnectionState())
  {
    cout << "You are already signed in!" << endl;
    return ERROR;
  }

  if(params.size() != 3)
  {
    cout << params.size() << endl;
    printUsage();
    return ERROR;
  }

  // Save ID
  calendar.setID(params[0].c_str());

  // Variables
  int sock;
  struct sockaddr_in serv_addr;

  // Creating Socket
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
    cout << "Socket() Error" << endl;
    return ERROR;
  }

  // Connecting Socket to the SERVER
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

  // Handshake : Send con
  write(sock, "con", 3);

  read(sock, ack_buffer, sizeof(ack_buffer));
  if(strcmp(ack_buffer, "ack") != 0)
  {
    cout << "Handshake Error : con not ack" << endl;
    return ERROR;
  }

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

  cout << "Client successfully connected to server!" << endl;

  calendar.setServerSock(sock);
  calendar.setConnectionState(true);

  pthread_t server_com;
  pthread_create( &server_com, NULL, threadFunc, &calendar);
  calendar.setServerThread(server_com);

  return SUCCESS;
}

