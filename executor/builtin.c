#include "builtin.h"

static int	exit_err(char *arg)
{
	write(STDERR_FILENO, "minishell: exit: ", 17);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	return (255);
}

static int	_atoi(char *arg)
{
	unsigned long long	num;
	int					sign;
	size_t				i;

	num = 0;
	i = 0;
	sign = 1 - 2 * (arg[i] == '-');
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if ('0' <= arg[i] && arg[i] <= '9')
			num = num * 10 + arg[i++] - '0';
		else
			return (exit_err(arg));
		if (num > 9223372036854775808ULL || (num == 9223372036854775808ULL && sign > 0))
			return (exit_err(arg));		
	}
	return ((num * sign) % 256);
}

static void	__exit(t_node *node, t_exec_info *info)
{
	if (node->cmd_args[1])
		info->exit_code = _atoi(node->cmd_args[1]);
	clear_all(info->ast);
	exit(info->exit_code);
}

void	builtin(t_node *node, t_exec_info *info)
{
	if (node->builtin == NOT_BUILTIN)
		return ;
	else if (node->builtin == EXPORT)
		export(node, info);
	else if (node->builtin == ENV)
		env(node, info);
	else if (node->builtin == UNSET)
		unset(node, info);
	else if (node->builtin == PWD)
		pwd(info);
	else if (node->builtin == CD)
		cd(node, info);
	else if (node->builtin == EXIT)
		__exit(node, info);
	else if (node->builtin == ECHO_BUILTIN)
		echo(node);
}
