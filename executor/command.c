/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:55:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/27 20:44:38 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


void	command_in_child(t_node *node, t_exec_info *info)
{
	char	*cmdpath;

	if (!connect_pipe(node, info))
		exit(err("error: dup2"));
	if (node->builtin == NOT_BUILTIN)
	{
		cmdpath = get_cmdpath(node->cmd_args[0]);
		if (execve(cmdpath, node->cmd_args, g_info.env) < 0)
			exit(err("error: execve"));
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
			info->exit_code = EXIT_FAILURE;
			perror("error: dup2");
			return ;
		}
		builtin(node, info);
	}
}
