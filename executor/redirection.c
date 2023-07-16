/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 21:50:47 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/16 21:46:26 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	redir_in(t_node *node)
{
	int	fd;

	fd = open(node->cmd_args[0], O_RDONLY);
	if (fd < 0)
		return (0);
	if (dup2(fd, STDIN_FILENO) < 0)
		return (0);
	return (1);
}

int	redir_out(t_node *node)
{
	int	fd;

	fd = open(node->cmd_args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (0);
	return (1);
}

int	redir_append(t_node *node)
{
	int	fd;

	fd = open(node->cmd_args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (0);
	if (dup2(fd, STDOUT_FILENO) < 0)
		return (0);
	return (1);
}

void	redirection(t_node *node, t_exec_info *info)
{
	int	success;
	
	if (node->parent_type == AST_COMMAND || node->parent_type == AST_BUILTIN)
		return ;
	if (node->type == AST_REDIR_IN)
		success = redir_in(node);
	else if (node->type == AST_REDIR_OUT)
		success = redir_out(node);
	else if (node->type == AST_REDIR_APPEND)
		success = redir_append(node);
	else if (node->type == AST_HEREDOC)
		success = heredoc(node, info);
	if (!success)
		err();
}