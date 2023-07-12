#include "executor.h"

void	str_copy(char *dst, char *src, size_t size)
{
	size_t	i;

	if (!src)
		return ;
	i = 0;
	while (src[i] && i + 1 < size)
	{
		// printf("dst: %c, src: %c\n", dst[i], src[i]);
		dst[i] = src[i];
		i++;
	}
	dst[i] = 0;
}

static char	*str_join(char *str, char *buf, size_t len)
{
	char	*ret;

	ret = malloc(sizeof(char) * (len + 2));
	if (!ret)
	{
		if (str)
			free(str);
		err();
	}
	str_copy(ret, str, len + 1);
	str_copy(ret + len, buf, 2);
	if (str)
		free(str);
	return (ret);
}

char	*get_next_line(int fd, size_t *str_len)
{
	char	buf[2];
	char	*str;
	size_t	len;

	str = NULL;
	len = 0;
	*buf = 0;
	buf[1] = 0;
	while (*buf != '\n')
	{
		if (read(fd, buf, 1) < 0)
		{
			if (str)
				free(str);
			err();
		}
		if (*buf == 0)
			return (0);
		str = str_join(str, buf, len++);
	}
	*str_len = len;
	return (str);
}

void	write_heredoc(char *limiter, t_pipe_info *info, int fd)
{
	char	*str;
	size_t	lmt_len;
	size_t	str_len;

	lmt_len = ft_strlen(limiter);
	write(info->stdin_fd, "heredoc> ", 9);
	str = get_next_line(info->stdin_fd, &str_len);
	while (str && !(!ft_strncmp(str, limiter, lmt_len) && str[lmt_len] == '\n'))
	{
		write(fd, str, str_len);
		free(str);
		write(info->stdin_fd, "heredoc> ", 9);
		str = get_next_line(info->stdin_fd, &str_len);
	}
	free(str);
}

void	heredoc(t_node *node, t_pipe_info *info)
{
	int	fd;

	if (node->parent_type == AST_COMMAND)
		return ;
	// .tmp로 이동
	fd = open(".heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		err();
	write_heredoc(node->cmd_args[0], info, fd);
	close(fd);
	fd = open(".heredoc", O_RDONLY);
	if (fd < 0)
		err();
	if (dup2(fd, STDIN_FILENO) < 0)
		err();
	unlink(".heredoc");
}
