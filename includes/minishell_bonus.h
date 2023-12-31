/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:50:07 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:50:07 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

/* ================ INCLUDES ================= */
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <dirent.h>
# include "readline/readline.h"
# include "readline/history.h"

/* ================== TYPES ================== */
typedef int					t_bool;
typedef unsigned long long	t_size;
typedef long long			t_ssize;
typedef t_bool				(*t_cmp)(char);

typedef enum e_token_state
{
	NORMAL,
	SQUOTE,
	DQUOTE,
	META_CH,
	END,
}	t_state;

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
}	t_type;

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
}	t_builtin;

typedef struct s_token
{
	t_type	type;
	char	*val;
}	t_token;

typedef struct s_lexer
{
	t_token	*tokens;
	t_size	num_tokens;
}	t_lexer;

typedef enum e_parse_state
{
	ERR,
	CMD,
	REDIR_LIST,
	PIPELINE,
	LIST,
	SUBSHELL,
	PARSE_STATES_CNT,
}	t_parse_state;

typedef enum e_node_type
{
	AST_NULL,
	AST_CMD,
	AST_PIPE,
	AST_REDIR_IN,
	AST_HEREDOC,
	AST_REDIR_OUT,
	AST_REDIR_APPEND,
	AST_REDIR_LIST,
	AST_SUBSHELL,
	AST_OR,
	AST_AND,
	AST_WORD_LIST,
	AST_ERR,
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	char			**cmd_args;
	t_size			num_args;
	struct s_node	*left;
	struct s_node	*right;
	int				pipe_open;
	int				fd;
	t_builtin		builtin;
}	t_node;

typedef struct s_parser
{
	t_token			*tokens;
	t_size			cur;
	t_size			size;
}	t_parser;

typedef struct s_search_info
{
	char	**files;
	t_size	num_files;
	t_size	*prev_pos;
	char	**res;
}	t_search;

typedef struct s_minishell_info
{
	char	*pwd;
	char	*line;
	int		exit_code;
	t_node	*ast;
	t_size	env_cnt;
	t_size	num_tokens;
	t_token	*tokens;
}	t_sh_info;

extern char					**g_env;

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

# define ANDOR					4
# define REDIR					4

# define OLDPWD					"OLDPWD="
# define SHLVL					"SHLVL="
# define SHLVL_INIT				"SHLVL=1"

/* ============= GENERAL MACRO =============== */
# define MINISHELL				"minishell: "
# define COLON					": "
# define SIGINT_ECHO            "^C\n"
# define SIGQUIT_ECHO           "^\\Quit: 3\n"

/* =============== PROMPT GIT ================ */
# define GIT_BRANCH				".git/HEAD"
# define REF_HEAD				"ref: refs/heads/"
# define DETACHED_HEAD			"detached HEAD"
# define MAX_BRANCH_LEN			100

/* ================= PARSER ================== */
# define PIPE                   "|"
# define REDIR_IN               "<"
# define REDIR_OUT              ">"
# define APPEND                 ">>"
# define HEREDOC                "<<"
# define NL                     "newline"
# define AND					"&&"
# define OR						"||"

/* ================= BUILTIN ================= */
# define CD_BUILTIN				"cd"
# define EXP_BUILTIN			"export"
# define UNSET_BUILTIN			"unset"

/* ================ ERR MSG ================== */
# define TCGETATTR				"tcgetattr"
# define CD_HOME_NOT_SET		"HOME not set\n"
# define CD_OLDPWD_NOT_SET		"OLDPWD not set\n"
# define NOT_VALID_ID			"not a valid identifier\n"
# define CMD_NOT_FOUND			"command not found\n"
# define NO_FILE_DIR			"No such file or directory\n"
# define QUOTE_NOT_CLOSED		"minishell: syntax error: unmatched quote\n"
# define PAREN_NOT_CLOSED		"minishell: syntax error:\
 unmatched parenthesis\n"
# define MALLOC_ERR				"malloc() error\n"

