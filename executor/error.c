/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:26 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/28 21:42:41 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

void	free_ptr(char **ptr)
{
	size_t	i;

	i = 0;
	while (ptr[i])
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

void	err_exit(char *str, t_exec_info *info)
{
	clear_all(g_info.root);
	exit(err(str, info));
}

// TODO: display_cmd 가 cmd_not_found 를 대채할 예정
void	cmd_not_found(void)
{
	clear_all(g_info.root);
	write(STDERR_FILENO, "error: command not found\n", 25);
	exit(EXIT_CMD_NOT_FOUND);
}

// TODO: display_cmd fmf err_exit 과 합치는 것에 대해 생각해보기
void	display_cmd(char *msg)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, CMD_NOT_FOUND, ft_strlen(CMD_NOT_FOUND));
	clear_all(g_info.root);
	exit(EXIT_CMD_NOT_FOUND);
}
