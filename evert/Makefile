CXXFLAGS   = -g3 -O0 -fstack-protector-all -std=c++14
#CXXFLAGS   = -O3 -std=c++14
LDFLAGS    =
EXECUTABLE = algo
EXTENSION  = cpp
SOURCES    = $(basename $(shell find . -maxdepth 2 -name '*.$(EXTENSION)'))


$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<


.PHONY: all
all: $(EXECUTABLE)
	@echo ----------------------------------------------
	
.PHONY: clean
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*~'  -delete
