/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:10:03 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/17 13:58:32 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node *parse_redir(t_token **tokens, t_size *token_idx, t_size num_tokens, t_node *left_node);

t_node *create_node(t_node_type type)
{
	t_node	*new_node;
	new_node = (t_node *) ft_calloc(1, sizeof(t_node));
	if (!new_node)
		return (0);
	new_node->type = type;
	new_node->cmd_args = 0;
	new_node->num_args = 0;
	new_node->left = 0;
	new_node->right = 0;
	new_node->pipe_open = 0;
	new_node->parent_type = AST_UNSET;
	return (new_node);
}

void append_child_node(t_node *parent, t_node *child)
{
	if (!parent->left)
		parent->left = child;
	else if (!parent->right)
		parent->right = child;
}

t_node *parse_cmd(t_token **tokens, t_size *token_idx, t_size num_tokens)
{
	t_node	*cmd_node;

	if (*token_idx >= num_tokens || (*tokens)[*token_idx].type != TOKEN_WORD)
		return (0);
	cmd_node = create_node(AST_COMMAND);
	if (!cmd_node)
		return (0);
	// skip the command token
	// (*token_idx)++;
	// Allocate memory for cmd_args
	cmd_node->cmd_args = ft_calloc(num_tokens - *token_idx + 1, sizeof(char *));
	if (!cmd_node->cmd_args)
	{
		free(cmd_node);
		return (0);
	}
	// while next tokens are arguments, append them to the command
	while (*token_idx < num_tokens && ((*tokens)[*token_idx].type == TOKEN_WORD))
	{
		cmd_node->cmd_args[cmd_node->num_args] = ft_strdup((*tokens)[*token_idx].value);
		if (!cmd_node->cmd_args[cmd_node->num_args])
		{
			// TODO: remove for loop
			for (t_size i = 0; i < cmd_node->num_args; i++)
				free(cmd_node->cmd_args[i++]);
			free(cmd_node);
			return (0);
		}
		cmd_node->num_args++;
		// Move to the next token
		(*token_idx)++;
	}
	if (*token_idx < num_tokens &&
        ((*tokens)[*token_idx].type == TOKEN_REDIR_IN || (*tokens)[*token_idx].type == TOKEN_REDIR_OUT)) {
        cmd_node = parse_redir(tokens, token_idx, num_tokens, cmd_node);
    }
	return (cmd_node);
}

t_node *parse_redir(t_token **tokens, t_size *token_idx, t_size num_tokens, t_node *left_node)
{
    t_node *redir_node;
    if (*token_idx >= num_tokens)
        return (left_node);
    if ((*tokens)[*token_idx].type == TOKEN_REDIR_OUT)
        redir_node = create_node(AST_REDIR_OUT);
    else if ((*tokens)[*token_idx].type == TOKEN_REDIR_IN)
	   redir_node = create_node(AST_REDIR_IN);
    else
        return (left_node);
	if (!redir_node)
		return (0);
    (*token_idx)++;
    if (*token_idx >= num_tokens || (*tokens)[*token_idx].type != TOKEN_WORD)
	{
        // TODO: Handle syntax error - expected filename after redirection
        return (0);
    }
    redir_node->cmd_args = ft_calloc(2, sizeof(char *));
    if (!redir_node->cmd_args)
	{
        free(redir_node);
        return (0);
    }
    redir_node->cmd_args[0] = ft_strdup((*tokens)[*token_idx].value);
	if (!redir_node->cmd_args[0])
		return (0);
    redir_node->num_args = 1;
    redir_node->left = left_node;
    (*token_idx)++;
    return (redir_node);
}

t_node *parse_pipe(t_token **tokens, t_size *token_idx, t_size num_tokens)
{
    t_node	*left_node;
	t_node	*right_node;
	t_node	*pipe_node;
	left_node = parse_cmd(tokens, token_idx, num_tokens);
    if (*token_idx < num_tokens && (*tokens)[*token_idx].type == TOKEN_PIPE)
	{
		(*token_idx)++;
		right_node = parse_pipe(tokens, token_idx, num_tokens);
		if (!right_node)
			return (0);
		pipe_node = create_node(AST_PIPE);
		if (!pipe_node)
			return (0);
		pipe_node->left = left_node;
		pipe_node->right = right_node;
		return (pipe_node);
    }
    return (left_node);
}

/**
 * @brief build AST based on the grammar, using array of tokens received
 * 
 * @param tokens 
 * @param num_tokens 
 * @return t_node* 
 */
t_node *parse_tokens(t_token *tokens, t_size num_tokens)
{
	t_size	token_idx;

	token_idx = 0;
	return (parse_pipe(&tokens, &token_idx, num_tokens));
}

void free_ast(t_node *root)
{
	// t_size i;

	if (root == 0)
		return ;
	if (root->left != 0)
		free_ast(root->left);
	if (root->right != 0)
		free_ast(root->right);
	if (root->cmd_args)
	{
		for (t_size i = 0; i < root->num_args; i++)
			free(root->cmd_args[i]);
		free(root->cmd_args);
	}
	free(root);
}
