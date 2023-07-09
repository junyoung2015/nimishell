/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/09 18:23:25 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	run_cmd(t_node *root, t_pipe_info *info)
{
	if (root->type == AST_NULL)
		return ;
	else if (root->type == AST_PIPE)
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

void	save_std_fd(t_pipe_info *info)
{
	info->stdin_fd = dup(STDIN_FILENO);
	if (info->stdin_fd < 0)
		err();
	info->stdout_fd = dup(STDOUT_FILENO);
	if (info->stdin_fd < 0)
		err();
}

void	revert_std_fd(t_pipe_info *info)
{
	if (dup2(info->stdin_fd, STDIN_FILENO) < 0)
		err();
	if (dup2(info->stdout_fd, STDOUT_FILENO) < 0)
		err();
	close(info->stdin_fd);
	close(info->stdout_fd);
}

int	executor(t_node *root)
{
	int			status;
	t_pipe_info	info;
	size_t		i;

	info.fork_cnt = 0;
	info.prev_pipe_fd = -1;
	save_std_fd(&info);
	preorder_traversal(root, &info);
	revert_std_fd(&info);
	if (info.fork_cnt)
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
