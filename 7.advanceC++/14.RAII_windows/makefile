# Global definitions

CC = g++
#SFLAGS = -g -shared -Wl,-soname,
CXXFLAGS = -g -Wall -ansi -pedantic -fPIC
# Directory structure
INC_DIR = ../inc/
CXXFLAGS += -I$(INC_DIR)
LIB_DIR = ../../lib
#LDFLAGS += -L$(LIB_DIR) 
LIB_NAME = libcNthread.1.0
LIB_LINK = libcNthread.so.1
LDLIBS =-pthread
LIB_FILE=$(LIB_DIR)/$(LIB_NAME)
# all contents of current directory
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
# Target and sources
TARGET = test
#LIB_SRCS = ../communication/comm.c
#LIB_OBJS = $(LIB_SRCS:.c=.o)
# Dependencies
all: $(TARGET)

$(TARGET): $(OBJS) $(LIB_OBJS)
	$(CC) -I$(INC_DIR) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDLIBS)

include depends

depends:
	$(CC) -MM -I$(INC_DIR) $(OBJS:.o=.cpp) > depends

commit: $(LIB_FILE) $(LIB_DIR)/$(LIB_LINK) $(LIB_DIR)/libcNthread.so

$(LIB_FILE):
	gcc $(CFLAGS) -shared -Wl,-soname,$(LIB_LINK) -o $(LIB_NAME) *.o 
	mv $(LIB_NAME) $(LIB_DIR)
	
$(LIB_DIR)/$(LIB_LINK):
	ln -sf $(LIB_NAME) $(LIB_DIR)/$(LIB_LINK)
	
$(LIB_DIR)/libcNthread.so:
	ln -sf $(LIB_NAME) $(LIB_DIR)/libcNthread.so

#$(LIB_FILE): $(LIB_OBJS)
#   $(CC) $(SFLAGS)$(LIB_NAME).1 -o $(LIB_NAME).1.0 $(LIB_OBJS)
#   ln -sf $(LIB_NAME).1.0 $(LIB_FILE).1
#   ln -sf $(LIB_NAME).1.0 $(LIB_FILE)
#   mv $(LIB_NAME).1.0 $(LIB_DIR)/
clean:
	-rm -f *.o
	-rm -f smartHouse.out
	-rm -f depends
#   -rm -f $(LIB_FILE) $(LIB_FILE).1 $(LIB_FILE).1.0

run: all
	./test

gdb: all
	gdb ./test
