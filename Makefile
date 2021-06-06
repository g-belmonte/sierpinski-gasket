src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)

CXXFLAGS = -Wall -g -MMD
LDFLAGS = -lglfw -lGLEW -lGL

sierpinski: $(obj)
		$(CXX) -o $@ $^ $(LDFLAGS)

-include $(dep)

.PHONY: clean
clean:
		$(RM) sierpinski $(obj)

.PHONY: clean-dep
clean-dep:
		$(RM) $(dep)

.PHONY: clean-all
clean-all: clean clean-dep
