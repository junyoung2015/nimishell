/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_process_fn.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:47:18 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:47:29 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*env_str(char **in, t_exec_info *info)
{
	char	*start;
	char	*tmp;
	char	*result;

	tmp = 0;
	start = *in;
	while (**in && !is_dollar(**in))
		(*in)++;
	if (*in > start)
	{
		tmp = ft_substr(start, 0, *in - start);
		if (!tmp)
			return (0);
	}
	result = tmp;
	if (!**in || !is_dollar(**in))
		return (tmp);
	result = handle_dollar_sign(in, tmp, 0, info);
	return (result);
}

char	*env_squote(char **in, t_exec_info *info)
{
	char	*start;
	char	*result;

	(void)info;
	start = *in;
	(*in)++;
	while (**in && !is_squote(**in))
		(*in)++;
	result = ft_substr(start, 0, *in - start + is_squote(**in));
	if (!result)
		return (0);
	if (is_squote(**in))
		(*in)++;
	return (result);
}

char	*process_env_dquote(char **in, char **res, char **s, t_exec_info *info)
{
	char	*tmp;
	char	*prev;

	while (**in && !is_dquote(**in) && !is_dollar(**in))
		(*in)++;
	if (*in > *s || is_dquote(**in) || is_dollar(**in))
	{
		prev = *res;
		tmp = ft_substr(*s, 0, *in - *s);
		if (!tmp)
			return (0);
		*res = tmp;
		tmp = ft_strjoin(prev, tmp);
		free(prev);
		free(*res);
		*res = tmp;
		if (tmp && is_dollar(**in))
			*res = handle_dollar_sign(in, tmp, "\"", info);
	}
	*s = *in;
	return (*res);
}

/**
 * @brief Process double-quoted string, substituting environment variables.
 * 
 * @param in 
 * @param state 
 * @return char* 
 */
char	*env_dquote(char **in, t_exec_info *info)
{
	char	*start;
	char	*end;
	char	*tmp;
	char	*result;

	tmp = 0;
	result = 0;
	start = (*in)++;
	end = *in;
	while (*end && !is_dquote(*end))
		end++;
	while (**in && start < end)
		result = process_env_dquote(in, &result, &start, info);
	tmp = result;
	result = ft_strjoin(result, "\"");
	free(tmp);
	(*in)++;
	return (result);
}
