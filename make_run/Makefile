###################
# Project: Bottle grabing - Rover control
# Benjamin IOLLER
# Inspired by: 
# PROJECT: DXL Protocol 2.0 Read/Write Example Makefile
# AUTHOR : ROBOTIS Ltd.
###################

TARGET      =  rover_example

# compiler options
CC          = gcc
CX          = g++
CCFLAGS     = -O2 -O3 -DLINUX -D_GNU_SOURCE -Wall $(INCLUDES) $(OPENCVflags) -g
CXFLAGS     = -std=c++11 -O2 -O3 -DLINUX -D_GNU_SOURCE -Wall $(INCLUDES) $(OPENCVflags) -g
LNKCC       = $(CX)
LNKFLAGS    = $(CXFLAGS) #-Wl,-rpath,$(DIR_THOR)/lib

# Includes
DIR_MAV    = ../
INCLUDES   += -I$(DIR_MAV)/mavlink/include/mavlink/v2.0

# Sources:
SOURCES = class_rover_test.cpp
SOURCES += serial_port.cpp
SOURCES += rover.cpp

# Object sources:
DIR_OBJS   = .objects
OBJECTS  = $(addsuffix .o,$(addprefix $(DIR_OBJS)/,$(basename $(notdir $(SOURCES)))))

#---------------------------------------------------------------------
# Compiling Rules
#---------------------------------------------------------------------
$(TARGET): make_directory $(OBJECTS)
	$(LNKCC) $(LNKFLAGS) $(LIBRARIES) $(OBJECTS) -o $(TARGET) 

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(DIR_OBJS) core *~ *.a *.so *.lo

make_directory:
	mkdir -p $(DIR_OBJS)/

$(DIR_OBJS)/%.o: ../%.c
	$(CC) $(CCFLAGS) -c $? -o $@

$(DIR_OBJS)/%.o: ../%.cpp
	$(CX) $(CXFLAGS) -c $? -o $@

#---------------------------------------------------------------------
# End of Makefile
#---------------------------------------------------------------------
