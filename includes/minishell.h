/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/04 14:16:50 by aemustaf          #+#    #+#             */
/*   Updated: 2021/04/04 14:16:52 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHEL_H
# define MINISHEL_H

# include "../libft/libft.h"
# include "../gnl/get_next_line.h"
# include <curses.h>
# include <term.h>
# include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <signal.h>

/*              fd[0] - stdin
				fd[1] - stdout
				args[0] - command name
				command->child - next command after pipe (if it exist)
*/
typedef struct s_command
{
	char **argv;
	int fd[2];
	int count_redir;
} t_cmd;

typedef struct s_env
{
	char *key;
	char *value;
} t_env;

typedef	struct s_line
{
	char	*str;
	int 	len;
	int		pos;

} t_line;

typedef struct s_valid
{
	int in_dqt;
	int	in_qt;
	int pipe;
}t_valid;

typedef struct s_all
{
	char **env_strs;
	t_list *envp;
	//
	t_list *exp;
	t_cmd *cmds;
	char **history;
	int pos;
	int hist_len;
	t_list *hist_list;
	struct termios term;
	char *term_name;
	int cmds_num;
	// Нужно инициализировать массив пайпов
	int   *fd[2];
	t_line *src;
	t_valid val;
	
} t_all;


// Функции логики

//Глобальная переменная
int		gl_fd[4];

void     execute_cmd(t_all *all);
void    ft_cd(t_cmd *cmd, t_list *lst);
void    ft_echo(t_cmd *cmd);
void    ft_pwd(t_cmd *cmd);
void    ft_env(t_cmd *cmd, t_list *envp);
void    ft_export(t_cmd *cmd, t_list *envp, t_list *exp);
void    ft_unset(t_cmd *cmd, t_list *envp, t_list *exp);
void    ft_exit(t_cmd *cmd);

//Инициализация
void    init_shlvl(t_list *envp, t_list *exp);

//Мелкие полезные функции не забыть добавить в либу
int     ft_isdigit_str(char *str);
int     is_delim(char *line);

// Функции для работы с листами
int     len_arr(char **str);
int     is_max(int n, int m);
int     is_min(int n, int m);
t_list *parse_env(char **envp);
char	**ft_sort(char **env, int size);
//Печать env export
void    print_lst(t_list *lst);
void    print_env(t_list *lst);
void    print_export(t_list **lst);

//Редиректы
int    ft_redir(t_cmd *cmd, char **arr,  int i, int count_redir);

char    *search_key(t_list *lst, char *key);
t_list	*sorting(t_list *lst);
int     set_value(t_list *lst, char *key, char *new_val);
char    **lst_to_array(t_list *lst);
char    **lst_to_argv(t_list *lst);
void    add_key(t_list *lst, char *key, char *value);
void	del_key(t_list **lst, char *key);
char     *search_path(t_all *all, t_cmd *cmd, t_list *envp);


#endif
