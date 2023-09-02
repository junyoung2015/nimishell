/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qsort_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 19:55:21 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:56:32 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	cmp_ascii(void *a, void *b)
{
	return (ft_strcmp((const char *)a, (const char *)b));
}

void	ft_swap(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	ft_partition(void **arr, t_ssize l, t_ssize h, int (*cmp)(void *, void *))
{
	void	*pivot;
	t_ssize	i;
	t_ssize	j;

	pivot = arr[h];
	i = l - 1;
	j = l;
	while (j < h)
	{
		if (cmp(arr[j], pivot) < 0)
		{
			i++;
			ft_swap(&arr[i], &arr[j]);
		}
		j++;
	}
	ft_swap(&arr[i + 1], &arr[h]);
	return (i + 1);
}

void	_qs(void **arr, t_ssize l, t_ssize h, int (*cmp)(void *, void *))
{
	t_ssize	pivot;

	if (l < h)
	{
		pivot = ft_partition(arr, l, h, cmp);
		_qs(arr, l, pivot - 1, cmp);
		_qs(arr, pivot + 1, h, cmp);
	}
}
