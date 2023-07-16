/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/16 22:35:28 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	run_cmd(t_node *root, t_exec_info *info)
{
	if (root->type == AST_NULL)
		return ;
	else if (root->type == AST_PIPE)
		open_pipe(info);
	else if (root->type == AST_REDIR_IN || root->type == AST_REDIR_OUT \
				|| root->type == AST_REDIR_APPEND || root->type == AST_HEREDOC)
		redirection(root, info);
	else if (root->type == AST_COMMAND)
		command(root, info);
	else if (root->type == AST_BUILTIN)
		builtin(root, info);
}

void	preorder_traversal(t_node *root, t_exec_info *info)
{
	if (!root)
		return ;
	if (root->left)
		root->left->parent_type = root->type;
	if (root->right)
		root->right->parent_type = root->type;
	run_cmd(root, info);
	if (root->type == AST_PIPE)
	{
		root->left->pipe_open = root->pipe_open + 1;
		root->right->pipe_open = 2;
	}
	preorder_traversal(root->left, info);
	preorder_traversal(root->right, info);
}

void	execute_in_child(t_node *root)
{
	t_exec_info	info;
	size_t		i;
	int			status;

	info.fork_cnt = 0;
	info.prev_pipe = -1;
	preorder_traversal(root, &info);
	if (!info.fork_cnt)
		exit(EXIT_SUCCESS);
	waitpid(info.pid, &status, 0);
	i = 0;
	while (i + 1 < info.fork_cnt)
	{
		wait(NULL);
		i++;
	}
	exit(WEXITSTATUS(status));
}

int	execute(t_node *root)
{
	t_exec_info	info;
	size_t		i;
	int			status;

	info.exit_code = 0;
	info.fork_cnt = 0;
	info.prev_pipe = -1;
	preorder_traversal(root, &info);
	dup2(g_info.stdin_fd, STDIN_FILENO);
	dup2(g_info.stdout_fd, STDOUT_FILENO);
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

int	executor(t_node *root)
{
	pid_t	pid;
	int		status;

	if (root->type == AST_BUILTIN)
		return (execute(root));
	pid = fork();
	if (pid < 0)
		err();
	else if (!pid)
		execute_in_child(root);
	waitpid(pid, &status, 0);
	clear_all(g_info.root);
	return (WEXITSTATUS(status));
}
