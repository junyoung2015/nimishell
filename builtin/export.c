#include "builtin.h"

int	is_in_env(char *key, char *env)
{

}

int	get_pos(char *key)
{
	int	i;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		if (is_in_env(key, g_info.env[i]))
			return (i);
	}
	return (-1);
}

void	export(t_node *node, t_pipe_info info)
{
	int	idx;

	idx = get_pos(node->cmd_args[0]);
}
