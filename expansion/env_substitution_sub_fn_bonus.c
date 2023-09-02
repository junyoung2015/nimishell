/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_sub_fn_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:50:40 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:55:52 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_bonus.h"

t_size	split_env_substituted(char ***splitted, char *env_var)
{
	t_size	len;
	char	*start;
	char	*end;
	char	*substr;

	len = 0;
	start = env_var;
	end = start;
	*splitted = ft_calloc(1, sizeof(char **));
	while (end && *end)
	{
		while (*end && is_not_space(*end))
			end++;
		substr = ft_substr(start, 0, end - start);
		if (!substr)
			return (0);
		len = ft_arr_append_back(splitted, substr, len);
		if (is_space(*end))
			end++;
		start = end;
	}
	free(env_var);
	return (len);
}

t_bool	substitute_env_var(char *quot, char *val, char *result, char ***split)
{
	t_size	len;
	t_size	idx;

	len = 0;
	result = ft_strtrim(val + 1, " ");
	if (!result)
		return (FALSE);
	else if (is_split_required(result))
		len = split_env_substituted(split, result);
	else
	{
		*split = ft_calloc(2, sizeof(char **));
		(*split)[0] = result;
	}
	idx = -1;
	while (++idx < len)
		(*split)[idx] = wrap_env_var((*split)[idx], quot);
	return (TRUE);
}

char	**substitute(char *env_var, char *quote)
{
	char	*result;
	char	*key;
	char	*value;
	char	**splitted;

	result = 0;
	splitted = 0;
	key = ft_getenv(env_var);
	if (!key)
		return (0);
	value = ft_strchr(key, '=');
	if (value)
	{
		if (!substitute_env_var(quote, value, result, &splitted))
			return (0);
	}
	else
	{
		splitted = ft_calloc(2, sizeof(char **));
		splitted[0] = ft_strdup("");
	}
	return (splitted);
}

char	**sub_exit_code(char **in, char *tmp, t_exec_info *info)
{
	char	*substituted;
	char	**result;

	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	if (**in == '?')
	{
		substituted = ft_itoa(info->prev_exit_code);
		result[0] = ft_strjoin(tmp, substituted);
		free(substituted);
		(*in)++;
	}
	return (result);
}

char	**sub_env_var(char **in, char *tmp, char *quote)
{
	char	**substituted;
	char	*env_var;
	char	*start;

	start = *in;
	while (**in && is_env_var(**in))
		(*in)++;
	env_var = ft_substr(start, 0, *in - start);
	if (!env_var)
		return (0);
	substituted = substitute(env_var, quote);
	free(env_var);
	if (ft_arrlen(substituted) > 1)
		ft_arr_append_front(&substituted, tmp, ft_arrlen(substituted));
	else
	{
		if (!substituted)
			substituted = ft_calloc(2, sizeof(char **));
		if (!substituted)
			return (0);
		start = substituted[0];
		substituted[0] = ft_strjoin(tmp, substituted[0]);
		free(start);
	}
	return (substituted);
}
