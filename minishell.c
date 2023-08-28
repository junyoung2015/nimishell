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

char	**g_env;

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

static void	init_sh_info(int ac, char ** av, char **envp, t_sh_info *info)
{
	t_size	i;
	t_size	cnt;

	(void) ac;
	(void) av;
	cnt = 0;
	info->ast = 0;
	info->exit_code = 0;
	while (envp && envp[cnt])
		cnt++;
	info->env_cnt = cnt;
	g_env = ft_calloc((cnt + 1), sizeof(char *));
	if (!g_env)
		exit_err_with_msg(1, 0, MALLOC_ERR, 0);
	i = 0;
	while (i < cnt)
	{
		g_env[i] = ft_strdup(envp[i]);
		if (!g_env[i])
			exit_err_with_msg(1, 0, MALLOC_ERR, 0);
		i++;
	}
	g_env[i] = NULL;
}

void	init_terminal(void)
{
	struct termios	term;
	int				status;

	status = tcgetattr(0, &term);
	if (status == -1)
		exit_err_with_msg(1, "tcgetattr", strerror(errno), 0);
	term.c_lflag &= ~ECHOCTL;
	status = tcsetattr(0, 0, &term);
	if (status == -1)
		exit_err_with_msg(1, "tcgetattr", strerror(errno), 0);
	print_logo();
}

void	init_prompt(char **tokens, t_size *num_tokens, char **pwd)
{
	*tokens = 0;
	*num_tokens = 0;
	set_parent_signal();
	*pwd = get_prompt();
}

int	main(int ac, char **av, char **envp)
{
	t_sh_info	info;
	char		*line;
	char		*pwd;
	t_size		num_tokens;
	t_token		*tokens;

	init_sh_info(ac, av, envp, &info);
	init_terminal();
	while (TRUE)
	{
		init_prompt(&line, &num_tokens, &pwd);
		line = readline(pwd);
		if (line)
		{
			add_history(line);
			if (*line)
			{
				tokens = tokenize_input(line, 0, &num_tokens);
				if (!tokens || num_tokens == 0)
				{
					free(line);
					free(pwd);
					continue ;
				}
				else if (num_tokens >= 1 && tokens[num_tokens - 1].type == TOKEN_ERROR)
				{
					// write(STD_ERR, "minishell: syntax error near unexpected token `", 47);
					write(STD_ERR, tokens[num_tokens - 1].val, ft_strlen(tokens[num_tokens - 1].val));
					// write(STD_ERR, "`\n", 2);
				}
				else
				{
					info.ast = parse_tokens(tokens, num_tokens);
					info.exit_code = executor(&info);
				}
			}
			if (tokens)
				free_tokens(tokens, num_tokens);
			free(line);
			free(pwd);
		}
		else
			break ;
	}
	exit(info.exit_code);
	return (0);
}
