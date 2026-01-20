SER=server
CLI=client
CC=g++
SER_SRC=server.cc
CLI_SRC=client.cc

.PHONY:all
all:$(SER) $(CLI)

$(SER):$(SER_SRC)
	$(CC) -o $@ $^ -std=c++11
$(CLI):$(CLI_SRC)
	$(CC) -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f $(SER) $(CLI)

