CXXFLAGS   = -g3 -O0 -fstack-protector-all -std=c++11 $(shell find -type d -exec echo "-I {}" \;)
#CXXFLAGS   = -O3 -std=c++14
LDFLAGS    =
EXECUTABLE = convertor
EXTENSION  = cpp
INCLUDES   = $(shell find -type d -exec echo "-I {}" \;)
SOURCES    = $(basename $(shell find . -name '*.$(EXTENSION)'))
SHELL      = /bin/bash

$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $(LDFLAGS) $(INCLUDES)  $^ -o $@


%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<




.PHONY: all
all: $(EXECUTABLE)
	@echo ----------------------------------------------

.PHONY: clean
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*~'  -delete

.PHONY: echo
echo:
	@echo INCLUDES = $(INCLUDES)
	@echo SOURCES = $(SOURCES)
	@echo CXXFLAGS = $(CXXFLAGS)

# ----[ Different algorithms ]-----------------------------

# Executing any of these targets will clean the given directory,
# build the convertor, and test it with the files you want.

.PHONY: TFA
TFA:
	make clean
	make all
	./convertor TFA ./TFA/DFA.xml

.PHONY: eNFA-DFA
eNFA-DFA:
	make clean
	make all
	#TODO :)

.PHONY: RE-eNFA
RE-eNFA:
	make clean
	make all
	@i=1; cat ./RE-eNFA/tests | while read -r line; do echo Converting to eNFA \"$$line\"; ./convertor RE-eNFA $$line ./RE-eNFA/regex_$$i.dot; ((i++)); done
	@find ./RE-eNFA/* -name *.dot -exec dot -Tpng {} -o {}.png \;

.PHONY: Product
Product:
	
