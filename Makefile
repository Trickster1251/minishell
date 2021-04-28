NAME = minishell
INCLUDES =  includes/minishell.h
SRCS	=	./gnl/get_next_line.c\
			./gnl/get_next_line_utils.c\
			./main.c \
			./execute_cmd.c \
			./ft_echo.c \
			./signals.c \
			./redirect.c \
			./ft_pwd.c \
			ft_lists_utils.c \
			./builtins_function.c \
			./builtins_func_utils.c \
				
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