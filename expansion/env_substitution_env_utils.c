/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_env_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:33:14 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/03 01:03:30 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*ft_getenv(char *env_var)
{
	t_size	i;
	char	*start;
	char	*end;
	char	*tmp;

	i = 0;
	while (g_env && g_env[i])
	{
		start = g_env[i];
		if (start)
			end = ft_strchr(start, '=');
		else
			return (0);
		if (!end)
			return (0);
		tmp = ft_substr(start, 0, end - start);
		if (tmp && ft_strcmp(tmp, env_var) == 0)
		{
			free(tmp);
			return (start);
		}
		free(tmp);
		i++;
	}
	return (0);
}

char	*wrap_env_var(char *env_var, char *quote)
{
	char	*tmp;
	char	*wrapper;

	tmp = env_var;
	if (is_squote(*env_var))
		wrapper = "\"";
	else
		wrapper = 0;
	env_var = ft_strjoin(wrapper, env_var);
	free(tmp);
	tmp = env_var;
	env_var = ft_strjoin(env_var, wrapper);
	free(tmp);
	if (quote)
	{
		tmp = env_var;
		env_var = ft_strjoin(env_var, quote);
		free(tmp);
		tmp = env_var;
		env_var = ft_strjoin(quote, env_var);
		free(tmp);
	}
	return (env_var);
}

t_bool	is_split_required(char *cmd_arg)
{
	char	*tmp;
	t_bool	in_quotes;

	tmp = cmd_arg;
	in_quotes = FALSE;
	while (*tmp)
	{
		if (is_quote(*tmp))
			in_quotes = !in_quotes;
		if (is_space(*tmp) && !in_quotes)
			return (TRUE);
		tmp++;
	}
	return (FALSE);
}
