#include "executor.h"

char	**get_path(char **env, char *cmd, t_exec_info *info)
{
	char	**path;
	size_t	i;
	
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		write(STDERR_FILENO, "minishell: ", 11);
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

t_bool	check_access(char *filepath, char **path, char *cmd, t_exec_info *info)
{
	if (!access(filepath, F_OK))
	{
		if (!access(filepath, X_OK))
			return (TRUE);
		else
		{
			free_ptr(path);
			write(STDERR_FILENO, "minishell: ", 11);
			err_exit(info, cmd, EXIT_NOT_EXECUTABLE);
		}
	}
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
		return (cmd);
	path = get_path(g_env, cmd, info);
	i = 0;
	while (path && path[i])
	{
		cmdpath = join_path(path[i], cmd);
		if (!cmdpath)
		{
			free_ptr(path);
			err_exit(info, "minishell: malloc", EXIT_FAILURE);
		}
		if (check_access(cmdpath, path, cmd, info))
			return (cmdpath);
		free(cmdpath);
		i++;
	}
	free_ptr(path);
	return (NULL);
}
