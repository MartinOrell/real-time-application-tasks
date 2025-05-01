exFile := main.exe
srcDirs := . caps libs/myRealTime/src
incDirs := . ./caps/ ./libs/myRealTime/include ./libs/myRealTime/mod_include

#Flags to make rules for .h dependencies (look for changes in included files)
DEPFLAGS := -MP -MD

#Add -I into each include directory
INCLUDES := $(foreach D,$(incDirs),-I$(D))

#Find all .cpp files and put into variable
CPPFiles := $(foreach D,$(srcDirs),$(wildcard $(D)/*.cpp))

#Get all object (.o) files from CPPFiles. Put them in obj/ folder (ignoring ./ prefix since obj/./main.o is wrong)
objFiles := $(addprefix obj/,$(patsubst ./%,%,$(patsubst %.cpp,%.o,$(CPPFiles))))
#do the same with dependecy (.d) files
depFiles := $(addprefix obj/,$(patsubst ./%,%,$(patsubst %.cpp,%.d,$(CPPFiles))))

all: $(exFile)

#Create executable from object files
$(exFile): $(objFiles)
	g++ -std=c++20 -o $@ $^
	echo $(depFiles)

#create object files and dependency files (create folder if it does not exist)
obj/%.o:%.cpp
	@mkdir -p $(dir $@)
	g++ -std=c++20 $(INCLUDES) $(DEPFLAGS) -c -o $@ $<

#Remove executable file and obj folder container all object files and dependency files
clean:
	rm -f $(exFile)
	rm -rf ./obj

# include the dependencies
-include $(depFiles)