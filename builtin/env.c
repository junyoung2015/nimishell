#include "builtin.h"

void	env(void)
{
	size_t	i;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		write(STDOUT_FILENO, g_info.env[i], ft_strlen(g_info.env[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}
