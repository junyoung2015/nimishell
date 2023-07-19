#include "builtin.h"

void	echo(t_node *node)
{
	t_bool	option_n;
	size_t	i;

	option_n = FALSE;
	i = 1;
	if (node->cmd_args[1] && node->cmd_args[1][0] == '-' && node->cmd_args[1][1] == 'n')
	{
		option_n = TRUE;
		i++;
	}
	while (node->cmd_args[i])
	{
		write(STDOUT_FILENO, node->cmd_args[i], ft_strlen(node->cmd_args[i]));
		if (node->cmd_args[++i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!option_n)
		write(STDOUT_FILENO, "\n", 1);
}