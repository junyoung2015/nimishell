/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_sub_fn.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:50:40 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/30 18:02:03 by jusohn           ###   ########.fr       */
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

char	**substitute(char *env_var, char *quote)
{
	char	*result;
	char	*key;
	char	*value;
	char	**splitted;
	t_size	len;

	len = 0;
	result = 0;
	splitted = 0;
	key = ft_getenv(env_var);
	if (!key)
		return (0);
	value = ft_strchr(key, '=');
	if (value)
	{
		result = ft_strtrim(value + 1, " ");
		if (!result)
			return (0);
		else if (is_split_required(result))
		{
			len = split_env_substituted(&splitted, result);
		}
		else
		{
			splitted = ft_calloc(2, sizeof(char **));
			splitted[0] = result;
		}
		for (t_size i = 0; i < len; i ++)
		{
			splitted[i] = wrap_env_var(splitted[i], quote);
			// free(splitted[i]);
		}
		// result = wrap_env_var(result, quote);
	}
	else
	{
		splitted = ft_calloc(2, sizeof(char **));
		splitted[0] = ft_strdup("");
	}
	// return (result);
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
		free(tmp);
		// ft_arr_append_back(&result, substituted, 1);
		// result = ft_strjoin(tmp, substituted);
		// result[1] = substituted;
		(*in)++;
		// free(substituted);
	}
	return (result);
}

char	**sub_env_var(char **in, char *tmp, char *quote)
{
	char	**substituted;
	// char	**prev;
	char	*env_var;
	char	*start;
	char	*temp;

	// prev = 0;
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
		{
			substituted = ft_calloc(2, sizeof(char **));
			if (!substituted)
				return (0);
		}
		// else
		// 	prev = substituted;
		temp = substituted[0];
		substituted[0] = ft_strjoin(tmp, substituted[0]);
		// ft_arrfree(prev);
		// free(substituted);
		free(temp);
		// free(tmp);
	}
	return (substituted);
}
