/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:04:54 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/31 01:05:26 by sejinkim         ###   ########.fr       */
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

int		execute(t_sh_info *info);
void	ast_search(t_node *root, t_exec_info *info);

/* expansion */
char	**env_substitution(t_node *node, t_exec_info *info);
t_bool	is_number(int c);
t_bool	is_alpha(int c);
t_bool	is_alnum(int c);
t_bool	is_env_var(char ch);
t_bool	is_dollar(char ch);
char	**env_str(char **in, t_exec_info *info);
char	**env_squote(char **in, t_exec_info *info);
char	**process_env_dquote(char **in, char ***res, char **s, t_exec_info *info);
char	**env_dquote(char **in, t_exec_info *info);
char	*ft_getenv(char *env_var);
char	**substitute(char *env_var, char *quote);
char	**sub_exit_code(char **in, char *tmp, t_exec_info *info);
char	**sub_env_var(char **in, char *tmp, char *quote);
char	**trim_single_char(char **in, char *tmp);
char	**handle_dollar_sign(char **in, char *tmp, char *quo, t_exec_info *info);
char	**check_env_var(t_size len, char **substrs, char *cmd_arg, t_exec_info *info);
typedef char	**(*t_env_fn)(char **, t_exec_info *);

/* pipe */
void	open_pipe(t_node *node, t_exec_info *info);
int		connect_pipe(t_node *node, t_exec_info *info);
void	close_pipe(t_node *node, t_exec_info *info);

/* redirection */
void	redirection(t_node *node, t_exec_info *info);
t_bool	check_heredoc(t_node *node);

/* util */
void	free_ptr(char **ptr);
void	clear_all(t_node *root);

/* error */
char	*heredoc_err(char *str, char *msg);
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