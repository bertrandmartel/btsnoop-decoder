# Declaration of variables
CC = g++
CC_FLAGS = -w -g

VERSION=0.1

# File names
EXEC = snoop-test
SOURCES = $(wildcard btsnoop/*.cpp *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
RELEASE_DIR=build-btsnoop-$(VERSION)

# Main target
libbtsnoop.a: $(OBJECTS)
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)/include
	ar rcs $(RELEASE_DIR)/libbtsnoop.a $^
	rsync -avm --include=*/ --include=*.h --exclude=* . $(RELEASE_DIR)/include
	zip -r $(RELEASE_DIR)/btsnoop-v$(VERSION)-release.zip $(RELEASE_DIR)/*

# To obtain object files
%.o: %.cpp
	$(CC) -I./btsnoop -I. -c $(CC_FLAGS) $< -o $@

# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)
	rm -rf ./$(RELEASE_DIR)/*