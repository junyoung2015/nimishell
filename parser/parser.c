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
    t_node          *cmd_node;
    t_node          *redir_in_node;
    t_node          *redir_out_node;
    t_token_type    type;

	redir_in_node = 0;
	redir_out_node = 0;
    if (*token_idx >= num_tokens || (*tokens)[*token_idx].type != TOKEN_WORD)
        return (0);
    cmd_node = create_node(AST_COMMAND);
    if (!cmd_node)
        return (0);
    cmd_node->cmd_args = ft_calloc(num_tokens - *token_idx + 1, sizeof(char *));
    if (!cmd_node->cmd_args)
    {
        free(cmd_node);
        return (0);
    }
    while (*token_idx < num_tokens && ((*tokens)[*token_idx].type == TOKEN_WORD))
    {
        cmd_node->cmd_args[cmd_node->num_args] = ft_strdup((*tokens)[*token_idx].value);
        if (!cmd_node->cmd_args[cmd_node->num_args])
        {
            for (t_size i = 0; i < cmd_node->num_args; i++)
                free(cmd_node->cmd_args[i++]);
            free(cmd_node);
            return (0);
        }
        cmd_node->num_args++;
        (*token_idx)++;
    }
    while (*token_idx < num_tokens)
    {
        type = (*tokens)[*token_idx].type;
        if (type == TOKEN_REDIR_IN)
        {
            (*token_idx)++;
            if (*token_idx < num_tokens && (*tokens)[*token_idx].type == TOKEN_WORD)
            {
                redir_in_node = create_node(AST_REDIR_IN);
                if (!redir_in_node)
                {
                    free_ast(cmd_node);
                    return (0);
                }
                redir_in_node->cmd_args = ft_calloc(2, sizeof(char *));
                redir_in_node->cmd_args[0] = ft_strdup((*tokens)[*token_idx].value);
                redir_in_node->num_args = 1;
                cmd_node->left = redir_in_node;
            }
        }
        else if (type == TOKEN_REDIR_OUT)
        {
            (*token_idx)++;
            if (*token_idx < num_tokens && (*tokens)[*token_idx].type == TOKEN_WORD)
            {
                redir_out_node = create_node(AST_REDIR_OUT);
                if (!redir_out_node)
                {
                    free_ast(cmd_node);
                    return (0);
                }
                redir_out_node->cmd_args = ft_calloc(2, sizeof(char *));
                redir_out_node->cmd_args[0] = ft_strdup((*tokens)[*token_idx].value);
                redir_out_node->num_args = 1;
                cmd_node->right = redir_out_node;
            }
        }
        else
            break ;
        (*token_idx)++;
    }
    return (cmd_node);
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
