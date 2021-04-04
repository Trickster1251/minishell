/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 16:01:34 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/14 14:38:37 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	isn(int b)
{
	if (b >= '0' && b <= '9')
		return (1);
	return (0);
}

static int	isal(int a)
{
	if ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
		return (1);
	return (0);
}

int			ft_isalnum(int c)
{
	if (isal(c) == 0 && isn(c) == 0)
		return (0);
	return (1);
}
