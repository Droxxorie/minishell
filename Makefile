# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eraad <eraad@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/04 18:58:38 by eraad             #+#    #+#              #
#    Updated: 2025/10/08 16:55:09 by eraad            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ---- Binary -----------------------------------------------------------------
NAME            := minishell

# ---- Toolchain ---------------------------------------------------------------
CC              := cc
CFLAGS          := -Wall -Wextra -Werror -g3
CPPFLAGS        := -Iincludes -Ilibft/include
LDFLAGS         := -Llibft -lft -lreadline

# ---- Debug / Sanitizers (cibles dédiées, hors build standard) ---------------
NAME_DEBUG      := $(NAME)_debug
NAME_ASAN       := $(NAME)_asan
DEBUG_FLAGS     := -O0 -g3 -DDEBUG
ASAN_FLAGS      := -O0 -g3 -fsanitize=address -fno-omit-frame-pointer

# ---- Colors ------------------------------------------------------------------
DEF             := \033[0m
G               := \033[0;92m
R               := \033[0;91m
Y               := \033[0;93m
CYN             := \033[0;36m
NEON            := \033[38;5;82m
ORANGE          := \033[38;5;208m

# ---- Project layout ----------------------------------------------------------
SRC_DIR         := sources
OBJ_DIR         := obj

LIBFT_DIR       := libft
LIBFT_A         := $(LIBFT_DIR)/libft.a

# ==============================================================================
#                                   SOURCES
# ==============================================================================

# ---- Core --------------------------------------------------------------------
SRCS := \
	$(SRC_DIR)/core/loop/main.c \
	$(SRC_DIR)/core/init/init.c \
	$(SRC_DIR)/core/init/env_copy.c \
	$(SRC_DIR)/core/init/env_list.c \
	$(SRC_DIR)/core/init/env_vars.c \
	$(SRC_DIR)/core/cleanup/reset.c \
	$(SRC_DIR)/core/cleanup/free_env.c \
	$(SRC_DIR)/core/cleanup/free_pipes.c \
	$(SRC_DIR)/core/cleanup/free_tokens.c \
	$(SRC_DIR)/core/cleanup/free_commands.c \

# ---- Errors ------------------------------------------------------------------
SRCS += \
	$(SRC_DIR)/errors/error.c \
	$(SRC_DIR)/errors/syntax.c \

# ---- Execution / Builtins ----------------------------------------------------
SRCS += \
	$(SRC_DIR)/execution/builtins/builtins.c \
	$(SRC_DIR)/execution/builtins/cd.c \
	$(SRC_DIR)/execution/builtins/echo.c \
	$(SRC_DIR)/execution/builtins/env.c \
	$(SRC_DIR)/execution/builtins/exit.c \
	$(SRC_DIR)/execution/builtins/pwd.c \
	$(SRC_DIR)/execution/builtins/unset.c \

# ---- Execution / Builtins / Export -------------------------------------------
SRCS += \
	$(SRC_DIR)/execution/builtins/export/export.c \
	$(SRC_DIR)/execution/builtins/export/export_list.c \
	$(SRC_DIR)/execution/builtins/export/export_parse.c \
	$(SRC_DIR)/execution/builtins/export/export_mutate.c \
	$(SRC_DIR)/execution/builtins/export/export_errors.c \

# ---- Execution / Pipeline & Executor -----------------------------------------
SRCS += \
	$(SRC_DIR)/execution/executor.c \
	$(SRC_DIR)/execution/pipes.c \
	$(SRC_DIR)/execution/pipeline.c \
	$(SRC_DIR)/execution/pipeline_wait.c \

# ---- Execution / Externals ---------------------------------------------------
SRCS += \
	$(SRC_DIR)/execution/externals/externals.c \
	$(SRC_DIR)/execution/externals/externals_utils.c \
	$(SRC_DIR)/execution/externals/external_path.c \
	$(SRC_DIR)/execution/externals/external_path_utils.c \

# ---- Expanding ---------------------------------------------------------------
SRCS += \
	$(SRC_DIR)/expanding/expander.c \
	$(SRC_DIR)/expanding/expander_utils.c \

# ---- Input (Signals) ---------------------------------------------------------
SRCS += \
	$(SRC_DIR)/input/signals.c \
	$(SRC_DIR)/input/signals_child.c \

# ---- Lexing ------------------------------------------------------------------
SRCS += \
	$(SRC_DIR)/lexing/lexer.c \
	$(SRC_DIR)/lexing/lexer_quotes.c \
	$(SRC_DIR)/lexing/lexer_tokens.c \
	$(SRC_DIR)/lexing/lexer_operators.c \
	$(SRC_DIR)/lexing/lexer_posprocess.c \

# ---- Parsing -----------------------------------------------------------------
SRCS += \
	$(SRC_DIR)/parsing/parser.c \
	$(SRC_DIR)/parsing/parser_command_list.c \
	$(SRC_DIR)/parsing/parser_command_nodes.c \
	$(SRC_DIR)/parsing/parser_argv_builder.c \

# ---- Redirections ------------------------------------------------------------
SRCS += \
	$(SRC_DIR)/redirections/heredoc.c \
	$(SRC_DIR)/redirections/redirs.c \
	$(SRC_DIR)/redirections/redirs_open.c \
	$(SRC_DIR)/redirections/redirs_apply.c \

# ---- Utils -------------------------------------------------------------------
SRCS += \
	$(SRC_DIR)/utils/io.c \
	$(SRC_DIR)/utils/env.c \
	$(SRC_DIR)/utils/fd.c \
	$(SRC_DIR)/utils/list.c \
	$(SRC_DIR)/utils/str.c \
	$(SRC_DIR)/utils/fd_close.c \
	$(SRC_DIR)/utils/str_append.c

# ---- Objects -----------------------------------------------------------------
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)

# ---- Runtime args for convenience targets -----------------------------------
ARGS ?=

# ---- Default build (règles 42, sans sanitizers) ------------------------------
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	@echo "$(CYN)\n[Link] $(NAME)$(DEF)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo "$(NEON)  ✔ $(NAME) built successfully$(DEF)"

# ---- Objects (création arborescente) ----------------------------------------
$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(Y)[CC]$<$(DEF)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# ---- Libft -------------------------------------------------------------------
libft:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(LIBFT_A): libft
	@true

# ---- Convenience -------------------------------------------------------------
run: $(NAME)
	@echo "$(G)[RUN] ./$(NAME) $(ARGS)$(DEF)"
	@./$(NAME) $(ARGS)

debug: $(LIBFT_A) $(OBJS)
	@echo "$(CYN)\n[Link] $(NAME_DEBUG)$(DEF)"
	@$(CC) $(DEBUG_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME_DEBUG)
	@echo "$(NEON)  ✔ $(NAME_DEBUG) built$(DEF)"

asan: $(LIBFT_A) $(OBJS)
	@echo "$(CYN)\n[Link] $(NAME_ASAN)$(DEF)"
	@$(CC) $(ASAN_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME_ASAN)
	@echo "$(NEON)  ✔ $(NAME_ASAN) built (AddressSanitizer)$(DEF)"

gdb: debug
	@echo "$(G)[GDB] $(NAME_DEBUG) $(ARGS)$(DEF)"
	@gdb --args ./$(NAME_DEBUG) $(ARGS)

valgrind: debug
	@echo "$(G)[VG] $(NAME_DEBUG) $(ARGS)$(DEF)"
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --errors-for-leak-kinds=all \
		--suppressions=valgrind_readline.supp --run-libc-freeres=yes \
		--num-callers=25 ./$(NAME_DEBUG) $(ARGS)

# ---- Cleanup -----------------------------------------------------------------
clean:
	@echo "$(ORANGE)[RM] objects$(DEF)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(R)[RM] $(NAME) $(NAME_DEBUG) $(NAME_ASAN) and libft.a$(DEF)"
	@rm -f $(NAME) $(NAME_DEBUG) $(NAME_ASAN)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

# ---- Phony -------------------------------------------------------------------
.PHONY: all clean fclean re libft run gdb valgrind debug asan