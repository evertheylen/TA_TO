tinystr.o: tinyxml/tinystr.cpp tinyxml/tinystr.h
	$(CC) $(CXXFLAGS) -c $< -o $@
