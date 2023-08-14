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
#  define DEBUG 1
# endif

/* ================ INCLUDES ================= */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include "readline/readline.h"
# include "readline/history.h"

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
	END,
} t_token_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SQ_STR,
	TOKEN_DQ_STR,
	TOKEN_OPERATOR,
	TOKEN_PIPE,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_REDIR_IN,
	TOKEN_HEREDOC,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_L_PAREN,
	TOKEN_WILDCARD,
	TOKEN_R_PAREN,
	TOKEN_WHITESPACE,
	TOKEN_ERROR,
	TOKEN_UNKNOWN,
	TOKEN_TYPES_CNT,
} t_token_type;

typedef enum e_builtin
{
	NOT_BUILTIN,
	ECHO_BUILTIN,
	CD,
	PWD,
	EXPORT,
	UNSET,
	ENV,
	EXIT,
} t_builtin;

typedef struct	s_token
{
	t_token_type type;
	char *val;
} t_token;

typedef struct	s_lexer
{
	t_token	*tokens;
	t_size	num_tokens;
} t_lexer;

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
	AST_REDIR_LIST,
	AST_SUBSHELL,
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
	struct s_node	*sibling; 
	int				pipe_open;
	t_node_type		parent_type;
	t_builtin		builtin;
}   t_node;

typedef struct s_parser
{
	t_token			*tokens;
	t_size			cur;
	t_size			size;
} t_parser;

typedef struct s_global_info
{
	char	**env;
	size_t	env_cnt;
	int		stdin_fd;
	int		stdout_fd;
	t_node	*root;
	int		exit_status;
} t_global_info;

typedef struct s_search_info
{
	char	**files;
	t_size	num_files;
	t_size	*prev_pos;
} t_search;

// TODO: re-write the global variable 'extern' declaration
extern t_global_info g_info;

/* ================== MACRO ================== */
# define STD_IN					0
# define STD_OUT				1
# define STD_ERR				2

# define READ					0
# define WRITE					1

# define EXIT_NORMAL			0
# define EXIT_ERR				1
# define EXIT_NOT_EXECUTABLE	126
# define EXIT_CMD_NOT_FOUND		127

# define TRUE					1
# define FALSE					0

/* ============= GENERAL MACRO =============== */
# define MINISHELL				"minishell: "
# define COLON					": "

/* =============== PROMPT GIT ================ */
# define GIT_BRANCH				".git/HEAD"
# define REF_HEAD				"ref: refs/heads/"
# define DETACHED_HEAD			"detached HEAD"
# define MAX_BRANCH_LEN			100

/* ================= BUILTIN ================= */
# define CD_BUILTIN				"cd"


/* ================ ERR MSG ================== */
# define TCGETATTR				"tcgetattr"
# define CD_HOME_NOT_SET		"HOME not set\n"
# define CD_OLDPWD_NOT_SET		"OLDPWD not set\n"
# define CMD_NOT_FOUND			"command not found\n"
# define NO_FILE_DIR			"No such file or directory\n"
# define QUOTE_NOT_CLOSED		"Syntax Error: unmatched quote\n"
# define PAREN_NOT_CLOSED		"Syntax Error: unmatched parenthesis\n"
# define MALLOC_ERR				"malloc() error."

/* ============== MEMORY_UTILS =============== */
void			ft_bzero(void *s, t_size n);
void			*ft_calloc(t_size count, t_size size);
void			*ft_memset(void *b, int c, t_size len);
void			*ft_memcpy(void *dst, const void *src, t_size n);

/* ================ STR_UTILS ================ */
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, t_size n);
t_size			ft_strlen(const char *str);
t_size			ft_strlcpy(char *dst, const char *src, t_size dstsize);
char			*ft_itoa(int n);
char			*ft_strchr(const char *s, int c);

/* ================== EXPANSION ================== */
typedef t_bool	(*t_cmp)(char);
int				cmp_ascii(void *a, void *b);
void			ft_qsort(void **arr, t_ssize low, t_ssize high, int (*cmp)(void *, void *));
char			**env_substitution(t_node *node);
char		    **remove_quotes(t_node *node);
char			**wildcard_substitution(t_node *node);
char			*trim_outer_quotes(char *cmd_arg);
char			*trim(char	**cmd_arg, t_cmp cmp);
char			**str_expansion(t_node *node);
void			ft_arrfree(char **arr);

t_search	*create_search_info(char **files, int num_files);
void		free_search_info(t_search *info);
// t_ssize 	match_pattern(char *str, char *pattern, int prev_pos);
t_bool		search_files(t_search *info, char *pattern);
t_bool		is_wildcard(char ch);

/* ================== INIT ================== */
void			print_logo(void);

/* ================ STR_UTILS_CREATE ================ */
char			*ft_strtrim(char *str, char c);
char			*ft_strdup(char *src);
char			*ft_substr(char const *s, t_size start, t_size len);
char			**ft_split(char const *str, char c);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strnstr(const char *haystack, const char *needle, t_size n);

/* ================== TOKENIZER ================== */
typedef t_token *(*t_tokenizer_fn)(char **, t_token_state *);

void			print_tokens(t_token *tokens, t_size num_tokens);
t_token			*free_tokens(t_token *tokens, t_size size);
t_token			*create_token(t_token_type type, const char *buffer, t_size buf_len);

t_bool			is_alnum(int c);
t_bool			is_escaped(char ch);
t_bool			is_meta_ch(char ch);
t_bool			is_squote(char ch);
t_bool			is_dquote(char ch);
t_bool			is_quote(char ch);
t_bool			is_space(char ch);
t_bool			is_not_space(char ch);
t_bool			is_env_var(char ch); // ?
// FSM //
t_token			*tokenize_input(char *input, t_size alloced, t_size *num_tokens);
t_token_state	update_state(char ch);
t_token			*tokenize_normal(char **input, t_token_state *state);
t_token			*tokenize_squote(char **input, t_token_state *state);
t_token			*tokenize_dquote(char **input, t_token_state *state);
t_token			*tokenize_meta(char **input, t_token_state *state);
t_token			*tokenize_whitespace(char **input, t_token_state *state);
t_cmp			get_cmp_fn(char ch);

/* ================== LEXER ================== */
void			categorize_tokens(t_token *tokens, t_size num_tokens);

/* ================== PARSER ================== */
t_node			*parse_tokens(t_token *tokens, t_size num_tokens);
t_node			*parse_pipe(t_token **tokens, t_size *token_idx, t_size num_tokens);
void			free_ast(t_node *root);
t_node			*create_node(t_node_type type);
void			append_child_node(t_node *parent, t_node *child);
void			free_ast(t_node *root);
/* LL(1) */
t_node			*parse_tokens_ll(t_token *tokens, t_size num_tokens);
void			is_builtin_node(t_node *node);

/* ================== EXECUTOR ================== */
int				executor(t_node *root);

/* ================== BUILTIN ================== */
char			*get_env(char *env_var);
void			display_cmd_err(char *msg);

#endif