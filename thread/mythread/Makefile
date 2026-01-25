bin=thread
src=$(wildcard *.cc)
obj=$(src:.cc=.o)
cc=g++
ldflags=-std=c++17 -lpthread

$(bin):$(src)
	$(cc) -o $@ $^ $(ldflags)
%.o:%.cc
	$(cc) -c $< $(ldflags)
.PHONY:clean
clean:
	rm -f $(bin) $(obj)