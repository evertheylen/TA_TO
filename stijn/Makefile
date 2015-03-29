CXXFLAGS   = -Wall -Wextra -g -fstack-protector-all -std=c++11
CXX = g++
EXECUTABLE = ProductAutomaton
EXTENSION  = cpp
SOURCES    = $(basename $(shell find . -name '*.$(EXTENSION)'))

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $(LDFLAGS) $^ -o $@
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*~'  -delete

%.d: %.$(EXTENSION)
	$(CXX) $(CXXFLAGS) -MM $< -o $@
	echo sed -i 's/:/ $@:/' $@
	printf '\t$$(CC) $$(CXXFLAGS) -c $$< -o $$@\n' >>$@

-include $(addsuffix .d,$(SOURCES))

.PHONY: clean
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*~'  -delete
	find . -name '*.png' -delete
	find . -name '*.gv' -delete
	find . -name '*.ps' -delete
	find . -name 'ProductAutomaton' -delete

