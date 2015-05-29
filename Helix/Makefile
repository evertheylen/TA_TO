
INCLUDES = -I ./lib -I ./lib/FSM -I ./lib/tinyxml  -I ./engine_r -I ./engine_s -I ./etc -I ./lib/gtest/include
# Don't forget to update ./GUI/Helix.pro

CXX        = g++
CXXFLAGS   = -g3 -O0 -std=c++11 -Werror=nonnull -Werror=return-type -Werror=main -Werror=missing-include-dirs -fstack-protector-all $(INCLUDES)
#CXXFLAGS   = -O3 -std=c++11 $(INCLUDES)

OBJ_FILES  = $(shell find . -mindepth 2 \( -name '*.cpp' -or -name '*.c' \) -and -not -path './lib/*' -and -not -path './tests/*' -and -not -path './GUI/*')
OBJS       = $(addsuffix .o, $(basename $(OBJ_FILES)))


LIB_FILES  = $(shell find . -mindepth 2 \( -name '*.cpp' -or -name '*.c' \) -and -path './lib/*' -and -not -path './lib/gtest/*')
LIBS       = $(addsuffix .o, $(basename $(LIB_FILES)))

TEST_FILES = $(shell find . -mindepth 2 \( -name '*.cpp' -or -name '*.c' \) -and -path './tests/*')
TESTS      = $(addsuffix .o, $(basename $(TEST_FILES)))
TEST_LIBS  = -L./lib/gtest/lib
TEST_STATIC_LIBS = -lgtest -lgtest_main

SHELL      = /bin/bash
TARGETS    = convertor GUI test


#STATICLIBS = -lreadline




.PHONY: convertor
convertor: $(OBJS) $(LIBS) convertor.o
	@echo -e "\033[1;38;2;20;255;20;48;2;0;0;0m --> Compiling and linking $@ \033[0m"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS) convertor.o


.PHONY: evert_gedoe
evert_gedoe: $(OBJS) $(LIBS) evert_gedoe.o
	@echo -e "\033[1;38;2;20;255;20;48;2;0;0;0m --> Compiling and linking $@ \033[0m"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBS) evert_gedoe.o


.PHONY: GUI
GUI: $(OBJS) $(LIBS)
	@echo -e "\033[1;38;2;20;255;20;48;2;0;0;0m --> Building GUI \033[0m"
	@echo -e "  |-> Preparing qmake"
	@echo "$(OBJS) $(LIBS)" > ./GUI/bad_obj_files  # 'communicating' with qmake
	@sed "s:\./:\.\./:g" GUI/bad_obj_files > GUI/obj_files          # replacing ./ with ../
	@rm GUI/bad_obj_files
	@echo -e "  |-> Calling qmake in GUI"
	@cd GUI; qmake;
	@echo -e "  |-> Calling make in GUI"
	@echo -e "  |-> All hope lies in you, Qt!"
	@cd GUI; make;


.PHONY: test
test: $(OBJS) $(LIBS) $(TESTS) test.o
	@echo -e "\033[1;38;2;20;255;20;48;2;0;0;0m --> Compiling and linking $@ \033[0m"
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) test.o $(LIBS) $(TEST_LIBS) $(TEST_STATIC_LIBS) -pthread


.PHONY: gtest
gtest:
	cd lib/gtest;\
	rm CMakeCache.txt;\
	cmake CMakeLists.txt;\
	make;\
	mkdir lib;\
	mv libgtest.a lib/libgtest.a;\
	mv libgtest_main.a lib/libgtest_main.a;


.PHONY: libs
libs: $(LIBS)
	@echo -e "\033[1;38;2;0;255;255;48;2;0;0;0m --> Built libs \033[0m"




# ---[ Varia ]---------------------------------------

.PHONY: all
all: $(TARGETS)

.PHONY: echo
echo:
	@echo INCLUDES = $(INCLUDES)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo OBJS = $(OBJS)
	@echo LIBS = $(LIBS)
	@echo TESTS = $(TESTS)





# ---[ Cleaning ]------------------------------------

.PHONY: clean_objs
clean_objs:
	@rm -f $(OBJS)
	@echo -e "\033[1;38;2;250;170;0;48;2;0;0;0m --> Cleaned object files \033[0m"

.PHONY: clean_libs
clean_libs:
	@rm -f $(LIBS)
	@echo -e "\033[1;38;2;250;170;0;48;2;0;0;0m --> Cleaned lib object files \033[0m"

.PHONY: clean_tests
clean_tests:
	@rm -f $(TESTS)
	@echo -e "\033[1;38;2;250;170;0;48;2;0;0;0m --> Cleaned test object files \033[0m"

.PHONY: clean_GUI
clean_GUI:
	@find . -name '*.o' -path './GUI/*' -delete
	@cd GUI; make clean;
	@echo -e "\033[1;38;2;250;170;0;48;2;0;0;0m --> Cleaned GUI object files \033[0m"

.PHONY: clean
clean:
	@rm -f convertor test
	@rm -f *.o
	@echo -e "\033[1;38;2;255;170;0;48;2;0;0;0m --> Cleaned executables ($(TARGETS)) \033[0m"

.PHONY: cleann
cleann: clean clean_objs clean_libs clean_tests clean_GUI
	@echo -e "\033[1;38;2;255;0;0;48;2;0;0;0m --> Cleaned everything \033[0m"




# ---[ Basics ]--------------------------------------

%.o: %.cpp %.h
	@echo -e "\033[1;38;2;0;255;255;48;2;0;0;0m --> Compiling $@ \033[0m"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

