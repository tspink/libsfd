q := @

top-dir := $(CURDIR)
src-dir := $(top-dir)/src
inc-dir := $(top-dir)/inc
out-dir := $(top-dir)/out

out := $(out-dir)/libsfd.so
src := $(shell find $(src-dir) | grep -E "\.cpp$$")
obj := $(src:.cpp=.o)
dep := $(src:.cpp=.d)

cxxflags := -g -Wall -std=gnu++14 -fPIC -I$(inc-dir) -O3
ldflags  := -shared

TARGET_NAME = $@

all: $(out)

clean: .FORCE
	$(q)rm -f $(out)
	$(q)rm -f $(obj)
	$(q)rm -f $(dep)

$(out): $(obj) $(out-dir)
	@echo "  LD    $(TARGET_NAME)"
	$(q)g++ -o $@ $(ldflags) $(obj)

$(out-dir):
	$(q)mkdir $@

%.o: %.cpp %.d
	@echo "  C++   $(TARGET_NAME)"
	$(q)g++ -c -o $@ $(cxxflags) $<

%.d: %.cpp
	$(q)g++ -M -MT $(@:.d=.o) -o $@ $(cxxflags) $<

-include $(dep)

.PHONY: .FORCE
