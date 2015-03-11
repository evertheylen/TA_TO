CXXFLAGS   = -Wall -Wextra -g -fstack-protector-all -std=c++11
#CXXFLAGS   = -O3 -std=c++11
LDFLAGS    =
EXECUTABLE = algo
EXTENSION  = cpp
SOURCES    = $(basename $(shell find . -name '*.$(EXTENSION)'))

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $(LDFLAGS) $^ -o $@

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

.PHONY: all_and_clean
all_and_clean:
	make all
	make clean

.PHONY: run
run:
	./algo epsilon-NFA.xml

.PHONY: test
test:
	make clean
	make all
	make run