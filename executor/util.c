/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 01:00:32 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/31 01:00:33 by sejinkim         ###   ########.fr       */
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
