/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/04 20:04:43 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	run_cmd(t_node *root, t_pipe_info *info)
{
	if (root->type == AST_PIPE)
		open_pipe(info);
	else if (root->type == AST_REDIR_IN)
		redir_in(root);
	else if (root->type == AST_REDIR_OUT)
		redir_out(root);
	else if (root->type == AST_REDIR_APPEND)
		redir_append(root);
	else if (root->type == AST_HEREDOC)
		heredoc(root, info);
	else if (root->type == AST_COMMAND)
		command(root, info);
}

void	preorder_traversal(t_node *root, t_pipe_info *info)
{
	if (!root)
		return ;
	run_cmd(root, info);
	if (root->type == AST_PIPE)
	{
		root->left->pipe_open = root->pipe_open + 1;
		root->right->pipe_open = 2;
	}
	preorder_traversal(root->left, info);
	preorder_traversal(root->right, info);
}

int	executor(t_node *root)
{
	int			status;
	t_pipe_info	info;
	size_t		i;

	info.fork_cnt = 0;
	info.prev_pipe_fd = -1;
	preorder_traversal(root, &info);
	waitpid(info.pid, &status, 0);
	i = 0;
	while (i + 1 < info.fork_cnt)
	{
		wait(NULL);
		i++;
	}
	clear_all(g_info.root);
	return (status);
}
