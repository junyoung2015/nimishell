#include "builtin.h"

t_bool ft_isalpha(char c)
{
	if ('A' <= c && c <= 'Z')
		return (TRUE);
	if ('a' <= c && c <= 'z')
		return (TRUE);
	return (FALSE);
}

t_bool ft_isalnum(char c)
{
	if (ft_isalpha(c))
		return (TRUE);
	if ('0' <= c && c <= '9')
		return (TRUE);
	return (FALSE);
}

int arg_check(char *str, t_builtin func)
{
	size_t	i;

	if (str[0] == '#')
		return (0);
	if (!ft_isalpha(str[0]))
		return (-1);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (-1);
		i++;
	}
	if (str[i] == '=')
		return (2 - 3 * (func == UNSET));
	return (1 + (func == UNSET));
}

void	arg_err(char *func_name, char *arg, t_exec_info *info)
{
	write(STDERR_FILENO, "error: ", 7);
	write(STDERR_FILENO, func_name, ft_strlen(func_name));
	write(STDERR_FILENO, ": '", 3);
	write(STDERR_FILENO, arg, ft_strlen(arg)); 
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	info->exit_code = EXIT_FAILURE;
}

