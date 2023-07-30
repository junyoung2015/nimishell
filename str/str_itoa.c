#include "minishell.h"

static size_t	get_num_digits(long long n)
{
	size_t	digits;

	digits = 0;
	if (n == 0)
		digits++;
	else if (n < 0)
	{
		digits++;
		n = -n;
	}
	while (n > 0)
	{
		n /= 10;
		digits++;
	}
	return (digits);
}

static void	put_num(char *result, long long n, int digits)
{
	if (n >= 10 && digits > 0)
	{
		put_num(result, n / 10, digits - 1);
	}
	result[digits] = n % 10 + '0';
}

char	*ft_itoa(int n)
{
	char		*result;
	size_t		digits;
	long long	n_long;

	n_long = (long long) n;
	digits = get_num_digits(n_long);
	result = (char *) ft_calloc((digits + 1), sizeof(char));
	if (!result)
		return (NULL);
	if (n_long < 0)
	{
		result[0] = '-';
		n_long = -n_long;
	}
	put_num(result, n_long, digits - 1);
	return (result);
}
