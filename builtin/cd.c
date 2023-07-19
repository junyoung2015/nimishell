#include "builtin.h"

void    cd(t_node *node, t_exec_info *info)
{
    if (chdir(node->cmd_args[1]) < 0)
	{
		info->exit_code = EXIT_FAILURE;
		perror("error: cd");
	}
}