/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_sub_fn.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:50:40 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:51:39 by jusohn           ###   ########.fr       */
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
		wrapper = "'";
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

char	*substitute(char *env_var, char *quote)
{
	char	*result;
	char	*key;
	char	*value;

	result = 0;
	key = ft_getenv(env_var);
	if (!key)
		return (0);
	value = ft_strchr(key, '=');
	if (value)
	{
		result = ft_strtrim(value + 1, " ");
		result = wrap_env_var(result, quote);
	}
	return (result);
}

char	*sub_exit_code(char **in, char *tmp, t_exec_info *info)
{
	char	*substituted;
	char	*result;

	result = 0;
	if (**in == '?')
	{
		substituted = ft_itoa(info->prev_exit_code);
		result = ft_strjoin(tmp, substituted);
		(*in)++;
		free(substituted);
	}
	return (result);
}

char	*sub_env_var(char **in, char *tmp, char *quote)
{
	char	*substituted;
	char	*result;
	char	*env_var;
	char	*start;

	result = 0;
	start = *in;
	while (**in && is_env_var(**in))
		(*in)++;
	env_var = ft_substr(start, 0, *in - start);
	if (!env_var)
		return (0);
	substituted = substitute(env_var, quote);
	result = ft_strjoin(tmp, substituted);
	free(substituted);
	free(env_var);
	return (result);
}