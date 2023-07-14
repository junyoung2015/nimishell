#include "builtin.h"

void	builtin(t_node *node, t_pipe_info *info)
{
	if (node->builtin == EXPORT)
		export(node);
}
