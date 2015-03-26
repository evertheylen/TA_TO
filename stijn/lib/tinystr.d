tinystr.o: lib/tinystr.cpp lib/tinystr.h
	$(CC) $(CXXFLAGS) -c $< -o $@
