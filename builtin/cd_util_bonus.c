/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_util_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 19:50:39 by sejinkim          #+#    #+#             */
/*   Updated: 2023/09/02 15:54:46 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_bonus.h"

/**
 * @brief	Return the value of the environment variable 'env_var'
 * 
 * @param env_var	name of the environment variable
 * @return char*	value of the environment variable 'env_var'
 */
char	*get_env(char *env_var)
{
	t_size	i;
	char	*start;
	char	*end;
	char	*tmp;

	i = 0;
	while (g_env && g_env[i])
	{
		start = g_env[i];
		end = ft_strchr(start, '=');
		if (!end)
			return (0);
		tmp = ft_substr(start, 0, end - start);
		if (tmp)
		{
			if (ft_strcmp(tmp, env_var) == 0)
			{
				free(tmp);
				return (end + 1);
			}
		}
		free(tmp);
		i++;
	}
	return (0);
}

void	append_new_env(char *new_env, t_exec_info *info)
{
	t_size	i;
	char	**env;

	env = ft_calloc(info->env_cnt + 2, sizeof(char *));
	if (!env)
	{
		err("minishell: malloc", info);
		return ;
	}
	i = 0;
	while (g_env && g_env[i])
	{
		env[i] = g_env[i];
		g_env[i++] = 0;
	}
	env[i++] = new_env;
	env[i] = 0;
	free(g_env);
	g_env = env;
}

/**
 * @brief	update the envirnoment variable with new_env
 * 
 * @param new_env	new environment variable to update, including variable name
 * @return t_bool	TRUE if success, FALSE otherwise
 */
void	update_env(char *new_env, t_exec_info *info)
{
	t_size	i;
	char	*start;
	char	*tmp;

	i = -1;
	while (g_env && g_env[++i])
	{
		start = g_env[i];
		if (ft_strchr(start, '='))
		{
			tmp = ft_substr(start, 0, ft_strchr(start, '=') - start);
			if (tmp && ft_strncmp(tmp, new_env, ft_strlen(tmp)) == 0)
			{
				free(g_env[i]);
				g_env[i] = new_env;
				free(tmp);
				return ;
			}
			free(tmp);
		}
	}
	append_new_env(new_env, info);
	if (!info->exit_code)
		info->env_cnt += 1;
}

void	update_pwd(char *cwd, t_exec_info *info)
{
	char	*pwd;
	char	*oldpwd;

	pwd = ft_strjoin("PWD=", cwd);
	oldpwd = ft_strjoin("OLDPWD=", get_env("PWD"));
	if (!pwd || !oldpwd)
		err("minishell: malloc", info);
	update_env(pwd, info);
	update_env(oldpwd, info);
}

t_bool	try_chdir(char *path, t_exec_info *info)
{
	if (chdir(path))
	{
		write(STDERR_FILENO, "minishell: cd: ", 15);
		perror(path);
		info->exit_code = EXIT_FAILURE;
		return (FALSE);
	}
	return (TRUE);
}
