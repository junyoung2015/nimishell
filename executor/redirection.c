/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 21:50:47 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/04 18:53:51 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	redir_in(t_node *node)
{
	int	fd;
	
	if (node->parent_type == AST_COMMAND)
		return ;
	fd = open(node->cmd_args[0], O_RDONLY);
	if (fd < 0)
		err();
	if (dup2(fd, STDIN_FILENO) < 0)
		err();
}

void	redir_out(t_node *node)
{
	int	fd;
	
	if (node->parent_type == AST_COMMAND)
		return ;
	fd = open(node->cmd_args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		err();
	if (dup2(fd, STDOUT_FILENO) < 0)
		err();
}

void	redir_append(t_node *node)
{
	int	fd;
	
	if (node->parent_type == AST_COMMAND)
		return ;
	fd = open(node->cmd_args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		err();
	if (dup2(fd, STDOUT_FILENO) < 0)
		err();
}

void	check_redir(t_node *node, t_pipe_info *info)
{
	if (node->left && node->left->type == AST_REDIR_IN)
		redir_in(node->left);
	else if (node->left && node->left->type == AST_HEREDOC)
		heredoc(node->left, info);
	if (node->right && node->right->type == AST_REDIR_OUT)
		redir_out(node->right);
	else if (node->right && node->right->type == AST_REDIR_APPEND)
		redir_append(node->right);
}