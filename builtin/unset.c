#include "builtin.h"

t_bool	is_env(char *key, char *env)
{
	size_t	i;

	i = 0;
	while (key && key[i])
	{
		if (key[i] != env[i])
			return (FALSE);
		i++;
	}
	if (env[i] == '=')
		return (TRUE);
	return (FALSE);
}

size_t	get_idx(char *key)
{
	size_t	idx;

	idx = 0;
	while (g_info.env && g_info.env[idx])
	{
		if (is_env(key, g_info.env[idx]))
			return (idx);
		idx++;
	}
	return (idx);
}

void	remove_env(t_exec_info *info, size_t idx)
{
	char	**env;
	size_t	i;
	size_t	j;

	env = malloc(sizeof(char *) * g_info.env_cnt);
	if (!env)
	{
		err("error: malloc", info);
		return ;
	}
	i = 0;
	j = 0;
	while (i + 1 < g_info.env_cnt)
	{
		if (i == idx)
			j = 1;
		env[i] = g_info.env[i + j];
		g_info.env[i++ + j] = NULL;
	}
	free(g_info.env[idx]);
	free(g_info.env);
	g_info.env = env;
	g_info.env_cnt -= 1;
}

void	unset(t_node *node, t_exec_info *info)
{
	size_t	idx;

	idx = get_idx(node->cmd_args[1]);
	if (idx == g_info.env_cnt)
		return ;
	remove_env(info, idx);
}
