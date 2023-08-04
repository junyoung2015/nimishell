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
	t_size	size;

	size = 0;
	result = 0;
	start = cmd_arg;
	while (start && *start)
	{
		if (is_wsplit(*start))
		{
			if (is_wildcard(*start))
				size = append_str(&result, ft_strdup("*"), size);
			else if (is_squote(*start))
			{
				end = start + 1;
				while (*end && !is_squote(*end))
					end++;
				size = append_str(&result, ft_substr(start, 0, end - start + 1), size);
				if (!size)
					return (0);
			}
			else if (is_dquote(*start))
			{
				end = start + 1;
				while (*end && !is_dquote(*end))
					end++;
				size = append_str(&result, ft_substr(start, 0, end - start + 1), size);
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
	// char	*start;
	char	**files;
	char	**result;
	char	**new;

	idx = 0;
	result = 0;
	size = 0;
	if (!node || !node->cmd_args)
		return (0);
	files = get_all_files();	// TODO: free files
	if (!files)
		return (0);
	result = (char **)ft_calloc(1, sizeof(char *));
	if (!result)
		return (node->cmd_args);
	while (idx < node->num_args)
	{
		if (is_wildcard_expansion(node->cmd_args[idx]))
		{
			new = wsplit(node->cmd_args[idx]);
			for (t_size i = 0; new[i]; i++)
				printf("%s\n", new[i]);
			// new = wildcard_expansion(files, start, idx);
			// if (!new)
			// 	return (0);
			size = arr_cat(&result, new, size);
			if (!result)
				return (0);
		}
		else
		{
			new = (char **)ft_calloc(2, sizeof(char *));
			if (!new)
				return (0);
			new[0] = ft_strdup(node->cmd_args[idx]);
			size = arr_cat(&result, new, size);
		}
		// else
		// 	result = arr_cat(&result, ft_split(node->cmd_args[idx], ' '), arr_len(result));
		idx++;
	}
	return (result);
}
