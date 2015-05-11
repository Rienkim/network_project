SUBMISSION = cnw
EXECUTABLE = $(SUBMISSION)
SOURCES    = $(wildcard *.cpp)
OBJECTS    = $(patsubst %,%,${SOURCES:.cpp=.o})
CXX        = g++
CXXFLAGS   = -Wall -g -c -o
LDFLAGS    = 
LDLIBS     =
#-------------------------------------------------------------------------------

#make executable
all: $(EXECUTABLE) 

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $@ $< -MMD -MF ./$@.d

#link Objects
$(EXECUTABLE) : $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

#make clean
clean:
	rm -f ./*.o
	rm -f ./*.o.d
	rm -f $(EXECUTABLE)

#make cleanw for windows
cleanw :
	del /Q .\*.o
	del /Q .\*.o.d
	del /Q $(EXECUTABLE).exe

#make valgrind
valgrind:
	valgrind --tool=memcheck --leak-check=full ./$(EXECUTABLE)

#make submission zip archive for PALME
submission:
	zip $(SUBMISSION).zip *.cpp *.h Makefile

.PHONY: clean cleanw submission

#The dependencies:
-include $(wildcard *.d)
