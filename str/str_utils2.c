/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 13:31:23 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 13:32:15 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strnstr(const char *haystack, const char *needle, t_size n)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	*src;
	char	*dst;

	i = -1;
	size = n;
	src = (char *) haystack;
	dst = (char *) needle;
	if (!*dst)
		return (src);
	while (src[++i] && 0 < n)
	{
		j = 0;
		if (src[i] == dst[j])
		{
			while (0 < n && i + j < size && dst[j] && src[i + j] == dst[j])
				j++;
			if (dst[j] == '\0')
				return (src + i);
		}
		n--;
	}
	return (0);
}
