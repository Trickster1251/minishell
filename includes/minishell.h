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
				command->onichan - next command after ";" nano desu! Nippa~⭑
*/
typedef struct s_command
{
	char **args;
	int redir_flag;
	int fd[2];
	int children_num;
	struct s_command *child;
	struct s_command *onichan;
} t_command;

typedef struct s_env
{
	char *key;
	char *value;
} t_env;

typedef struct s_all
{
	char **env_strs;
	t_list *env;
	t_command *cmds;
	
} t_all;


#endif
