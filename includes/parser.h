/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:22:20 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/14 16:23:12 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

/*
	Production rule, in BNF grammar form
	========== NOTE ==========
	// {} : 0 or more occurrences
	// [] : denotes 0 or 1 occurrence
	//  | : "OR"
	// ?: not sure
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
	?<REDIRECTION-LIST> -> <REDIRECTION-LIST-TAIL>?

	<REDIRECTION-LIST-TAIL> -> <REDIRECTION> <REDIRECTION-LIST-TAIL>
	<REDIRECTION-LIST-TAIL> -> ε

	<SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>

	<SIMPLE-COMMAND-TAIL> -> <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
	?<SIMPLE-COMMAND-TAIL> -> ε?

	<COMMAND> -> <SIMPLE-COMMAND>
	<COMMAND> -> <SUBSHELL>
	<COMMAND> -> <SUBSHELL> <REDIRECTION-LIST>
	?<COMMAND> -> <SUBSHELL> <PIPELINE>?

	<SUBSHELL> -> ( <LIST> )

	<LIST> -> <PIPELINE> <LIST-TAIL>

	<LIST-TAIL> -> && <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> || <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> ε

	<PIPELINE> -> <COMMAND> <PIPELINE-TAIL>
	?<PIPELINE> -> <PIPELINE-TAIL>?

	?<PIPELINE-TAIL> -> | <PIPELINE>?				// advance() in this case
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

	ROW1	TOKEN_WORD
	ROW2	TOKEN_PIPE
	ROW3	TOKEN_OR
	ROW4	TOKEN_AND
	ROW5	TOKEN_REDIR_IN
	ROW6	TOKEN_HEREDOC
	ROW7	TOKEN_REDIR_OUT
	ROW8	TOKEN_APPEND
	ROW9	TOKEN_DOLLAR_SIGN // TODO: remove this when refactoring
	ROW10	TOKEN_L_PAREN
	ROW11	TOKEN_WILDCARD
	ROW12	TOKEN_SQ_STR
	ROW13	TOKEN_DQ_STR
 */
#define ROW1	"\2\5\6\6\4\4\4\4\1\0\2\2\2"
#define ROW2	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW3	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW4	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW5	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW6	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW7	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW8	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW9	"\1\0\0\0\0\0\0\0\3\0\0\0\0"
#define ROW10	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW11	"\2\5\6\6\4\4\4\4\2\0\0\2\2"
#define ROW12	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define ROW13	"\2\5\6\6\4\4\4\4\2\0\2\2\2"

char	*parse_word(t_parser *parser, t_node *parent);
t_node	*parse_word_list(t_parser *parser, t_node *parent);
t_node	*parse_redir(t_parser *parser, t_node *parent);
t_node	*parse_simple_cmd_element(t_parser *parser, t_node *parent);
t_node	*p_redir_l(t_parser *parser, t_node *parent);
t_node	*p_redir_l_tail(t_parser *parser, t_node *parent);
t_node	*parse_simple_cmd(t_parser *parser, t_node *parent);
t_node	*parse_simple_cmd_tail(t_parser *parser, t_node *parent);
t_node	*p_cmd(t_parser *parser, t_node *parent);
t_node	*p_sub(t_parser *parser, t_node *parent);
t_node	*p_l(t_parser *parser, t_node *parent);
t_node	*p_l_tail(t_parser *parser, t_node *parent);
t_node	*p_pipe_l(t_parser *parser, t_node *parent);
t_node	*p_pipe_l_tail(t_parser *parser, t_node *parent);
t_node	*p_err(t_parser *parser, t_node *parent);

# endif