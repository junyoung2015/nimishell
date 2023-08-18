/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/17 17:07:41 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	init_info(t_exec_info *info, t_node *ast, size_t env_cnt)
{
	info->ast = ast;
	info->env_cnt = env_cnt;
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

int	executor(t_node *ast, size_t *env_cnt)
{
	t_exec_info	info;	
	size_t		i;
	int			status;

	init_info(&info, ast, *env_cnt);
	ast_search(ast, &info);
	close_fd(&info);
	clear_all(ast);
	*env_cnt = info.env_cnt;
	if (!info.fork_cnt)
		return (info.exit_code);
	waitpid(info.pid, &status, 0);
	i = 0;
	while (i + 1 < info.fork_cnt)
	{
		wait(NULL);
		i++;
	}
	return (WEXITSTATUS(status));
}
