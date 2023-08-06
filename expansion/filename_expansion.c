#include "minishell.h"

t_bool	is_wildcard(char ch)
{
	return (ch == '*');
}

t_bool	is_wsplit(char ch)
{
	return (is_quote(ch) || is_wildcard(ch));
}

t_bool	is_wildcard_expansion(char *cmd_arg)
{
    char            *tmp;
	t_bool			in_quotes;
	
	tmp = cmd_arg;
	in_quotes = FALSE;
	while (*tmp)
	{
		if (is_squote(*tmp))
			in_quotes = !in_quotes;  // flip the value if we encounter a quote
		if (is_wildcard(*tmp) && !in_quotes)  // only return true if we're not inside quotes
			return (TRUE);
		tmp++;
	}
    return (FALSE);
}

void	ft_arrfree(char **arr)
{
	t_size	idx;

	idx = 0;
	while (arr && arr[idx])
		free(arr[idx++]);
	free(arr);
}

t_size	ft_arrlen(char **arr)
{
	t_size	len;

	len = 0;
	while (arr && arr[len])
		len++;
	return (len);
}

t_size ft_arrcat(char ***arr, char **new_arr, t_size size)
{
    t_size	new;
	t_size	idx;

	if (!new_arr)
		return (size);
	idx = -1;
	new = size;
    while (new_arr[new - size])
        new++;
    char **tmp = (char **)ft_calloc((new + 1), sizeof(char *));
    if (!tmp)
        return (0);
	while (++idx < size)
		tmp[idx] = (*arr)[idx];
	while (size < new)
	{
		tmp[size] = new_arr[size - idx];
		size++;
	}
    free(new_arr);
    *arr = tmp;
    return (new);
}

t_size ft_arr_append(char ***arr, char *str, t_size size)
{
	if (!str)
		return (size);
    char **tmp = (char **)ft_calloc((size + 2), sizeof(char *));
    if (!tmp)
        return (0);
    for (t_size i = 0; i < size; i++)	// TODO: remove for loop
        tmp[i] = (*arr)[i];
    tmp[size] = str;
	if (*arr)
		free(*arr);
    *arr = tmp;
    return (size + 1);
}

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
		else if (entry->d_name[0] != '.')
		{
			size = ft_arr_append(&result, ft_strdup(entry->d_name), size);
			if (!size)
				break ;
		}
		else if (first_arg && *first_arg == '.')
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

char	**wsplit(char *cmd_arg)
{
	char	*start;
	char	*end;
	char	**result;
	char	**splited;
	char	*substr;
	char	*trimmed;
	char	*tmp;
	t_size	size;
	t_size	idx;

	idx = 0;
	size = 0;
	result = 0;
	start = cmd_arg;
	end = start;
	while (start && *start)
	{
		if (is_wsplit(*start))
		{
			if (is_wildcard(*start))
			{
				end = start;
				size = ft_arr_append(&result, ft_strdup("*"), size);
			}
			else if (is_squote(*start))
			{
				end = start + 1;
				while (*end && !is_squote(*end))
					end++;
				substr = ft_substr(start, 0, end - start + 1);
				if (!substr)
					return (0);
				tmp = substr;
				trimmed = trim(&substr, is_squote);
				free(tmp);
				if (!trimmed)
					return (0);
				size = ft_arr_append(&result, trimmed, size);
				if (!size)
					return (0);
			}
			else if (is_dquote(*start))
			{
				end = start + 1;
				while (*end && !is_dquote(*end))
					end++;
				substr = ft_substr(start, 0, end - start + 1);
				if (!substr)
					return (0);
				tmp = substr;
				trimmed = trim(&substr, is_dquote);
				free(tmp);
				if (!trimmed)
					return (0);
				splited = wsplit(trimmed);
				free(trimmed);
				while (splited[idx])
				{
					size = ft_arr_append(&result, splited[idx], size);
					if (!size)
						return (0);
					idx++;
				}
				if (!size)
					return (0);
			}
			end++;
		}
		else
		{
			end = start + 1;
			while (*end && !is_wsplit(*end))
				end++;
			size = ft_arr_append(&result, ft_substr(start, 0, end - start), size);
			if (!size)
				return (0);
		}
		start = end;
	}
	return (result);
}

char	**match_pattern_last(t_search *info, char *pattern, t_size last)
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
	return (result);
}

char	**match_pattern_middle(t_search *info, char *pattern)
{
	char	**result;
	t_size	*prev_pos;
	// t_size	pos_len;
	t_size	size;
	t_size	idx;

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
	idx = 0;
	prev_pos = ft_calloc(sizeof(t_size), size);
	if (!prev_pos)
		return (0);
	while (idx < size)
	{
		prev_pos[idx] = ft_strnstr(result[idx], pattern, ft_strlen(result[idx])) - result[idx] + 1;
		idx++;
	}
	ft_arrfree(info->files);
	free(info->prev_pos);
	info->files = result;
	info->prev_pos = prev_pos;
	return (result);
}

char	**match_pattern_first(t_search *info, char *pattern)
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
	return (result);
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

	idx = 0;
	result = 0;
	size = 0;
	while (pattern[idx])
	{
		// If current pattern is not wildcard, filter the files
		if (!is_wildcard_expansion(pattern[idx]))
		{
			if (idx == 0)
				info->files = match_pattern_first(info, pattern[idx]);
			else
				info->files = match_pattern_middle(info, pattern[idx]);
			if (!info->files)
				return (0);
		}
		idx++;
	}
	if (idx && !is_wildcard(*(pattern[idx - 1])))	// If last pattern is not wildcard, filter the files with last pattern
		info->files = match_pattern_last(info, pattern[idx - 1], ft_strlen(pattern[idx - 1]));
	else if (!result && !info->files)
	{
		ft_arrfree(pattern);
		return (0);
	}
	size = ft_arrcat(&result, info->files, size);
	ft_arrfree(pattern);
	return (result);
}

/**
 * @brief   Expand the wildcard inside cmd_args. Also expand the string inside
 *        quotes if needed.
 * 
 * @param node      node to expand
 * @return char**   expanded string
 */
char    **str_expansion(t_node *node)
{
	t_size		idx;
	t_size		len;
	t_search	*info;
	char		**result;
	char		**new;

	idx = 0;
	len = 0;
	if (!node || !node->cmd_args)
		return (0);
	while (idx < node->num_args)
	{
		if (is_wildcard_expansion(node->cmd_args[idx]))
		{
			if (node->cmd_args[idx][0] == '.')
				info = get_all_files(node->cmd_args[idx]);
			else
				info = get_all_files(0);	// TODO: free files?
			if (!info)
				return (0);
			ft_qsort((void **)info->files, 0, ft_arrlen(info->files) - 1, cmp_ascii);	// sort files in ascii order
			new = wsplit(node->cmd_args[idx]);
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
	// TODO: free node->cmd_args later?
	// size = ft_arrcat(&result, files, size);
	return (result);
}
