/*
 * Servermode.h
 *
 *  Created on: 2015. 6. 4.
 *      Author: suck
 */

#ifndef SERVERMODE_H_
#define SERVERMODE_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

//----------------------------------------
//---Constants
static const int BUFSIZE = 100;
static const int NOTCONNECTED = 0;
static const int TXONLY = 1;
static const int RXONLY = 2;
static const int TXRXBOTH = 3;

//----------------------------------------
//---Functions
void* clnt_tx_connection(void* arg);
void* clnt_rx_connection(void* arg);
void send_message(char* message, int len);
void error_handling(const char* message);
void runServer();

//----------------------------------------
//---Variables
extern int  clnt_number;
extern int clnt_tx_socks[10];
extern int clnt_rx_socks[10];
extern int clnt_state[10];
extern char clnt_ID[10][40];
extern pthread_mutex_t mutx;
extern bool isAllClientNotified;
extern  vector<string> eventdata;


#endif /* SERVERMODE_H_ */
