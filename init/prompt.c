#include "minishell.h"

void	get_git_branch(char *branch)
{
	int     fd;
	t_ssize	size;

	if (access(GIT_BRANCH, F_OK) == -1)
		ft_strlcpy(branch, "", 2);
	else
	{
		fd = open(GIT_BRANCH, O_RDONLY);
		if (fd == -1)
			ft_strlcpy(branch, "", 2);
		else
		{
			size = read(fd, branch, MAX_BRANCH_LEN);
			close(fd);
			if (size == -1 || size == 0)
				branch = 0;
			else
				branch[size - 1] = 0;
			if (ft_strncmp(branch, REF_HEAD, ft_strlen(REF_HEAD)) == 0)
				ft_strlcpy(branch, branch + 16, ft_strlen(branch) - 15);
			else
				ft_strlcpy(branch, DETACHED_HEAD, ft_strlen(DETACHED_HEAD) + 1);
		}
	}
}

char    *get_prompt(void)
{
    char    *tmp;
    char    *pwd;
    char	branch[MAX_BRANCH_LEN + 1];

    tmp = getcwd(NULL, 0);
    if (!tmp)
        return (0);
    get_git_branch(branch);
    if (*branch)
    {
        pwd = ft_strjoin(tmp, " \033[0;35;40m[\033[0;32;40m");
        free(tmp);
        tmp = ft_strjoin(pwd, branch);
        free(pwd);
        pwd = ft_strjoin(tmp, "\033[0;35;40m]\033[0m ");
    }
    else
        pwd = ft_strjoin(tmp, "> ");
    free(tmp);
    return (pwd);
}
