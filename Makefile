top-dir := $(CURDIR)
src-dir := $(top-dir)/src
inc-dir := $(top-dir)/inc
out-dir := $(top-dir)/out
	
out := $(out-dir)/libsfd.so
src := $(shell find $(src-dir) | grep -E "\.cpp$$")
obj := $(src:.cpp=.o)
dep := $(src:.cpp=.d)

cxxflags := -g -Wall -std=gnu++14 -fPIC -I$(inc-dir)
ldflags := -shared

all: $(out)

clean: .FORCE
	rm -f $(out)
	rm -f $(obj)
	rm -f $(dep)

$(out): $(obj) $(out-dir)
	g++ -o $@ $(ldflags) $(obj)
	
$(out-dir):
	mkdir $@ 

%.o: %.cpp %.d
	g++ -c -o $@ $(cxxflags) $<
	
%.d: %.cpp
	g++ -M -MT $(@:.d=.o) -o $@ $(cxxflags) $<
	
-include $(dep)

.PHONY: .FORCE
