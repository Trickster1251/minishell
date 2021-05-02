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

typedef struct s_command
{
	char **argv;
	int fd[2];
	int count_redir;
	int args_num;
} t_cmd;

typedef struct s_env
{
	char *key;
	char *value;
} t_env;

typedef struct s_tok
{
	char **res;
	char *tmp;
	char *tmp_redir;
	char *str_tmp;
} t_tok;

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
	char **hist;
	int pos;
	int hist_len;
	t_list *hist_list;
	struct termios term;
	char *home;
	char *file;
	char *term_name;
	int cmds_num;
	char **env_arr;
	// Нужно инициализировать массив пайпов
	int   *fd[2];
	t_line *src;
	t_valid val;
	
} t_all;

// Функции логики

//Глобальная переменная
int		gl_fd[4];
void	search_and_destroy(t_list *lst, char *key);
void     execute_cmd(t_all *all);
void    ft_cd(t_cmd *cmd, t_list *lst, t_list *exp);
void    ft_echo(t_cmd *cmd);
void    ft_pwd(t_cmd *cmd);
void    ft_env(t_cmd *cmd, t_list *envp);
void    ft_export(t_cmd *cmd, t_list *envp, t_list *exp);
void    ft_unset(t_cmd *cmd, t_list *envp, t_list *exp);
void    ft_exit(t_cmd *cmd);
void	ft_export_arg(t_cmd * cmd, t_list *envp, t_list *exp);

// history
int		read_history(t_all *all);
int		save_history(t_all *all);
int		hist_strjoin(t_all *all, char *str);
int		get_hist_array(t_all *all);
int		new_line(t_all *all, char *str);
int 	read_line(t_all *all, char *str);
void 	ctrl_d_term(t_all *all);
//

int		is_valid_id(char *key);

int    **pipes_fd(t_all *a);
void    forking(pid_t *pid, int i, int ***fd, t_all *a);
void	exec_echo(t_all *a, pid_t *pid, int i, int **pfd);
void	exec_env(t_all *a, pid_t *pid, int i, int **pfd);
void	exec_export(t_all *a, pid_t *pid, int i, int **pfd);
void	exec_pwd(t_all *a, pid_t *pid, int i, int **pfd);

//Сигналы
void    ctrl_slash(int sig);
void    ctrl_c(int sig);

//Пайпы
void     dup_fd(int **pfd, int i, int cmd_com, t_cmd *cmd);

//Редиректы
void	create_open_fd(t_all *a, t_cmd *cmd, char **arr);
int		is_redir_type(char *str);
int     count_redir(char **arr);

//Инициализация
void	shlvl_ini(t_all *all);
void	new_env(t_all *all);
void	free_struct(t_all *all, char **argv, char ***cmds);
void	free_cmd(t_all *all);
t_all 	*init_all(char **envp);

//Мелкие полезные функции не забыть добавить в либу
int		ft_isdigit_str(char *str);
int		is_delim(char *line);

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
void    add_key(t_list *lst, char *key, char *value, int f);
void	del_key(t_list **lst, char *key);
char     *search_path(t_all *all, t_cmd *cmd, t_list *envp);
int 	unshield(char *str);
void	free_str_arr(char **arr);
char 	*get_env_val(t_list *envp, char *key);

//termcaps
int		ft_putint(int c);
void 	canon(t_all *all);
void	nocanon(t_all *all);
void 	up_arrow(t_all *all);
void	down_arrow(t_all *all);
void	backspace_key(t_all *all);
void	print_previus(t_all *all);
void	print_next(t_all *all);
//parser
int		print_merror(t_all *all);
int		parser(t_all *all);
int		make_cmd(t_all *all);
char	*var_replace(t_all *all, char *str, char *d_pointer, char *end);
int		shield(t_all *all, t_line *src);
char 	*end_var(char *s);
char	*if_no_var(t_all *all, char *str, char *d_pointer, char *end);
int		make_struct(t_all *all, char *str);
char 	*after_var(char *s);
void	free_cmd(t_all *all);
int		argv_len(char **argv);
char**	tokenize(char *str, t_all *all);
int		check_redir(char **argv);
int		m_struct(t_all *all, char ***argv);
int		check_argv(t_all *all, char **argv);
void	check_quotes(t_all *all, t_line *src);
void 	shield_sym(t_all *all, t_line *src);
int 	is_right_syntax_pipes(t_line *src, t_all *all, int *k);
int		is_right_redir_syntax(t_all *all, t_line *src, int *redir, int *rev_redir);
int		is_right_revd_syntax(t_all *all, t_line *src, int *rev_redir, int *redir);
char	prev_char(t_line *src);
char**	tokenize(char *str, t_all *all);
char	**arg_join(t_all *all, char **argv, char *str);
char	**realoc_argv(char **src);
char	*ft_charjoin(char const *s1, char c);
int		print_merror(t_all *all);
void	print_next(t_all *all);
void	print_previus(t_all *all);
int		remove_ch(t_line *src);

#endif