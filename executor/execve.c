/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:15 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/02 16:45:20 by sejinkim         ###   ########.fr       */
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

static char	*_filepath_(char const *s1, char const *s2)
{
	int		len;
	char	*str;

	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (*s1)
		*str++ = *s1++;
	*str++ = '/';
	while (*s2)
		*str++ = *s2++;
	*str = 0;
	return (str - len);
}

char	*get_filepath(char *filename)
{
	char	**path;
	char	*filepath;
	size_t	i;

	if (*filename == '/')
		return (filename);
	path = get_path(g_info.env);
	i = 0;
	while (path && path[i])
	{
		filepath = _filepath_(path[i], filename);
		if (!filepath)
			err();
		if (!access(filepath, F_OK))
		{
			if (!access(filepath, X_OK))
				return (filepath);
			else
			{
				free(filepath);
				err();
			}
		}
		free(filepath);
		i++;
	}
	return (NULL);
}

void	ft_execve(t_node *node)
{
	char	*filepath;

	filepath = get_filepath(node->cmd_args[0]);
	if (execve(filepath, node->cmd_args, g_info.env) < 0)
		err();
}