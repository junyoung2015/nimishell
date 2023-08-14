
#include "minishell.h"

/**
 * @brief Check whether current token is a word or not. SQ_STR and DQ_STR are
 * 		also considered as a word, like bash.
 * 
 * @param parser	parser struct
 * @return t_bool	TRUE if current token is a word, FALSE otherwise
 */
t_bool	is_word_token(t_parser *parser)
{
	t_token_type	type;

	type = cur_type(parser);
	if (parser->cur < parser->size)
		return (type == TOKEN_WORD || type == TOKEN_SQ_STR || type == TOKEN_DQ_STR);
	return (FALSE);
}

/**
 * @brief Check whether current token is a redirection or not.
 * 
 * @param parser	parser struct
 * @return t_bool	TRUE if current token is a redirection, FALSE otherwise
 */
t_bool	is_redir_token(t_parser *parser)
{
	t_token_type	type;

	type = cur_type(parser);
	if (parser->cur < parser->size)
		return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || \
			type == TOKEN_APPEND || type == TOKEN_HEREDOC);
	return (FALSE);
}

/**
 * @brief Check whether the command (cmd_args[0]) is a BUILTIN or not.
 * 
 * @param node	node to check
 */
void	is_builtin_node(t_node *node)
{
	if (!node->cmd_args[0])
		return ;
	else if (!ft_strcmp(node->cmd_args[0], "echo"))
		node->builtin = ECHO_BUILTIN;
	else if (!ft_strcmp(node->cmd_args[0], "cd"))
		node->builtin = CD;
	else if (!ft_strcmp(node->cmd_args[0], "pwd"))
		node->builtin = PWD;
	else if (!ft_strcmp(node->cmd_args[0], "export"))
		node->builtin = EXPORT;
	else if (!ft_strcmp(node->cmd_args[0], "unset"))
		node->builtin = UNSET;
	else if (!ft_strcmp(node->cmd_args[0], "env"))
		node->builtin = ENV;
	else if (!ft_strcmp(node->cmd_args[0], "exit"))
		node->builtin = EXIT;
}

/**
 * @brief Update the parse state, to decide which parse function to call.
 *
 * @param table			char ** 2D parsing table
 * @param parser		parser struct
 * @param parse_state	current state of the parser
 */
void update_p_state(char **table, t_parser *parser, t_parse_state *parse_state)
{
	t_token			cur;
	t_token_type	next;

	if (parser->cur == 0)
	{
		*parse_state = LIST;
		return ;
	}
	cur = parser->tokens[parser->cur];
	next = peek(parser);
	if (cur.type > TOKEN_R_PAREN)
		*parse_state = ERR;
	else
		*parse_state = table[cur.type][next];
}
