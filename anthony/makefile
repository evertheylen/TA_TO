INCLUDE = 	 -ITinyXML

CXX =		g++

CXXFLAGS =	-c -Wall -fstack-protector-all -std=c++11 $(INCLUDE)

LDFLAGS=

LIBS =

SOURCES =	$(shell find ./TinyXML -name "*.cpp")
OBJECTS =	$(SOURCES:.cpp=.o)


MAINSRC =	$(shell find ./ -name "*.cpp")
MAINOBJ =	$(MAINSRC:.cpp=.o)

EXEC = 		Main

.PHONY : all
all: $(EXEC)

.PHONY: main
Main: $(OBJECTS) $(MAINOBJ)
	$(CXX) $(LDFLAGS) $^ -o $@.exe

.PHONY: clean
clean:
	find . -name "*.o" -delete
	rm -f $(EXEC:=.exe)
	rm -rf $(OBJDIR)
