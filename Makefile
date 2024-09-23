# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/30 17:25:14 by bschneid          #+#    #+#              #
#    Updated: 2024/09/19 17:01:23 by bschneid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
HEADER = header/minishell.h
LIBFT = src/0_Libft_extended
SRC_DIR = src
BI = src/1_builtins
ST = src/2_syntax_tree
PA = src/3_parsing
EX = src/4_execution
OT = src/5_others
OBJ_DIR = obj
SOURCES = $(BI)/cd.c $(BI)/echo.c $(BI)/env.c $(BI)/exit.c $(BI)/export.c\
	$(BI)/linkedlist_utils.c $(BI)/pwd.c $(BI)/unset.c $(BI)/utils.c\
	$(BI)/var_init.c $(BI)/variables.c\
	$(ST)/ast.c $(ST)/tokens_help.c $(ST)/tokens.c\
	$(PA)/helpers.c $(PA)/heredoc.c $(PA)/parsing.c $(PA)/redirections.c\
	$(EX)/exec_help.c $(EX)/exec.c $(EX)/split_cmd.c \
	$(EX)/var_expand.c $(EX)/var_helpers.c $(EX)/waiting.c $(EX)/wildcard.c\
	$(OT)/debug_funcs.c $(OT)/error_memory.c $(OT)/init.c $(OT)/signals.c\
	$(OT)/proof.c\
	src/main.c
	 
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
	FLAGS = -Wall -Werror -Wextra -g3

all:	$(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	cc $(FLAGS) -c $< -o $@

$(NAME): $(OBJECTS) $(HEADER) $(LIBFT)/libft.a
	cc $(FLAGS) $(OBJECTS) -o $@ -lreadline -L./$(LIBFT) -lft

$(LIBFT)/libft.a:
	$(MAKE) -C $(LIBFT) -f Makefile bonus
	$(MAKE) -C $(LIBFT) -f Makefile clean

clean:
	rm -rf $(OBJ_DIR)

bonus:	all

fclean:	clean
	rm -f $(NAME)
	rm -f $(LIBFT)/libft.a

re:	fclean all

.PHONY:	all bonus clean fclean re
