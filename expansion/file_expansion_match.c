#include "minishell.h"

t_size	match_pattern_last(t_search *info, char *pattern, t_size last)
{
	char	**result;
	t_size	size;
	t_size	idx;

	size = 0;
	result = 0;
	idx = 0;
	while (info->files[idx])
	{
		if (ft_strlen(info->files[idx]) >= last)
		{
			if (ft_strnstr(info->files[idx] + ft_strlen(info->files[idx]) - last, pattern, last))
			{
				size = ft_arr_append(&result, ft_strdup(info->files[idx]), size);
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

t_size	match_pattern_middle(t_search *info, char *pattern)
{
	char	**result;
	t_size	*prev_pos;
	t_size	size;
	t_ssize	idx;

	size = 0;
	result = 0;
	idx = 0;
	while (info->files[idx])
	{
		if (ft_strlen(info->files[idx]) > info->prev_pos[idx])
		{
			if (ft_strnstr((info->files[idx] + info->prev_pos[idx]), pattern, ft_strlen(info->files[idx]) - info->prev_pos[idx]))
			{
				info->prev_pos[idx] = ft_strnstr(info->files[idx] + info->prev_pos[idx], pattern, ft_strlen(info->files[idx]) - info->prev_pos[idx]) - info->files[idx] + 1;
				size = ft_arr_append(&result, ft_strdup(info->files[idx]), size);
				if (!size)
					return (0);
			}
		}
		idx++;
	}
	idx = -1;
	prev_pos = ft_calloc(sizeof(t_size), size);
	if (!prev_pos)
		return (0);
	while (++idx < size)
		prev_pos[idx] = ft_strnstr(result[idx], pattern, ft_strlen(result[idx])) - result[idx] + 1;
	ft_arrfree(info->files);
	info->files = result;
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
	while (info->files[idx])
	{
		if (ft_strncmp(info->files[idx], pattern, ft_strlen(pattern)) == 0)
		{
			info->prev_pos[idx] = ft_strnstr(info->files[idx], pattern, ft_strlen(info->files[idx])) - info->files[idx] + 1;
			size = ft_arr_append(&result, ft_strdup(info->files[idx]), size);
			if (!size)
				return (0);
		}
		idx++;
	}
	idx = 0;
	ft_arrfree(info->files);
	info->files = result;
	return (size);
}
