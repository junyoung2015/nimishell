/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 21:50:47 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/03 15:15:36 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	redir_in(t_node *node)
{
	int	fd;
	
	if (node->parent_type == AST_COMMAND)
		return ;
	fd = open_rdonly(node->cmd_args[0]);
	if (dup2(fd, STDIN_FILENO) < 0)
		err();
}

void	redir_out(t_node *node)
{
	int	fd;
	
	if (node->parent_type == AST_COMMAND)
		return ;
	fd = open_wronly_trunc(node->cmd_args[0]);
	if (dup2(fd, STDOUT_FILENO) < 0)
		err();
}

void	redir_append(t_node *node)
{
	int	fd;
	
	if (node->parent_type == AST_COMMAND)
		return ;
	fd = open_wronly_trunc(node->cmd_args[0]);
	if (dup2(fd, STDOUT_FILENO) < 0)
		err();
}
