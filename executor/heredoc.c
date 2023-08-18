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

static char	*heredoc_err(t_exec_info *info, char *str, char *msg)
{
	if (str)
		free(str);
	err_exit(info, msg, EXIT_FAILURE);
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
			return (heredoc_err(info, str, "minishell: heredoc: read"));
		if (buf == 0)
			return (str);
		str = str_join(str, buf, len++);
		if (!str)
			return (heredoc_err(info, NULL, "minishell: heredoc: malloc"));
	}
	*str_len = len;
	return (str);
}

void	create_heredoc(char *limiter, t_exec_info *info)
{
	int		fd;
	char	*str;
	size_t	lmt_len;
	size_t	str_len;

	fd = open("/tmp/.heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		err_exit(info, "minishell: heredoc: open", EXIT_FAILURE);
	lmt_len = ft_strlen(limiter);
	write(info->stdin_fd, "heredoc> ", 9);
	str = get_next_line(info->stdin_fd, info, &str_len);
	while (str && !(!ft_strncmp(str, limiter, lmt_len) && str[lmt_len] == '\n'))
	{
		// str 치환
		write(fd, str, str_len);
		free(str);
		write(info->stdin_fd, "heredoc> ", 9);
		str = get_next_line(info->stdin_fd, info, &str_len);
	}
	if (str)
		free(str);
	close(fd);
}

void	heredoc(t_node *node, t_exec_info *info)
{
	pid_t	pid;
	int		status;

	
	pid = fork();
	if (pid < 0)
		return (err("minishell: heredoc: fork", info));
	else if (!pid)
	{
		create_heredoc(node->cmd_args[0], info);
		clear_all(info->ast);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	info->exit_code = WEXITSTATUS(status);
	if (info->exit_code != EXIT_SUCCESS)
		return ;
	if (info->fd_in >= 0)
		close(info->fd_in);
	info->fd_in = open("/tmp/.heredoc", O_RDONLY);
	if (info->fd_in < 0)
		err("minishell: heredoc: open", info);
	unlink("/tmp/.heredoc");
}
