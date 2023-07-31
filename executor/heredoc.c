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

static char	*heredoc_err(t_exec_info *info, char *str, int code)
{
	if (str)
		free(str);
	if (code)
		err("error: read", info);
	else
		err("error: malloc", info);
	if (info->is_fork)
	{
		clear_all(g_info.root);
		exit(EXIT_FAILURE);
	}
	return (NULL);
}

char	*get_next_line(int fd, t_exec_info *info, size_t *str_len)
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
			return (heredoc_err(info, str, 1));
		if (buf == 0)
			return (str);
		str = str_join(str, buf, len++);
		if (!str)
			return (heredoc_err(info, NULL, 0));
	}
	*str_len = len;
	return (str);
}

void	write_heredoc(char *limiter, t_exec_info *info, int fd)
{
	char	*str;
	size_t	lmt_len;
	size_t	str_len;

	lmt_len = ft_strlen(limiter);
	write(g_info.stdin_fd, "heredoc> ", 9);
	str = get_next_line(g_info.stdin_fd, info, &str_len);
	while (str && !(!ft_strncmp(str, limiter, lmt_len) && str[lmt_len] == '\n'))
	{
		write(fd, str, str_len);
		free(str);
		write(g_info.stdin_fd, "heredoc> ", 9);
		str = get_next_line(g_info.stdin_fd, info, &str_len);
	}
	if (str)
		free(str);
}

int	heredoc(t_node *node, t_exec_info *info)
{
	int	fd;

	fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (1);
	write_heredoc(node->cmd_args[0], info, fd);
	if (info->exit_code != EXIT_SUCCESS)
		return (2);
	close(fd);
	info->fd_in = open("/tmp/.heredoc", O_RDONLY);
	if (info->fd_in < 0)
		return (1);
	unlink("/tmp/.heredoc");
	return (0);
}
