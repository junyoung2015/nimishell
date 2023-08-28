/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expansion_pattern.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:38:45 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:38:46 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_size	handle_wildcard(char ***result, char **start, char **end, t_size size)
{
	*end = *start;
	size = ft_arr_append(result, ft_strdup("*"), size);
	(*end)++;
	*start = *end;
	return (size);
}

t_size	handle_quotes(char ***result, char **start, char **end, t_size size)
{
	char	*substr;
	t_cmp	cmp;

	cmp = is_squote;
	if (is_dquote(**start))
		cmp = is_dquote;
	*end = ++(*start);
	while (**end && !cmp(**end))
		(*end)++;
	substr = ft_substr(*start, 0, *end - *start);
	if (!substr)
		return (0);
	size = ft_arr_append(result, substr, size);
	(*end)++;
	*start = *end;
	return (size);
}

t_size	handle_normal(char ***result, char **start, char **end, t_size size)
{
	*end = *start + 1;
	while (**end && !is_wsplit(**end))
		(*end)++;
	size = ft_arr_append(result, ft_substr(*start, 0, *end - *start), size);
	*start = *end;
	return (size);
}

char	**get_search_pattern(char *cmd_arg, t_size size)
{
	char	*start;
	char	*end;
	char	**result;

	result = 0;
	start = cmd_arg;
	end = start;
	while (start && *start)
	{
		if (is_wsplit(*start) && is_wildcard(*start))
			size = handle_wildcard(&result, &start, &end, size);
		else if (is_wsplit(*start))
			size = handle_quotes(&result, &start, &end, size);
		else
			size = handle_normal(&result, &start, &end, size);
		if (!size)
			return (0);
		else if (!*(result[size - 1]))
		{
			free(result[size - 1]);
			result[size-- - 1] = 0;
		}
	}
	return (result);
}
