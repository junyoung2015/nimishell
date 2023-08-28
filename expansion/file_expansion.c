#include "minishell.h"

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
		*size = ft_arr_append(result, tmp, *size);
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
		size = ft_arr_append(&result, ft_strdup(""), size);
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

/**
 * @brief   Find all files that match the pattern
 * 
 * @param files		List of files in current direcotry
 * @param pattern	Pattern of the files to search for
 * @return char**	List of files that match the pattern
 */
char	**find_matching_files(t_search *info, char **pattern)
{
	t_size	i;
	char	**result;
	char	*trimmed;
	char	*tmp;
	t_cmp	cmp;

	i = 0;
	result = 0;
	while (pattern[i])
		match_except_last(info, pattern, &i, &cmp);
	if (i && !(is_wildcard(*(pattern[i - 1])) && ft_strlen(pattern[i - 1]) == 1))	// If last pattern is not wildcard, filter the files with last pattern
	{
		trimmed = pattern[i - 1];
		if (is_quote(*(pattern[i - 1])))
		{
			tmp = pattern[i - 1];
			trimmed = trim(&tmp, cmp, 0);
		}
		match_pattern_last(info, trimmed, ft_strlen(trimmed));
	}
	ft_arrfree(pattern);
	if (!info->files)
		return (0);
	ft_arrcat(&result, info->files, 0);
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

t_size	handle_wildcard_expansion(t_size len, char *cmd_arg, char ***result)
{
	char		**new;
	t_search	*info;

	new = get_search_pattern(cmd_arg, 0);
	if (!new)
		return (0);
	info = init_search_info(new[0]);
	if (!info)
		return (0);
	info->files = find_matching_files(info, new);
	if (!info->files || (*(info->files) && !(*(info->files))[0]))
		len = file_not_found(result, len, cmd_arg);
	else
		len = ft_arrcat(result, info->files, len);
	free_search_info(info);
	if (!result)
		return (0);
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
	char		**result;

	idx = 0;
	len = 0;
	result = 0;
	if (!node || !node->cmd_args)
		return (0);
	while (idx < node->num_args)
	{
		if (is_wildcard_expansion(node->cmd_args[idx]))
			len = handle_wildcard_expansion(len, node->cmd_args[idx], &result);
		else
			len = file_not_found(&result, len, node->cmd_args[idx]);
		idx++;
	}
	node->num_args = len;
	ft_arrfree(node->cmd_args);
	return (result);
}
