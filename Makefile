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

$(out): $(obj)
	g++ -o $@ $(ldflags) $(obj)

%.o: %.cpp %.d
	g++ -c -o $@ $(cxxflags) $<
	
%.d: %.cpp
	g++ -M -MT $(@:.d=.o) -o $@ $(cxxflags) $<
	
-include $(dep)