#include "builtin.h"

char	*get_val(char *key)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		len = ft_strlen(key);
		if (!ft_strncmp(g_info.env[i], key, len) && g_info.env[i][len] == '=')
			return (g_info.env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*create_val(char **args)
{
	char	*ret;
	char	*tmp;
	
	ret = NULL;
	while (*args)
	{
		if (*args[0] == '$')
		{
			tmp = get_val((*args)+1);
			if (tmp)
				join(ret, tmp);
		}
	}
}

void	export(t_node *node, t_pipe_info info)
{
	char	*key;
	char	*val;

	val = create_val((node->cmd_args)+2);
	key = get_key();
}
