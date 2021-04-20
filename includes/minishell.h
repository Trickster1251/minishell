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

/*              fd[0] - stdin
				fd[1] - stdout
				args[0] - command name
				command->child - next command after pipe (if it exist)
*/
typedef struct s_command
{
	char **args;
	int fd[2];
	// struct s_command *child;
} t_command;

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
	t_list *env;
	t_command *cmds;
	char **history;
	int pos;
	int hist_len;
	t_list *hist_list;
	struct termios term;
	char *term_name;
	int cmds_num;
	t_line *src;
	t_valid val;
	
} t_all;


#endif
