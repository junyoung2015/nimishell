/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:18:10 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/17 14:01:35 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ================== UTILS ================== */
void	free_tokens(t_token **tokens)
{
	t_size	i;

	i = 0;
	while (tokens[i])
		free(tokens[i]++);
	// free(tokens);
}

/* =================== MISC ================== */
void	update_exit_status(int code)
{
	(void) code;
}

/* ================== LEXER ================== */
void	sig_handler(int signal)
{
	if (signal != SIGINT)
		return;
	printf("Ctrl + C\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

void	chk_leaks(void)
{
	system("leaks minishell");
}

void print_ast(t_ast_node *node, int depth, const char *indent)
{
	if (node == NULL)
	{
		printf("%*s[%s]\n", depth * 4, "", indent);
		return ;
	}
	print_ast(node->right, depth + 1, "/");
	printf("%*c", depth * 6, 1);
	for (t_size i = 0; i < node->num_args; i++)
		printf("%s ", node->cmd_args[i]);
	printf("\n");
	print_ast(node->left, depth + 1, "\\");
}

int	main(void)
{
	char		*line;
	int			status;
	t_token		*tokens;
	t_size		num_tokens;
	t_ast_node	*ast;

	// atexit(chk_leaks);
	// display_logo();

	tokens = 0;
	ast = 0;
	signal(SIGINT, sig_handler);
	while (FT_TRUE)
	{
		line = readline("minishell> ");
		if (line)
		{
			if (ft_strcmp(line, "exit") == 0)
				exit (0);
			printf("input: %s\n", line);
			add_history(line);
			// tokenize the input into an array of tokens
			if (line && *line)
			{
				tokens = tokenize(line, &num_tokens);
				if (!tokens)
					return (0);
				categorize_tokens(tokens, num_tokens);
				ast = parse_tokens(tokens, num_tokens);
				print_ast(ast, 0, "");
				status = execute_ast(ast);
			}
			if (tokens && DEBUG)
				for (t_size i = 0; i < num_tokens; i++)
					printf("token[%llu]: %d, %s\n", i, tokens[i].type, tokens[i].value);
			
			if (status)
				update_exit_status(status);
			// free_tokens(&tokens);
			// tokens = 0;
			free_ast(ast);
			free(line);
			line = 0;
		}
	}
	return (0);
}
