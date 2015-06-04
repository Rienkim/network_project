/*
 * Signin.cpp
 *
 *  Created on: 2015. 6. 3.
 *      Author: suck
 */

#include "Signin.h"
#include "Calendar.h"
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>

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

void Signin::printUsage()
{
  cout << "Usage : signin <IP> <Port>" << endl;
}

void* Signin::tx_message(void* arg)
{
  thread_in thread_input = *((thread_in *)arg);
  int server_socket = thread_input.sock;
  FILE *writefp;
  writefp = fdopen(server_socket, "w");
  fputs("tx", writefp);
  fputs("", writefp);

  return NULL;
}

void* Signin::rx_message(void* arg)
{
  thread_in thread_input = *((thread_in *)arg);

  cout << "Hello world";
  return NULL;
}

//------------------------------------------------------------------------------
int Signin::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 3)
  {
    cout << params.size() << endl;
    printUsage();
    return ERROR;
  }

  //--------------------------------------------------
  //---------Variables
  int sock_tx, sock_rx;
  struct sockaddr_in serv_addr;
  pthread_t tx_thread, rx_thread;
  void * thread_result;


  //--------------------------------------------------
  //---------Creating Socket
  sock_tx = socket(PF_INET, SOCK_STREAM, 0);
  sock_rx = socket(PF_INET, SOCK_STREAM, 0);
  if(sock_tx == -1)
  {
    cout << "Socket() Error" << endl;
    return ERROR;
  }
  if(sock_rx == -1)
  {
    cout << "Socket() Error" << endl;
    close(sock_tx);
    return ERROR;
  }

  //--------------------------------------------------
  //---------Connecting Socket to the SERVER
  memset( &serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(params[2].c_str()));
  serv_addr.sin_addr.s_addr = inet_addr(params[1].c_str());

  if(connect(sock_tx, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
  {
    cout << "tx_connect() error" << endl;
    return ERROR;
  }
  if(connect(sock_rx, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
  {
	cout << "rx_connect() error" << endl;
	close(sock_tx);
	return ERROR;
  }

  //--------------------------------------------------
  //---------Create Threads
  thread_in tx_thread_input, rx_thread_input;
  tx_thread_input.calendar = &(calendar);
  tx_thread_input.sock = sock_tx;
  rx_thread_input.calendar = &(calendar);
  rx_thread_input.sock = sock_rx;

  pthread_create( &tx_thread, NULL, Signin::tx_message,
      (void *) &tx_thread_input);
  pthread_create( &rx_thread, NULL, Signin::rx_message,
      (void *) &rx_thread_input);

  calendar.setConnectionState(true);
  return SUCCESS;
}

