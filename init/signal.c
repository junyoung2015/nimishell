#include "minishell.h"

void sig_handler(int signal)
{	
	if (signal == SIGINT)
	{
		write(STD_OUT, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	set_ignore_signal(void)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
}

void    set_child_signal(void)
{
    signal(SIGQUIT, SIG_DFL);
    signal(SIGINT, SIG_DFL);
}

void    set_parent_signal(void)
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sig_handler);
}
