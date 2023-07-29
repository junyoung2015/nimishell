
#include "minishell.h"

/**
 * @brief	Look for $ in the cmg_args, while checking if it's quoted or not.
 *
 * @param cmd_arg
 * @return
 */
char    *check_env_var(char *cmd_arg)
{
	t_size idx;
	t_bool is_squoted;
	char    *result;

	idx = 0;
	is_squoted = FALSE;
	while (idx < ft_strlen(cmd_arg))
	{
		idx++;
	}
}

/**
 * @brief	Find and substitute environment variables in the node->cmd_args.
 * 			If the string is not quoted or double-quoted, search for $ and
 * 			substitute the environment variable ONLY.
 * 			If the string is single-quoted, do nothing.
 *
 * @param node		node to substitute env var
 * @return char**	substituted cmd_args
 */
char    **check_and_substitute_env(t_node *node)
{
	char    **result;
	t_size  idx;

	idx = 0;
	if (!node->cmd_args)
		return (0);
	result = (char **)ft_calloc(node->num_args + 1, sizeof(char *));
	if (!result)
		return (0);
	while (idx < node->num_args)
	{
		result[idx] = check_env_var(node->cmd_args[idx]);
		if (!result[idx])
		{
			// free result up to now
			return (0);
		}
	}
	return (result);
}