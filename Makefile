ext     := cpp
cc      := g++
ldflags :=
ccflags := -g -D DEBUG

sources := $(shell find -type f -name "*.$(ext)")
require := $(sources:.$(ext)=.d)
objects := $(sources:.$(ext)=.o)
dirs    := $(shell echo $(sources) | xargs -n1 dirname)
target  := a.out

$(target): $(require) $(objects)
	$(cc) -o $(target) $(ldflags) $(objects)

%.d: %.cpp
	@rm -f $@;\
	$(cc) -MM $(ccflags) $< >> $@;\
	sed -i "s:$(shell basename $(@:.d=.o)):$@:" $@;\
	$(cc) -MM $(ccflags) $< >> $@;\
	sed -i "s:$(shell basename $(@:.d=.o)):$(@:.d=.o):" $@;\
	echo "\t$(cc) -c -o $(@:.d=.o) $(ccflags) $(@:.d=.$(ext))" >> $@

include $(require)

.PHONY: clean
clean:
	rm -f $(target) $(objects) $(require) *.dot *.pdf
