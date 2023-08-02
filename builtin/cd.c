#include "builtin.h"

/**
 * @brief	Return the value of the environment variable 'env_var'
 * 
 * @param env_var	name of the environment variable
 * @return char*	value of the environment variable 'env_var'
 */
char	*get_env(char *env_var)
{
	t_size	i;
	char		*start;
	char	*end;
	char		*tmp;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		start = g_info.env[i];
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

/**
 * @brief	update the envirnoment variable with new_env
 * 
 * @param new_env	new environment variable to update, including variable name
 * @return t_bool	TRUE if success, FALSE otherwise
 */
void	update_env(char *new_env)
{
	t_size	i;
	char	*start;
	char	*end;
	char	*tmp;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		start = g_info.env[i];
		end = ft_strchr(start, '=');
		if (end)
		{
			tmp = ft_substr(start, 0, end - start);
			if (tmp)
			{
				if (ft_strncmp(tmp, new_env, ft_strlen(tmp)) == 0)
				{
					free(g_info.env[i]);
					g_info.env[i] = new_env;
					free(tmp);
					return ;
				}
			}
		}		
		i++;
	}
	
}

void	update_pwd(char *cwd, t_exec_info *info)
{
	char	*pwd;
	char	*oldpwd;

	// TODO: PWD/OLDPWD 가 없을 경우 bash 처럼 처리 필요
	pwd = ft_strjoin("PWD=", cwd);
	oldpwd = ft_strjoin("OLDPWD=", get_env("PWD"));
	if (!pwd || !oldpwd)
		err("error: malloc", info);
	update_env(pwd);
	update_env(oldpwd);
	// free(pwd);
	// free(oldpwd);
}

t_bool	cd(t_node *node, t_exec_info *info)
{
	char	*home;
	char	*cwd;

	home = get_env("HOME");
	// TODO: HOME 이 unset 됐을 경우 처리 필요
	if (!home)
		err("error: malloc", info);
	if (node->num_args == 1)
	{
		if (chdir(home))
			err("error: cd", info);
		cwd = getcwd(0, 0);
		update_pwd(cwd, info);
		free(cwd);
	}
	else if (node->num_args >= 2)
	{
		if (chdir(node->cmd_args[1]))
			err("error: cd", info);
		cwd = getcwd(0, 0);
		update_pwd(cwd, info);
		free(cwd);
	}
	return (TRUE);
}
