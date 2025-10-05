# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eraad <eraad@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/04 18:58:38 by eraad             #+#    #+#              #
#    Updated: 2025/10/05 11:57:46 by eraad            ###   ########.fr        #
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

# ---- Sources -----------------------------------------------------------------
SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/errors/report_error.c \
	$(SRC_DIR)/errors/syntaxe_error.c \
	$(SRC_DIR)/execution/builtins/builtins.c \
	$(SRC_DIR)/execution/builtins/builtins_utils.c \
	$(SRC_DIR)/execution/builtins/cd/builtin_cd.c \
	$(SRC_DIR)/execution/builtins/echo/builtin_echo.c \
	$(SRC_DIR)/execution/builtins/env/builtin_env.c \
	$(SRC_DIR)/execution/builtins/exit/builtin_exit.c \
	$(SRC_DIR)/execution/builtins/export/builtin_export.c \
	$(SRC_DIR)/execution/builtins/export/builtin_export_utils.c \
	$(SRC_DIR)/execution/builtins/export/builtin_export_utils_1.c \
	$(SRC_DIR)/execution/builtins/pwd/builtin_pwd.c \
	$(SRC_DIR)/execution/builtins/unset/builtin_unset.c \
	$(SRC_DIR)/execution/executor.c \
	$(SRC_DIR)/execution/externals/external_path.c \
	$(SRC_DIR)/execution/externals/external_path_utils.c \
	$(SRC_DIR)/execution/externals/externals.c \
	$(SRC_DIR)/execution/externals/externals_utils.c \
	$(SRC_DIR)/execution/pipeline.c \
	$(SRC_DIR)/expanding/expander.c \
	$(SRC_DIR)/expanding/expander_utils.c \
	$(SRC_DIR)/free/free.c \
	$(SRC_DIR)/free/free_utils.c \
	$(SRC_DIR)/free/reset_command_context.c \
	$(SRC_DIR)/init/init.c \
	$(SRC_DIR)/init/init_env.c \
	$(SRC_DIR)/init/init_utils.c \
	$(SRC_DIR)/lexing/lexer.c \
	$(SRC_DIR)/lexing/lexer_operators.c \
	$(SRC_DIR)/lexing/lexer_postprocess.c \
	$(SRC_DIR)/lexing/lexer_quotes.c \
	$(SRC_DIR)/lexing/lexer_tokens.c \
	$(SRC_DIR)/parsing/parser.c \
	$(SRC_DIR)/parsing/parser_argv_builder.c \
	$(SRC_DIR)/parsing/parser_command_list.c \
	$(SRC_DIR)/parsing/parser_command_nodes.c \
	$(SRC_DIR)/parsing/parser_redirections.c \
	$(SRC_DIR)/signals/signals.c \
	$(SRC_DIR)/signals/signals_child.c \
	$(SRC_DIR)/utils/utils.c \
	$(SRC_DIR)/utils/utils_1.c

# ---- Objects -----------------------------------------------------------------
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)

# ---- Runtime args for convenience targets -----------------------------------
ARGS ?=

# ---- Phony -------------------------------------------------------------------
.PHONY: all clean fclean re libft run gdb valgrind debug asan

# ---- Default build (règles 42, sans sanitizers) ------------------------------
all: $(NAME)

$(NAME): $(LIBFT_A) $(OBJS)
	@echo "$(CYN)\n[Link] $(NAME)$(DEF)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo "$(NEON)  ✔ $(NAME) built successfully$(DEF)"

# ---- Objects (création arborescente) ----------------------------------------
$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(Y)[CC ]$<$(DEF)"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# ---- Libft -------------------------------------------------------------------
libft:
	@$(MAKE) -C $(LIBFT_DIR)

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
	@echo "$(G)[VG ] $(NAME_DEBUG) $(ARGS)$(DEF)"
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --errors-for-leak-kinds=all \
		--num-callers=25 ./$(NAME_DEBUG) $(ARGS)

# ---- Cleanup -----------------------------------------------------------------
clean:
	@echo "$(ORANGE)[RM ] objects$(DEF)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(R)[RM ] $(NAME) $(NAME_DEBUG) $(NAME_ASAN) and libft.a$(DEF)"
	@rm -f $(NAME) $(NAME_DEBUG) $(NAME_ASAN)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
