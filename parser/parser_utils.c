/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:57:04 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:18:19 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Production rule, in BNF grammar form
	========== NOTE ==========
	// {} : 0 or more occurrences
	// [] : denotes 0 or 1 occurrence
	//  | : "OR"
	// : not sure
	<NONSPACE> -> any character satisfying the condition !isspace(character)

	<WORD> -> <NONSPACE>

	<ENV-VAR> -> $ <WORD>

	<WORD-LIST> -> <WORD> <WORD-LIST-TAIL>

	<WORD-LIST-TAIL> -> <WORD> <WORD-LIST-TAIL>  // advance() in this case
	<WORD-LIST-TAIL> -> ε

	<ASSIGNMENT-WORD> -> <WORD> = <WORD>

	<REDIRECTION> -> > <WORD-LIST>	 // advance() in this case
	<REDIRECTION> -> < <WORD-LIST>	 // advance() in this case
	<REDIRECTION> -> >> <WORD-LIST>	 // advance() in this case
	<REDIRECTION> -> << <WORD-LIST>	 // advance() in this case

	<SIMPLE-COMMAND-ELEMENT> -> <WORD-LIST>
	<SIMPLE-COMMAND-ELEMENT> -> <ASSIGNMENT-WORD>
	<SIMPLE-COMMAND-ELEMENT> -> <REDIRECTION-LIST>
	<SIMPLE-COMMAND-ELEMENT> -> <ENV-VAR>

	<REDIRECTION-LIST> -> <REDIRECTION> <REDIRECTION-LIST-TAIL>
	<REDIRECTION-LIST> -> <REDIRECTION-LIST-TAIL>

	<REDIRECTION-LIST-TAIL> -> <REDIRECTION> <REDIRECTION-LIST-TAIL>
	<REDIRECTION-LIST-TAIL> -> ε

	<SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>

	<SIMPLE-COMMAND-TAIL> -> <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
	<SIMPLE-COMMAND-TAIL> -> ε

	<COMMAND> -> <SIMPLE-COMMAND>
	<COMMAND> -> <SUBSHELL>
	<COMMAND> -> <SUBSHELL> <REDIRECTION-LIST>
	<COMMAND> -> <SUBSHELL> <PIPELINE>

	<SUBSHELL> -> ( <LIST> )

	<LIST> -> <PIPELINE> <LIST-TAIL>

	<LIST-TAIL> -> && <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> || <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> ε

	<PIPELINE> -> <COMMAND> <PIPELINE-TAIL>
	<PIPELINE> -> <PIPELINE-TAIL>i

	<PIPELINE-TAIL> -> | <PIPELINE>				// advance() in this case
	 <PIPELINE-TAIL> -> | <COMMAND> <PIPELINE-TAIL>	// advance() in this case
	<PIPELINE-TAIL> -> ε
*/

/*
	Rules for parsing table:
	0		ERR
	1		ENV_VAR
	2		CMD
	3		WORD
	4		REDIR_LIST
	5		PIPELINE
	6		LIST
	7		SUBSHELL

	R1	TOKEN_WORD
	R2	TOKEN_PIPE
	R3	TOKEN_OR
	R4	TOKEN_AND
	R5	TOKEN_REDIR_IN
	R6	TOKEN_HEREDOC
	R7	TOKEN_REDIR_OUT
	R8	TOKEN_APPEND
	R9	TOKEN_DOLLAR_SIGN // TODO: remove this when refactoring
	R10	TOKEN_L_PAREN
	R11	TOKEN_SQ_STR
	R12	TOKEN_DQ_STR
	R13	TOKEN_OPERATOR
*/

#define R1	"\2\5\6\6\4\4\4\4\1\0\2\2\2"
#define R11	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define R12	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define R13	"\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define R2	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define R3	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define R4	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define R5	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define R6	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define R7	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define R8	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define R9	"\1\0\0\0\0\0\0\0\3\0\0\0\0"
#define R10	"\2\0\0\0\0\4\0\0\1\7\0\2\2"

/**
 * @brief Check whether current token is a word or not. SQ_STR and DQ_STR are
 * 		also considered as a word, like bash.
 * 
 * @param parser	parser struct
 * @return t_bool	TRUE if current token is a word, FALSE otherwise
 */
t_bool	is_word_token(t_parser *parser)
{
	t_type	type;

	type = cur_type(parser);
	if (parser->cur < parser->size)
		return (type == TOKEN_WORD || type == TOKEN_SQ_STR \
		|| type == TOKEN_DQ_STR);
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
	t_type	type;

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
 * @param parser		parser struct
 * @param parse_state	current state of the parser
 */
void	update_p_state(t_parser *parser, t_parse_state *parse_state)
{
	t_token		cur;
	t_type		next;
	const char	*rule[14] = {R1, R11, R12, R13, R2, R3, R4, R5, R6, R7, R8, \
	R9, R10};

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
		*parse_state = rule[cur.type][next];
}
