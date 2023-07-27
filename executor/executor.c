/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/27 21:17:40 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

t_bool	is_stop(t_node *node, t_exec_info *info)
{
	int	status;
	
	if (node->type != AST_AND && node->type != AST_OR)
		return (FALSE);
	if (info->is_fork)
	{
		waitpid(info->pid, &status, 0);
		info->fork_cnt -= 1;
		info->exit_code = WEXITSTATUS(status);
	}
	info->is_fork = FALSE;
	if (info->exit_code != EXIT_SUCCESS && node->type == AST_AND)
		return (TRUE);
	if (info->exit_code == EXIT_SUCCESS && node->type == AST_OR)
		return (TRUE);
	return (FALSE);
}

pid_t	_fork(t_node *node, t_exec_info *info)
{
	pid_t	pid;
	
	pid = fork();
	if (pid < 0)
		perror("error: fork");
	else
	{
		info->fork_cnt += 1;
		info->is_fork = TRUE;
		info->pid = pid;
		if (pid > 0)
			close_pipe(node, info);
	}
	return (pid);
}

void	tree_search(t_node *root, t_exec_info *info)
{
	pid_t	pid;
	
	if (!root)
		return ;
	if (root->left)
		root->left->parent_type = root->type;
	if (root->right)
		root->right->parent_type = root->type;
	open_pipe(root, info);
	pid = 0;
	if (root->parent_type == AST_PIPE || (root->type == AST_CMD && root->builtin == NOT_BUILTIN))
		pid = _fork(root, info);
	if (pid)
		return ;
	redirection(root, info);
	tree_search(root->left, info);
	if (is_stop(root, info))
		return ;
	tree_search(root->right, info);
	command(root, info);
}

void	init_info(t_exec_info *info)
{
	info->is_fork = FALSE;
	info->fd_in = -1;
	info->fd_out = -1;
	info->exit_code = EXIT_SUCCESS;
	info->fork_cnt = 0;
	info->prev_pipe = -1;
}

int	executor(t_node *root)
{
	t_exec_info	info;	
	size_t		i;
	int			status;

	init_info(&info);
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
