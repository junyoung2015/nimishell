/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 21:50:47 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/30 01:31:34 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	redir_in(t_node *node, t_exec_info *info)
{
	if (info->fd_in >= 0)
		close(info->fd_in);
	info->fd_in = open(node->cmd_args[0], O_RDONLY);
	if (info->fd_in < 0)
		return (1);
	return (0);
}

int	redir_out(t_node *node, t_exec_info *info, int opt)
{
	if (info->fd_out >= 0)
		close(info->fd_out);
	info->fd_out = open(node->cmd_args[0], O_WRONLY | O_CREAT | opt, 0644);
	if (info->fd_out < 0)
		return (1);
	return (0);
}

void	redir_change(t_node *node, t_exec_info *info)
{
	if (info->fd_in >= 0)
		close(info->fd_in);
	info->fd_in = node->fd;
}

static void	redir_err(char *filename, t_exec_info *info)
{
	write(STDERR_FILENO, MINISHELL, 11);
	perror(filename);
	info->exit_code = EXIT_FAILURE;
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
		is_err = redir_out(node, info, O_TRUNC);
	else if (node->type == AST_REDIR_APPEND)
		is_err = redir_out(node, info, O_APPEND);
	else if (node->type == AST_HEREDOC)
		redir_change(node, info);
	else
		return ;
	if (is_err)
		redir_err(node->cmd_args[0], info);
	if (info->exit_code != EXIT_SUCCESS && info->is_fork)
	{
		clear_all(info->ast);
		exit(info->exit_code);
	}
}
