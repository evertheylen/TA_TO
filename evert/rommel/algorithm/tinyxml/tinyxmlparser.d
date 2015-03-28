tinyxmlparser.o: tinyxml/tinyxmlparser.cpp tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@
