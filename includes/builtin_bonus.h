/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:55:11 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 16:05:49 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_BONUS_H
# define BUILTIN_BONUS_H

# include "minishell_bonus.h"
# include "executor_bonus.h"

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