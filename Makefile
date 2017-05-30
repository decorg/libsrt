.PHONY: all clean rebuild archive

CC=gcc

# We define the compilation flags
CFLAGS_DEBUG=-g -ansi -pedantic -Wall -Wextra
CFLAGS_RELEASE=-DNDEBUG -O3
CFLAGS=$(CFLAGS_DEBUG)

# Folders to create
OBJ=obj
BIN=bin

# Main targets
TARGET=$(BIN)/libsrt.a
CDEPS = $(wildcard *.c)
DEPS = $(CDEPS:.c=.o)
TDEPS=$(patsubst %.o, $(OBJ)/%.o, $(DEPS))
ARCHIVE=libsrt.tar.gz

vpath %.o $(OBJ)

all: $(OBJ) $(BIN) $(DEPS)
	ar -crv $(TARGET) $(TDEPS)
	ranlib $(TARGET)
	
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(OBJ)/$@

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $< -o $(OBJ)/$@
	
$(OBJ):
	@mkdir $(OBJ)
	
$(BIN):
	@mkdir $(BIN)
	
clean:
	rm -f $(OBJ)/*.o
	rm -f $(TARGET)
	rm -f $(ARCHIVE)
	
rebuild: clean all

archive:
	tar cvzf $(ARCHIVE) *.c *.h Makefile

