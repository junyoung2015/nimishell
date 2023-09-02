/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 19:52:09 by sejinkim          #+#    #+#             */
/*   Updated: 2023/09/02 15:54:52 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_bonus.h"

void	env(t_node *node, t_exec_info *info)
{
	size_t	i;

	i = 0;
	if (node->cmd_args[1] && node->cmd_args[1][0] != '#')
	{
		write(STDERR_FILENO, "env: ", 6);
		write(STDERR_FILENO, node->cmd_args[1], ft_strlen(node->cmd_args[1]));
		write(STDERR_FILENO, ": No such file or directory\n", 28);
		info->exit_code = EXIT_CMD_NOT_FOUND;
		return ;
	}
	while (g_env && g_env[i])
	{
		write(STDOUT_FILENO, g_env[i], ft_strlen(g_env[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}
