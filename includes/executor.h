/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:04:54 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/16 21:46:40 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "minishell.h"

typedef struct s_exec_info
{
	size_t	fork_cnt;
	pid_t	pid;
	int		exit_code;
	int		pipe[2];
	int		prev_pipe;
}	t_exec_info;

/* pipe.c */
void	open_pipe(t_exec_info *info);
int		connect_pipe(t_node *node, t_exec_info *info);
void	close_pipe(t_node *node, t_exec_info *info);

/* redirection.c */
int		redir_in(t_node *node);
int		redir_out(t_node *node);
int		redir_append(t_node *node);
void	redirection(t_node *node, t_exec_info *info);

/* error.c */
void	free_ptr(char **ptr);
void	clear_all(t_node *root);
void	err(void);
void	cmd_not_found(void);

void	command(t_node *node, t_exec_info *info);
int		heredoc(t_node *node, t_exec_info *info);

#endif