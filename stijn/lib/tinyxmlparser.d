tinyxmlparser.o: lib/tinyxmlparser.cpp lib/tinyxml.h lib/tinystr.h
	$(CC) $(CXXFLAGS) -c $< -o $@
