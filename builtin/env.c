#include "builtin.h"

void	env(t_node *node, t_exec_info *info)
{
	size_t	i;

	i = 0;
	if (node->cmd_args[1] && node->cmd_args[1][0] != '#')
	{
		write(STDERR_FILENO, "env: ", 6);
		write(STDERR_FILENO, node->cmd_args[1], ft_strlen(node->cmd_args[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		info->exit_code = EXIT_CMD_NOT_FOUND;
		return ;
	}
	while (g_info.env && g_info.env[i])
	{
		write(STDOUT_FILENO, g_info.env[i], ft_strlen(g_info.env[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}
