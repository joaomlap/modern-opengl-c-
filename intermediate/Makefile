TARGET = main

SRC = src
INC = include
BIN = bin

SOURCE = $(wildcard $(SRC)/*.cpp)
OBJECT = $(patsubst %,$(BIN)/%, $(notdir $(SOURCE:.cpp=.o)))

CC= g++
CFLAGS = -I$(INC)
LIBS = -Ibuild/include -lglfw -ldl -lGLEW
FRAMEWORKS = -framework OpenGL

$(BIN)/$(TARGET): $(OBJECT)
	$(CC) -o $@ $^ $(LIBS) $(FRAMEWORKS)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: help run clean

run: $(BIN)/$(TARGET)
	$(BIN)/$(TARGET)

clean:
	rm -f $(OBJECT) $(BIN)/$(TARGET)

help:
	@echo "src: $(SOURCE)"	
	@echo "obj: $(OBJECT)"
