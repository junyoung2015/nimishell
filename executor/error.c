/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:26 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/15 15:10:34 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	free_ptr(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr && ptr[i])
	{
		free(ptr[i]);
		ptr[i++] = NULL;
	}
	free(ptr);
	ptr = NULL;
}

void	clear_all(t_node *root)
{
	if (!root)
		return ;
	clear_all(root->left);
	clear_all(root->right);
	if (root->cmd_args)
		free_ptr(root->cmd_args);
	free(root);
}

int	err(char *str, t_exec_info *info)
{
	if (info)
		info->exit_code = EXIT_FAILURE;
	perror(str);
	return (EXIT_FAILURE);
}

int	display_err(char *exec, char *file, char *msg, t_exec_info *info)
{
	if (info)
		info->exit_code = EXIT_FAILURE;
	write(STD_ERR, MINISHELL, ft_strlen(MINISHELL));
	if (exec)
	{
		write(STD_ERR, exec, ft_strlen(exec));
		write(STD_ERR, COLON, ft_strlen(COLON));		
	}
	if (file)
	{
		write(STD_ERR, file, ft_strlen(file));
		write(STD_ERR, COLON, ft_strlen(COLON));
	}
	if (msg)
		write(STD_ERR, msg, ft_strlen(msg));
	else
		perror(0);
	return (EXIT_FAILURE);
}

void	err_exit(int exit_code, char *msg)
{
	if (msg)
		perror(msg);
	clear_all(g_info.root);
	exit(exit_code);
}

// TODO: display_cmd_err 가 cmd_not_found 를 대채할 예정
void	cmd_not_found(char *filename)
{
	clear_all(g_info.root);
	write(STDERR_FILENO, "error: ", 7);
	write(STDERR_FILENO, filename, ft_strlen(filename));
	write(STDERR_FILENO, ": command not found\n", 20);
	exit(EXIT_CMD_NOT_FOUND);
}

// TODO: display_cmd_err fmf err_exit 과 합치는 것에 대해 생각해보기
void	display_cmd_err(char *filename)
{
	write(STDERR_FILENO, "minishell: ", 11);
	if (filename)
	{
		write(STDERR_FILENO, filename, ft_strlen(filename));
		write(STD_ERR, COLON, ft_strlen(COLON));
	}
	write(STDERR_FILENO, CMD_NOT_FOUND, ft_strlen(CMD_NOT_FOUND));
	clear_all(g_info.root);
	exit(EXIT_CMD_NOT_FOUND);
}
