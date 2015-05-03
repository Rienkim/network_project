main: main.o Calendar.o Command.o Create.o Event.o Quit.o Show.o
	g++ -o main main.o Calendar.o Command.o Create.o Event.o Quit.o Show.o
main.o : main.cpp
	g++ -c main.cpp
Calender.o : Calender.cpp
	g++ -c Calender.cpp
Command.o : Command.cpp
	g++ -c Command.cpp
Create.o : Create.cpp
	g++ -c Create.cpp
Event.o : Event.cpp
	g++ -c Event.cpp
Quit.o : Quit.cpp
	g++ -c Quit.cpp
Show.o : Show.cpp
	g++ -c Show.cpp
clean:
	rm -f *.o
	rm main
