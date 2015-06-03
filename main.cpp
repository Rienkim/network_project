//------------------------------------------------------------------------------
// main.cpp
//
// CSED353
//
// Paul Schreiner
// TODO: Add your name here.
//------------------------------------------------------------------------------
//

#include "Event.h"
#include "Calendar.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::ifstream;

//--------------------------------------------------------------------------
// Main Function
//
int main(int argc, char *argv[])
{
  if(argc > 2)
  {
    cout << "Usage: Start with ./cnw [filename] to load calendar "
        << "or ./cnw to make new calendar." << endl;
  }

  // New Calendar is created.
  if(argc == 1)
  {
    tm* begin1 = new tm;
    begin1->tm_hour = 21;
    begin1->tm_min = 00;
    begin1->tm_mday = 30;
    begin1->tm_mon = 5 - 1;
    begin1->tm_year = 2015 - 1900;

    tm* begin2 = new tm;
    memcpy(begin2, begin1, sizeof(tm));
    begin2->tm_mday = 18;

    Event* event1 = new Event("Event 1", begin1, 5 * 60);
    Event* event2 = new Event("Event 2", begin2, 3 * 60);

    list<Event*> calendar_queue;

    Calendar calendar(calendar_queue);

    calendar.addEvent(event1);
    calendar.addEvent(event2);

    calendar.run();
    return SUCCESS;
  }

  if(strcmp("-s", argv[1])==0)
  {
    //Server mode 
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    unsigned int clnt_addr_size;
    pthread_t thread;

    if(pthread_mutex_init(&mutx, NULL))
        error_handling("mutex error");

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    //Server Address Setting
    memset(&serv_addr,0x00, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5050);

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");

    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_number++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&thread, NULL, clnt_connection,(void *)clnt_sock);
        cout<<"New Connection : IP: "<<inet_ntoa(clnt_addr.sin_addr)<<endl;
    }
    return SUCCESS
  }


  // Else load calendar.
  string filename = argv[1];
  ifstream file;
  file.open(filename.c_str(), std::ios::in | std::ios::binary);

  if( !file.is_open())
  {
    std::cout << "Error: file " << filename << " cannot be opened.\n";
    return ERROR;
  }

  char check_input[50];
  for(int i = 0; i < 50; i++)
    check_input[i] = 0;
  file.read(check_input, 4);

  if( !(strcmp(check_input, "CNW\0") == 0))
  {
    cout << "File corrupted!" << endl;
    return ERROR;
  }

  int num_events;
  file.read((char*) &num_events, sizeof(int));

  list<Event*> calendar_queue;
  string event_name;
  short duration;

  for(int i = 0; i < num_events; i++)
  {
    Event* event = new Event();
    tm* time = new tm();
    int tm_data;

    getline(file, event_name, '\0');
    event->setName(event_name);

    file.read((char*) &tm_data, sizeof(int));
    time->tm_mday = tm_data;
    file.read((char*) &tm_data, sizeof(int));
    time->tm_mon = tm_data;
    file.read((char*) &tm_data, sizeof(int));
    time->tm_year = tm_data;
    file.read((char*) &tm_data, sizeof(int));
    time->tm_hour = tm_data;
    file.read((char*) &tm_data, sizeof(int));
    time->tm_min = tm_data;
    file.read((char*) &tm_data, sizeof(int));
    time->tm_sec = tm_data;
    file.read((char*) &duration, sizeof(short));

    event->setStartTime(time);
    event->setDuration(duration);

    calendar_queue.push_back(event);
  }

  Calendar calendar(calendar_queue);
  calendar.run();

  return SUCCESS;
}

void * clnt_connection(void * arg)
{
    int clnt_sock = (int)arg;
    int str_len = 0;
    char message[BUFSIZE];
    int i;
    FILE * readfp;

    readfp = fdopen(clnt_sock, "r");

    while(!feof(readfp))
    {
        fgets(message, BUFSIZE-1, readfp);
        send_message(message, strlen(message)+1);
    }
        //send_message(message, str_len);

    pthread_mutex_lock(&mutx);
    for(i=0;i<clnt_number;i++)
        if(clnt_sock == clnt_socks[i])
        {
            for(;i<clnt_number-1;i++)
                clnt_socks[i] = clnt_socks[i+1];
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
    for(i=0;i<clnt_number;i++)
    {
        writefp = fdopen(clnt_socks[i],"w");
        fputs(message, writefp);
        fflush(writefp);
        //write(clnt_socks[i], message, len);
    }
    pthread_mutex_unlock(&mutx);
}

void error_handling(const char* message)
{
    cout<<message<<endl;
    exit(1);

}
