/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:04:54 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/08 18:05:39 by jusohn           ###   ########.fr       */
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
	int		fd_in;
	int		fd_out;
	t_bool	is_fork;
	size_t	fork_cnt;
	pid_t	pid;
	int		exit_code;
	int		pipe[2];
	int		prev_pipe;
}	t_exec_info;

/* pipe */
void	open_pipe(t_node *node, t_exec_info *info);
int		connect_pipe(t_node *node, t_exec_info *info);
void	close_pipe(t_node *node, t_exec_info *info);

/* redirection */
void	redirection(t_node *node, t_exec_info *info);
int		heredoc(t_node *node, t_exec_info *info);

/* error */
void	free_ptr(char **ptr);
void	clear_all(t_node *root);
int		err(char *str, t_exec_info *info);
int		display_err(char *file, char *exec, char *msg, t_exec_info *info);
void	err_exit(int code, char *file, t_exec_info *info);
// void	err_exit(char *str, t_exec_info *info);
void	cmd_not_found(void);

/* command */
void	builtin(t_node *node, t_exec_info *info);
void	command(t_node *node, t_exec_info *info);
char	*get_cmdpath(char *filename, t_exec_info *info);

#endif