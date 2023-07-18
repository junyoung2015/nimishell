#ifndef BUILTIN_H
# define BUILTIN_H

# include "executor.h"

void	export(t_node *node, t_exec_info *info);
void	env(void);
void	unset(t_node *node, t_exec_info *info);

#endif