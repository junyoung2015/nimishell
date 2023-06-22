#include "minishell.h"

static void	_print_lexer_(t_node *head)
{
	t_node	*node;

	if (!head)
		return ;
	node = head;
	printf("======lexer======\n");
	while (node)
	{
		printf("[%s]\n", node->val);
		node = node->right;
	}
	printf("=================\n");
}

void	_leak_(void)
{
	system("leaks minishell");
}

int	main(int ac, char **av)
{
	t_node	*root;

	// atexit(_leak_);
	(void)ac;
	root = NULL;
	lexer(av[1], &root);
	_print_lexer_(root);
	parser(&root);
	free_tree(root);
	return (0);
}
