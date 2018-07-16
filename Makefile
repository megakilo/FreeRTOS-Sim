######## Build options ########

verbose = 0

######## Build setup ########

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)

# .o directory
ODIR            = obj

# Source VPATHS
VPATH           += $(SRCROOT)/Source
VPATH	        += $(SRCROOT)/Source/portable/MemMang
VPATH	        += $(SRCROOT)/Source/portable/GCC/POSIX
VPATH           += $(SRCROOT)/Demo/Common
VPATH			+= $(SRCROOT)/Project/FileIO
VPATH			+= $(SRCROOT)/Project

# FreeRTOS Objects
C_FILES			+= croutine.c
C_FILES			+= event_groups.c
C_FILES			+= list.c
C_FILES			+= queue.c
C_FILES			+= tasks.c
C_FILES			+= timers.c

# portable Objects
C_FILES			+= heap_3.c
C_FILES			+= port.c

# Demo Objects
C_FILES			+= Minimal/blocktim.c
C_FILES			+= Minimal/countsem.c
C_FILES			+= Minimal/GenQTest.c
C_FILES			+= Minimal/QPeek.c
C_FILES			+= Minimal/recmutex.c
C_FILES			+= Full/BlockQ.c
C_FILES			+= Full/death.c
C_FILES			+= Full/dynamic.c
C_FILES			+= Full/flop.c
C_FILES			+= Full/integer.c
C_FILES			+= Full/PollQ.c
C_FILES			+= Full/semtest.c
C_FILES			+= Full/print.c

C_FILES			+= Minimal/AbortDelay.c
C_FILES			+= Minimal/EventGroupsDemo.c
C_FILES			+= Minimal/IntSemTest.c
C_FILES			+= Minimal/QueueSet.c
C_FILES			+= Minimal/QueueSetPolling.c
C_FILES			+= Minimal/QueueOverwrite.c
C_FILES			+= Minimal/TaskNotify.c
C_FILES			+= Minimal/TimerDemo.c

# Main Object
C_FILES			+= main.c

# Include Paths
INCLUDES        += -I$(SRCROOT)/Source/include
INCLUDES        += -I$(SRCROOT)/Source/portable/GCC/POSIX/
INCLUDES        += -I$(SRCROOT)/Demo/Common/include
INCLUDES        += -I$(SRCROOT)/Project

# Generate OBJS names
OBJS = $(patsubst %.c,%.o,$(C_FILES))

######## C Flags ########

# Warnings
CWARNS += -W
CWARNS += -Wall
CWARNS += -Werror
CWARNS += -Wextra
CWARNS += -Wformat
CWARNS += -Wmissing-braces
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wmissing-prototypes

#CWARNS += -Wno-unused-function

CFLAGS += -m32
CFLAGS += -DDEBUG=1
#CFLAGS += -g -DUSE_STDIO=1 -D__GCC_POSIX__=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port. 
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2

######## Makefile targets ########

# Rules
.PHONY : all
all: FreeRTOS-Sim


# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: %.c
	@mkdir -p $(dir $@)
# If verbose, print gcc execution, else hide
ifeq ($(verbose),1)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<
else
	@echo ">> Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c -o $@ $<
endif

FreeRTOS-Sim: $(_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
else
	@$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
endif

	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) FreeRTOS-Sim
	@echo "--------------"
	@echo "CLEAN COMPLETE"
	@echo "--------------"


.PHONY: valgrind
valgrind: FreeRTOS-Sim
	valgrind.bin --tool=memcheck --leak-check=full --show-reachable=yes --track-fds=yes ./FreeRTOS-Sim
