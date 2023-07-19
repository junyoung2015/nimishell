#include "builtin.h"

void    pwd(t_exec_info *info)
{
    char    *cwd;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
		info->exit_code = EXIT_FAILURE;
		perror("error: pwd");
		return ;
	}
    write(STDOUT_FILENO, cwd, ft_strlen(cwd));
    write(STDOUT_FILENO, "\n", 1);
    free(cwd);
}