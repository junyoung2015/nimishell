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
	{
		tmp = ft_substr(start, 0, *in - start);
		if (!tmp)
			return (0);
	}
	ft_arr_append_back(&result, tmp, 0);
	prev = result;
	if (!**in || !is_dollar(**in))
		return (result);
	result = handle_dollar_sign(in, result[0], 0, info);
	ft_arrfree(prev);
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

t_bool	process_env_dquote(char **in, char ***res, char **s, t_exec_info *info)
{
	char	**dummy;
	char	*tmp;
	char	*temp;
	char	*prev;
	t_size	len;

	len = ft_arrlen(*res);
	while (**in && !is_dquote(**in) && !is_dollar(**in))
		(*in)++;
	if (*in > *s || is_dquote(**in) || is_dollar(**in))
	{
		if (len > 0)
			prev = (*res)[len - 1];
		else
			prev = 0;
		tmp = ft_substr(*s, 0, *in - *s);
		if (!tmp)
			return (FALSE);
		temp = tmp;
		tmp = ft_strjoin(prev, tmp);
		free(temp);
		free(prev);
		if (!tmp)
			return (FALSE);
		if (tmp && is_dollar(**in))
		{
			dummy = *res;
			*res = handle_dollar_sign(in, tmp, "\"", info);
			free(tmp);
			free(dummy);	
			if (!*res)
				return (FALSE);
			len = ft_arrlen(*res);
		}
		else
		{
			if (len > 0)
				(*res)[len - 1] = tmp;
			else
			{
				free((*res)[len]);
				(*res)[len] = tmp;
			}
		}
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
	t_size	len;
	char	*start;
	char	*end;
	char	*tmp;
	char	*temp;
	char	**dummy;
	char	*prev;
	char	**result;

	len = 0;
	start = *in;
	end = ++(*in);
	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	while (*end && !is_dquote(*end))
		end++;
	while (**in && start < end)
	{
		while (**in && !is_dquote(**in) && !is_dollar(**in))
			(*in)++;
		if (*in > start || is_dquote(**in) || is_dollar(**in))
		{
			if (len > 0)
				prev = result[len - 1];
			else
				prev = 0;
			tmp = ft_substr(start, 0, *in - start);
			if (!tmp)
				return (0);
			temp = tmp;
			tmp = ft_strjoin(prev, tmp);
			free(temp);
			free(prev);
			if (!tmp)
				return (0);
			if (tmp && is_dollar(**in))
			{
				dummy = result;
				result = handle_dollar_sign(in, tmp, "\"", info);
				free(tmp);
				free(dummy);	
				if (!result)
					return (0);
				len = ft_arrlen(result);
			}
			else
			{
				if (len > 0)
					result[len - 1] = tmp;
				else
				{
					free(result[len]);
					result[len] = tmp;
				}
			}
		}
		start = *in;
	}
	if (len > 0)
		dummy = &result[len - 1];
	else
		dummy = &result[len];
	tmp = ft_strjoin(*dummy, "\"");
	free(*dummy);
	*dummy = tmp;
	if (is_dquote(**in))
		(*in)++;
	return (result);
}
