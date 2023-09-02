/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:38:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/09/02 15:55:21 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor_bonus.h"

void	check_subshell_parent(t_node *node, t_exec_info *info)
{
	int	status;

	if (node->type != AST_SUBSHELL)
		return ;
	waitpid(info->pid, &status, 0);
	info->pid = -1;
	info->fork_cnt--;
	info->exit_code = WEXITSTATUS(status);
	info->is_fork = FALSE;
}

void	check_subshell_child(t_node *node, t_exec_info *info)
{
	t_sh_info	subshell;
	int			exit_code;

	if (node->type != AST_SUBSHELL)
		return ;
	if (node->pipe_open & 1)
	{
		dup2(info->pipe[1], STDOUT_FILENO);
		close(info->pipe[1]);
		close(info->pipe[0]);
	}
	if ((node->pipe_open >> 1) & 1)
	{
		dup2(info->prev_pipe, STDIN_FILENO);
		close(info->prev_pipe);
	}
	subshell.ast = node->left;
	node->left = NULL;
	subshell.env_cnt = info->env_cnt;
	subshell.exit_code = info->prev_exit_code;
	exit_code = execute(&subshell);
	clear_all(info->ast);
	exit(exit_code);
}
