#include "executor.h"

t_bool is_stop(t_node *node, t_exec_info *info)
{
	int status;

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

pid_t is_fork(t_node *node, t_exec_info *info)
{
	pid_t pid;

	if (node->type != AST_CMD || (node->builtin != NOT_BUILTIN && !node->pipe_open))
		return (0);
	pid = fork();
	if (pid < 0)
		err("minishell: fork", info);
	else
	{
		set_signal(pid, 0);
		info->fork_cnt += 1;
		info->is_fork = TRUE;
		info->pid = pid;
		if (pid > 0)
			close_pipe(node, info);
	}
	return (pid);
}

void reset_info(t_node *node, t_exec_info *info)
{
	if (node->type != AST_CMD)
		return;
	info->exit_code = EXIT_SUCCESS;
	info->is_fork = FALSE;
	info->fd_in = -1;
	info->fd_out = -1;
	if (node->builtin != NOT_BUILTIN)
	{
		dup2(info->stdin_fd, STDIN_FILENO);
		dup2(info->stdout_fd, STDOUT_FILENO);
	}
}

void ast_search(t_node *root, t_exec_info *info)
{
	if (!root)
		return;
	if (root->cmd_args && root->type != AST_HEREDOC)
	{
		if (root->pipe_open > 1)
			info->prev_exit_code = 0;
		root->cmd_args = env_substitution(root, info);
		root->cmd_args = str_expansion(root);
		root->cmd_args = remove_quotes(root);
		if (root->cmd_args)
			is_builtin_node(root);
	}
	if (root->type == AST_CMD && !root->cmd_args)
		err("minishell: malloc", info);
	open_pipe(root, info);
	reset_info(root, info);
	if (is_fork(root, info))
		return;
	redirection(root, info);
	ast_search(root->left, info);
	if (is_stop(root, info))
		return;
	ast_search(root->right, info);
	command(root, info);
}
