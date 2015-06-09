/*
 * Servermode.h
 *
 *  Created on: 2015. 6. 4.
 *      Author: suck
 */

#ifndef SERVERMODE_H_
#define SERVERMODE_H_

#include <pthread.h>

//----------------------------------------
//---Constants
static const int BUFSIZE = 100;
static const int NCON = 0;
static const int CON = 1;

//----------------------------------------
//---Functions
//void* clnt_tx_connection(void* arg);
//void* clnt_rx_connection(void* arg);
//void send_message(char* message, int len);
void runServer();
void error_handling(const char* message);
//bool isAlive(int sock);

//----------------------------------------
//---Variables
extern int  clnt_number;
extern int clnt_socks[10];
extern int clnt_state[10];
extern char clnt_ID[10][40];
extern pthread_mutex_t mutx;
//extern bool isAllClientNotified;
//extern  vector<string> eventdata;


#endif /* SERVERMODE_H_ */
