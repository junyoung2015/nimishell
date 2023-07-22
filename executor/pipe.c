/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 16:37:25 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/22 17:38:06 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	open_pipe(t_exec_info *info)
{
	if (pipe(info->pipe))
		exit(err("error: pipe:"));
}

static int	connect_left(t_node *node, t_exec_info *info)
{
	if (node->left && node->left->type == AST_REDIR_IN)
	{
		if (!redir_in(node->left))
			return (0);
	}
	else if (node->left && node->left->type == AST_HEREDOC)
	{
		if (!heredoc(node->left, info))
			return (0);
	}
	else if ((node->pipe_open >> 1) & 1)
	{
		if (dup2(info->prev_pipe, STDIN_FILENO) < 0)
			return (0);
	}
	return (1);
}

static int	connect_right(t_node *node, t_exec_info *info)
{
	if (node->right && node->right->type == AST_REDIR_OUT)
	{
		if (!redir_out(node->right))
			return (0);
	}
	else if (node->right && node->right->type == AST_REDIR_APPEND)
	{
		if (!redir_append(node->right))
			return (0);
	}
	else if ((node->pipe_open >> 0) & 1)
	{
		if (dup2(info->pipe[1], STDOUT_FILENO) < 0)
			return (0);
	}
	return (1);
}

int	connect_pipe(t_node *node, t_exec_info *info)
{
	if (!connect_left(node, info))
		return (0);
	if (!connect_right(node, info))
		return (0);
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
				exit(err("error: dup"));
		}
		else
		{
			if (dup2(info->pipe[0], info->prev_pipe) < 0)
				exit(err("error: dup2"));
		}
	}
	close(info->pipe[0]);
	close(info->pipe[1]);
}
