INCLUDE_DIR:= emulation .
INCLUDES:= $(addsuffix /*.h,INCLUDE_DIR) $(addsuffix /*.hpp,INCLUDE_DIR)
OBJ_DIR:= obj
SOURCES:= $(shell find -name '*.cpp')
OBJECTS:= $(SOURCES:./%.cpp=$(addprefix $(OBJ_DIR)/,%.o))
EXECUTABLE:= disasm
CXXFLAGS:= -std=c++11

includes:
	for folder in $(INCLUDE_DIR); do if [ "$(folder)" = "." ] && ! [ -d obj/$(folder) ]; then echo $(folder); fi; done
	
$(OBJ_DIR)/%.o: %.cpp includes
	echo I did it

#$(OBJ_DIR)/%.o: $%.cpp $(INCLUDES) $(OBJ_DIR)
#	g++ -c $(CXXFLAGS) $%.cpp -o obj/$%.o;
	    
disasm: $(OBJECTS)
	g++ $(OBJECTS) -o $(EXECUTABLE)

.PHONY: clean
clean:
	rm -f $(OBJECTS)
	rm -f $(EXECUTABLE)

all: disasm
	

   



    

