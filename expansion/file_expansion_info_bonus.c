/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expansion_info_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:59:26 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:56:17 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	get_all_files(char *first, t_size *size, char ***result)
{
	DIR				*dir;
	char			*tmp;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return ;
	while (TRUE)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (ft_strlen(first) >= 2 && ft_strncmp(first, "./", 2) == 0 && \
			(entry->d_name[0] != '.' || first[2] == '.'))
			tmp = ft_strjoin("./", entry->d_name);
		else if (entry->d_name[0] != '.' || (first && *first == '.'))
			tmp = ft_strdup(entry->d_name);
		else
			continue ;
		*size = ft_arr_append_back(result, tmp, *size);
		if (!*size)
			break ;
	}
	closedir(dir);
}

t_search	*get_search_info(char *first)
{
	char			**result;
	t_size			size;
	t_search		*info;

	size = 0;
	result = 0;
	get_all_files(first, &size, &result);
	if (!result && !size)
		size = ft_arr_append_back(&result, ft_strdup(""), size);
	info = create_search_info(result, size);
	if (!info)
		ft_arrfree(result);
	return (info);
}

t_search	*init_search_info(char *cmd_arg)
{
	t_search	*info;

	info = 0;
	if (cmd_arg[0] == '.')
		info = get_search_info(cmd_arg);
	else
		info = get_search_info(0);
	if (!info)
		return (0);
	_qs((void **)info->files, 0, ft_arrlen(info->files) - 1, cmp_ascii);
	return (info);
}