/* ================== INIT ================== */
void			print_logo(void);
char			*get_prompt(void);
void			exit_err_msg(int code, char *file, char *msg, t_node *root);
void			init_env(char **envp, t_sh_info *info);

/* ============== MEMORY_UTILS =============== */
void			ft_bzero(void *s, t_size n);
void			*ft_calloc(t_size count, t_size size);
void			*ft_memcpy(void *dst, const void *src, t_size n);

/* ================ STR_UTILS ================ */
int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, t_size n);
t_size			ft_strlen(const char *str);
t_size			ft_strlcpy(char *dst, const char *src, t_size dstsize);

/* =============== STR_UTILS_2 ================ */
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strnstr(const char *haystack, const char *needle, t_size n);

/* ============ STR_UTILS_CREATE ============= */
char			*ft_strtrim(char const *s1, char const *s2);
char			*ft_strdup(char *src);
char			*ft_substr(char const *s, t_size start, t_size len);
char			*ft_strjoin(char const *s1, char const *s2);

/* ================ STR_ITOA ================= */
char			*ft_itoa(int n);
int				ft_atoi(const char *str);
char			**ft_split(char const *str, char c);

/* ============= ENV_SUB_CMP_FN ============== */
t_bool			is_number(int c);
t_bool			is_alpha(int c);
t_bool			is_alnum(int c);
t_bool			is_env_var(char ch);
t_bool			is_dollar(char ch);

/* ================ EXPANSION ================ */
int				cmp_ascii(void *a, void *b);
void			_qs(void **a, t_ssize l, t_ssize h, int (*c)(void *, void *));
char			**remove_quotes(t_node *node);
char			*trim_outer_quotes(char *cmd_arg);
char			*trim(char	**cmd_arg, t_cmp cmp, t_state *state);
char			**str_expansion(t_node *node);
t_search		*get_search_info(char *first);
t_search		*init_search_info(char *cmd_arg);

/* ============= EXPANSION_UTILS ============= */
void			free_search_info(t_search *info);
t_bool			search_files(t_search *info, char *pattern);
t_ssize			match_pattern(char *str, char *pattern, int prev_pos);
t_search		*create_search_info(char **files, int num_files);
void			get_all_files(char *first, t_size *size, char ***result);

/* ============= EXPANSION_SPLIT ============= */
t_bool			is_wildcard(char ch);
t_bool			is_wsplit(char ch);
t_bool			is_wildcard_expansion(char *cmd_arg);

/* ============ EXPANSION_PATTERN ============ */
char			**get_search_pattern(char *cmd_arg, t_size size);
t_size			handle_wildcard(char ***r, char **s, char **e, t_size size);
t_size			handle_quotes(char ***r, char **s, char **e, t_size size);
t_size			handle_normal(char ***r, char **s, char **e, t_size size);

/* ============= EXPANSION_MATCH ============= */
void			match_except_last(t_search *i, char **p, t_size *n, t_cmp *c);
t_size			match_pattern_first(t_search *info, char *pattern);
t_size			match_pattern_middle(t_search *info, char *pattern);
t_size			match_pattern_last(t_search *info, char *pattern, t_size last);

/* =============== ARRAY_UTILS =============== */
void			ft_arrfree(char **arr);
t_size			ft_arrlen(char **arr);
t_size			ft_arrcat(char ***arr, char **new_arr, t_size size);
t_size			ft_arr_append_back(char ***arr, char *str, t_size size);
t_size			ft_arr_append_front(char ***arr, char *str, t_size size);

/* ================ TOKENIZER ================ */
t_token			*tokenize_input(char *in, t_size alloced, t_size *num);
t_bool			init_tokenizer(char *i, t_token **t, t_size *a, t_state *s);

/* ============= CMP_FUNC_QUOTE ============== */
t_bool			is_squote(char ch);
t_bool			is_dquote(char ch);
t_bool			is_quote(char ch);
t_cmp			get_cmp_fn(char ch);

/* ============== CMP_FUNC_META ============== */
int				is_dmeta_str(char *input);
t_bool			is_meta(char ch);
t_bool			is_dmeta_ch(char ch);
t_bool			is_space(char ch);
t_bool			is_not_space(char ch);

