#include "builtin.h"

static int	_atoi(char *str)
{
	int	num;

	num = 0;
	while (str && *str)
	{
		if ('0' <= *str && *str <= '9')
			num = num * 10 + *str++ - '0';
		else
		{
			write(STDERR_FILENO, "error: exit: numeric argument required\n", 39);
			return (255);
		}
	}
	return (num % 256);
}

static void	__exit(t_node *node, t_exec_info *info)
{
	if (node->parent_type == AST_PIPE)
		return ;
	if (node->cmd_args[1])
		info->exit_code = _atoi(node->cmd_args[1]);
	clear_all(g_info.root);
	exit(info->exit_code);
}

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
	else if (node->builtin == UNSET)
		unset(node, info);
	else if (node->builtin == PWD)
		pwd(info);
	else if (node->builtin == CD)
		cd(node, info);
	else if (node->builtin == EXIT)
		__exit(node, info);
	else if (node->builtin == ECHO)
		echo(node);
}
