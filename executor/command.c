/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:55:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/09 20:52:19 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	**get_path(char **env)
{
	char	**path;

	while (*env && ft_strncmp(*env, "PATH=", 5))
		(*env)++;
	if (!*env)
		return (NULL);
	path = ft_split(*env + 5, ':');
	if (!path)
		err();
	return (path);
}

char	*join_path(char *path, char *filename)
{
	int		len;
	char	*filepath;

	len = ft_strlen(path) + ft_strlen(filename) + 1;
	filepath = (char *)malloc(sizeof(char) * (len + 1));
	if (!filepath)
		return (NULL);
	while (*path)
		*filepath++ = *path++;
	*filepath++ = '/';
	while (*filename)
		*filepath++ = *filename++;
	*filepath = 0;
	return (filepath - len);
}

t_bool	check_access(char *filepath, char **path)
{
	if (access(filepath, F_OK) == 0)
	{
		if (access(filepath, X_OK) == 0)
			return (TRUE);
		else
		{
			free_ptr(path);
			free(filepath);
			err();
		}
	}
	return (FALSE);
}

char	*get_filepath(char *filename)
{
	char	**path;
	char	*filepath;
	size_t	i;

	if (*filename == '/' || *filename == '.')
		return (filename);
	path = get_path(g_info.env);
	i = 0;
	while (path && path[i])
	{
		filepath = join_path(path[i], filename);
		if (!filepath)
		{
			free_ptr(path);
			err();
		}
		if (check_access(filepath, path))
			return (filepath);
		free(filepath);
		i++;
	}
	free_ptr(path);
	cmd_not_found();
	return (NULL);
}

void	command(t_node *node, t_pipe_info *info)
{
	char	*filepath;

	info->pid = fork();
	if (info->pid < 0)
		err();
	else if (!info->pid)
	{
		connect_pipe(node, info);
		filepath = get_filepath(node->cmd_args[0]);
		if (execve(filepath, node->cmd_args, g_info.env) < 0)
			err();
	}
	close_pipe(node, info);
	if (node->left)
		node->left->parent_type = AST_COMMAND;
	if (node->right)
		node->right->parent_type = AST_COMMAND;
	info->fork_cnt += 1;
}
