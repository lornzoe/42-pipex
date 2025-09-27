# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/27 15:41:46 by lyanga            #+#    #+#              #
#    Updated: 2025/09/27 15:49:26 by lyanga           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Executable name and output directory
NAME        =   pipex
OUTDIR      =   .

# Compiler and flags
CC          =   cc
CFLAGS      =   -Wall -Werror -Wextra -std=gnu89
DEBUG_FLAGS =   -g -O0
RM          =   rm -f

# Source files and objects
SRCS        =   main.c \
OBJS        =   $(SRCS:.c=.o)

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

# Compile C files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(LIBFT_DIR)/headers

# Build libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Clean object files
clean:
	$(RM) $(OBJS)
	make clean -C $(LIBFT_DIR)

# Full clean (removes executable and object files)
fclean: clean
	$(RM) $(NAME)
	make fclean -C $(LIBFT_DIR)

# Rebuild the project
re: fclean all

# Phony targets to prevent conflicts with files of the same name
.PHONY: pipex all clean fclean re libft