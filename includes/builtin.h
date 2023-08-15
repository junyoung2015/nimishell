/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:55:11 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 12:55:12 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include "executor.h"

/* built-in function */
void	export(t_node *node, t_exec_info *info);
void	env(void);
void	unset(t_node *node, t_exec_info *info);
void    pwd(t_exec_info *info);
// void    cd(t_node *node, t_exec_info *info);
t_bool	cd(t_node *node, t_exec_info *info);
void	echo(t_node *node);

/* util function */
int 	arg_check(char *str, t_builtin func);
void	arg_err(char *func_name, char *arg, t_exec_info *info);

#endif