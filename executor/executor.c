/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:19 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/02 21:25:16 by jusohn           ###   ########.fr       */
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
		info->is_fork = FALSE;
	}
	if (info->exit_code != EXIT_SUCCESS && node->type == AST_AND)
		return (TRUE);
	if (info->exit_code == EXIT_SUCCESS && node->type == AST_OR)
		return (TRUE);
	return (FALSE);
}

pid_t	is_fork(t_node *node, t_exec_info *info)
{
	pid_t	pid;
	
	if (node->type != AST_CMD
		|| (node->builtin != NOT_BUILTIN && node->parent_type != AST_PIPE))
		return (0);
	pid = fork();
	if (pid < 0)
		err("error: fork", info);
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

void	init_info(t_node *node, t_exec_info *info)
{
	if (node->type != AST_CMD && node->parent_type != AST_NULL)
		return ;
	info->exit_code = EXIT_SUCCESS;
	info->is_fork = FALSE;
	info->fd_in = -1;
	info->fd_out = -1;
	if (node->builtin != NOT_BUILTIN)
	{
		dup2(g_info.stdin_fd, STDIN_FILENO);
		dup2(g_info.stdout_fd, STDOUT_FILENO);
	}
}

void	tree_search(t_node *root, t_exec_info *info)
{
	if (!root)
		return ;
	if (root->cmd_args && root->type != AST_HEREDOC)
	{
		root->cmd_args = env_substitution(root);
		for (t_size i = 0; i < root->num_args; i++)
			printf("[ENV]cmd_args[%llu]: %s\n", i, root->cmd_args[i]);
		// root->cmd_args = wildcard_substitution(root);
		// for (t_size i = 0; i < root->num_args; i++)
		// 	printf("[WILD]cmd_args[%llu]: %s\n", i, root->cmd_args[i]);
		root->cmd_args = str_expansion(root);
		for (t_size i = 0; i < root->num_args; i++)
			printf("[STR_EXP]cmd_args[%llu]: %s\n", i, root->cmd_args[i]);
		root->cmd_args = remove_quotes(root);
		for (t_size i = 0; i < root->num_args; i++)
			printf("[QUOTE]cmd_args[%llu]: %s\n", i, root->cmd_args[i]);
		if (root->cmd_args)
			is_builtin_node(root);
	}
	if (root->type == AST_CMD && !root->cmd_args)
		err("error: malloc", info);
	if (root->left)
		root->left->parent_type = root->type;
	if (root->right)
		root->right->parent_type = root->type;
	open_pipe(root, info);
	init_info(root, info);
	if (is_fork(root, info))
		return ;
	redirection(root, info);
	tree_search(root->left, info);
	if (is_stop(root, info))
		return ;
	tree_search(root->right, info);
	command(root, info);
}

int	executor(t_node *root)
{
	t_exec_info	info;	
	size_t		i;
	int			status;

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
