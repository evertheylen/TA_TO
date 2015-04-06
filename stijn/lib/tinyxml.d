tinyxml.o: lib/tinyxml.cpp lib/tinyxml.h lib/tinystr.h
	$(CC) $(CXXFLAGS) -c $< -o $@
