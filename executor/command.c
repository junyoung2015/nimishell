/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:28 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/03 15:12:41 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	command(t_node *node, t_pipe_info *info)
{
	info->pid = fork();
	if (info->pid < 0)
		err();
	else if (!info->pid)
	{
		connect_pipe(node, info);
		if (node->left && node->left->type == AST_REDIR_IN)
			redir_in(node->left);
		if (node->right && node->right->type == AST_REDIR_OUT)
			redir_out(node->right);
		if (node->right && node->right->type == AST_REDIR_APPEND)
			redir_append(node->right);
		ft_execve(node);
	}
	close_pipe(node, info);
	if (node->left)
		node->left->parent_type = AST_COMMAND;
	if (node->right)
		node->right->parent_type = AST_COMMAND;
	info->fork_cnt += 1;
}
