MAIN = \
    main-validate.cpp \
    main-tune-epochs.cpp \
    main-tune-degrees.cpp \
    main-tune-widths.cpp \

SRC =

BINARY = $(MAIN:.cpp=)
OBJ = $(SRC:.cpp=.o)

DEPS = Makefile.deps
DATS = train.dat devel.dat test.dat

STAGES = 2

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

COMPILE = $(CXX) $(ACPPFLAGS)
LINK = $(CXX) -o $@ $^ $(ACPPFLAGS) $(ALDFLAGS)

ifeq ($(STAGE),)
all: stage$(STAGES)
else ifeq ($(STAGE),1)
all: $(DEPS)
else ifeq ($(STAGE),2)
all: $(BINARY) $(DATS)
endif

stage1:
	$(MAKE) STAGE=1

stage2: stage1
	$(MAKE) STAGE=2

$(DEPS):
	$(COMPILE) -MM $(MAIN) >$@
.PHONY: $(DEPS)

ifeq ($(STAGE),2)
include $(DEPS)
endif

main-validate: main-validate.o
	$(LINK)

main-tune-epochs: main-tune-epochs.o
	$(LINK)

main-tune-degrees: main-tune-degrees.o
	$(LINK)

main-tune-widths: main-tune-widths.o
	$(LINK)

%.o: %.cpp
	$(COMPILE) -c $< -o $@

train.dat:
	tail -n +1001 optdigits.tra >$@

devel.dat:
	head -n 1000 optdigits.tra >$@

test.dat:
	cp optdigits.tes $@

clean:
	-rm -f $(OBJ) $(DEPS)

distclean: clean
	-rm -f $(BINARY)

archive:
	git archive master -o mick-parking.tar.gz \
	    --prefix=mick-parking/

.PHONY: all clean distclean archive
