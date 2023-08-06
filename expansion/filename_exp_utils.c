#include "minishell.h"

t_search	*create_search_info(char **files, int num_files)
{
	t_search	*info;
	
	info = ft_calloc(1, sizeof(t_search));
	if (!info)
		return (0);
	info->files = files;
	info->prev_pos = ft_calloc(num_files, sizeof(t_ssize));
	if (!info->prev_pos)
	{
		free(info);
		return (0);
	}
	info->num_files = num_files;
	return (info);
}

void	free_search_info(t_search *info)
{
	if (!info)
		return ;
	// if (info->files)
	//     free(info->files);
	if (info->prev_pos)
		free(info->prev_pos);
	free(info);
}

t_ssize match_pattern(char *str, char *pattern, int prev_pos)
{
	t_ssize	pos;

	pos = prev_pos;
	while (str[pos])
	{
		if (str[pos] == pattern[0])
		{
			if (ft_strncmp(&str[pos], pattern, ft_strlen(pattern)) == 0)
				return (pos);
		}
		pos++;
	}
	return (-1);
}

t_bool search_files(t_search *info, char *pattern)
{
	t_ssize	pos;
	t_size	idx;

	idx = 0;
	while (idx < info->num_files)
	{
		pos = match_pattern(info->files[idx], pattern, info->prev_pos[idx]);
		if (pos != -1)
		{
			info->prev_pos[idx] = pos;
			return (TRUE);
		}
		idx++;
	}
	return (FALSE);
}