/* ============= TOKENIZER_UTILS ============= */
t_token			*create_token(t_type type, const char *buffer, t_size buf_len);
t_token			*free_tokens(t_token *tokens, t_size size);
t_type			get_operator_type(char ch);

/* ============ TOKENIZER_STATES ============= */
t_state			update_state(char ch);
t_token			*tokenize(char **input, t_cmp cmp, t_type type, t_state *state);
t_token			*tokenize_operator(char **input, t_state *state);
t_token			*tokenize_meta(char **input, t_state *state);
t_bool			init_tokenizer(char *i, t_token **t, t_size *a, t_state *s);

/* ========= TOKENIZER_ERR_HANDLING ========== */
t_token			*create_err_token(char *msg);
t_token			*set_err_token(t_token *tokens, t_size *num_tokens, char *msg);
t_bool			check_parenthesis(t_token *tokens, t_size num_tokens);

/* ============= TOKENIZER_SPLIT ============= */
t_bool			move_until_cmp(char **input, t_cmp cmp);
t_token			*split_input(char *start, char **input, t_cmp cmp, t_type type);

/* ============ TOKENIZER_REALLOC ============ */
t_token			*realloc_tokens(t_token *tokens, t_size *num, t_size new_size);
t_token			*should_realloc(t_token *tokens, t_size *num, t_size *alloced);

/* ================= PARSER ================== */
typedef t_node				*(*t_parse)(t_parser *parser, t_node *parent);
t_node			*parse_tokens(t_token *tokens, t_size num_tokens);
void			free_ast(t_node *root);
t_node			*create_node(t_node_type type);
void			append_child_node(t_node *parent, t_node *child);
void			free_ast(t_node *root);
void			is_builtin_node(t_node *node);

char			*parse_word(t_parser *parser);
t_node			*parse_word_list(t_parser *parser, t_node *parent);
t_node			*parse_redir(t_parser *parser, t_node *parent);
t_node			*p_redir_l_tail(t_parser *parser, t_node *parent);
t_node			*p_redir_l(t_parser *parser, t_node *parent);
t_node			*parse_simple_cmd_element(t_parser *parser, t_node *parent);
t_node			*parse_simple_cmd_tail(t_parser *parser, t_node *parent);
t_node			*parse_simple_cmd(t_parser *parser, t_node *parent);
t_node			*p_cmd(t_parser *parser, t_node *parent);
t_node			*p_sub(t_parser *parser, t_node *parent);
t_node			*parse_subshell_list(t_parser *parser, t_node *parent);
t_node			*p_l_tail(t_parser *parser, t_node *parent);
t_node			*p_l(t_parser *parser, t_node *parent);
t_node			*p_pipe_l_tail(t_parser *parser, t_node *parent);
t_node			*p_pipe_l(t_parser *parser, t_node *parent);
t_node			*p_err(t_parser *parser, t_node *parent);

void			postorder_traversal(t_node *node, t_node **err_node);
t_bool			check_err_node(t_node *new_node);
t_node			*p_err(t_parser *parser, t_node *parent);
char			*tok_type(t_type type);

void			append_redir_node(t_node *parent, t_node *child);
void			append_child_node(t_node *parent, t_node *child);

/* ============== PARSER_UTILS =============== */
t_bool			is_word_token(t_parser *parser);
t_bool			is_redir_token(t_parser *parser);
void			is_builtin_node(t_node *node);
void			update_p_state(t_parser *parser, t_parse_state *parse_state);

/* =========== PARSER_STATUS_UTILS =========== */
void			advance(t_parser *parser);
t_bool			check(t_parser *parser, t_type type);
t_type			cur_type(t_parser *parser);
t_type			peek(t_parser *parser);

/* ================== EXECUTOR ================== */
int				executor(t_sh_info *sh_info);

/* ================= SIGNAL ================== */
void			set_parent_signal(void);
void			set_signal(pid_t pid);

#endif