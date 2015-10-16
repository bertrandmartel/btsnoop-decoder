# Declaration of variables
CC = g++
CC_FLAGS = -w -g

VERSION=0.1
RELEASE_DIR=build-btsnoop-$(VERSION)

# File names
EXEC = snoop-test
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
snoop-test: $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) -L../snoop-stream-lib/$(RELEASE_DIR) -lpthread -lbtsnoop

# To obtain object files
%.o: %.cpp
	$(CC) -I. -I../snoop-stream-lib -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)