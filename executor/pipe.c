/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:37:25 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/09 20:53:31 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	open_pipe(t_pipe_info *info)
{
	if (pipe(info->pipe))
		err();
}

static void	connect_left(t_node *node, t_pipe_info *info)
{
	if (node->left && node->left->type == AST_REDIR_IN)
		redir_in(node->left);
	else if (node->left && node->left->type == AST_HEREDOC)
		heredoc(node->left, info);
	else if ((node->pipe_open >> 1) & 1)
		if (dup2(info->prev_pipe_fd, STDIN_FILENO) < 0)
			err();
}

static void	connect_right(t_node *node, t_pipe_info *info)
{
	if (node->right && node->right->type == AST_REDIR_OUT)
		redir_out(node->right);
	else if (node->right && node->right->type == AST_REDIR_APPEND)
		redir_append(node->right);
	else if ((node->pipe_open >> 0) & 1)
		if (dup2(info->pipe[1], STDOUT_FILENO) < 0)
			err();
}

void	connect_pipe(t_node *node, t_pipe_info *info)
{
	if (!node->pipe_open)
		return ;
	connect_left(node, info);
	connect_right(node, info);
	close(info->prev_pipe_fd);
	close(info->pipe[0]);
	close(info->pipe[1]);
}

void	close_pipe(t_node *node, t_pipe_info *info)
{
	if (!node->pipe_open)
		return ;
	if ((node->pipe_open >> 0) & 1)
	{
		if (info->prev_pipe_fd < 0)
		{
			info->prev_pipe_fd = dup(info->pipe[0]);
			if (info->prev_pipe_fd < 0)
				err();
		}
		else
		{
			if (dup2(info->pipe[0], info->prev_pipe_fd) < 0)
				err();
		}
	}
	close(info->pipe[0]);
	close(info->pipe[1]);
}
