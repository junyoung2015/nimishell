#include "minishell.h"

int	cmp_ascii(void *a, void *b)
{
	return ft_strcmp((const char *)a, (const char *)b);
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

void	ft_qsort(void **arr, t_ssize l, t_ssize h, int (*cmp)(void *, void *))
{
	t_ssize	pivot;

	if (l < h)
	{
		pivot = ft_partition(arr, l, h, cmp);
		ft_qsort(arr, l, pivot - 1, cmp);
		ft_qsort(arr, pivot + 1, h, cmp);
	}
}

