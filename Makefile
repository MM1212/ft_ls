
PROJECT_NAME = LS

NAME = ft_ls

SRC_FILES = $(shell find src -type f -name "*.c" | sed 's/src\///g')

SRC_DIR = src

SRCS = $(addprefix $(SRC_DIR)/, $(SRC_FILES))

OBJ_DIR = objs
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

DEP_DIR = deps
DEP_FILES = $(addprefix $(DEP_DIR)/, $(SRCS:.c=.d))

LIBFT_PATH = libs/libft
LIBFT_BIN = $(addprefix $(LIBFT_PATH)/,bin)
LIBFT_INCLUDES = $(addprefix $(LIBFT_PATH)/,includes/)
LIBFT_ARCH = $(addprefix $(LIBFT_BIN)/,libft.a)

CLI_PATH = libs/cli
CLI_BIN = $(addprefix $(CLI_PATH)/,bin)
CLI_INCLUDES = $(addprefix $(CLI_PATH)/,includes/)
CLI_ARCH = $(addprefix $(CLI_BIN)/,libcli.a)

INCLUDES = -Iincludes -I. -I$(LIBFT_INCLUDES) -I$(CLI_INCLUDES)
LIBS =  -L$(CLI_BIN) -lcli -L$(LIBFT_BIN) -lft

CC = clang

DEBUG_ON = 0

### COLORS ###

RED = \033[0;31m
GREEN = \033[0;92m
YELLOW = \033[93m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
CYAN = \033[96m
ORANGE = \033[0;33m
RESET = \033[0m

CFLAGS = \
		$(INCLUDES) \
		-MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d \
		-Wall -Wextra -Werror \
		-DVERSION="\"$$(cat VERSION)\"" \
		-g -gdwarf-2 -g3 \
		$(if $(DEBUG_ON),-g -fsanitize=address,undefined,) \
		$(if $(DEBUG_ON),-DDEBUG,) \
		-DCOLORS_RED="\"$(RED)\"" \
		-DCOLORS_GREEN="\"$(GREEN)\"" \
		-DCOLORS_YELLOW="\"$(YELLOW)\"" \
		-DCOLORS_BLUE="\"$(BLUE)\"" \
		-DCOLORS_MAGENTA="\"$(MAGENTA)\"" \
		-DCOLORS_CYAN="\"$(CYAN)\"" \
		-DCOLORS_ORANGE="\"$(ORANGE)\"" \
		-DCOLORS_RESET="\"$(RESET)\""



TAG = [$(CYAN)$(PROJECT_NAME)$(RESET)]

### END OF COLORS ###

all: $(NAME)

$(NAME): $(OBJ_FILES) $(LIBFT_ARCH) $(CLI_ARCH)
	@echo "$(TAG) linking $(YELLOW)$@$(RESET).."
	@$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(LIBS)
	@echo "$(TAG) done$(RESET)!"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "$(TAG) compiling $(YELLOW)$<$(RESET).."
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(DEP_DIR)/$*.d)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	@mkdir -p $@
$(DEP_DIR):
	@mkdir -p $@

$(LIBFT_ARCH):
	@make -C $(LIBFT_PATH) --no-print-directory

$(CLI_ARCH):
	@make -C $(CLI_PATH) --no-print-directory

$(DEP_FILES):

include $(wildcard $(DEP_FILES))

clean:
	@make -C $(LIBFT_PATH) clean --no-print-directory
	@make -C $(CLI_PATH) clean --no-print-directory
	@rm -rf $(OBJ_DIR)
	@rm -rf $(DEP_DIR)
	@echo "$(TAG) cleaned $(YELLOW)objects$(RESET)!"

fclean: clean
	@make -C $(LIBFT_PATH) fclean --no-print-directory
	@make -C $(CLI_PATH) fclean --no-print-directory
	@rm -f $(NAME)
	@echo "$(TAG) cleaned $(YELLOW)executable$(RESET)!"


re: fclean
	@make -C $(LIBFT_PATH) re --no-print-directory --jobs=$(shell nproc)
	@make -C $(CLI_PATH) re --no-print-directory --jobs=$(shell nproc)
	@make $(MAKE_MT) all --jobs=$(shell nproc) --output-sync=target --no-print-directory

watch:
	@while true; do \
		make $(MAKE_MT) all --no-print-directory --no-print; \
		inotifywait -qre close_write --exclude ".*\.d" $(SRCS) $(INCLUDES); \
		echo "$(TAG) $(YELLOW)recompiling$(RESET).."; \
	done

.PHONY: all clean fclean re
