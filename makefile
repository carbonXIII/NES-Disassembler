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
	    
disasm: $(OBJECTS) disassembler/ops.bin disassembler/ops.bin
	g++ $(OBJECTS) -o $(EXEC_DIR)/disasm && \
	cp disassembler/ops.bin $(EXEC_DIR)/ops.bin
	
compressor: compressor/main.cpp compressor/ops.csv
	g++ $(CXXFLAGS) compressor/main.cpp -o compressor/main && \
	compressor/./main compressor/ops.csv -o compressor/ops.bin && \
	cp compressor/ops.bin disassembler/ops.bin

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(EXEC_DIR)/disasm

all: compressor disasm
	

   



    

