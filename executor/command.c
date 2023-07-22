/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 21:55:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/22 17:46:32 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"


void	command_after_fork(t_node *node, t_exec_info *info)
{
	char	*cmdpath;

	info->pid = fork();
	if (info->pid < 0)
		exit(err("error: fork"));
	else if (!info->pid)
	{
		if (!connect_pipe(node, info))
			exit(err("error"));
		if (node->builtin == NOT_BUILTIN)
		{
			cmdpath = get_cmdpath(node->cmd_args[0]);
			if (execve(cmdpath, node->cmd_args, g_info.env) < 0)
				exit(err("error: execve"));
		}
		builtin(node, info);
		clear_all(g_info.root);
		return ;
	}
	close_pipe(node, info);
	info->fork_cnt += 1;
}

void	command(t_node *node, t_exec_info *info)
{
	if (node->builtin == NOT_BUILTIN || node->parent_type == AST_PIPE)
		command_after_fork(node, info);
	else
	{
		if (!connect_pipe(node, info))
		{
			info->exit_code = EXIT_FAILURE;
			perror("error");
			return ;
		}
		builtin(node, info);
	}
}
