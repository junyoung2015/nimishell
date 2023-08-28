/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:10:56 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:11:00 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	set_signal(pid_t pid, t_bool flag)
{
	if (flag)
		return ;
	if (!pid)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
	}
}

void    set_parent_signal(void)
{
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sig_handler);
}
