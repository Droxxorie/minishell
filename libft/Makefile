# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eraad <eraad@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/15 12:06:42 by eraad             #+#    #+#              #
#    Updated: 2025/10/09 19:09:39 by eraad            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#* ==============================================================================
#*                                   BINARY
#* ==============================================================================
NAME        := libft.a

#* ==============================================================================
#*                                   TOOLCHAIN
#* ==============================================================================
CC          := cc
AR          := ar rcs
CFLAGS      := -Wall -Wextra -Werror -MMD -MP
INCLUDES    := -Iinclude

#* ==============================================================================
#*                                   COLORS/SYMBOLS/STYLES
#* ==============================================================================

# NORMAL = 0
# BOLD = 1
# LOW INTENSITY = 2
# ITALIC = 3
# UNDERLINED = 4
# BLINKING = 5
# INVERTED/UNDERLINE = 7
# HIDDEN = 8
# CROSSED OUT = 9

RESET   := \033[0m
BLACK   := \033[0;30m
RED     := \033[0;31m
GREEN   := \033[1;32m
YELLOW  := \033[0;33m
BLUE    := \033[0;34m
PURPLE  := \033[1;35m
CYAN    := \033[1;36m
WHITE   := \033[0;37m

OK      := ✓
KO      := ✗
INFO    := ⓘ

#* ==============================================================================
#*                                   LAYOUT
#* ==============================================================================
SRC_DIR         := sources
OBJ_DIR         := objects

#* ==============================================================================
#*                                   SOURCES
#* ==============================================================================
SRCS := \
	$(SRC_DIR)/ft_is/ft_isalnum.c \
	$(SRC_DIR)/ft_is/ft_isalpha.c \
	$(SRC_DIR)/ft_is/ft_isascii.c \
	$(SRC_DIR)/ft_is/ft_isdigit.c \
	$(SRC_DIR)/ft_is/ft_isprint.c \
	$(SRC_DIR)/ft_is/ft_iswhitespace.c \

SRCS += \
	$(SRC_DIR)/ft_lst/ft_lstadd_back.c \
	$(SRC_DIR)/ft_lst/ft_lstadd_front.c \
	$(SRC_DIR)/ft_lst/ft_lstclear.c \
	$(SRC_DIR)/ft_lst/ft_lstdelone.c \
	$(SRC_DIR)/ft_lst/ft_lstiter.c \
	$(SRC_DIR)/ft_lst/ft_lstlast.c \
	$(SRC_DIR)/ft_lst/ft_lstmap.c \
	$(SRC_DIR)/ft_lst/ft_lstnew.c \
	$(SRC_DIR)/ft_lst/ft_lstsize.c \

SRCS += \
	$(SRC_DIR)/ft_mem/ft_bzero.c \
	$(SRC_DIR)/ft_mem/ft_calloc.c \
	$(SRC_DIR)/ft_mem/ft_memchr.c \
	$(SRC_DIR)/ft_mem/ft_memcmp.c \
	$(SRC_DIR)/ft_mem/ft_memcpy.c \
	$(SRC_DIR)/ft_mem/ft_memmove.c \
	$(SRC_DIR)/ft_mem/ft_memset.c \
	$(SRC_DIR)/ft_mem/ft_realloc.c \

SRCS += \
	$(SRC_DIR)/ft_put/ft_putchar_fd.c \
	$(SRC_DIR)/ft_put/ft_putendl_fd.c \
	$(SRC_DIR)/ft_put/ft_putnbr_fd.c \
	$(SRC_DIR)/ft_put/ft_putstr_fd.c \

SRCS += \
	$(SRC_DIR)/ft_str/ft_strchr.c \
	$(SRC_DIR)/ft_str/ft_strdup.c \
	$(SRC_DIR)/ft_str/ft_striteri.c \
	$(SRC_DIR)/ft_str/ft_strjoin.c \
	$(SRC_DIR)/ft_str/ft_strlcat.c \
	$(SRC_DIR)/ft_str/ft_strlcpy.c \
	$(SRC_DIR)/ft_str/ft_strlen.c \
	$(SRC_DIR)/ft_str/ft_strmapi.c \
	$(SRC_DIR)/ft_str/ft_strncmp.c \
	$(SRC_DIR)/ft_str/ft_strnstr.c \
	$(SRC_DIR)/ft_str/ft_strrchr.c \
	$(SRC_DIR)/ft_str/ft_strtrim.c \
	$(SRC_DIR)/ft_str/ft_substr.c \
	
SRCS += \
	$(SRC_DIR)/gnl/get_next_line.c \

SRCS += \
	$(SRC_DIR)/maths/ft_abs.c \
	$(SRC_DIR)/maths/ft_fabs.c \
	$(SRC_DIR)/maths/ft_pow.c \
	$(SRC_DIR)/maths/ft_argmax.c \
	$(SRC_DIR)/maths/ft_max.c \
	$(SRC_DIR)/maths/ft_min.c \
	$(SRC_DIR)/maths/ft_argmin.c \

SRCS += \
	$(SRC_DIR)/printf/ft_printaddr_fd.c \
	$(SRC_DIR)/printf/ft_printchar_fd.c \
	$(SRC_DIR)/printf/ft_printdbl.c \
	$(SRC_DIR)/printf/ft_printf.c \
	$(SRC_DIR)/printf/ft_printhex_fd.c \
	$(SRC_DIR)/printf/ft_printnbr_fd.c \
	$(SRC_DIR)/printf/ft_printstr_fd.c \
	$(SRC_DIR)/printf/ft_printunbr_fd.c \
	
SRCS += \
	$(SRC_DIR)/utils/ft_atodbl.c \
	$(SRC_DIR)/utils/ft_atoi.c \
	$(SRC_DIR)/utils/ft_count_words.c \
	$(SRC_DIR)/utils/ft_dbltoa.c \
	$(SRC_DIR)/utils/ft_intlen.c \
	$(SRC_DIR)/utils/ft_itoa.c \
	$(SRC_DIR)/utils/ft_split.c \
	$(SRC_DIR)/utils/ft_tolower.c \
	$(SRC_DIR)/utils/ft_toupper.c \
	$(SRC_DIR)/utils/ft_free_str_array.c \
	$(SRC_DIR)/utils/ft_free_2d_str_array.c \

OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)

DEPS      := $(OBJS:.o=.d)
SRC_COUNT := $(words $(SRCS))

#* ==============================================================================
#*                                   RULES
#* ==============================================================================
all: $(NAME)

$(NAME): entry_message $(OBJS)
	@echo "${GREEN} DONE${RESET}"
	@$(AR) $@ $(OBJS)
	@echo "$(PURPLE)${NAME} created\n$(RESET)"

$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@printf "$(GREEN)$(OK)$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

entry_message:
	@echo "$(GREEN)Compiling ${NAME}$(RESET)"
	@echo "$(YELLOW)$(INFO) ${SRC_COUNT} files$(RESET)"

#* ==============================================================================
#*                                   DEPENDENCIES
#* ==============================================================================
-include $(DEPS)

.PHONY: all clean fclean re entry_message
