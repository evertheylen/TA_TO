algorithm.o: algorithm.cpp lib/tinyxml.h lib/tinystr.h algorithm.h
	$(CC) $(CXXFLAGS) -c $< -o $@
