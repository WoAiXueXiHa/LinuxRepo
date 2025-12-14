# 变量定义
SRC = main.cpp add.cpp 
OBJ = $(SRC:.cpp=.o)
BIN = myapp

# 默认目标
$(BIN): $(OBJ)
	g++ -o $@ $^

# 编译目标
%.o: %.cpp 
	g++ -c $< -o $@

# 清理目标
.PHONY: clean 
clean:
	rm -f $(OBJ) $(BIN)

