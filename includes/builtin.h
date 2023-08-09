#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include "executor.h"

void	export(t_node *node, t_exec_info *info);void	env(void);
void	unset(t_node *node, t_exec_info *info);
void    pwd(t_exec_info *info);
// void    cd(t_node *node, t_exec_info *info);
t_bool	cd(t_node *node, t_exec_info *info);
void	echo(t_node *node);

#endif