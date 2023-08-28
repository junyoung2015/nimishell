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

static void	init_sh_info(int ac, char **av, char **envp, t_sh_info *info)
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
		exit_err_msg(1, 0, MALLOC_ERR, 0);
	i = 0;
	while (i < cnt)
	{
		g_env[i] = ft_strdup(envp[i]);
		if (!g_env[i])
			exit_err_msg(1, 0, MALLOC_ERR, 0);
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
		exit_err_msg(1, "tcgetattr", strerror(errno), 0);
	term.c_lflag &= ~ECHOCTL;
	status = tcsetattr(0, 0, &term);
	if (status == -1)
		exit_err_msg(1, "tcgetattr", strerror(errno), 0);
	print_logo();
}

void	init_prompt(t_sh_info *info)
{
	info->tokens = 0;
	info->num_tokens = 0;
	set_parent_signal();
	info->pwd = get_prompt();
	if (!info->pwd)
		exit_err_msg(1, 0, MALLOC_ERR, 0);
	info->line = readline(info->pwd);
}

void	handle_input(t_sh_info *info)
{
	add_history(info->line);
	if (*(info->line))
	{
		info->tokens = tokenize_input(info->line, 0, &info->num_tokens);
		if (!info->tokens || info->num_tokens == 0)
		{
			free(info->line);
			free(info->pwd);
			return ;
		}
		else if (info->num_tokens >= 1 && \
			info->tokens[info->num_tokens - 1].type == TOKEN_ERROR)
			write(STD_ERR, info->tokens[info->num_tokens - 1].val, \
			ft_strlen(info->tokens[info->num_tokens - 1].val));
		else
		{
			info->ast = parse_tokens(info->tokens, info->num_tokens);
			info->exit_code = executor(info);
		}
	}
	if (info->tokens)
		free_tokens(info->tokens, info->num_tokens);
	free(info->line);
	free(info->pwd);
}

int	main(int ac, char **av, char **envp)
{
	t_sh_info	info;

	init_sh_info(ac, av, envp, &info);
	init_terminal();
	while (TRUE)
	{
		init_prompt(&info);
		if (info.line)
			handle_input(&info);
		else
			break ;
	}
	exit(info.exit_code);
	return (0);
}
