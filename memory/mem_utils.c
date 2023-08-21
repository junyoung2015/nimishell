/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 13:55:09 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/15 13:55:52 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_bzero(void *s, t_size n)
{
	unsigned char	*tmp;

	tmp = (unsigned char *) s;
	while (0 < n--)
		*(tmp++) = 0;
}

void	*ft_calloc(t_size count, t_size size)
{
	void	*ptr;

	ptr = malloc (size * count);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, size * count);
	return (ptr);
}

void	*ft_memcpy(void *dst, const void *src, t_size n)
{
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	tmp_src = (unsigned char *) src;
	tmp_dst = (unsigned char *) dst;
	if (!src && !dst)
		return (dst);
	while (0 < n--)
		*(tmp_dst++) = *(tmp_src++);
	return (dst);
}
