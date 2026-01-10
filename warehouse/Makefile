libmain.a:%.o
	@ar -rc $@ $^
	@echo "bulid $^ to %@ ..."

%.o:%.c
	@gcc -c $<
	@echo "compling $< to $@ ..."

PHONY:clean
clean:
	@rm -rf *.a *.o stdc*
	@echo "clean ..."

.PHONY:output
output:
	@mkdir -p stdc/include 
	@mkdir -p stdc/lib 
	@cp -f *.h stdc/include
	@cp -f *.a stdc/lib 
	@zip stdc.zip stdc 
