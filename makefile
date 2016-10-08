SOURCE_DIR:= emulation disassembler .
INCLUDES:= $(addsuffix /*.h,SOURCE_DIR) $(addsuffix /*.hpp,SOURCE_DIR)
OBJ_DIR:= obj
#SOURCES:= $(SOURCE_DIR: %=$(shell find -name '$(addsuffix %,*.cpp)'))
SOURCES:= $(SOURCE_DIR: %=hella%)
OBJECTS:= $(SOURCES:./%.cpp=$(addprefix $(OBJ_DIR)/,%.o))
EXECUTABLE:= disasm
CXXFLAGS:= -std=c++11

includes:
	for folder in $(SOURCE_DIR); do if ! [ -d obj/$$folder ]; then mkdir obj/$$folder; fi; done

$(OBJ_DIR)/%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@
	    
disasm: $(OBJECTS)
	g++ $(OBJECTS) -o $(EXECUTABLE)
	
compressor/main: compressor/main.cpp
	g++ $(CXXFLAGS) compressor/main.cpp -o compressor/main

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

all: disasm
	

   



    

