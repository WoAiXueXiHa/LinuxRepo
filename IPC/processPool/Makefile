BIN=processpool
CC=g++
# 编译标志：-c 只编译，-Wall 警告，-std=c++11 支持function/lambda
CFLAGS=-c -Wall -std=c++11
# 链接标志：无需额外库，仅需C++11
LDFLAGS=-std=c++11

# 源文件：仅main.cpp（所有.hpp被include，无需加入）
SRC=main.cpp
# 目标文件：main.cpp -> main.o
OBJ=$(SRC:.cpp=.o)

# 链接规则：生成可执行文件
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# 编译规则：编译main.cpp为main.o
main.o: main.cpp ProcessPool.hpp Channel.hpp Task.hpp
	$(CC) $(CFLAGS) -o $@ $<

# 清理规则：删除可执行文件和目标文件
.PHONY: clean
clean:
	rm -f $(BIN) $(OBJ)