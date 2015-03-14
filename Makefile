BINARY = main
MAIN = main.cpp

SRC = \
      $(MAIN) \

OBJ = $(SRC:.cpp=.o)

ACPPFLAGS += -Wall -Wextra -Werror -std=c++11 -pipe
ACPPFLAGS += -Wshadow -Wpointer-arith -Wcast-qual \
             -Wconversion -Wformat=2 -Wstrict-overflow=5 \
             -Wno-missing-braces -Wstrict-aliasing \
             -Wredundant-decls -Wwrite-strings -Wmissing-include-dirs \
             -Wuninitialized
ACPPFLAGS += $(CPPFLAGS)

ALDFLAGS += -lm
ALDFLAGS += $(LDFLAGS)

ifeq ($(DEBUG), 1)
    ACPPFLAGS += -O0 -g
endif

ifeq ($(OPTIMIZE), 1)
    ACPPFLAGS += -flto -O2
    ALDFLAGS += -flto -O2
endif

ifeq ($(NATIVE), 1)
    ACPPFLAGS += -march=native
    ALDFLAGS += -march=native
endif

ifeq ($(PROFILE), 1)
    ACPPFLAGS += -g -O0 -pg
    ALDFLAGS += -g -pg
endif

all: $(BINARY)

$(BINARY): $(OBJ)
	$(CXX) -o $@ $^ $(ALDFLAGS)

%.o: %.cpp
	$(CXX) -c $(ACPPFLAGS) $< -o $@

clean:
	-rm -f $(OBJ)

distclean: clean
	-rm -f $(BINARY)

archive:
	git archive master -o mick-parking.tar.gz \
	    --prefix=mick-parking/

.PHONY: all clean distclean archive
