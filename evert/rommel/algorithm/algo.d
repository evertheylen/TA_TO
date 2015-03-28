algo.o: algo.cpp tinyxml/tinyxml.h FSM.h
	$(CC) $(CXXFLAGS) -c $< -o $@
