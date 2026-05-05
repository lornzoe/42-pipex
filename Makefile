# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/27 15:41:46 by lyanga            #+#    #+#              #
#    Updated: 2026/05/05 20:07:35 by lyanga           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Executable name and output directory
NAME        =   pipex
BONUS_NAME  =   pipex_bonus
OUTDIR      =   .

# Compiler and flags
CC          =   cc
CFLAGS      =   -Wall -Werror -Wextra -g -O0
DEBUG_FLAGS =   -g -O0
RM          =   rm -f

ifeq ($(DEBUG),1)
    CFLAGS += $(DEBUG_FLAGS)
endif

# Source files and objects
SRCS        =   main.c parse.c path.c utils.c ft_realloc.c \
				cmd_split.c cmd_split_utils.c exec_chain.c exec_chain_2.c				
OBJS        =   $(SRCS:.c=.o)

BONUS_SRCS  =   main_bonus.c parse_bonus.c path_bonus.c ft_realloc_bonus.c \
				cmd_split_bonus.c cmd_split_utils_bonus.c exec_chain_bonus.c \
				exec_chain_2_bonus.c utils_bonus.c heredoc_bonus.c
BONUS_OBJS  =   $(BONUS_SRCS:.c=.o)

# Directories for external libraries
LIBFT_DIR   =   libft

# Library names and flags
LIBFT       =   $(LIBFT_DIR)/libft.a
LIBFT_FLAGS =   -L$(LIBFT_DIR) -lft

# Default target
all: $(NAME)

# Link the executable
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_FLAGS) -o $(NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT_FLAGS) -o $(BONUS_NAME)

# Compile C files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(LIBFT_DIR)/headers

# Build libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Clean object files
clean:
	$(RM) $(OBJS)
	$(RM) $(BONUS_OBJS)
	make clean -C $(LIBFT_DIR)

# Full clean (removes executable and object files)
fclean: clean
	$(RM) $(NAME)
	$(RM) $(BONUS_NAME)
	make fclean -C $(LIBFT_DIR)

# Rebuild the project
re: fclean all

bonus: $(BONUS_NAME)

# Phony targets to prevent conflicts with files of the same name
.PHONY: all clean fclean re libft bonus