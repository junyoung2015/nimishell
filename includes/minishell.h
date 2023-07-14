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
# define MINISHELL_H

# ifndef DEBUG
#	define DEBUG					1
# endif

/* ================ INCLUDES ================= */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include "readline/readline.h"
# include "readline/history.h"

/* ================== TYPES ================== */
typedef int							t_bool;
typedef unsigned long long			t_size;
typedef long long					t_ssize;

typedef enum e_token_state
{
	NORMAL,
	SQUOTE,
	DQUOTE,
	META_CH,
	WSPACE,
	END,
} t_token_state;

typedef enum	e_token_type
{
	TOKEN_UNKNOWN,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_WILDCARD,
	TOKEN_L_PARENTHESIS,
	TOKEN_R_PARNTHESIS,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
    TOKEN_HEREDOC_DELIM,
	TOKEN_APPEND,
	TOKEN_DOLLAR_SIGN,
	TOKEN_ENV_VAR,
	TOKEN_WORD,
	TOKEN_WHITESPACE,
	TOKEN_OPERATOR,
	TOKEN_SQ_STR,
	TOKEN_DQ_STR,
	TOKEN_SUBSHELL,
}	t_token_type;

typedef struct  s_token
{
  t_token_type	type;
  char			*value;
}   t_token;

typedef enum e_node_type
{
    AST_NULL,
    AST_COMMAND,
    AST_ARGUMENT,
    AST_PIPE,
    AST_REDIR_IN,
    AST_REDIR_OUT,
    AST_REDIR_APPEND,
	AST_HEREDOC,
	AST_SUBSHELL
} t_node_type;

typedef struct  s_node
{
	t_node_type		type;
	char			**cmd_args;
	t_size			num_args;
	struct s_node	*left;
	struct s_node	*right;
	int				pipe_open;
	t_node_type		parent_type;
}   t_node;

typedef struct s_global_info
{
	char	**env;
	char 	**path;
	int		exit_code;
	t_node	*root;
} t_global_info;

extern t_global_info	g_info;

/* ================== MACRO ================== */
# define STD_IN						0
# define STD_OUT					1
# define STD_ERR					2

# define READ						0
# define WRITE						1

# define EXIT_NORMAL				0
# define EXIT_ERR					1
# define EXIT_NOT_EXECUTABLE		126
# define EXIT_CMD_NOT_FOUND			127

# define TRUE					1
# define FALSE					0

# define MALLOC_ERR					"malloc() error."

/* ================== MEMORY_UTILS ================== */
void		ft_bzero(void *s, t_size n);
void		*ft_calloc(t_size count, t_size size);
void		*ft_memset(void *b, int c, t_size len);
void		*ft_memcpy(void *dst, const void *src, t_size n);

/* ================== STR_UTILS ================== */
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, t_size n);
t_size		ft_strlen(const char *str);
t_size		ft_strlcpy(char *dst, const char *src, t_size dstsize);
int			ft_isalnum(int c);

/* ================ STR_UTILS_CREATE ================ */
char		*ft_strtrim(char *str, char c);
char		*ft_strdup(char *src);
char		**ft_split(char const *str, char c);

/* ================== TOKENIZER ================== */
t_token 	*tokenize(char *input, t_size *num_tokens);
void 		print_tokens(t_token *tokens, t_size num_tokens);
void 		categorize_tokens(t_token *tokens, t_size num_tokens);
// FSM //
t_token *tokenize_cmd(char *input, t_size *num_tokens);

/* ================== LEXER ================== */
t_token		*create_token(t_token_type type, const char *buffer, int buffer_length);
int			free_tokens(t_token *tokens, t_size size);

/* ================== PARSER ================== */
t_node	*parse_tokens(t_token *tokens, t_size num_tokens);
void		free_ast(t_node *root);

/* ================== EXECUTOR ================== */
int			executor(t_node *root);

# endif