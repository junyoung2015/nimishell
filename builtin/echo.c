#include "builtin.h"

/*

할일
2. env 모두 복사
3. exit | ls 안됨
4. echo | cat 안됨

*/

t_bool	is_option(char *str)
{
	if (!str)
		return (FALSE);
	if (*str++ != '-')
		return (FALSE);
	if (!*str)
		return (FALSE);
	while (*str == 'n')
		str++;
	if (*str)
		return (FALSE);
	return (TRUE);
}

void	echo(t_node *node)
{
	t_bool	option_n;
	size_t	i;

	option_n = is_option(node->cmd_args[1]);
	i = 1;
	if (option_n)
		i++;
	while (node->cmd_args[i])
	{
		write(STDOUT_FILENO, node->cmd_args[i], ft_strlen(node->cmd_args[i]));
		if (node->cmd_args[++i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (!option_n)
		write(STDOUT_FILENO, "\n", 1);
}