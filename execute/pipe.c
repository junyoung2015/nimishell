#include "execute.h"

void	open_pipe(t_pipe_info *info)
{
	if (pipe(info->pipe))
		err();
}

void	connect_pipe(t_node *node, t_pipe_info *info)
{
	if ((node->pipe_open >> 0) & 1)
		if (dup2(info->fd, STDIN_FILENO) < 0)
			err();
	if ((node->pipe_open >> 1) & 1)
		if (dup2(info->pipe[1], STDOUT_FILENO) < 0)
			err();
	close(info->fd);
	close(info->pipe[0]);
	close(info->pipe[1]);
}

void	connect_pipe2(t_node *node, t_pipe_info *info)
{
	if ((node->pipe_open >> 0) & 1)
		if (dup2(info->pipe[0], STDIN_FILENO) < 0)
			err();
	if ((node->pipe_open >> 1) & 1)
		if (dup2(info->fd, STDOUT_FILENO) < 0)
			err();
	close(info->fd);
	close(info->pipe[0]);
	close(info->pipe[1]);
}

void	close_pipe(t_node *node, t_pipe_info *info)
{
	if ((node->pipe_open >> 0) & 1)
	{
		if (info->fd < 0)
		{
			info->fd = dup(info->pipe[0]);
			if (info->fd < 0)
				err();
		}
		else
		{
			if (dup2(info->pipe[0], info->fd) < 0)
				err();
		}
	}
	close(info->pipe[0]);
	close(info->pipe[1]);
}