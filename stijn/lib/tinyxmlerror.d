tinyxmlerror.o: lib/tinyxmlerror.cpp lib/tinyxml.h lib/tinystr.h
	$(CC) $(CXXFLAGS) -c $< -o $@
