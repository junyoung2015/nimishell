/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/01 22:05:26 by sejinkim          #+#    #+#             */
/*   Updated: 2023/07/02 16:45:20 by sejinkim         ###   ########.fr       */
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
	free(root->cmd_args);
	free(root);
}

void	err(void)
{
	clear_all(g_info.root);
	perror("error");
	exit(EXIT_FAILURE);
}