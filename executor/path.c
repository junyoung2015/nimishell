#include "executor.h"

char	**get_path(char **env)
{
	char	**path;
	size_t	i;
	
	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	path = ft_split(env[i] + 5, ':');
	if (!path)
		exit(err("error: malloc"));
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
	if (!access(filepath, F_OK))
	{
		if (!access(filepath, X_OK))
			return (TRUE);
		else
		{
			free_ptr(path);
			free(filepath);
			exit(err("error: access"));
		}
	}
	return (FALSE);
}

char	*get_cmdpath(char *filename)
{
	char	**path;
	char	*cmdpath;
	size_t	i;

	if (*filename == '/' || *filename == '.')
		return (filename);
	path = get_path(g_info.env);
	i = 0;
	while (path && path[i])
	{
		cmdpath = join_path(path[i], filename);
		if (!cmdpath)
		{
			free_ptr(path);
			exit(err("error: malloc"));
		}
		if (check_access(cmdpath, path))
			return (cmdpath);
		free(cmdpath);
		i++;
	}
	free_ptr(path);
	cmd_not_found();
	return (NULL);
}