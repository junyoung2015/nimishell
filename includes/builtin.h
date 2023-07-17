#ifndef BUILTIN_H
# define BUILTIN_H

# include "executor.h"

void	builtin(t_node *node, t_exec_info *info);
void	export(t_node *node, t_exec_info *info);
void	env(void);

#endif