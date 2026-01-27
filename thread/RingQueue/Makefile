bin=rq
src=$(wildcard *.cc)
obj=$(src:.cc=.o)
cc=g++
ldflags=-std=c++11 -lpthread

$(bin):$(src)
	$(cc) -o $@ $^ $(ldflags)
%.o:%.cc
	$(cc) -o $< $(ldflags)

.PHONY:clean
clean:
	rm -f $(bin) $(obj)

	