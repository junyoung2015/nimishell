#include "builtin.h"

void	export_err(t_exec_info *info)
{
	write(STDERR_FILENO, "error: export: not a valid identifier\n", 38);
	info->exit_code = EXIT_FAILURE;
	return ;
}