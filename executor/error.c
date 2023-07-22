/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:26 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/22 17:22:26 by sejinkim         ###   ########.fr       */
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

int	err(char *str)
{
	clear_all(g_info.root);
	perror(str);
	return (EXIT_FAILURE);
}

void	cmd_not_found(void)
{
	clear_all(g_info.root);
	write(STDERR_FILENO, "error: command not found\n", 25);
	exit(EXIT_CMD_NOT_FOUND);
}
