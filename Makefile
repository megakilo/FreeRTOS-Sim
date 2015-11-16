# Build options
###############################
verbose = 0
demo = 1
###############################

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)

# .o directory
ODIR            = obj

# Source VPATHS
VPATH           += $(SRCROOT)/Source
VPATH	        += $(SRCROOT)/Source/portable/MemMang
VPATH	        += $(SRCROOT)/Source/portable/GCC/POSIX
ifeq ($(demo),1)
VPATH           += $(SRCROOT)/Demo/Common/Full
VPATH           += $(SRCROOT)/Demo/Common/Minimal
VPATH			+= $(SRCROOT)/Demo/POSIX/FileIO
VPATH			+= $(SRCROOT)/Demo/POSIX/ParTest
VPATH			+= $(SRCROOT)/Demo/POSIX
else
VPATH			+= $(SRCROOT)/Project
VPATH			+= $(SRCROOT)/Project/FileIO
VPATH			+= $(SRCROOT)/Project/ParTest
endif

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

ifeq ($(demo),1)
# Minimal Demo Objects
C_FILES			+= blocktim.c
C_FILES			+= countsem.c
C_FILES			+= crflash.c
C_FILES			+= crhook.c
C_FILES			+= GenQTest.c
C_FILES			+= QPeek.c
C_FILES			+= recmutex.c

# Full Demo Objects
C_FILES			+= BlockQ.c
C_FILES			+= death.c
C_FILES			+= dynamic.c
C_FILES			+= events.c
C_FILES			+= flop.c
C_FILES			+= integer.c
C_FILES			+= PollQ.c
C_FILES			+= print.c
C_FILES			+= semtest.c

# IO objects
C_FILES			+= fileIO.c
C_FILES			+= ParTest.c
endif

# Main Object
C_FILES			+= main.c

# Include Paths
INCLUDES        += -I$(SRCROOT)/Source/include
INCLUDES        += -I$(SRCROOT)/Source/portable/GCC/POSIX/
ifeq ($(demo),1)
INCLUDES        += -I$(SRCROOT)/Demo/Common/include
INCLUDES        += -I$(SRCROOT)/Demo/POSIX
else
INCLUDES        += -I$(SRCROOT)/Project
endif

# Generate OBJS names
OBJS = $(patsubst %.c,%.o,$(C_FILES))

# C Flags
CFLAGS += -m32
CFLAGS += -DDEBUG=1
CFLAGS += -g -pthread -DUSE_STDIO=1 -D__GCC_POSIX__=1

# Warnings
COMMON_WARNINGS += -W
COMMON_WARNINGS += -Wall
COMMON_WARNINGS += -Werror
COMMON_WARNINGS += -Wextra
COMMON_WARNINGS += -Wformat
COMMON_WARNINGS += -Wmissing-braces
COMMON_WARNINGS += -Wno-cast-align
COMMON_WARNINGS += -Wparentheses
COMMON_WARNINGS += -Wshadow
COMMON_WARNINGS += -Wno-sign-compare
COMMON_WARNINGS += -Wswitch
COMMON_WARNINGS += -Wuninitialized
COMMON_WARNINGS += -Wunknown-pragmas
COMMON_WARNINGS += -Wunused-function
COMMON_WARNINGS += -Wunused-label
COMMON_WARNINGS += -Wunused-parameter
COMMON_WARNINGS += -Wunused-value
COMMON_WARNINGS += -Wunused-variable

CWARNS          += $(COMMON_WARNINGS)
CWARNS          += -Wmissing-prototypes

CFLAGS          += $(INCLUDES) $(COMMONFLAGS) $(CWARNS) -O2

# Rules
.PHONY : all
all: setup FreeRTOS-Sim

.PHONY : setup
setup:
# Make obj directory
	@mkdir -p $(ODIR)

# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: %.c
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
