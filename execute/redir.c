/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 21:50:47 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/01 22:05:52 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	redir_in(t_node *node, t_pipe_info *info)
{
	info->pid = fork();
	if (info->pid < 0)
		err();
	else if (!info->pid)
	{
		info->fd = open_rdonly(node->cmd_args[0]);
		connect_pipe(node, info);
		ft_execve(node->left);
	}
	close_pipe(node, info);
	info->fork_cnt += 1;
	free_ptr(node->left->cmd_args);
	free(node->left);
	node->left = NULL;
}

void	redir_out(t_node *node, t_pipe_info *info)
{
	info->pid = fork();
	if (info->pid < 0)
		err();
	else if (!info->pid)
	{
		info->fd = open_wronly_trunc(node->cmd_args[0]);
		connect_pipe2(node, info);
		ft_execve(node->left);
	}
	close_pipe(node, info);
	info->fork_cnt += 1;
	free_ptr(node->left->cmd_args);
	free(node->left);
	node->left = NULL;
}

void	redir_append(t_node *node, t_pipe_info *info)
{
	info->pid = fork();
	if (info->pid < 0)
		err();
	else if (!info->pid)
	{
		info->fd = open_wronly_append(node->cmd_args[0]);
		connect_pipe2(node, info);
		ft_execve(node->left);
	}
	close_pipe(node, info);
	info->fork_cnt += 1;
	free_ptr(node->left->cmd_args);
	free(node->left);
	node->left = NULL;
}
