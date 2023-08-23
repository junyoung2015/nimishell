#include "minishell.h"

void	get_all_files(char *first, char *tmp, t_size *size, char ***result)
{
	DIR				*dir;
	struct dirent	*entry;

	dir = opendir(".");
	if (!dir)
		return ;
	while (TRUE)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (entry->d_name[0] != '.' || (first && *first == '.'))
		{
			if (ft_strlen(first) > 1 && first[0] == '.' && first[1] == '/')
			{
				tmp = ft_strjoin("./", entry->d_name);
				*size = ft_arr_append(result, tmp, *size);
			}
			else
				*size = ft_arr_append(result, ft_strdup(entry->d_name), *size);
			if (!*size)
				break ;
		}
	}
	closedir(dir);
}

t_search	*init_search_info(char *first)
{
	char			**result;
	t_size			size;
	t_search		*info;

	size = 0;
	result = 0;
	get_all_files(first, 0, &size, &result);
	if (!result && !size)
		size = ft_arr_append(&result, ft_strdup(""), size);
	info = create_search_info(result, size);
	if (!info)
		ft_arrfree(result);
	return (info);
}

/**
 * @brief   Find all files that match the pattern
 * 
 * @param files		List of files in current direcotry
 * @param pattern	Pattern of the files to search for
 * @return char**	List of files that match the pattern
 */
char	**find_matching_files(t_search *info, char **pattern)
{
	t_size	idx;
	t_size	size;
	char	**result;
	char	*trimmed;
	char	*tmp;
	t_cmp	cmp;

	idx = 0;
	result = 0;
	size = 0;
	while (pattern[idx])
	{
		trimmed = pattern[idx];
		if (!is_wildcard_expansion(pattern[idx]))
		{
			cmp = get_cmp_fn(*(pattern[idx]));
			if (is_quote(*(pattern[idx])))
			{
				tmp = pattern[idx];
				trimmed = trim(&tmp, cmp, 0);
			}
			if (idx == 0)
				size = match_pattern_first(info, trimmed);
			else
				size = match_pattern_middle(info, trimmed);
			if (!size)
				return (0);
		}
		idx++;
	}
	if (idx && !(is_wildcard(*(pattern[idx - 1])) && ft_strlen(pattern[idx - 1]) == 1))	// If last pattern is not wildcard, filter the files with last pattern
	{
		trimmed = pattern[idx - 1];
		if (is_quote(*(pattern[idx - 1])))
		{
			tmp = pattern[idx - 1];
			trimmed = trim(&tmp, cmp, 0);
		}
		size = match_pattern_last(info, trimmed, ft_strlen(trimmed));
	}
	ft_arrfree(pattern);
	if (!info->files)
		return (0);
	size = ft_arrcat(&result, info->files, 0);
	return (result);
}

t_size	file_not_found(char ***result, t_size len, char *cmd_arg)
{
	char	**new;

	new = (char **)ft_calloc(2, sizeof(char *));
	if (!new)
		return (0);
	new[0] = ft_strdup(cmd_arg);
	len = ft_arrcat(result, new, len);
	return (len);
}

/**
 * @brief   Expand the wildcard inside cmd_args. Also expand the string inside
 *        quotes if needed.
 * 
 * @param node      node to expand
 * @return char**   expanded string
 */
char	**str_expansion(t_node *node)
{
	t_size		idx;
	t_size		len;
	t_search	*info;
	char		**result;
	char		**new;

	idx = 0;
	len = 0;
	result = 0;
	if (!node || !node->cmd_args)
		return (0);
	while (idx < node->num_args)
	{
		if (is_wildcard_expansion(node->cmd_args[idx]))
		{
			if (node->cmd_args[idx][0] == '.')
				info = init_search_info(node->cmd_args[idx]);
			else
				info = init_search_info(0);
			if (!info)
				return (0);
			ft_qsort((void **)info->files, 0, ft_arrlen(info->files) - 1, cmp_ascii);
			new = split_pattern(node->cmd_args[idx]);
			if (!new)
				return (0);
			info->files = find_matching_files(info, new);
			if (!info->files || (*(info->files) && !(*(info->files))[0]))
			{
				ft_arrfree(new);
				len = file_not_found(&result, len, node->cmd_args[idx]);
			}
			else
				len = ft_arrcat(&result, info->files, len);
			free_search_info(info);
			if (!result)
				return (0);
		}
		else
			len = file_not_found(&result, len, node->cmd_args[idx]);
		idx++;
	}
	node->num_args = len;
	ft_arrfree(node->cmd_args);
	return (result);
}
