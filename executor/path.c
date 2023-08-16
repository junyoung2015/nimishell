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
		info->exit_code = EXIT_CMD_NOT_FOUND; 
		err_exit(info, NULL);
	}
	path = ft_split(env[i] + 5, ':');
	if (!path)
	{
		info->exit_code = EXIT_FAILURE;
		err_exit(info, "minishell: malloc");
	}
	return (path);
}

char	*join_path(char *path, char *cmd, t_exec_info *info)
{
	int		len;
	char	*filepath;

	len = ft_strlen(path) + ft_strlen(cmd) + 1;
	filepath = (char *)malloc(sizeof(char) * (len + 1));
	if (!filepath)
	{
		info->exit_code = EXIT_FAILURE;
		return (NULL);
	}
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
			info->exit_code = EXIT_NOT_EXECUTABLE;
			err_exit(info, cmd);
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
	path = get_path(g_info.env, cmd, info);
	i = 0;
	while (path && path[i])
	{
		cmdpath = join_path(path[i], cmd, info);
		if (!cmdpath)
		{
			free_ptr(path);
			err_exit(info, "minishell: malloc");
		}
		if (check_access(cmdpath, path, cmd, info))
			return (cmdpath);
		free(cmdpath);
		i++;
	}
	free_ptr(path);
	return (NULL);
}
