#include "builtin.h"

t_bool	is_in_env(char *key, char *env)
{
	size_t	i;

	i = 0;
	while (key[i] != '=')
	{
		if (key[i] != env[i])
			return (FALSE);
	}
	if (env[i] == '=')
		return (TRUE);
	return (FALSE);
}

void	put_env(t_node *node, char **env, size_t i)
{
	env[i] = node->cmd_args[1];
	node->cmd_args[1] = NULL;
	free_ptr(node->cmd_args);
	node->cmd_args = NULL;
}

void	append_env(t_node *node, t_exec_info *info)
{
	char **env;
	size_t	i;

	env = malloc(sizeof(char *) * (g_info.env_cnt + 2));
	if (!env)
	{
		info->exit_code = EXIT_FAILURE;
		return ;
	}
	i = 0;
	while (g_info.env && g_info.env[i])
	{
		env[i] = g_info.env[i];
		g_info.env[i++] = NULL;
	}
	put_env(node, env, i++);
	env[i] = NULL;
	// free(g_info.env);
	g_info.env = env;
}

void	export(t_node *node, t_exec_info *info)
{
	size_t	i;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		if (is_in_env(node->cmd_args[1], g_info.env[i]))
		{
			free(g_info.env[i]);	
			put_env(node, g_info.env, i);
			return ;
		}
		i++;
	}
	append_env(node, info);
	if (!info->exit_code)
		g_info.env_cnt += 1;
}
