/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:37:25 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/27 14:42:17 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	open_pipe(t_node *node, t_exec_info *info)
{
	if (node->type != AST_PIPE)
		return ;
	node->left->pipe_open = node->pipe_open + 1;
	node->right->pipe_open = 2;
	if (pipe(info->pipe))
		err("minishell: pipe:", info);
}

int	connect_pipe(t_node *node, t_exec_info *info)
{
	if (info->fd_in < 0 && (node->pipe_open >> 1) & 1)
		info->fd_in = info->prev_pipe;
	if (info->fd_out < 0 && node->pipe_open & 1)
		info->fd_out = info->pipe[1];
	if (info->fd_in >= 0)
	{
		if (dup2(info->fd_in, STDIN_FILENO) < 0)
			return (0);
	}
	if (info->fd_out >= 0)
	{
		if (dup2(info->fd_out, STDOUT_FILENO) < 0)
			return (0);
	}
	if (node->pipe_open)
	{
		close(info->prev_pipe);
		close(info->pipe[0]);
		close(info->pipe[1]);
	}
	return (1);
}

void	close_pipe(t_node *node, t_exec_info *info)
{
	if (!node->pipe_open)
		return ;
	if ((node->pipe_open >> 0) & 1)
	{
		if (info->prev_pipe < 0)
		{
			info->prev_pipe = dup(info->pipe[0]);
			if (info->prev_pipe < 0)
				err("minishell: dup", info);
		}
		else
		{
			if (dup2(info->pipe[0], info->prev_pipe) < 0)
				err("minishell: dup2", info);
		}
	}
	close(info->pipe[0]);
	close(info->pipe[1]);
}
