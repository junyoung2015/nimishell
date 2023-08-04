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
	in_quotes = 0;

	tmp = cmd_arg;
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


t_size arr_cat(char ***arr, char **new_arr, t_size size)
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
	if (*arr)
		free(*arr);
    free(new_arr);
    *arr = tmp;
    return (new);
}

t_size append_str(char ***arr, char *str, t_size size)
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

char	**get_all_files(void)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**result;
	t_size			size;

	size = 0;
	result = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.')
		{
			size = append_str(&result, ft_strdup(entry->d_name), size);
			if (!size)
			{
				closedir(dir);
				return (0);
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (result);
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
				size = append_str(&result, ft_strdup("*"), size);
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
				size = append_str(&result, trimmed, size);
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
					size = append_str(&result, splited[idx], size);
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
			size = append_str(&result, ft_substr(start, 0, end - start), size);
			if (!size)
				return (0);
		}
		start = end;
	}
	return (result);
}

char	**match_pattern(char **files, char *pattern, t_size start)
{
	char	**result;
	t_size	size;
	t_size	idx;

	size = 0;
	result = 0;
	idx = 0;
	while (files[idx])
	{
		if (ft_strnstr(files[idx] + start, pattern, ft_strlen(pattern)))
		{
			size = append_str(&result, ft_strdup(files[idx]), size);
			if (!size)
				return (0);
		}
		// if (ft_strncmp(files[idx] + ft_strlen(files[idx]) - start, pattern, ft_strlen(pattern)) == 0)
		// {
		// 	size = append_str(&result, ft_strdup(files[idx]), size);
		// 	if (!size)
		// 		return (0);
		// }
		idx++;
	}
	while(*files)
		free(*files++);
	return (result);
}

/**
 * @brief   Find all files that match the pattern
 * 
 * @param files		List of files in current direcotry
 * @param pattern	Pattern of the files to search for
 * @return char**	List of files that match the pattern
 */
char	**find_matching_files(char **files, char **pattern)
{
	t_size	idx;
	t_size	size;
	t_size	start;
	char	**result;
	char	**new;
	// char	**tmp;

	idx = 0;
	result = 0;
	new = files;
	size = 0;
	start = 0;
	while (pattern[idx])
	{
		if (is_wildcard_expansion(pattern[idx]))
		{
			// new = match_pattern(files, "", start);
			// size = arr_cat(&result, files, size);
		}
		else
		{
			start += ft_strlen(pattern[idx]);
			// tmp = new;
			new = match_pattern(new, pattern[idx], start);
			if (!new)
				return (0);
			size = arr_cat(&result, new, size);
			// size = append_str(&result, ft_strdup(pattern[idx]), size);
		}
		// if (is_squote(*(pattern[idx])))
		// 	trimmed = ft_substr(pattern[idx], 1, ft_strlen(pattern[idx]));
		// free files
		idx++;
	}
	// if (!result)
	// {
	// 	size = arr_cat(&result, files, size);
	// }
	idx = 0;
	while (pattern[idx])
		free(pattern[idx++]);
	free(pattern);
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
	t_size	idx;
	t_size	size;
	t_size	len;
	char	**files;
	char	**result;
	char	**pattern;
	char	**new;

	idx = 0;
	size = 0;
	len = 0;
	pattern = 0;
	if (!node || !node->cmd_args)
		return (0);
	pattern = (char **)ft_calloc(1, sizeof(char *));
	if (!pattern)
		return (node->cmd_args);
	while (idx < node->num_args)
	{
		if (is_wildcard_expansion(node->cmd_args[idx]))
		{
			files = get_all_files();	// TODO: free files?
			if (!files)
				return (0);
			new = wsplit(node->cmd_args[idx]);
			if (!new)
				return (0);
			for (t_size i = 0; new[i]; i++)
				printf("%s\n", new[i]);
			size = arr_cat(&pattern, new, size);
			if (!pattern)
				return (0);
			files = find_matching_files(files, pattern);
			len = arr_cat(&result, files, len);
			if (!result)
				return (0);
		}
		else
		{
			new = (char **)ft_calloc(2, sizeof(char *));
			if (!new)
				return (0);
			new[0] = ft_strdup(node->cmd_args[idx]);
			len = arr_cat(&result, new, len);
			if (!pattern)
				return (0);
		}
		idx++;
	}
	node->num_args = len;
	// size = arr_cat(&result, files, size);
	return (result);
}
