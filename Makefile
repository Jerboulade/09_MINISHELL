# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jcarere <jcarere@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/02 22:28:18 by jcarere           #+#    #+#              #
#    Updated: 2022/07/15 14:06:23 by jcarere          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address
RLINCL = -I/usr/local/Cellar/readline/8.1.2/include
RLLIB = -L/usr/local/Cellar/readline/8.1.2/lib -lreadline
SRCDIR = ./src/
OBJDIR = ./obj/
MVOBJ =
DATADIR = ./data/
HISTORYFILE = $(DATADIR)history.log
INCL = ./includes/
LIBDIR = ./libft/
LIB = $(LIBDIR)libft.a
MAIN_DIR = ./src/main/
ENV_DIR = ./src/env/
PARS_DIR = ./src/parsing/parser/
TOK_DIR = ./src/parsing/token/
EXP_DIR = ./src/parsing/expansion/
LEX_DIR = ./src/parsing/lexer/
EXEC_DIR = ./src/exec/
BTIN_DIR = ./src/builtin/
HIST_DIR = ./src/history/
TOOL_DIR = ./src/tool/
FREE_DIR = ./src/free/
SRC =	$(MAIN_DIR)main.c $(MAIN_DIR)minishell.c $(MAIN_DIR)init.c \
		$(ENV_DIR)env_update.c $(ENV_DIR)env_utils.c $(ENV_DIR)env_utils2.c \
		$(PARS_DIR)parsing.c $(PARS_DIR)parsing_utils.c \
		$(TOK_DIR)tokenizer.c $(TOK_DIR)token_pop.c $(TOK_DIR)token_utils.c \
		$(EXP_DIR)expand.c $(EXP_DIR)expand_utils.c \
		$(LEX_DIR)lexer.c $(LEX_DIR)lexer_path_utils.c \
		$(EXEC_DIR)executor.c $(EXEC_DIR)handle_pipe.c $(EXEC_DIR)handle_redir.c \
		$(BTIN_DIR)msh_cd.c $(BTIN_DIR)msh_echo.c $(BTIN_DIR)msh_env.c \
		$(BTIN_DIR)msh_exit.c $(BTIN_DIR)msh_export.c $(BTIN_DIR)msh_pwd.c \
		$(BTIN_DIR)msh_unset.c \
 		$(HIST_DIR)history.c \
		$(TOOL_DIR)is_check.c $(TOOL_DIR)is_check2.c $(TOOL_DIR)is_check3.c $(TOOL_DIR)print.c \
		$(FREE_DIR)free.c $(FREE_DIR)free_utils.c
OBJ = $(SRC:%.c=%.o)
REMOVE = /bin/rm -rf
C_CYAN = \033[1;96m
C_GREEN = \033[1;32m
C_MAG = \033[1;35m
C_RED = \033[0;31m
C_ORANGE = \033[0;33m
C_RESET = \033[0m
L_CLEAR = \033[K

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@ -I $(INCL) $(RLINCL)
	@printf "$(C_MAG)Compiling 'minishell':         \
	$(C_CYAN)[$(C_ORANGE)$<$(C_CYAN)] $(C_RESET) $(L_CLEAR)\r"

$(NAME): $(DATADIR) $(OBJ)
	@printf "$(L_CLEAR)\r"
	@make -s -C $(LIBDIR)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIB) $(RLLIB)
	@printf "$(C_CYAN)'minishell': $(C_GREEN)ready             \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)"
	@printf "$(L_CLEAR)\n"

$(DATADIR):
	@mkdir -p $(DATADIR)

all: $(NAME)

clear_history:
	@$(REMOVE) $(HISTORYFILE)
	@printf "$(C_CYAN)'minishell': $(C_RED)history cleared   \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"

clean:
	@make -C $(LIBDIR) clean
	@$(REMOVE) $(OBJ)
	@printf "$(C_CYAN)'minishell': $(C_RED)objects deleted   \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"

fclean: clean
	@$(REMOVE) $(LIB)
	@$(REMOVE) $(DATADIR)
	@printf "$(C_CYAN)'libft.a'  : $(C_RED)deleted           \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"
	@$(REMOVE) $(NAME)
	@printf "$(C_CYAN)'minishell': $(C_RED)deleted           \
	$(C_CYAN)[$(C_GREEN)✔$(C_CYAN)]$(C_RESET)\n"

re: fclean all

.PHONY: all clean fclean re clear_history
