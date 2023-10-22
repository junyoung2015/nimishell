/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:53:56 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:53:56 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	t_size	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, t_size n)
{
	t_size	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i < n)
		i++;
	if (i == n)
		return (0);
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

t_size	ft_strlen(const char *str)
{
	t_ssize	idx;

	if (!str || !*str)
		return (0);
	idx = -1;
	while (str[++idx])
		;
	return (idx);
}

t_size	ft_strlcpy(char *dst, const char *src, t_size dstsize)
{
	t_size	idx;
	t_size	cp_size;

	idx = 0;
	cp_size = dstsize;
	while (cp_size > 1 && src[idx])
	{
		dst[idx] = src[idx];
		idx++;
		cp_size--;
	}
	if (dstsize != 0)
		dst[idx] = '\0';
	return (ft_strlen(src));
}
