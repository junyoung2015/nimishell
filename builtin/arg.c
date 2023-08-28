/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 19:48:33 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/28 19:48:34 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static t_bool	_isalpha(char c)
{
	if (c == '_')
		return (TRUE);
	if ('A' <= c && c <= 'Z')
		return (TRUE);
	if ('a' <= c && c <= 'z')
		return (TRUE);
	return (FALSE);
}

static t_bool	_isalnum(char c)
{
	if (_isalpha(c))
		return (TRUE);
	if ('0' <= c && c <= '9')
		return (TRUE);
	return (FALSE);
}

int	arg_check(char *str, t_builtin func)
{
	size_t	i;

	if (str[0] == '#')
		return (0);
	if (str[0] == '_' && str[1] == '=' * (func == EXPORT))
		return (1);
	if (!_isalpha(str[0]))
		return (-1);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!_isalnum(str[i]))
			return (-1);
		i++;
	}
	if (str[i] == '=')
		return (2 - 3 * (func == UNSET));
	return (1 + (func == UNSET));
}

void	arg_err(t_builtin func, char *arg, t_exec_info *info)
{
	if (func == UNSET)
		write(STDERR_FILENO, "minishell: unset: '", 19);
	else
		write(STDERR_FILENO, "minishell: export: '", 20);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	info->exit_code = EXIT_FAILURE;
}
