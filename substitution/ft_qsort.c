#include "minishell.h"

int	cmp_ascii(const void *a, const void *b)
{
	return ft_strcmp((const char *)a, (const char *)*b);
}

int	cmp_len(const void *a, const void *b)
{
	return ft_strlen(*(char **)a) - ft_strlen(*(char **)b);
}

void	ft_swap(void **a, void **b)
{
	void	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

int	ft_partition(void **arr, t_ssize low, t_ssize high, int (*cmp)(const void *, const void *))
{
	void	*pivot;
	t_ssize	i;
	t_ssize	j;

	pivot = arr[high];
	i = low - 1;
	j = low;
	while (j < high)
	{
		if (cmp(arr[j], pivot) < 0)
		{
			i++;
			ft_swap(&arr[i], &arr[j]);
		}
		j++;
	}
	ft_swap((void **) &arr[i + 1], &(arr[high]));
	return (i + 1);
}

void	ft_qsort(void **arr, t_ssize low, t_ssize high, int (*cmp)(const void *, const void *))
{
	t_ssize	pivot;

	if (low < high)
	{
		pivot = ft_partition(arr, low, high, cmp);
		ft_qsort(arr, low, pivot - 1, cmp);
		ft_qsort(arr, pivot + 1, high, cmp);
	}
}

