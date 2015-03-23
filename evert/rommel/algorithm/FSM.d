FSM.o: FSM.cpp tinyxml/tinyxml.h FSM.h
	$(CC) $(CXXFLAGS) -c $< -o $@
