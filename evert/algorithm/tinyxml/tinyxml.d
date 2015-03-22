tinyxml.o: tinyxml/tinyxml.cpp tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@
