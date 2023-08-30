/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:55:11 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 23:14:46 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include "executor.h"

/* built-in function */
void	export(t_node *node, t_exec_info *info);
void	env(t_node *node, t_exec_info *info);
void	unset(t_node *node, t_exec_info *info);
void	pwd(t_exec_info *info);
t_bool	cd(t_node *node, t_exec_info *info);
void	echo(t_node *node);

/* cd util function */
char	*get_env(char *env_var);
void	append_new_env(char *new_env, t_exec_info *info);
void	update_env(char *new_env, t_exec_info *info);
void	update_pwd(char *cwd, t_exec_info *info);
t_bool	try_chdir(char *path, t_exec_info *info);

/* export/unset util function */
int		arg_check(char *str, t_builtin func);
void	arg_err(t_builtin func, char *arg, t_exec_info *info);

#endif