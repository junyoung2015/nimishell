/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expansion_match.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:39:31 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 21:06:22 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_size	match_pattern_last(t_search *info, char *pattern, t_size last)
{
	char	**result;
	t_size	size;
	t_size	idx;

	size = 0;
	result = 0;
	idx = 0;
	while (info->files && info->files[idx])
	{
		if (ft_strlen(info->files[idx]) >= last)
		{
			if (ft_strnstr(info->files[idx] + ft_strlen(info->files[idx]) \
			- last, pattern, last))
			{
				size = ft_arr_append(&result, ft_strdup(info->files[idx]), \
				size);
				if (!size)
					return (0);
			}
		}
		idx++;
	}
	ft_arrfree(info->files);
	info->files = result;
	return (size);
}

t_size	match_middle(t_search *info, char *pattern, t_size *idx, t_size *size)
{
	if (ft_strlen(info->files[*idx]) > info->prev_pos[*idx])
	{
		if (ft_strnstr((info->files[*idx] + info->prev_pos[*idx]), pattern, \
		ft_strlen(info->files[*idx]) - info->prev_pos[*idx]))
		{
			info->prev_pos[*idx] = ft_strnstr(info->files[*idx] + \
			info->prev_pos[*idx], pattern, ft_strlen(info->files[*idx]) - \
			info->prev_pos[*idx]) - info->files[*idx] + 1;
			*size = ft_arr_append(&info->res, ft_strdup(info->files[*idx]), \
			*size);
			if (!*size)
				return (0);
		}
	}
	return (*size);
}

t_size	match_pattern_middle(t_search *info, char *pattern)
{
	t_size	*prev_pos;
	t_size	size;
	t_size	idx;

	size = 0;
	info->res = 0;
	idx = 0;
	while (info->files && info->files[idx])
	{
		if (ft_strlen(info->files[idx]) > info->prev_pos[idx])
			size = match_middle(info, pattern, &idx, &size);
		idx++;
	}
	idx = -1;
	prev_pos = ft_calloc(sizeof(t_size), size);
	if (!prev_pos)
		return (0);
	while (++idx < size)
		prev_pos[idx] = ft_strnstr(info->res[idx], pattern, \
		ft_strlen(info->res[idx])) - info->res[idx] + 1;
	ft_arrfree(info->files);
	info->files = info->res;
	free(info->prev_pos);
	info->prev_pos = prev_pos;
	return (size);
}

t_size	match_pattern_first(t_search *info, char *pattern)
{
	char	**result;
	t_size	size;
	t_size	idx;

	size = 0;
	result = 0;
	idx = 0;
	while (info->files && info->files[idx])
	{
		if (ft_strncmp(info->files[idx], pattern, ft_strlen(pattern)) == 0)
		{
			info->prev_pos[idx] = ft_strnstr(info->files[idx], pattern, \
			ft_strlen(info->files[idx])) - info->files[idx] + 1;
			size = ft_arr_append(&result, ft_strdup(info->files[idx]), size);
			if (!size)
				return (0);
		}
		idx++;
	}
	ft_arrfree(info->files);
	info->files = result;
	return (size);
}

void	match_except_last(t_search *info, char **pat, t_size *idx, t_cmp *cmp)
{
	char	*tmp;
	char	*trimmed;

	while (pat[*idx])
	{
		trimmed = pat[*idx];
		if (!is_wildcard_expansion(pat[*idx]))
		{
			*cmp = get_cmp_fn(*(pat[*idx]));
			if (is_quote(*(pat[*idx])))
			{
				tmp = pat[*idx];
				trimmed = trim(&tmp, *cmp, 0);
			}
			if (*idx == 0)
				match_pattern_first(info, trimmed);
			else
				match_pattern_middle(info, trimmed);
		}
		(*idx)++;
	}
}
