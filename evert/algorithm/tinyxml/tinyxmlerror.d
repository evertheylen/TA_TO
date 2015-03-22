tinyxmlerror.o: tinyxml/tinyxmlerror.cpp tinyxml/tinyxml.h
	$(CC) $(CXXFLAGS) -c $< -o $@
