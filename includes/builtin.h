#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include "executor.h"

void	export(t_node *node, t_exec_info *info);
void	env(void);

#endif