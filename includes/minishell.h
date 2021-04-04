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

typedef struct s_command
{
	char *command;
	char **args;
	int redir_flag;
	int stdin;
	int stdout;
	char *out;
	char *in;
} t_command;

typedef struct s_env
{
	char *key;
	char *value;
} t_env;

typedef struct s_all
{
	t_list *env;
	t_list *commands;
	
} t_all;


#endif
