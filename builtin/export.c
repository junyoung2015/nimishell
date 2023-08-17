#include "builtin.h"

t_bool	is_in_env(char *key, char *env)
{
	size_t	i;

	i = 0;
	while (key && key[i] != '=')
	{
		if (key[i] != env[i])
			return (FALSE);
		i++;
	}
	if (env[i] == '=')
		return (TRUE);
	return (FALSE);
}

void	append_last(char *arg, t_exec_info *info)
{
	char	**env;
	char	*str;
	size_t	i;

	env = ft_calloc(info->env_cnt + 2, sizeof(char *));
	str = ft_strdup(arg);
	if (!env || !arg)
	{
		err("minishell: malloc", info);
		return ;
	}
	i = 0;
	while (g_info.env && g_info.env[i])
	{
		env[i] = g_info.env[i];
		g_info.env[i++] = NULL;
	}
	env[i++] = str;
	env[i] = NULL;
	free(g_info.env);
	g_info.env = env;
	info->env_cnt += 1;
}

void	print_env()
{
	size_t	i;
	size_t	len;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		len = 0;
		while (g_info.env[i][len] != '=')
			len++;
		write(STDOUT_FILENO, g_info.env[i], ++len);
		write(STDOUT_FILENO, "\"", 1);
		write(STDOUT_FILENO, g_info.env[i] + len, ft_strlen(g_info.env[i]) - len);
		write(STDOUT_FILENO, "\"\n", 2);
		i++;
	}
}

void	add_env(char *arg, t_exec_info *info)
{
	size_t	i;
	char	*str;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		if (is_in_env(arg, g_info.env[i]))
		{
			str = ft_strdup(arg);
			if (str)
			{
				free(g_info.env[i]);
				g_info.env[i] = str;
			}
			else
				err("minishell: malloc", info);
			return ;
		}
		i++;
	}
	append_last(arg, info);
}

void	export(t_node *node, t_exec_info *info)
{

	size_t	i;
	int		result;

	if (!node->cmd_args[1] || node->cmd_args[1][0] == '#')
	{
		print_env();
		return ;
	}
	i = 1;
	result = 1;
	while (result && node->cmd_args[i])
	{
		result = arg_check(node->cmd_args[i], EXPORT);
		if (result < 0)
			arg_err(EXPORT, node->cmd_args[i], info);
		else if (result >> 1)
			add_env(node->cmd_args[i], info);
		i++;
	}
}


