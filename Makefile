NAME = minishell
INCLUDES =  includes/minishell.h
SRCS	=	./gnl/get_next_line.c \
			./gnl/get_next_line_utils.c \
			./main.c \
			./execute_cmd.c \
			./signals.c \
			./redirect.c \
			./history.c \
			./init.c \
			./key_events.c \
			./read_line.c \
			./parser.c \
			./parser2.c \
			./lexer.c \
			./lexer2.c \
			./tokenize.c \
			./tokenize2.c \
			./lexer3.c \
			./parser_utils.c \
			./history_print.c \
			./builtins_function/ft_cd.c \
			./builtins_function/ft_pwd.c \
			./builtins_function/ft_export.c \
			./builtins_function/ft_env.c \
			./builtins_function/ft_echo.c \
			./builtins_function/ft_exit.c \
			./builtins_function/ft_unset.c \
			./ft_pipes.c \
			./builtins_function/exec_builtins_func.c \
			./lists_utils.c \
			./lists_utils1.c \
			./func_utils.c \
				
FLAGS	=	-Wall -Wextra -Werror
LIBFT	=	 -L./libft -lft
OBJS	= $(SRCS:.c=.o)
BONUS_OBJS	= $(BONUS_SRCS:.c=.o)

all:	$(NAME)

$(NAME): $(OBJS) $(INCLUDES)
		@echo "------------>Please wait, minishell compile...<------------"
		@make bonus -C ./libft;
		@gcc -g -o minishell $(SRCS) $(MAIN_SRCS) $(LIBFT) -ltermcap

clean:	
		@make clean -C ./libft
		@rm -rf $(OBJS)
		@echo "------------>Delete all .o files<------------>"

fclean:	clean
		@make fclean -C./libft
		@rm -f $(NAME)
		@echo "------------>Delete all .o files and ./minishell<------------>"
re:		fclean $(NAME) all

.PHONY:		all clean fclean re