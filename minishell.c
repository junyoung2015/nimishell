/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:18:10 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/30 13:39:43 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_global_info g_info;

void	sig_handler(int signal)
{
	if (signal != SIGINT)
		return ;
	printf("Ctrl + C\n");
	rl_on_new_line();
	rl_replace_line("", 1);
	rl_redisplay();
}

# ifdef DEBUG

// TODO: remove for
void print_ast(t_node *node, int depth, const char *indent)
{
	if (node == NULL)
	{
		printf("%*s[%s]\n", depth * 6, "", indent);
		return ;
	}
	print_ast(node->right, depth + 1, "/");
	printf("%*c", depth * 4, 1);
	if (node->type == AST_PIPE)
		printf("|");
	else if (node->type == AST_HEREDOC)
		printf("%s", node->cmd_args[0]);
	else if (node->type == AST_NULL)
		printf("NULL");
	else if (node->type == AST_REDIR_IN)
		printf("<");
	else if (node->type == AST_REDIR_OUT)
		printf(">");
	else if (node->type == AST_REDIR_APPEND)
		printf(">>");
	for (t_size i = 0; i < node->num_args; i++)
		printf("[%d][%s] ", node->type, node->cmd_args[i]);
	printf("\n");
	print_ast(node->left, depth + 1, "\\");
}

# endif

# ifdef DEBUG

void	chk_leaks(void)
{
	system("leaks minishell");
}

# endif

# ifdef DEBUG

void print_tokens(t_token *tokens, t_size num_tokens)
{
	printf("\n================= TOKENS =================\n");
	printf("num of tokens: %llu\n", num_tokens);
	for (t_size i = 0; i < num_tokens; i++)
		printf("token[%llu]: [%d], [%s]\n", i, tokens[i].type, tokens[i].value);
	printf("==========================================\n");
}

# endif

void	init_g_info(char **envp)
{
	g_info.env_cnt = 0;
	g_info.env = envp;
	while (envp && envp[g_info.env_cnt])
		g_info.env_cnt++;
	g_info.stdin_fd = dup(STDIN_FILENO);
	g_info.stdout_fd = dup(STDOUT_FILENO);
}

int	main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;
	int			exit_code;
	char		*line;
	t_token		*tokens;
	t_size		num_tokens;
	t_node		*ast;
	// t_global_info	g_info;

	// if (DEBUG)
	// 	atexit(chk_leaks);
	// TODO: display_logo();
	init_g_info(envp);
	tokens = 0;
	ast = 0;
	status = 0;
	signal(SIGINT, sig_handler);
	while (TRUE)
	{
		line = readline("minishell> ");
		if (line)
		{
			if (ft_strcmp(line, "exit") == 0 || ft_strcmp(line, "quit") == 0)
				exit (0);
			add_history(line);
			// TODO: ft_strtrim(line, space);
			// remove spaces at the start and end
			// tokenize the input into an array of tokens
			if (line && *line)
			{
				// tokens = tokenize(line, &num_tokens);
				tokens = tokenize_cmd(line, &num_tokens);
				if (!tokens)
					return (0);
				categorize_tokens(tokens, num_tokens);
				if (tokens && DEBUG)
					print_tokens(tokens, num_tokens);
				ast = parse_tokens(tokens, num_tokens);
				if (ast && DEBUG)
				{
					printf("\n================== AST ==================\n");
					print_ast(ast, 0, "");
					printf("=========================================\n");
				}
				g_info.root = ast;
				status = executor(g_info.root);
				g_info.exit_code = WEXITSTATUS(status);
			}
			// if (status)
			// 	update_exit_status(status);
			if (tokens)
				free_tokens(tokens, num_tokens);
			tokens = 0;
			free(line);
			line = 0;
		}
	}
	exit(exit_code);
	return (0);
}
