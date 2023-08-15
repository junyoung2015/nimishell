#include "minishell.h"

t_search	*get_all_files(char *first_arg)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	t_size			size;
	t_search		*info;

	size = 0;
	result = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	while (TRUE)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		else if (entry->d_name[0] != '.' || (first_arg && *first_arg == '.'))
		{
			size = ft_arr_append(&result, ft_strdup(entry->d_name), size);
			if (!size)
				break ;
		}
	}
	closedir(dir);
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
				trimmed = trim(&tmp, cmp);
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
			trimmed = trim(&tmp, cmp);
		}
		size = match_pattern_last(info, trimmed, ft_strlen(trimmed));
	}
	ft_arrfree(pattern);
	if (!info->files)
		return (0);
	size = ft_arrcat(&result, info->files, 0);
	return (result);
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
				info = get_all_files(node->cmd_args[idx]);
			else
				info = get_all_files(0);
			if (!info)
				return (0);
			ft_qsort((void **)info->files, 0, ft_arrlen(info->files) - 1, cmp_ascii);
			new = split_pattern(node->cmd_args[idx]);
			if (!new)
				return (0);
			info->files = find_matching_files(info, new);
			if (!info->files || (*(info->files) && !(*(info->files))[0]))
			{
				new = (char **)ft_calloc(2, sizeof(char *));
				if (!new)
					return (0);
				new[0] = ft_strdup(node->cmd_args[idx]);
				len = ft_arrcat(&result, new, len);
			}
			else
				len = ft_arrcat(&result, info->files, len);
			if (!result)
				return (0);
		}
		else
		{
			new = (char **)ft_calloc(2, sizeof(char *));
			if (!new)
				return (0);
			new[0] = ft_strdup(node->cmd_args[idx]);
			len = ft_arrcat(&result, new, len);
		}
		idx++;
	}
	node->num_args = len;
	ft_arrfree(node->cmd_args);
	return (result);
}
