#include "minishell.h"

/**
 * @brief	Check whether cmd_arg contains wildcard characters, and if so,
 * 			substitute them with matching file names.
 * 
 * @param cmd_arg	argument to check for wildcard characters
 * @return char*	Substituted file names
 */
char	*match_pattern(char *cmd_arg)
{
	DIR				*dir;
	struct	dirent	*entry;
	char			*tmp;
	char			*result;
	char			*before;
	char			*after;

	result = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	tmp = ft_strchr(cmd_arg, '*');
	if (!tmp)
		return (cmd_arg);
	before = ft_substr(cmd_arg, 0, tmp - cmd_arg);
	tmp++;	// skip '*'
	after = ft_substr(tmp, 0, ft_strlen(tmp));
	while (entry)
	{
		if (ft_strncmp(entry->d_name, before, ft_strlen(before) == 0))
		{
			tmp = result;
			if (ft_strncmp(after, entry->d_name + ft_strlen(entry->d_name) - ft_strlen(after), ft_strlen(after)) == 0)
			{
				result = ft_strjoin(tmp, entry->d_name);
				free(tmp);
				if (!result)
				{
					free(before);
					free(after);
					return (0);
				}
				tmp = result;
				result = ft_strjoin(tmp, " ");
				free(tmp);
				if (!result)
				{
					free(before);
					free(after);
					return (0);
				}
			}
		}
		entry = readdir(dir);
	}
	closedir(dir);
	free(before);
	free(after);
	return (result);
}

/**
 * @brief Substitute wildcard characters in 'node' with matching file names.
 * 
 * @param node		AST node to substitute wildcard characters
 * @return char**	Substituted file names
 */
char	**wildcard_substitution(t_node *node)
{
	char	**result;
	char	**splitted;
	t_size	idx;

	idx = 0;
	if (!node || !node->cmd_args)
		return (0);
	result = (char **)ft_calloc(node->num_args + 1, sizeof(char *));
	if (!result)
		return (0);
	while (idx < node->num_args)
	{
		result[idx] = match_pattern(node->cmd_args[idx]);
		if (!result[idx])
		{
			while(idx != 0)
			{
				idx--;
				free(result[idx]);
			}
			free(result);
			return (0);
		}
		if (node->cmd_args[idx] != result[idx])
		{
			free(node->cmd_args[idx]);
			splitted = ft_split(result[idx], ' ');
			free(result[idx]);
			if (!splitted)
			{
				while(idx != 0)
				{
					idx--;
					free(result[idx]);
				}
				free(result);
				return (0);
			}
		}
		idx++;
	}
	free(node->cmd_args);
	return (result);
}
