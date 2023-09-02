/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_env_fn.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:47:18 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/30 16:57:45 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	**env_str(char **in, t_exec_info *info)
{
	char	*start;
	char	*tmp;
	char	**prev;
	char	**result;

	tmp = 0;
	start = *in;
	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	while (**in && !is_dollar(**in))
		(*in)++;
	if (*in > start)
		tmp = ft_substr(start, 0, *in - start);
	ft_arr_append_back(&result, tmp, 0);
	prev = result;
	if (!**in || !is_dollar(**in) || !prev)
		return (result);
	result = handle_dollar_sign(in, result[0], 0, info);
	free(prev[0]);
	free(prev);
	return (result);
}

char	**env_squote(char **in, t_exec_info *info)
{
	char	*start;
	char	**result;

	(void)info;
	start = *in;
	(*in)++;
	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	while (**in && !is_squote(**in))
		(*in)++;
	result[0] = ft_substr(start, 0, *in - start + is_squote(**in));
	if (!result[0])
	{
		free(result);
		return (0);
	}
	if (is_squote(**in))
		(*in)++;
	return (result);
}

t_bool	append_or_concat_str(char *tmp, char **in, char ***res, t_exec_info *info)
{
	char	**arr;
	t_size	len;

	len = ft_arrlen(*res);
	if (tmp && is_dollar(**in))
	{
		arr = *res;
		*res = handle_dollar_sign(in, tmp, "\"", info);
		free(tmp);
		free(arr);
		if (!*res)
			return (FALSE);
		len = ft_arrlen(*res);
	}
	else if (len > 0)
		(*res)[len - 1] = tmp;
	else
	{
		free((*res)[len]);
		(*res)[len] = tmp;
	}
	return (TRUE);
}

t_bool	process_env_dquote(char **in, char ***res, char **s, t_exec_info *info)
{
	char	*tmp;
	char	*temp;
	char	*prev;

	while (**in && !is_dquote(**in) && !is_dollar(**in))
		(*in)++;
	if (*in > *s || is_dquote(**in) || is_dollar(**in))
	{
		prev = 0;
		if (ft_arrlen(*res) > 0)
			prev = (*res)[ft_arrlen(*res) - 1];
		tmp = ft_substr(*s, 0, *in - *s);
		if (!tmp)
			return (FALSE);
		temp = tmp;
		tmp = ft_strjoin(prev, tmp);
		free(temp);
		free(prev);
		if (!tmp)
			return (FALSE);
		if (!append_or_concat_str(tmp, in, res, info))
			return (FALSE);
	}
	*s = *in;
	return (TRUE);
}

/**
 * @brief Process double-quoted string, substituting environment variables.
 * 
 * @param in 
 * @param state 
 * @return char* 
 */
char	**env_dquote(char **in, t_exec_info *info)
{
	char	*start;
	char	*end;
	char	*tmp;
	char	**dummy;
	char	**result;

	start = *in;
	end = ++(*in);
	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	while (*end && !is_dquote(*end))
		end++;
	while (**in && start < end)
		if (!process_env_dquote(in, &result, &start, info))
			return (0);
	dummy = &result[ft_arrlen(result)];
	if (ft_arrlen(result) > 0)
		dummy = &result[ft_arrlen(result) - 1];
	tmp = ft_strjoin(*dummy, "\"");
	free(*dummy);
	*dummy = tmp;
	if (is_dquote(**in))
		(*in)++;
	return (result);
}
