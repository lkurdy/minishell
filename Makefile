NAME			=	minishell

FLAGS			=	-Wall -Wextra -Werror

DIR_SRC			=	src
DIR_OBJ			=	obj
SUB_OBJ			=	obj/builtins obj/exec obj/lexer obj/parser

SRC				=	builtins/builtins_utils.c builtins/builtins.c builtins/cd.c builtins/echo.c builtins/env.c builtins/export.c builtins/init_env.c\
					exec/cmd.c exec/exec_utils.c exec/ft_split.c exec/ft_splity.c \
					lexer/get_input.c lexer/is_utils.c lexer/split_input.c \
					parser/data.c parser/init_data.c parser/parse_utils.c parser/parsing.c \
					libft.c minishell.c

OBJ				=	$(SRC:%.c=$(DIR_OBJ)/%.o)
DEP				=	$(SRC:%.c=$(DIR_OBJ)/%.d)

RED				=	$'\e[0;31m
GREEN			=	$'\e[32m
YELLOW			=	$'\e[33m
BOLD			=	$'\e[1m
UNDER			=	$'\e[4m
END				=	$'\e[0m

$(DIR_OBJ)/%.o:		$(DIR_SRC)/%.c
					mkdir -p $(DIR_OBJ) $(SUB_OBJ)
					echo "Compiling - ${YELLOW}${BOLD}${UNDER}$<${END}..."
					$(CC) $(FLAGS) -MMD -c $< -o $@
					echo "${GREEN}Done !${END}"

$(NAME):			$(OBJ)
					echo "Compiling - ${YELLOW}${BOLD}${UNDER}./minishell${END}..."
					$(CC) $(OBJ) -o $(NAME) -lreadline
					echo "${GREEN}Done !${END}"
					echo "${GREEN}${BOLD}${UNDER}Task completed successfully.${END}"

all:				$(NAME)

clean:
					echo "Deleting - ${RED}${BOLD}${UNDER}./$(DIR_OBJ)/${END}..."
					rm -rf $(DIR_OBJ)
					echo "${GREEN}Done !${END}"
					echo "${GREEN}${BOLD}${UNDER}Task completed successfully.${END}"

fclean:
					echo "Deleting - ${RED}${BOLD}${UNDER}./$(DIR_OBJ)/${END}..."
					rm -rf $(DIR_OBJ)
					echo "${GREEN}Done !${END}"
					echo "Deleting - ${RED}${BOLD}${UNDER}./minishell${END}..."
					rm -rf $(NAME)
					echo "${GREEN}Done !${END}"
					echo "${GREEN}${BOLD}${UNDER}Task completed successfully.${END}"

re:
					echo "Deleting - ${RED}${BOLD}${UNDER}./$(DIR_OBJ)/${END}..."
					rm -rf $(DIR_OBJ)
					echo "${GREEN}Done !${END}"
					echo "Deleting - ${RED}${BOLD}${UNDER}./minishell${END}..."
					rm -rf $(NAME)
					echo "${GREEN}Done !${END}"
					make --no-print-directory all

git:
					git add .
					git commit -m "$(NAME)"
					git push

-include $(DEP)

.PHONY:				all clean fclean re git
.SILENT:
