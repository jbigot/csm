
## include specify options for configuration
include ./make_config
###


LIBDIR          := ./lib
BIN             := ./bin
SRC             = $(wildcard *.cpp)
OBJ             = $(SRC:.cpp=.o)

ifeq ($(DEBUG),yes)
	Compil_msg=" Compiling using debug mode "
	Display_command=
else
	Compil_msg=" Compiling using release mode"
	Display_command= > "/dev/null"
endif

LIBPATH  := -L$(LIBDIR)

DIR_SRC  :=./Sources

DIR_HEAD :=./Headers

DIR_BENCH := ./Benchmarks

TARGET   := FullSWOF_2D

LIBRARY  :=  -lschemes -lsave -lreconstructions -llimitations -lboundaryconditions -lflux -lparameters -lparser -lskelgis


OBJECTS := FullSWOF_2D.o

INCPATH := -I./$(DIR_HEAD)/libboundaryconditions \
		-I./$(DIR_HEAD)/libflux \
		-I./$(DIR_HEAD)/libparameters \
		-I./$(DIR_HEAD)/libparser \
		-I./$(DIR_HEAD)/liblimitations \
		-I./$(DIR_HEAD)/libreconstructions \
		-I./$(DIR_HEAD)/libsave \
		-I./$(DIR_HEAD)/libschemes

#################################################
# Rules to compile all librarie calling Makefiles
##################################################

all: msg construction $(TARGET) clean
	@mv -f $(TARGET) $(BIN)
	@echo " $(TARGET)  => $(BIN)"

msg:
	@echo $(Compil_msg)

$(TARGET): $(OBJECTS)
	@$(CPP) -o $@ $^ $(LIBPATH) $(LIBRARY)


%.o: $(DIR_SRC)/%.cpp
	@echo @$(CPP) -o $@ -c $<  $(CPPFLAGS) $(INCPATH)
	@$(CPP) -o $@ -c $<  $(CPPFLAGS) $(INCPATH)

construction :
	@cd $(DIR_SRC)/libskelgis && make  $(Display_command)
	@cd $(DIR_SRC)/libboundaryconditions && make  $(Display_command)
	@cd $(DIR_SRC)/libflux && make  $(Display_command)
	@cd $(DIR_SRC)/libparameters && make  $(Display_command)
	@cd $(DIR_SRC)/libparser && make  $(Display_command)
	@cd $(DIR_SRC)/liblimitations && make  $(Display_command)
	@cd $(DIR_SRC)/libreconstructions && make  $(Display_command)
	@cd $(DIR_SRC)/libsave && make  $(Display_command)
	@cd $(DIR_SRC)/libschemes && make  $(Display_command)

####################################################
# Rules to remove .o and .a files calling Makefiles
###################################################

clean:
	@cd $(DIR_SRC)/libskelgis && make clean $(Display_command)
	@cd $(DIR_SRC)/libboundaryconditions && make clean $(Display_command)
	@cd $(DIR_SRC)/libflux && make clean $(Display_command)
	@cd $(DIR_SRC)/libparameters && make clean $(Display_command)
	@cd $(DIR_SRC)/libparser && make clean $(Display_command)
	@cd $(DIR_SRC)/liblimitations && make clean $(Display_command)
	@cd $(DIR_SRC)/libreconstructions && make clean $(Display_command)
	@cd $(DIR_SRC)/libsave && make clean $(Display_command)
	@cd $(DIR_SRC)/libschemes && make clean $(Display_command)
	@rm -f $(OBJECTS)

cleanall : clean
	rm -f ./lib/*.a
	rm -f $(BIN)/$(TARGET)

