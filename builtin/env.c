/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 19:52:09 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/28 19:52:10 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

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
