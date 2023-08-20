/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:55:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/20 22:46:14 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	validate_path(char *cmdpath, char *cmd, t_exec_info *info)
{
	if (!cmdpath)
	{	
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd, ft_strlen(cmd));
		write(STDERR_FILENO, ": command not found\n", 20);
		clear_all(info->ast);
		exit(EXIT_CMD_NOT_FOUND);
	}
	else if (access(cmdpath, X_OK) < 0)
	{
		if (cmd[0] != '/' && !(cmd[0] == '.' && cmd[1] == '/'))
			free(cmdpath);
		write(STDERR_FILENO, "minishell: ", 11);
		err_exit(info, cmd, EXIT_NOT_EXECUTABLE);
	}
	// is_directory();
}

void	command_in_child(t_node *node, t_exec_info *info)
{
	char	*cmdpath;

	if (!connect_pipe(node, info))
		err_exit(info, "minishell: dup2", EXIT_FAILURE);
	if (node->builtin == NOT_BUILTIN)
	{
		cmdpath = get_cmdpath(node->cmd_args[0], info);
		validate_path(cmdpath, node->cmd_args[0], info);
		if (execve(cmdpath, node->cmd_args, g_env) < 0)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			err_exit(info, node->cmd_args[0], EXIT_CMD_NOT_FOUND);
		}
	}
	builtin(node, info);
	clear_all(info->ast);
	exit(info->exit_code);
}

void	command(t_node *node, t_exec_info *info)
{
	if (node->type != AST_CMD)
		return ;
	if (info->is_fork)
		command_in_child(node, info);
	else
	{
		if (info->exit_code != EXIT_SUCCESS)
			return ;
		if (!connect_pipe(node, info))
		{
			err("minishell: dup2", info);
			return ;
		}
		builtin(node, info);
	}
}
