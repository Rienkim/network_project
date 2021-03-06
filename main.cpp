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
#include "Servermode.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <cstdlib>

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
	if (argc > 2)
	{
		cout << "Usage: Start with ./cnw [filename] to load calendar "
				<< "or ./cnw to make new calendar." << endl;
	}

	// New Calendar is created.
	if (argc == 1)
	{
		tm* begin1 = new tm;
		begin1->tm_hour = 9;
		begin1->tm_min = 20;
		begin1->tm_mday = 10;
		begin1->tm_mon = 6 - 1;
		begin1->tm_year = 2015 - 1900;

		Event* event1 = new Event("Presentation", begin1, 30);

		list<Event*> calendar_queue;

		Calendar calendar(calendar_queue);

		calendar.addEvent(event1);
		calendar.setConnectionState(false);
		calendar.run();
		return SUCCESS;
	}

	if (strcmp("-s", argv[1]) == 0)
	{
		//Server mode
		cout << "You've entered server mode!" << endl;
		runServer();
		return SUCCESS;
	}

	// Else load calendar
	string filename = argv[1];
	ifstream file;
	file.open(filename.c_str(), std::ios::in | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << "Error: file " << filename << " cannot be opened.\n";
		return ERROR;
	}

	char check_input[50];
	for (int i = 0; i < 50; i++)
		check_input[i] = 0;
	file.read(check_input, 4);

	if (!(strcmp(check_input, "CNW\0") == 0))
	{
		cout << "File corrupted!" << endl;
		return ERROR;
	}

	int num_events;
	file.read((char*) &num_events, sizeof(int));

	list<Event*> calendar_queue;
	string event_name;
	short duration;

	for (int i = 0; i < num_events; i++)
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
	calendar.setConnectionState(false);
	calendar.run();

	return SUCCESS;
}

