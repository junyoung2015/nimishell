/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution_env_fn.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:47:18 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/31 20:59:04 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	**env_str(char **in, t_exec_info *info)
{
	t_size	len;
	char	*start;
	char	*tmp;
	char	**prev;
	char	**result;

	len = 0;
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
	len = ft_arr_append_back(&result, tmp, len);
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

char	*process_env_dquote(char **in, char *res, char **s, t_exec_info *info)
{
	char	*tmp;
	char	*dol;
	char	*result;

	result = NULL;
	while (**in && !is_dquote(**in) && !is_dollar(**in))
		(*in)++;
	if (*in > *s || is_dquote(**in) || is_dollar(**in))
	{
		dol = ft_substr(*s, 0, *in - *s);
		if (!dol)
			return (0);
		tmp = ft_strjoin(res, dol);
		free(res);
		free(dol);
		if (!tmp)
			return (0);
		if (is_dollar(**in))
			result = *handle_dollar_sign(in, tmp, "\"", info);
		free(tmp);
	}
	*s = *in;
	return (result);
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
	char	*temp;
	char	*tmp;
	char	**result;
	t_size	len;

	tmp = 0;
	len = 0;
	result = 0;
	start = (*in)++;
	end = *in;
	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	while (*end && !is_dquote(*end))
		end++;
	while (**in && start < end)
	{
		tmp = process_env_dquote(in, result[len], &start, info);
		temp = ft_strjoin(tmp, "\"");
		free(tmp);
		len = ft_arr_append_back(&result, temp, len);
		// free(tmp[0]);
		// free(tmp);
	}
	// tmp = result;
	// free(tmp);
	(*in)++;
	return (result);
}
