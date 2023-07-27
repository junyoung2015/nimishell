/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 21:50:47 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/27 21:32:10 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	redir_in(t_node *node, t_exec_info *info)
{
	info->fd_in = open(node->cmd_args[0], O_RDONLY);
	if (info->fd_in < 0)
		return (1);
	return (0);
}

int	redir_out(t_node *node, t_exec_info *info)
{
	info->fd_out = open(node->cmd_args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (info->fd_out < 0)
		return (1);
	return (0);
}

int	redir_append(t_node *node, t_exec_info *info)
{
	info->fd_out = open(node->cmd_args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (info->fd_out < 0)
		return (1);
	return (0);
}

static void	redir_err(char *str, t_exec_info *info)
{
	if (info->is_fork)
		exit(err(str));
	info->exit_code = EXIT_FAILURE;
	perror(str);	
}

void	redirection(t_node *node, t_exec_info *info)
{
	int	is_err;

	if (info->exit_code != EXIT_SUCCESS)
		return ;
	is_err = 0;
	if (node->type == AST_REDIR_IN)
		is_err = redir_in(node, info);
	else if (node->type == AST_REDIR_OUT)
		is_err = redir_out(node, info);
	else if (node->type == AST_REDIR_APPEND)
		is_err = redir_append(node, info);
	else if (node->type == AST_HEREDOC)
		is_err = heredoc(node, info);
	else
		return ;
	if (is_err == 1)
		redir_err("error: open", info);
	else if (is_err == 2)
		redir_err("error", info);
}
