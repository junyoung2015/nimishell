#include "executor.h"

char	**get_path(char **env, char *filename)
{
	char	**path;
	size_t	i;
	
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, filename, ft_strlen(filename));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		err_exit(EXIT_CMD_NOT_FOUND, NULL);
	}
	path = ft_split(env[i] + 5, ':');
	if (!path)
		err_exit(EXIT_FAILURE, "minishell: malloc");
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

t_bool	check_access(char *filepath, char **path, char *filename)
{
	if (!access(filepath, F_OK))
	{
		if (!access(filepath, X_OK))
			return (TRUE);
		else
		{
			free_ptr(path);
			write(STDERR_FILENO, "minishell: ", 11);
			err_exit(EXIT_NOT_EXECUTABLE, filename);
		}
	}
	return (FALSE);
}

char	*get_cmdpath(char *filename)
{
	char	**path;
	char	*cmdpath;
	size_t	i;

	if (!*filename || *filename == '/' || *filename == '.')
		return (filename);
	path = get_path(g_info.env, filename);
	i = 0;
	while (path && path[i])
	{
		cmdpath = join_path(path[i], filename);
		if (!cmdpath)
		{
			free_ptr(path);
			err_exit(EXIT_FAILURE, "minishell: malloc");
		}
		if (check_access(cmdpath, path, filename))
			return (cmdpath);
		free(cmdpath);
		i++;
	}
	free_ptr(path);
	return (NULL);
}
