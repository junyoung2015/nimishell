/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:04:54 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/28 20:09:01 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/stat.h>
# include "minishell.h"

typedef struct s_executor_info
{
	int		stdin_fd;
	int		stdout_fd;
	t_node	*ast;
	size_t	env_cnt;
	int		fd_in;
	int		fd_out;
	t_bool	is_fork;
	size_t	fork_cnt;
	pid_t	pid;
	int		prev_exit_code;
	int		exit_code;
	int		pipe[2];
	int		prev_pipe;
}	t_exec_info;

void	ast_search(t_node *root, t_exec_info *info);
char	**env_substitution(t_node *node, t_exec_info *info);
typedef char	*(*t_env_fn)(char **, t_exec_info *);

/* pipe */
void	open_pipe(t_node *node, t_exec_info *info);
int		connect_pipe(t_node *node, t_exec_info *info);
void	close_pipe(t_node *node, t_exec_info *info);

/* redirection */
void	redirection(t_node *node, t_exec_info *info);
void	heredoc(t_node *node, t_exec_info *info);

/* error */
void	free_ptr(char **ptr);
void	clear_all(t_node *root);
void	err(char *str, t_exec_info *info);
void	err_exit(t_exec_info *info, char *msg, int exit_code);

/* command */
void	builtin(t_node *node, t_exec_info *info);
void	command(t_node *node, t_exec_info *info);
char	*get_cmdpath(char *cmd, t_exec_info *info);

/* subshell */
void	check_subshell_parent(t_node *node, t_exec_info *info);
void	check_subshell_child(t_node *node, t_exec_info *info);

#endif