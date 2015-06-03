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
#include <iostream>
#include <cstdlib>

using std::string;
using std::vector;
using std::cout;
using std::endl;

//struct to use thread input parameter
typedef struct thread_in_ {
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
	cout<<"Usage : signin <IP> <Port>"<<endl;
}

void * Signin::send_message(void * arg)
{
	thread_in thread_input = *((thread_in *)arg);
	thread_input.calendar->getConnectionState();
	cout<<"Hello world";
}

void * Signin::recv_message(void * arg)
{
	cout<<"Hello world";
}

//------------------------------------------------------------------------------
int Signin::execute(Calendar& calendar, vector<string> &params)
{
  if(params.size() != 2)
  {
	  cout<<params.size()<<endl;
	  printUsage();
	  return ERROR;
  }

  int sock;
  struct sockaddr_in serv_addr;
  pthread_t snd_thread, rcv_thread;
  void * thread_result;
  thread_in thread_input;

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if(sock == -1)
  {
	  cout<<"Socket() Error"<<endl;
	  return ERROR;
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(params[1].c_str()));
  serv_addr.sin_addr.s_addr = inet_addr(params[0].c_str());

  if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
  {
	  cout<<"connect() error"<<endl;
	  return ERROR;
  }

  thread_input.calendar = &(calendar);
  thread_input.sock = sock;

  pthread_create(&snd_thread, NULL, Signin::send_message, (void *)&thread_input);
  pthread_create(&rcv_thread, NULL, Signin::recv_message, (void *)&thread_input);

  calendar.setConnectionState(true);
  return SUCCESS;
}


