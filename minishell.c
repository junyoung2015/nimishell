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

#include <signal.h>

/**
 * @brief	Display err msg and exit with exit code. Free AST node if exists.
 * 
 * @param code	exit code to exit
 * @param file	file name to display in front of err msg
 * @param msg	err msg to display
 * @param root	root of the ast node to free
 */
void	exit_err_with_msg(int code, char *file, char *msg, t_node *root)
{
	if (msg)
	{
		write(STD_ERR, MINISHELL, ft_strlen(MINISHELL));
		if (file)
		{
			write(STD_ERR, file, ft_strlen(file));
			write(STD_ERR, ": ", 2);
		}
		write(STD_ERR, msg, ft_strlen(msg));
	}
	else
	{
		write(STD_ERR, strerror(errno), ft_strlen(strerror(errno)));
	}
	write(STD_ERR, "\n", 1);
	if (root)
		free_ast(root);
	exit(code);
}

void sig_handler(int signal)
{	
	if (signal == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 1);
	}
	else if (signal == SIGQUIT)
	{
		// does nothing
		rl_redisplay();
	}
	else if (signal == SIGTSTP)
	{
		// does nothing
	}
	rl_redisplay();
}

#ifdef DEBUG

// TODO: remove for
void print_ast(t_node *node, int depth, const char *indent)
{
	if (node == NULL)
	{
		printf("%*s[%s]\n", depth * 6, "", indent);
		return;
	}
	print_ast(node->right, depth + 1, "/");
	printf("%*c", depth * 4, 1);
	if (node->type == AST_PIPE)
		printf("|");
	else if (node->type == AST_NULL)
		printf("NULL");
	else if (node->type == AST_REDIR_IN)
		printf("<");
	else if (node->type == AST_REDIR_OUT)
		printf(">");
	else if (node->type == AST_REDIR_APPEND)
		printf(">>");
	else if (node->type == AST_AND)
		printf("&&");
	else if (node->type == AST_OR)
		printf("||");
	else if (node->type == AST_SUBSHELL)
		printf("(SUB)");
	else if (node->builtin != NOT_BUILTIN)
		printf("[%d] ", node->builtin);
	// printf("%*c", depth * 4, 32);
	for (t_size i = 0; i < node->num_args; i++)
		printf("[%d][%s] ", node->type, node->cmd_args[i]);
	printf("\n");
	print_ast(node->left, depth + 1, "\\");
}

#endif

//#ifdef DEBUG
//
//void chk_leaks(void)
//{
//	system("leaks minishell");
//}
//
//#endif

#ifdef DEBUG

void print_tokens(t_token *tokens, t_size num_tokens)
{
	printf("\n================= TOKENS =================\n");
	printf("num of tokens: %llu\n", num_tokens);
	for (t_size i = 0; i < num_tokens; i++)
		printf("token[%llu]: [%d], [%s]\n", i, tokens[i].type, tokens[i].val);
	printf("==========================================\n");
}

# endif

void	init_g_info(char **envp)
{
	t_size	i;
	
	g_info.env_cnt = 0;
	while (envp && envp[g_info.env_cnt])
		g_info.env_cnt++;
	g_info.env = ft_calloc((g_info.env_cnt + 1), sizeof(char *));
	// 널가드 추가
	i = 0;
	while (i < g_info.env_cnt)
	{
		g_info.env[i] = ft_strdup(envp[i]);
		// 널가드
		i++;
	}
	g_info.env[i] = NULL;
	g_info.stdin_fd = dup(STDIN_FILENO);
	g_info.stdout_fd = dup(STDOUT_FILENO);
}

int	main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;
	int			exit_code;
	char		*line;
	char		*pwd;
	char		*tmp;
	t_token		*tokens;
	t_size		num_tokens;
	t_node		*ast;
	// struct termios	term;
	// int	status;
	// t_global_info	g_info;

//	 if (DEBUG)
//		atexit(chk_leaks);
	init_g_info(envp);
	tokens = 0;
	ast = 0;
	exit_code = 0;
	// status = tcgetattr(0, &term);
	// if (status == -1)
	// {
	// 	exit_err_with_msg(EXIT_ERR, TCGETATTR, strerror(errno), 0);
	// 	// write(STD_ERR, "minishell: tcgetattr: ", 22);
	// 	// write(STD_ERR, strerror(errno), ft_strlen(strerror(errno)));
	// 	// write(STD_ERR, "\n", 1);
	// 	// exit (1);
	// }
	// term.c_lflag &= ~ECHOCTL;
	// status = tcsetattr(0, 0, &term);
	// if (status == -1)
	// {
	// 	exit_err_with_msg(EXIT_ERR, TCGETATTR, strerror(errno), 0);
	// 	// write(STD_ERR, "minishell: tcgetattr: ", 22);
	// 	// write(STD_ERR, strerror(errno), ft_strlen(strerror(errno)));
	// 	// write(STD_ERR, "\n", 1);
	// 	// exit (1);
	// }
	signal(SIGINT, sig_handler);
	print_logo();
	while (TRUE)
	{
		num_tokens = 0;
		tmp = getcwd(NULL, 0);
		pwd = ft_strjoin(tmp, "> ");
		free(tmp);
		line = readline(pwd);
		if (line)
		{
			add_history(line);
			// TODO: ft_strtrim(line, space);
			// Maybe - remove spaces at the start and end
			// tokenize the input into an array of tokens
			if (*line)
			{
				tokens = tokenize_input(line, 0, &num_tokens);
				if (!tokens || num_tokens == 0)
				{
					free(line);
					free(pwd);
					continue ;
					// return (0);
				}
				else if (num_tokens >= 1 && tokens[num_tokens - 1].type == TOKEN_ERROR)
				{
					// write(STD_ERR, "minishell: syntax error near unexpected token `", 47);
					write(STD_ERR, tokens[num_tokens - 1].val, ft_strlen(tokens[num_tokens - 1].val));
					// write(STD_ERR, "`\n", 2);
				}
				else
				{
					if (DEBUG)
						print_tokens(tokens, num_tokens);
					ast = parse_tokens_ll(tokens, num_tokens);
					// ast = parse_tokens(tokens, num_tokens);
					if (ast && DEBUG)
					{
						printf("\n=================== AST ==================\n");
						print_ast(ast, 0, "");
						printf("==========================================\n");
					}
					g_info.root = ast;
					exit_code = executor(g_info.root);
					g_info.exit_status = exit_code; // do I need this here?
				}
			}
			// if (status)
			// 	update_exit_status(status);
			if (tokens)
				free_tokens(tokens, num_tokens);
			tokens = 0;
			free(line);
			free(pwd);
			line = 0;
		}
	}
	exit(exit_code);
	return (0);
}
