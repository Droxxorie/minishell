# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eraad <eraad@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/04 18:58:38 by eraad             #+#    #+#              #
#    Updated: 2025/10/11 15:59:24 by eraad            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#* ==============================================================================
#*                                   BINARY
#* ==============================================================================
NAME            := minishell

#* ==============================================================================
#*                                   TOOLCHAIN
#* ==============================================================================
CC              := cc
CFLAGS          := -Wall -Wextra -Werror -g3
CPPFLAGS        := -Iincludes -Ilibft/include
LDFLAGS         := -Llibft -lft -lreadline

NAME_DEBUG      := $(NAME)_debug
NAME_ASAN       := $(NAME)_asan
DEBUG_FLAGS     := -O0 -g3 -DDEBUG
ASAN_FLAGS      := -O0 -g3 -fsanitize=address -fno-omit-frame-pointer

ARGS			:=

#* ==============================================================================
#*                                   COLORS/SYMBOLS/STYLES
#* ==============================================================================
#* Attributs
RESET        := \033[0m
BOLD         := \033[1m
DIM          := \033[2m
ITALIC       := \033[3m
UNDERLINE    := \033[4m
INVERT       := \033[7m

#* Palette
BLACK        := \033[30m
RED          := \033[31m
GREEN        := \033[32m
YELLOW       := \033[33m
BLUE         := \033[34m
MAGENTA      := \033[35m
CYAN         := \033[36m
WHITE        := \033[37m

BRIGHT_BLACK   := \033[90m
BRIGHT_RED     := \033[91m
BRIGHT_GREEN   := \033[92m
BRIGHT_YELLOW  := \033[93m
BRIGHT_BLUE    := \033[94m
BRIGHT_MAGENTA := \033[95m
BRIGHT_CYAN    := \033[96m
BRIGHT_WHITE   := \033[97m

ROSE         := \033[38;5;216m
ORANGE       := \033[38;5;208m

OK      := ✓
KO      := ✗
INFO    := ⓘ

#* ==============================================================================
#*                                   LAYOUT
#* ==============================================================================
SRC_DIR         := sources
OBJ_DIR         := obj

LIBFT_DIR       := libft
LIBFT_A         := $(LIBFT_DIR)/libft.a

#* ==============================================================================
#*                                   SOURCES
#* ==============================================================================

#* ---- Core ----
SRCS := \
	$(SRC_DIR)/core/loop/main.c \
	$(SRC_DIR)/core/init/init.c \
	$(SRC_DIR)/core/init/env_copy.c \
	$(SRC_DIR)/core/init/env_list.c \
	$(SRC_DIR)/core/init/env_vars.c \
	$(SRC_DIR)/core/cleanup/reset.c \
	$(SRC_DIR)/core/cleanup/free_env.c \
	$(SRC_DIR)/core/cleanup/free_pipes.c \
	$(SRC_DIR)/core/loop/exit_minishell.c \
	$(SRC_DIR)/core/cleanup/free_tokens.c \
	$(SRC_DIR)/core/loop/launch_minishell.c \
	$(SRC_DIR)/core/cleanup/free_commands.c \

#* ---- Errors ----
SRCS += \
	$(SRC_DIR)/errors/error.c \
	$(SRC_DIR)/errors/syntax.c \

#* ---- Execution / Builtins ----
SRCS += \
	$(SRC_DIR)/execution/builtins/cd.c \
	$(SRC_DIR)/execution/builtins/env.c \
	$(SRC_DIR)/execution/builtins/pwd.c \
	$(SRC_DIR)/execution/builtins/echo.c \
	$(SRC_DIR)/execution/builtins/exit.c \
	$(SRC_DIR)/execution/builtins/unset.c \
	$(SRC_DIR)/execution/builtins/builtins.c \

#* ---- Execution / Builtins / Export ----
SRCS += \
	$(SRC_DIR)/execution/builtins/export/export.c \
	$(SRC_DIR)/execution/builtins/export/export_list.c \
	$(SRC_DIR)/execution/builtins/export/export_parse.c \
	$(SRC_DIR)/execution/builtins/export/export_mutate.c \
	$(SRC_DIR)/execution/builtins/export/export_errors.c \
	$(SRC_DIR)/execution/builtins/export/export_mutate_utils.c \

#* ---- Execution / Pipeline & Executor ----
SRCS += \
	$(SRC_DIR)/execution/executor.c \
	$(SRC_DIR)/execution/pipes.c \
	$(SRC_DIR)/execution/pipeline.c \
	$(SRC_DIR)/execution/pipeline_wait.c \

#* ---- Execution / Externals ----
SRCS += \
	$(SRC_DIR)/execution/externals/externals.c \
	$(SRC_DIR)/execution/externals/external_path.c \
	$(SRC_DIR)/execution/externals/externals_utils.c \
	$(SRC_DIR)/execution/externals/external_path_utils.c \

#* ---- Expanding ----
SRCS += \
	$(SRC_DIR)/expanding/expander.c \
	$(SRC_DIR)/expanding/expander_utils.c \

#* ---- Input (Signals) ----
SRCS += \
	$(SRC_DIR)/input/signals.c \
	$(SRC_DIR)/input/signals_utils.c \
	$(SRC_DIR)/input/signals_child.c \

#* ---- Lexing ----
SRCS += \
	$(SRC_DIR)/lexing/lexer.c \
	$(SRC_DIR)/lexing/lexer_quotes.c \
	$(SRC_DIR)/lexing/lexer_tokens.c \
	$(SRC_DIR)/lexing/lexer_operators.c \
	$(SRC_DIR)/lexing/lexer_posprocess.c \

#* ---- Parsing ----
SRCS += \
	$(SRC_DIR)/parsing/parser.c \
	$(SRC_DIR)/parsing/parser_utils.c \
	$(SRC_DIR)/parsing/parser_argv_builder.c \
	$(SRC_DIR)/parsing/parser_command_list.c \
	$(SRC_DIR)/parsing/parser_command_nodes.c \

#* ---- Redirections ----
SRCS += \
	$(SRC_DIR)/redirections/redirs.c \
	$(SRC_DIR)/redirections/heredoc.c \
	$(SRC_DIR)/redirections/redirs_open.c \
	$(SRC_DIR)/redirections/redirs_apply.c \
	$(SRC_DIR)/redirections/heredoc_utils.c \
	$(SRC_DIR)/redirections/redirs_apply_utils.c \

#* ---- Utils ----
SRCS += \
	$(SRC_DIR)/utils/io.c \
	$(SRC_DIR)/utils/env.c \
	$(SRC_DIR)/utils/fd.c \
	$(SRC_DIR)/utils/list.c \
	$(SRC_DIR)/utils/str.c \
	$(SRC_DIR)/utils/fd_close.c \
	$(SRC_DIR)/utils/str_append.c



OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)

SRC_COUNT := $(words $(SRCS))

#* ==============================================================================
#*                                   RULES
#* ==============================================================================
all: $(NAME) project_logo

$(NAME): $(LIBFT_A) entry_message $(OBJS)
	@echo "${GREEN}${BOLD} DONE${RESET}"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	@echo "$(MAGENTA)${BOLD}${NAME} created\n$(RESET)"


#* ---- Objects ----
$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
	@printf "$(GREEN)${BOLD}$(OK)$(RESET)"


#* ---- Libft ----
libft:
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(LIBFT_A): libft
	@true

#* ---- Convenience ----
run: $(NAME)
	@echo "$(YELLOW)${BOLD}[RUN] ./$(NAME) $(ARGS)$(RESET)"
	@-./$(NAME) $(ARGS)

debug: $(LIBFT_A) debug_message $(OBJS)
	@echo "${GREEN}${BOLD} DONE${RESET}"
	@$(CC) $(DEBUG_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME_DEBUG)
	@echo "$(MAGENTA)${BOLD}${NAME_DEBUG} created\n$(RESET)"

asan: $(LIBFT_A) asan_message $(OBJS)
	@echo "${GREEN}${BOLD} DONE${RESET}"
	@$(CC) $(ASAN_FLAGS) $(OBJS) $(LDFLAGS) -o $(NAME_ASAN)
	@echo "$(MAGENTA)${BOLD}${NAME_ASAN} created\n$(RESET)"

gdb: debug
	@echo "$(YELLOW)${BOLD}[GDB] ./$(NAME_DEBUG) $()$(RESET)"
	@gdb --args ./$(NAME_DEBUG) $(ARGS)

valgrind: debug
	@echo "$(YELLOW)${BOLD}[VG] ./$(NAME_DEBUG) $(ARGS)$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all \
		--track-origins=yes --errors-for-leak-kinds=all \
		--suppressions=valgrind_readline.supp --run-libc-freeres=yes \
		--num-callers=25 ./$(NAME_DEBUG) $(ARGS) || true

#* ---- Cleanup ----
clean:
	@echo "$(RED)[RM] objects$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)[RM] $(NAME)$(RESET)"
	@echo "$(RED)[RM] $(NAME_DEBUG)$(RESET)"
	@echo "$(RED)[RM] $(NAME_ASAN)$(RESET)"
	@echo "$(RED)[RM] libft.a$(RESET)\n"
	@rm -f $(NAME) $(NAME_DEBUG) $(NAME_ASAN)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re: fclean all

help:
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "$(BRIGHT_MAGENTA)all       →  ${RESET}Compile the program."
	@echo "$(BRIGHT_MAGENTA)clean     →  ${RESET}Removes temporary files."
	@echo "$(BRIGHT_MAGENTA)fclean    →  ${RESET}Deletes all generated files."
	@echo "$(BRIGHT_MAGENTA)re        →  ${RESET}Rebuilds the project."
	@echo "$(BRIGHT_MAGENTA)run       →  ${RESET}Compile and executes the program."
	@echo "$(BRIGHT_MAGENTA)valgrind  →  ${RESET}Check for memory leaks."
	@echo "$(BRIGHT_MAGENTA)debug     →  ${RESET}Compile with debug flags."
	@echo "$(BRIGHT_MAGENTA)asan      →  ${RESET}Compile with address sanitizer."
	@echo "$(BRIGHT_MAGENTA)gdb       →  ${RESET}Compile with debug flags and launch gdb."
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

	
#* ---- Messages ----
entry_message:
	@echo "$(GREEN)${BOLD}Compiling ${NAME}$(RESET)"
	@echo "$(YELLOW)$(INFO) ${SRC_COUNT} files$(RESET)"

debug_message:
	@echo "$(GREEN)${BOLD}Compiling ${NAME_DEBUG}$(RESET)"
	@echo "$(YELLOW)$(INFO) ${SRC_COUNT} files$(RESET)"

asan_message:
	@echo "$(GREEN)${BOLD}Compiling ${NAME_ASAN}$(RESET)"
	@echo "$(YELLOW)$(INFO) ${SRC_COUNT} files$(RESET)"

project_logo:
# 	@echo ""
# 	@echo "${CYAN}                        M  I  N  I  S  H  E  L  L                           ${RESET}"
# 	@echo        "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
# 	@echo "${ROSE}   ┏━┓     ┏━┓  ┏┓  ┏━┓   ┏━┓  ┏┓  ┏━━━━┓ ┏━┓  ┏━┓ ┏━━━━┓ ┏━┓     ┏━┓       ${RESET}"
# 	@echo "${ROSE}   ┃ ┣┓   ┏┫ ┃  ┃┃  ┃ ┣┓  ┃ ┃  ┃┃  ┃ ┏━━┛ ┃ ┃  ┃ ┃ ┃ ┏━━┛ ┃ ┃     ┃ ┃       ${RESET}"
# 	@echo "${ROSE}   ┃ ┃┣┓┃┏┫┃ ┃  ┃┃  ┃ ┃┃┃┏┫ ┃  ┃┃  ┃ ┗━━┓ ┃ ┣┓┏┫ ┃ ┃ ┗━┓  ┃ ┃     ┃ ┃       ${RESET}"
# 	@echo "${ROSE}   ┃ ┃┣┛┃┗┫┃ ┃  ┃┃  ┃ ┣┛┃┃┃ ┃  ┃┃  ┗━━┓ ┃ ┃ ┣┛┗┫ ┃ ┃ ┏━┛  ┃ ┃     ┃ ┃       ${RESET}"
# 	@echo "${ROSE}   ┃ ┣┛ ┃ ┗┫ ┃  ┃┃  ┃ ┃  ┗┫ ┃  ┃┃  ┏━━┛ ┃ ┃ ┃  ┃ ┃ ┃ ┗━━┓ ┃ ┗━━━┓ ┃ ┗━━━┓   ${RESET}"
# 	@echo "${ROSE}   ┗━┛     ┗━┛  ┗┛  ┗━┛   ┗━┛  ┗┛  ┗━━━━┛ ┗━┛  ┗━┛ ┗━━━━┛ ┗━━━━━┛ ┗━━━━━┛   ${RESET}"
# 	@echo        "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
# 	@echo "${CYAN}                                          by RAAD Eloi & BOIDIN Gaspard ${RESET}"
# 	@echo ""
	@echo ""
	@echo "${CYAN}${BOLD}                       M  I  N  I  S  H  E  L  L                        ${RESET}"
	@echo        "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "${WHITE}   ┏━┓    ┏━┓  ┏┓  ┏━┓  ┏━┓  ┏┓  ┏━━━━┓ ┏━┓  ┏━┓ ┏━━━━┓ ┏━┓     ┏━┓     ${RESET}"
	@echo "${WHITE}   ┃ ┣┓  ┏┫ ┃  ┃┃  ┃ ┣┓ ┃ ┃  ┃┃  ┃ ┏━━┛ ┃ ┃  ┃ ┃ ┃ ┏━━┛ ┃ ┃     ┃ ┃     ${RESET}"
	@echo "${WHITE}   ┃ ┃┣┓┏┫┃ ┃  ┃┃  ┃ ┃┃┏┫ ┃  ┃┃  ┃ ┗━━┓ ┃ ┣┓┏┫ ┃ ┃ ┗━┓  ┃ ┃     ┃ ┃     ${RESET}"
	@echo "${WHITE}   ┃ ┃┣┛┗┫┃ ┃  ┃┃  ┃ ┣┛┃┃ ┃  ┃┃  ┗━━┓ ┃ ┃ ┣┛┗┫ ┃ ┃ ┏━┛  ┃ ┃     ┃ ┃     ${RESET}"
	@echo "${WHITE}   ┃ ┣┛  ┗┫ ┃  ┃┃  ┃ ┃ ┗┫ ┃  ┃┃  ┏━━┛ ┃ ┃ ┃  ┃ ┃ ┃ ┗━━┓ ┃ ┗━━━┓ ┃ ┗━━━┓ ${RESET}"
	@echo "${WHITE}   ┗━┛    ┗━┛  ┗┛  ┗━┛  ┗━┛  ┗┛  ┗━━━━┛ ┗━┛  ┗━┛ ┗━━━━┛ ┗━━━━━┛ ┗━━━━━┛ ${RESET}"
	@echo        "  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "${CYAN}${BOLD}                                        by RAAD Eloi & BOIDIN Gaspard ${RESET}"
	@echo ""



#* ---- Phony -------------------------------------------------------------------
.PHONY: all clean fclean re libft run gdb valgrind debug asan