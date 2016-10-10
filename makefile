SOURCE_DIR:= emulation disassembler .
INCLUDES:= $(addsuffix /*.h,SOURCE_DIR) $(addsuffix /*.hpp,SOURCE_DIR)
OBJ_DIR:= obj
SOURCES:= $(shell find $(SOURCE_DIR) -maxdepth 1 -name "*.cpp")
OBJECTS:= $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
EXEC_DIR:= bin/linux_x86
CXXFLAGS:= -std=c++11

includes:
	for folder in $(SOURCE_DIR); do if ! [ -d obj/$$folder ]; then mkdir obj/$$folder; fi; done

$(OBJ_DIR)/%.o: %.cpp
	g++ -c $(CXXFLAGS) $< -o $@
	    
disasm: $(OBJECTS)
	g++ $(OBJECTS) -o $(EXEC_DIR)/disasm
	
compressor/main: compressor/main.cpp
	g++ $(CXXFLAGS) compressor/main.cpp -o compressor/main

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

all: disasm
	

   



    

