NAME = CASM

CC = gcc
LD = gcc
RM = rm -rf

DEFINES = #-DSHOW_EXECUTE

LIBS = 

SOURCE_DIR = source
BUILD_DIR = build
ASMFILES_DIR = asmtext

C_OPTS = -I. \
				 -std=c99 \
				 -O2 \
				 -Wall \
				 -fmessage-length=0 \
				 -g3 \
				 -gdwarf-2

C_FILES = main.c \
					hardware.c \
					parsing.c \
					core.c

ASMFILES_MODULE = $(ASMFILES_DIR)/swap_asm_1.txt $(ASMFILES_DIR)/swap_asm_2.txt
ASMFILES_DATA = $(ASMFILES_DIR)/swap_asm_data.txt
ASMFILES_PUSHPOP = $(ASMFILES_DIR)/swap_asm_pushpop.txt

C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)

ALL_CFLAGS = $(C_OPTS) $(DEFINES) $(CFLAGS)
ALL_LDFLAGS = $(LD_FLAGS) -g3 -gdwarf-2

AUTODEPENDENCY_CFLAGS = -MMD -MF$(@:.o=.d) -MT$@

.SUFFIXES: .o .c

all: $(NAME)
	./$(NAME) $(ASMFILES_MODULE)

clean:
	$(RM) $(BUILD_DIR) $(NAME)

data: $(NAME)
	./$(NAME) $(ASMFILES_DATA)

pushpop: $(NAME)
	./$(NAME) $(ASMFILES_PUSHPOP)

$(NAME): $(C_OBJS)
	$(LD) $(ALL_LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(ALL_CFLAGS) $(AUTODEPENDENCY_CFLAGS) -c $< -o $@
