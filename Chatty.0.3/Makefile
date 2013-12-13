#############################################################################
#   $ make           compile and link all programs
#   $ make clean     clean objects and the executable file
#   $ make distclean clean objects, the executable and dependencies
#===========================================================================

## Customizable Section: adapt those variables to suit your program.
##==========================================================================
# The pre-processor and compiler options.
MY_CFLAGS = -I./include -DBE_DEBUG -D_REENTRANT -Wall -D__EXTENSIONS__

# Libraries to link to, depending on OS
UNAME = $(shell uname)
ifeq ($(UNAME), SunOS)              # Sun OS
MY_LIBS = -lresolv -lsocket -lnsl
endif
ifeq ($(UNAME), Linux)              # Linux
MY_LIBS = -lresolv -lnsl -lpthread
endif
ifeq ($(UNAME), Darwin)             # Mac OS
MY_LIBS =
endif

# The options used in linking as well as in any direct use of ld.
LDFLAGS +=

# The executable file names.
PROGS   =  ChattyMe

## Implicit Section: change the following only when necessary.
##==========================================================================

CC := gcc
SRCS := $(wildcard *.c)

# objects other than those in PROGS
ALL_OBJS := ${SRCS:.c=.o}
ALL_NAMES := ${ALL_OBJS:.o=}
INC_DIR := ./include

# all object files other than those named $(PROGS).o
COMMON_OBJS :=
define APPEND_CO
ifeq (,$(findstring $(1), $(PROGS)))
COMMON_OBJS += $(1).o
endif
endef
$(foreach name,$(ALL_NAMES),$(eval $(call APPEND_CO, $(name))))

.PHONY: all clean distclean

all: $(PROGS)

define PROG_RULE
$(1) : $(COMMON_OBJS) $(1).o
	${CC} -I${INC_DIR} ${CFLAGS} ${MY_LIBS} ${COMMON_OBJS} $(1).c -o $(1)
endef

$(foreach program,$(PROGS),$(eval $(call PROG_RULE, $(program))))

## Objects are dependent on headers (and implicitly .c files)
##==========================================================================
define OBJ_RULE
$(1) : ${INC_DIR}/${1:.o=.h} ${1:.o=.c}
	${CC} -I${INC_DIR} ${CFLAGS} -c ${1:.o=.c}
endef

$(foreach obj_file,$(ALL_OBJS),$(eval $(call OBJ_RULE,$(obj_file))))

## clean things up
##==========================================================================
clean:
	@- $(RM) $(PROGS)
	@- $(RM) -rf $(ALL_OBJS) *.dSYM

distclean: clean
