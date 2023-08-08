/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:55:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/08 18:05:44 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


void	command_in_child(t_node *node, t_exec_info *info)
{
	char	*cmdpath;

	if (!connect_pipe(node, info))
		err_exit(EXIT_FAILURE, 0, info);
	if (node->builtin == NOT_BUILTIN)
	{
		cmdpath = get_cmdpath(node->cmd_args[0], info);
		if (access(cmdpath, F_OK) < 0)
			err_exit(EXIT_CMD_NOT_FOUND, cmdpath, info);
		if (access(cmdpath, X_OK) < 0)
			err_exit(EXIT_NOT_EXECUTABLE, cmdpath, info);
		if (execve(cmdpath, node->cmd_args, g_info.env) < 0)
			err_exit(EXIT_NOT_EXECUTABLE, cmdpath, info);
	}
	builtin(node, info);
	clear_all(g_info.root);
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
			err("error: dup2", info);
			return ;
		}
		builtin(node, info);
	}
}
