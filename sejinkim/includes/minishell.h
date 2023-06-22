#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_node
{
	char			*val;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

void	lexer(char *str, t_node **head);
void	parser(t_node **node);
void	_syntax_err_(t_node *root);
void	free_tree(t_node *root);

#endif