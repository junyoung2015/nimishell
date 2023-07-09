/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:04:54 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/09 20:52:54 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include "minishell.h"

typedef struct s_pipe_info
{
	size_t	fork_cnt;
	pid_t	pid;
	int		pipe[2];
	int		prev_pipe_fd;
	int		stdin_fd;
}	t_pipe_info;

/* pipe.c */
void	open_pipe(t_pipe_info *info);
void	connect_pipe(t_node *node, t_pipe_info *info);
void	close_pipe(t_node *node, t_pipe_info *info);

/* redirection.c */
void	redir_in(t_node *node);
void	redir_out(t_node *node);
void	redir_append(t_node *node);

/* error.c */
void	free_ptr(char **ptr);
void	clear_all(t_node *root);
void	err(void);
void	cmd_not_found(void);

void	command(t_node *node, t_pipe_info *info);
void	heredoc(t_node *node, t_pipe_info *info);

#endif