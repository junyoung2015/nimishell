#include "builtin.h"

void	builtin(t_node *node, t_exec_info *info)
{
	if (!connect_pipe(node, info))
	{
		info->exit_code = EXIT_FAILURE;
		perror("error");
		return ;
	}
	if (node->builtin == EXPORT)
		export(node, info);
	else if (node->builtin == ENV)
		env();
}
