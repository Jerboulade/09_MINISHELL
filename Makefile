# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/02 22:28:18 by jcarere           #+#    #+#              #
#    Updated: 2022/06/16 16:00:45 by jcarere          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CHECKNAME = checker
CC = gcc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
RLINCL = -I/usr/local/Cellar/readline/8.1.2/include
RLLIB = -L/usr/local/Cellar/readline/8.1.2/lib -lreadline
SRCDIR = ./src/
OBJDIR = ./obj/
INCL = ./includes/
LIBDIR = ./libft/
LIB = $(LIBDIR)libft.a
SRC =	main.c minishell.c parsing.c tokenizer.c parsing_utils.c print.c free.c
OBJ = $(addprefix $(OBJDIR), $(SRC:%.c=%.o))
REMOVE = /bin/rm -rf
C_CYAN = \033[1;96m
C_GREEN = \033[1;32m
C_MAG = \033[1;35m
C_RED = \033[0;31m
C_ORANGE = \033[0;33m
C_RESET = \033[0m
L_CLEAR = \033[K

$(OBJDIR)%.o: $(SRCDIR)%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INCL) $(RLINCL)
	@printf "$(C_MAG)Compiling 'minishell':         \
	$(C_CYAN)[$(C_ORANGE)$<$(C_CYAN)] $(C_RESET) $(L_CLEAR)\r"

$(NAME): $(OBJDIR) $(OBJ)
	@printf "$(L_CLEAR)\r"
	@make -s -C $(LIBDIR)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIB) $(RLLIB)
	@printf "$(C_CYAN)'minishell': $(C_GREEN)ready             \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)"
	@printf "$(L_CLEAR)\n"

$(OBJDIR):
	@mkdir -p $(OBJDIR)

all: $(NAME)

bonus: $(CHECKNAME)

clean:
	@make -C $(LIBDIR) clean
	@$(REMOVE) $(OBJDIR)
	@printf "$(C_CYAN)'minishell': $(C_RED)objects deleted   \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"

fclean: clean
	@$(REMOVE) $(LIB)
	@printf "$(C_CYAN)'libft.a'  : $(C_RED)deleted           \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"
	@$(REMOVE) $(NAME)
	@printf "$(C_CYAN)'minishell': $(C_RED)deleted           \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"
	@$(REMOVE) $(CHECKNAME)
	@printf "$(C_CYAN)'checker'  : $(C_RED)deleted           \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"
re: fclean all

.PHONY: all clean fclean re bonus
