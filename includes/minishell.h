/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 16:18:24 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/30 13:47:31 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#ifndef DEBUG
#define DEBUG 1
#endif

/* ================ INCLUDES ================= */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "readline/readline.h"
#include "readline/history.h"

/* ================== TYPES ================== */
typedef int t_bool;
typedef unsigned long long t_size;
typedef long long t_ssize;

typedef enum e_token_state
{
	NORMAL,
	SQUOTE,
	DQUOTE,
	META_CH,
	WSPACE,
	ESCAPE,
	END,
} t_token_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_REDIR_IN,
	TOKEN_HEREDOC,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_DOLLAR_SIGN,
	TOKEN_L_PAREN,
	TOKEN_WILDCARD,
	TOKEN_SQ_STR,
	TOKEN_DQ_STR,
	TOKEN_R_PAREN,
	TOKEN_SUBSHELL,
	TOKEN_ENV_VAR,
	TOKEN_WHITESPACE,
	TOKEN_OPERATOR,
	TOKEN_ERROR,
	TOKEN_UNKNOWN,
	TOKEN_TYPES_CNT,
} t_token_type;

typedef enum e_builtin
{
	NOT_BUILTIN,
	ECHO,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
} t_builtin;

typedef struct s_token
{
	t_token_type type;
	char *value;
} t_token;

typedef enum e_parse_state
{
	ERR,
	ENV_VAR,
	CMD,
	WORD,
	REDIR_LIST,
	PIPELINE,
	LIST,
	SIMPLE_CMD,
	ASSIGN_WORD,
	SUBSHELL,
	SIMPLE_CMD_ELE,
	REDIR,
	REDIR_TAIL,
	WORD_LIST,
	WORD_LIST_TAIL,
	SIMPLE_CMD_TAIL,
	PIPELINE_TAIL,
	LIST_TAIL,
	PARSE_STATES_CNT,
} t_parse_state;

typedef enum e_node_type
{
	AST_NULL,
	AST_SIMPLE_CMD,
	AST_SIMPLE_CMD_ELEMENT,
	AST_CMD,
	AST_LIST,
	AST_LIST_TAIL,
	AST_PIPE,
	AST_REDIR_IN,
	AST_HEREDOC,
	AST_REDIR_OUT,
	AST_REDIR_APPEND,
	AST_SUBSHELL,
	AST_BUILTIN,
	AST_AND,
	AST_OR,
	AST_WORD_LIST,
	AST_ERR,
} t_node_type;

typedef struct s_node
{
	t_node_type		type;
	char			**cmd_args;
	t_size			num_args;
	struct s_node	*left;
	struct s_node	*right;
	int				pipe_open;
	t_node_type		parent_type;
	t_builtin		builtin;
}   t_node;

typedef struct s_parser
{
	t_token *tokens;
	t_size cur;
	t_size size;
	t_node *tmp;
	void (*advance)(struct s_parser *);
	t_token_type (*cur_type)(struct s_parser *);
	t_bool (*check)(struct s_parser *, t_token_type);
	t_token_type (*peek)(struct s_parser *);
	t_token_type (*consume)(struct s_parser *);
} t_parser;

typedef struct s_global_info
{
	char	**env;
	size_t	env_cnt;
	int		stdin_fd;
	int		stdout_fd;
	t_node	*root;
} t_global_info;

typedef t_token *(*tokenizer_fn)(char **, t_token_state *);

// TODO: re-write the global variable 'extern' declaration
extern t_global_info g_info;

/* ================== MACRO ================== */
#define STD_IN 0
#define STD_OUT 1
#define STD_ERR 2

#define READ 0
#define WRITE 1

#define EXIT_NORMAL 0
#define EXIT_ERR 1
#define EXIT_NOT_EXECUTABLE 126
#define EXIT_CMD_NOT_FOUND 127

#define TRUE 1
#define FALSE 0

#define QUOTE_NOT_CLOSED "Syntax Error: unmatched quote\n"
#define PAREN_NOT_CLOSED "Syntax Error: unmatched parenthesis\n"
#define MALLOC_ERR "malloc() error."

/* ================== MEMORY_UTILS ================== */
void ft_bzero(void *s, t_size n);
void *ft_calloc(t_size count, t_size size);
void *ft_memset(void *b, int c, t_size len);
void *ft_memcpy(void *dst, const void *src, t_size n);

/* ================== STR_UTILS ================== */
int ft_strcmp(const char *s1, const char *s2);
int ft_strncmp(const char *s1, const char *s2, t_size n);
t_size ft_strlen(const char *str);
t_size ft_strlcpy(char *dst, const char *src, t_size dstsize);
int ft_isalnum(int c);

/* ================== INIT ================== */
void	print_logo(void);

/* ================ STR_UTILS_CREATE ================ */
char *ft_strtrim(char *str, char c);
char *ft_strdup(char *src);
char **ft_split(char const *str, char c);

/* ================== TOKENIZER ================== */
void print_tokens(t_token *tokens, t_size num_tokens);
int free_tokens(t_token *tokens, t_size size);
t_token *create_token(t_token_type type, const char *buffer, int buffer_length);
// FSM //
t_token *tokenize_cmd(char *input, t_size *num_tokens);
t_token_state update_state(char ch);
t_token *tokenize_normal(char **input, t_token_state *state);
t_token *tokenize_squote(char **input, t_token_state *state);
t_token *tokenize_dquote(char **input, t_token_state *state);
t_token *tokenize_meta(char **input, t_token_state *state);
t_token *tokenize_whitespace(char **input, t_token_state *state);

/* ================== LEXER ================== */
void categorize_tokens(t_token *tokens, t_size num_tokens);

/* ================== PARSER ================== */
t_node *parse_tokens(t_token *tokens, t_size num_tokens);
t_node *parse_pipe(t_token **tokens, t_size *token_idx, t_size num_tokens);
void free_ast(t_node *root);
t_node *create_node(t_node_type type);
void append_child_node(t_node *parent, t_node *child);
void free_ast(t_node *root);
/* LL(1) */
t_node *parse_tokens_ll(t_token *tokens, t_size num_tokens);

/* ================== EXECUTOR ================== */
int executor(t_node *root);

#endif