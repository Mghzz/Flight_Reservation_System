CXX = g++
CXXFLAGS = -Wall -g -std=c++20 -I$(INC_DIR)

SRC_DIR = cpp
INC_DIR = hpp
OBJ_DIR = build

TARGETS = FRS client

SRCS_FRS = $(wildcard $(SRC_DIR)/main.cpp $(SRC_DIR)/*.cpp)
SRCS_CLIENT = $(SRC_DIR)/client.cpp

OBJS_FRS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(filter-out $(SRC_DIR)/client.cpp,$(SRCS_FRS)))
OBJS_CLIENT = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS_CLIENT))

all: $(TARGETS)

FRS: $(OBJS_FRS)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

client: $(OBJS_CLIENT)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INC_DIR)/*.hpp)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR) $(TARGETS)

.PHONY: all clean
