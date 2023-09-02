/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:38:26 by sejinkim          #+#    #+#             */
/*   Updated: 2023/09/02 15:55:16 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_bonus.h"

char	**get_path(char **env, char *cmd, t_exec_info *info)
{
	char	**path;
	size_t	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		write(STDERR_FILENO, MINISHELL, 11);
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		err_exit(info, NULL, EXIT_CMD_NOT_FOUND);
	}
	path = ft_split(env[i] + 5, ':');
	if (!path)
		err_exit(info, "minishell: malloc", EXIT_FAILURE);
	return (path);
}

char	*join_path(char *path, char *cmd)
{
	int		len;
	char	*filepath;

	len = ft_strlen(path) + ft_strlen(cmd) + 1;
	filepath = (char *)malloc(sizeof(char) * (len + 1));
	if (!filepath)
		return (NULL);
	while (*path)
		*filepath++ = *path++;
	*filepath++ = '/';
	while (*cmd)
		*filepath++ = *cmd++;
	*filepath = 0;
	return (filepath - len);
}

t_bool	is_exist(char *cmdpath, char **path, t_exec_info *info)
{
	if (!cmdpath)
	{
		free_ptr(path);
		err_exit(info, "minishell: malloc", EXIT_FAILURE);
	}
	if (!access(cmdpath, F_OK))
	{
		if (path)
			free_ptr(path);
		return (TRUE);
	}
	if (path)
		free(cmdpath);
	return (FALSE);
}

char	*get_cmdpath(char *cmd, t_exec_info *info)
{
	char	**path;
	char	*cmdpath;
	size_t	i;

	if (!*cmd)
		return (NULL);
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
	{
		if (is_exist(cmd, NULL, info))
			return (cmd);
		write(STDERR_FILENO, MINISHELL, 11);
		err_exit(info, cmd, EXIT_CMD_NOT_FOUND);
	}
	path = get_path(g_env, cmd, info);
	i = 0;
	while (path && path[i])
	{
		cmdpath = join_path(path[i], cmd);
		if (is_exist(cmdpath, path, info))
			return (cmdpath);
		i++;
	}
	free_ptr(path);
	return (NULL);
}
