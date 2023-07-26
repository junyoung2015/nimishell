/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/24 21:24:18 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	execute_node(t_node *root, t_exec_info *info)
{
	if (root->type == AST_NULL)
		return ;
	else if (root->type == AST_PIPE)
		open_pipe(info);
	else if (root->type == AST_REDIR_IN || root->type == AST_REDIR_OUT \
				|| root->type == AST_REDIR_APPEND || root->type == AST_HEREDOC)
		redirection(root, info);
	else if (root->type == AST_CMD)
		command(root, info);
}

void	tree_search(t_node *root, t_exec_info *info)
{
	if (!root)
		return ;
	if (root->left)
		root->left->parent_type = root->type;
	if (root->right)
		root->right->parent_type = root->type;
	if (root->type != AST_CMD)
		execute_node(root, info);
	if (root->type == AST_PIPE)
	{
		root->left->pipe_open = root->pipe_open + 1;
		root->right->pipe_open = 2;
	}
	tree_search(root->left, info);
	tree_search(root->right, info);
	if (root->type == AST_CMD)
		execute_node(root, info);
}

void	execute_in_child(t_node *root)
{
	t_exec_info	info;
	size_t		i;
	int			status;

	info.fd_in = -1;
	info.fd_out = -1;
	info.exit_code = 0;
	info.fork_cnt = 0;
	info.prev_pipe = -1;
	tree_search(root, &info);
	clear_all(root);
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

int	execute_in_parent(t_node *root)
{
	t_exec_info	info;
	size_t		i;
	int			status;

	info.fd_in = -1;
	info.fd_out = -1;
	info.exit_code = 0;
	info.fork_cnt = 0;
	info.prev_pipe = -1;
	tree_search(root, &info);
	dup2(g_info.stdin_fd, STDIN_FILENO);
	dup2(g_info.stdout_fd, STDOUT_FILENO);
	clear_all(g_info.root);
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

	if (root->builtin != NOT_BUILTIN)
		return (execute_in_parent(root));
	pid = fork();
	if (pid < 0)
		return (err("error: fork"));
	else if (!pid)
		execute_in_child(root);
	clear_all(g_info.root);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
