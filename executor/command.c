/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:28 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/02 16:45:20 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	command(t_node *node, t_pipe_info *info)
{
	// int		fd;

	info->pid = fork();
	if (info->pid < 0)
		err();
	else if (!info->pid)
	{
		connect_pipe(node, info);
		ft_execve(node);
	}
	close_pipe(node, info);
	info->fork_cnt += 1;
}