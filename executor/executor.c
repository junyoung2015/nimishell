/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/27 01:42:18 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static void	init_exec_info(t_exec_info *info, t_sh_info *sh_info)
{
	info->ast = sh_info->ast;
	info->env_cnt = sh_info->env_cnt;
	info->prev_exit_code = sh_info->exit_code;
	info->fork_cnt = 0;
	info->prev_pipe = -1;
	info->stdin_fd = dup(STDIN_FILENO);
	info->stdout_fd = dup(STDOUT_FILENO);
	info->exit_code = EXIT_SUCCESS;
	info->is_fork = FALSE;
	info->fd_in = -1;
	info->fd_out = -1;
}

void	close_fd(t_exec_info *info)
{
	if (info->prev_pipe >= 0)
		close(info->prev_pipe);
	dup2(info->stdin_fd, STDIN_FILENO);
	dup2(info->stdout_fd, STDOUT_FILENO);
	close(info->stdin_fd);
	close(info->stdout_fd);
	if (info->fd_in >= 0)
		close(info->fd_in);
	if (info->fd_out >= 0)
		close(info->fd_out);
}

void	set_sh_info(t_exec_info *info, t_sh_info *sh_info)
{
	clear_all(sh_info->ast);
	sh_info->ast = NULL;
	sh_info->env_cnt = info->env_cnt;
}

int	get_exit_code(t_exec_info info)
{
	int			status;
	int			sig_num;
	
	if (info.pid > 0)
	{
		waitpid(info.pid, &status, 0);
		info.fork_cnt--;	
	}
	while (info.fork_cnt > 0)
	{
		wait(NULL);
		info.fork_cnt--;
	}
	if (WIFSIGNALED(status))
	{
		sig_num = WTERMSIG(status);
		if (sig_num == SIGINT)
			write(STD_ERR, SIGINT_ECHO, ft_strlen(SIGINT_ECHO));
		else if (sig_num == SIGQUIT)
			write(STD_ERR, SIGQUIT_ECHO, ft_strlen(SIGQUIT_ECHO));
		return (128 + sig_num);
	}
	if (info.pid > 0)
		return (WEXITSTATUS(status));
	return (info.exit_code);
}

int	executor(t_sh_info *sh_info)
{
	t_exec_info	info;	

	init_exec_info(&info, sh_info);
	ast_search(info.ast, &info);
	close_fd(&info);
	set_sh_info(&info, sh_info);
	if (!info.fork_cnt)
		return (info.exit_code);
	return (get_exit_code(info));
}
