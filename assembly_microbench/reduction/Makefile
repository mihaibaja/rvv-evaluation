CXX := g++
CXXFLAGS := -O3 -std=gnu++17 -Wall -Wextra -march=rv64gcv_zfh_zvfh
LDFLAGS := -static -nostdlib++

OBJDIR := build/obj
DRIVER_OBJ := $(OBJDIR)/driver.o
KERNEL_SRCS := $(wildcard kernels/*.cpp)
KERNEL_OBJS := $(patsubst kernels/%.cpp,$(OBJDIR)/kernels/%.o,$(KERNEL_SRCS))
EXE := $(patsubst kernels/%.cpp,build/%.x,$(KERNEL_SRCS))

.PHONY: all clean run

all: $(EXE)

build/%.x: $(OBJDIR)/kernels/%.o $(DRIVER_OBJ) | build
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

$(OBJDIR)/kernels/%.o: kernels/%.cpp | $(OBJDIR)/kernels
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(DRIVER_OBJ): driver.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build $(OBJDIR) $(OBJDIR)/kernels:
	mkdir -p $@

run: all
	./run_all.sh

clean:
	rm -rf build
