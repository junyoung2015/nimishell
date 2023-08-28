/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:26 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/18 14:43:20 by sejinkim         ###   ########.fr       */
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

void	err(char *str, t_exec_info *info)
{
	if (info)
		info->exit_code = EXIT_FAILURE;
	perror(str);
}

void	err_exit(t_exec_info *info, char *msg, int exit_code)
{
	if (exit_code)
		info->exit_code = exit_code;
	if (msg)
		perror(msg);
	clear_all(info->ast);
	exit(info->exit_code);
}

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
