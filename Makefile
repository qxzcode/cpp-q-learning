##### Basic Configuration #####

NAME := Q-learning
EXE  := q_learn

DEBUG := FALSE

SRC_DIR := src
BIN_DIR := build


##### Variables #####

# build commands
GCC := gcc
AS  := as
GXX := g++
LD  := ld

# build flags
GCCFLAGS  := -Wall -Wextra -std=c++14
LDFLAGS   := 

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -O3#s
else
	GCCFLAGS += -O0 -g
endif

# automatic dependency generation stuff
DEPFILE = $(@:.o=.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPFILE).tmp
FINISH_DEP = @mv -f $(DEPFILE).tmp $(DEPFILE) && touch $@

# list of object files to be built
OBJS := $(patsubst $(SRC_DIR)/%.c,   $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.c))
OBJS += $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.cpp))
OBJS += $(patsubst $(SRC_DIR)/%.S,   $(BIN_DIR)/%.o, $(shell find $(SRC_DIR) -name \*.S))
OBJS := $(strip $(OBJS))

# list of build directories to be created
OBJDIRS := $(sort $(dir $(OBJS)))
OBJDIRS := $(OBJDIRS:/=)  # remove trailing '/' from dirs
OBJDIRS := $(strip $(OBJDIRS))

# list of dependency files to create/include
DEPFILES := $(OBJS:.o=.d)


##### Recipes #####

## Phonies ##

.PHONY: all
all: $(EXE)

.PHONY: run
run: $(EXE)
	@echo "---- Running $(EXE)"
	@./$(EXE)

.PHONY: clean
clean:
	@echo Deleting all build products...
	@rm -f $(EXE)
	@rm -rf $(BIN_DIR)

.PHONY: rebuild
rebuild: clean all

## Regular recipes ##

# recipes to make the bin directories
$(foreach dir,$(OBJDIRS),$(eval $(dir):;@echo Creating directory \'$(dir)\'...;mkdir -p $(dir)))

# empty recipes to silence errors about missing .d files
$(foreach dep,$(DEPFILES),$(eval $(dep): ;))
.PRECIOUS: $(DEPFILES)

.SECONDEXPANSION:  # to enable the use of automatic vars in prerequisites
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c   $(BIN_DIR)/%.d | $$(@D)
	@echo "---- Compiling $<"
	@$(GCC) $(GCCFLAGS) $(DEPFLAGS) -c $< -o $@
	@$(FINISH_DEP)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp $(BIN_DIR)/%.d | $$(@D)
	@echo "---- Compiling $<"
	@$(GXX) $(GCCFLAGS) $(DEPFLAGS) -c $< -o $@
	@$(FINISH_DEP)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.s   $(BIN_DIR)/%.d | $$(@D)
	@echo "---- Assembling $<"
	$(warning This Makefile has not been tested for *.s sources)
	@$(AS) -c $< -o $@

$(EXE): $(OBJS)
	@echo ---- Linking...
	@$(GXX) $^ -o $(EXE) $(LDFLAGS)


# include the generated dependency files
include $(wildcard $(DEPFILES))
