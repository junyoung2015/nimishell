/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:37:34 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/31 01:18:40 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static char	*str_join(char *str, char buf, size_t len)
{
	char	*ret;
	size_t	i;

	ret = malloc(sizeof(char) * (len + 2));
	if (!ret)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	while (str && str[i])
	{
		ret[i] = str[i];
		i++;
	}
	ret[i++] = buf;
	ret[i] = 0;
	if (str)
		free(str);
	return (ret);
}

char	*get_next_line(int fd, size_t *str_len)
{
	char	buf;
	char	*str;
	size_t	len;

	str = NULL;
	len = 0;
	buf = 0;
	while (buf != '\n')
	{
		if (read(fd, &buf, 1) < 0)
			return (heredoc_err(str, "minishell: heredoc: read"));
		if (buf == 0)
			return (str);
		str = str_join(str, buf, len++);
		if (!str)
			return (heredoc_err(NULL, "minishell: heredoc: malloc"));
	}
	*str_len = len;
	return (str);
}

void	create_heredoc(char *limiter)
{
	int		fd;
	char	*str;
	size_t	lmt_len;
	size_t	str_len;

	fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		heredoc_err(NULL, "minishell: heredoc: open");
	lmt_len = ft_strlen(limiter);
	write(STDOUT_FILENO, "> ", 2);
	str = get_next_line(STDOUT_FILENO, &str_len);
	while (str && !(!ft_strncmp(str, limiter, lmt_len) && str[lmt_len] == '\n'))
	{
		write(fd, str, str_len);
		free(str);
		write(STDOUT_FILENO, "> ", 2);
		str = get_next_line(STDOUT_FILENO, &str_len);
	}
	if (str)
		free(str);
	close(fd);
}

int	heredoc(t_node *node)
{
	pid_t	pid;
	int		status;
	int		sig;

	pid = fork();
	if (pid < 0)
		return (-1);
	set_signal(pid);
	if (!pid)
	{
		create_heredoc(node->cmd_args[0]);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	sig = WTERMSIG(status);
	if (sig == SIGQUIT || sig == SIGINT || WEXITSTATUS(status) != EXIT_SUCCESS)
		return (1);
	node->fd = open("/tmp/.heredoc", O_RDONLY);
	if (node->fd < 0)
		perror("minishell: heredoc: open");
	unlink("/tmp/.heredoc");
	return (0);
}

t_bool	check_heredoc(t_node *node)
{
	int	result;
	
	if (!node)
		return (TRUE);
	if (node->type == AST_HEREDOC)
	{
		result = heredoc(node);
		if (result < 0)
		{
			perror("minishell: heredoc: fork");
			return (FALSE);
		}
		else if (result > 0)
			return (FALSE);
	}
	if (!check_heredoc(node->left))
		return (FALSE);
	if (!check_heredoc(node->right))
		return (FALSE);
	return (TRUE);
}
