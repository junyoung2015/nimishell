/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:04:54 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/03 15:16:50 by sejinkim         ###   ########.fr       */
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
	int		fd;
}	t_pipe_info;

/* open.c */
int		open_rdonly(char *filename);
int		open_wronly_trunc(char *filename);
int		open_wronly_append(char *filename);

/* pipe.c */
void	open_pipe(t_pipe_info *info);
void	connect_pipe(t_node *node, t_pipe_info *info);
void	close_pipe(t_node *node, t_pipe_info *info);

/* redir.c */
void	redir_in(t_node *node);
void	redir_out(t_node *node);
void	redir_append(t_node *node);

void	ft_execve(t_node *node);
void	free_ptr(char **ptr);
void	clear_all(t_node *root);
void	err(void);
void	command(t_node *node, t_pipe_info *info);

#endif