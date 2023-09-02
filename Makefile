CC = cc

UNAME := $(uname)

ifeq ($(UNAME), Darwin)
    READLINE_OPT = -L./lib/ -lreadline
else
    READLINE_OPT = -L./lib/ -lreadline -lncurses
endif

NAME = minishell

# -------------- DIRS  -------------- #
INCLUDES = ./includes/
INCLUDE_READLINE = ./include/
INIT_DIR = ./init/
EXECUTOR_DIR = ./executor/
BUILTIN_DIR	= ./builtin/
LEXER_DIR = ./lexer/
MEMORY_DIR = ./memory/
PARSER_DIR = ./parser/
STR_DIR = ./str/
EXPANSION = ./expansion/

# -------------- SRCS  -------------- #
SRCS = ./minishell.c							\
		$(INIT_DIR)logo.c						\
		$(INIT_DIR)prompt.c						\
		$(INIT_DIR)signal.c						\
		$(INIT_DIR)init_env.c					\
		$(LEXER_DIR)tokenizer.c					\
		$(LEXER_DIR)tokenizer_err_handling.c	\
		$(LEXER_DIR)tokenizer_utils.c			\
		$(LEXER_DIR)tokenizer_realloc.c			\
		$(LEXER_DIR)tokenizer_split.c			\
		$(LEXER_DIR)tokenize_states.c			\
		$(LEXER_DIR)cmp_func_quote.c			\
		$(LEXER_DIR)cmp_func_meta.c				\
		$(MEMORY_DIR)mem_utils.c				\
		$(PARSER_DIR)ast_utils.c				\
		$(PARSER_DIR)parser.c					\
		$(PARSER_DIR)parser_utils.c				\
		$(PARSER_DIR)parser_state_utils.c		\
		$(PARSER_DIR)parse_err.c				\
		$(PARSER_DIR)parse_list.c				\
		$(PARSER_DIR)parse_pipeline.c			\
		$(PARSER_DIR)parse_redir_list.c			\
		$(PARSER_DIR)parse_simple_cmd.c			\
		$(PARSER_DIR)parse_subshell.c			\
		$(PARSER_DIR)parse_word_list.c			\
		$(STR_DIR)str_itoa.c					\
		$(STR_DIR)str_utils.c					\
		$(STR_DIR)str_utils2.c					\
		$(STR_DIR)str_split.c					\
		$(STR_DIR)str_utils_create.c			\
		$(EXPANSION)2darray_utils.c				\
		$(EXPANSION)quote_removal.c				\
		$(EXPANSION)file_expansion.c			\
		$(EXPANSION)file_expansion_utils.c		\
		$(EXPANSION)file_expansion_info.c		\
		$(EXPANSION)file_expansion_match.c		\
		$(EXPANSION)file_expansion_split.c		\
		$(EXPANSION)file_expansion_pattern.c	\
		$(EXPANSION)ft_qsort.c					\
		$(EXPANSION)env_substitution.c			\
		$(EXPANSION)env_substitution_cmp_fn.c	\
		$(EXPANSION)env_substitution_env_fn.c	\
		$(EXPANSION)env_substitution_sub_fn.c	\
		$(EXPANSION)env_substitution_env_utils.c\
		$(EXECUTOR_DIR)util.c					\
		$(EXECUTOR_DIR)path.c					\
		$(EXECUTOR_DIR)command.c				\
		$(EXECUTOR_DIR)error.c					\
		$(EXECUTOR_DIR)executor.c				\
		$(EXECUTOR_DIR)ast_search.c				\
		$(EXECUTOR_DIR)pipe.c					\
		$(EXECUTOR_DIR)heredoc.c				\
		$(EXECUTOR_DIR)redirection.c			\
		$(EXECUTOR_DIR)builtin.c				\
		$(EXECUTOR_DIR)subshell.c				\
		$(BUILTIN_DIR)export.c					\
		$(BUILTIN_DIR)unset.c					\
		$(BUILTIN_DIR)env.c						\
		$(BUILTIN_DIR)pwd.c						\
		$(BUILTIN_DIR)echo.c					\
		$(BUILTIN_DIR)arg.c						\
		$(BUILTIN_DIR)cd.c						\
		$(BUILTIN_DIR)cd_util.c

# ------------ BO_SRCS  ------------- #
BO_SRCS = ./minishell_bonus.c							\
		$(INIT_DIR)logo_bonus.c							\
		$(INIT_DIR)prompt_bonus.c						\
		$(INIT_DIR)signal_bonus.c						\
		$(INIT_DIR)init_env_bonus.c						\
		$(LEXER_DIR)tokenizer_bonus.c					\
		$(LEXER_DIR)tokenizer_err_handling_bonus.c		\
		$(LEXER_DIR)tokenizer_utils_bonus.c				\
		$(LEXER_DIR)tokenizer_realloc_bonus.c			\
		$(LEXER_DIR)tokenizer_split_bonus.c				\
		$(LEXER_DIR)tokenize_states_bonus.c				\
		$(LEXER_DIR)cmp_func_quote_bonus.c				\
		$(LEXER_DIR)cmp_func_meta_bonus.c				\
		$(MEMORY_DIR)mem_utils_bonus.c					\
		$(PARSER_DIR)ast_utils_bonus.c					\
		$(PARSER_DIR)parser_bonus.c						\
		$(PARSER_DIR)parser_utils_bonus.c				\
		$(PARSER_DIR)parser_state_utils_bonus.c			\
		$(PARSER_DIR)parse_err_bonus.c					\
		$(PARSER_DIR)parse_list_bonus.c					\
		$(PARSER_DIR)parse_pipeline_bonus.c				\
		$(PARSER_DIR)parse_redir_list_bonus.c			\
		$(PARSER_DIR)parse_simple_cmd_bonus.c			\
		$(PARSER_DIR)parse_subshell_bonus.c				\
		$(PARSER_DIR)parse_word_list_bonus.c			\
		$(STR_DIR)str_itoa_bonus.c						\
		$(STR_DIR)str_utils_bonus.c						\
		$(STR_DIR)str_utils2_bonus.c					\
		$(STR_DIR)str_split_bonus.c						\
		$(STR_DIR)str_utils_create_bonus.c				\
		$(EXPANSION)2darray_utils_bonus.c				\
		$(EXPANSION)quote_removal_bonus.c				\
		$(EXPANSION)file_expansion_bonus.c				\
		$(EXPANSION)file_expansion_utils_bonus.c		\
		$(EXPANSION)file_expansion_info_bonus.c			\
		$(EXPANSION)file_expansion_match_bonus.c		\
		$(EXPANSION)file_expansion_split_bonus.c		\
		$(EXPANSION)file_expansion_pattern_bonus.c		\
		$(EXPANSION)ft_qsort_bonus.c					\
		$(EXPANSION)env_substitution_bonus.c			\
		$(EXPANSION)env_substitution_cmp_fn_bonus.c		\
		$(EXPANSION)env_substitution_env_fn_bonus.c		\
		$(EXPANSION)env_substitution_sub_fn_bonus.c		\
		$(EXPANSION)env_substitution_env_utils_bonus.c	\
		$(EXECUTOR_DIR)util_bonus.c						\
		$(EXECUTOR_DIR)path_bonus.c						\
		$(EXECUTOR_DIR)command_bonus.c					\
		$(EXECUTOR_DIR)error_bonus.c					\
		$(EXECUTOR_DIR)executor_bonus.c					\
		$(EXECUTOR_DIR)ast_search_bonus.c				\
		$(EXECUTOR_DIR)pipe_bonus.c						\
		$(EXECUTOR_DIR)heredoc_bonus.c					\
		$(EXECUTOR_DIR)redirection_bonus.c				\
		$(EXECUTOR_DIR)builtin_bonus.c					\
		$(EXECUTOR_DIR)subshell_bonus.c					\
		$(BUILTIN_DIR)export_bonus.c					\
		$(BUILTIN_DIR)unset_bonus.c						\
		$(BUILTIN_DIR)env_bonus.c						\
		$(BUILTIN_DIR)pwd_bonus.c						\
		$(BUILTIN_DIR)echo_bonus.c						\
		$(BUILTIN_DIR)arg_bonus.c						\
		$(BUILTIN_DIR)cd_bonus.c						\
		$(BUILTIN_DIR)cd_util_bonus.c

CFLAGS = -Wall -Wextra -Werror -g2 -fsanitize=address

HEADER = \
	$(INCLUDES)minishell.h	\
	$(INCLUDES)executor.h	\
	$(INCLUDES)builtin.h
BO_HEADER = \
	$(INCLUDES)minishell_bonus.h	\
	$(INCLUDES)executor_bonus.h		\
	$(INCLUDES)builtin_bonus.h

# -------------- OBJS  -------------- #
OBJS := $(SRCS:.c=.o)
BO_OBJS := $(BO_SRCS:.c=.o)

# -------------- BONUS -------------- #
ifdef BONUS
    SRCS := $(BO_SRCS)
    OBJS := $(BO_OBJS)
    HEADER := $(BO_HEADER)
endif

# ------------- RULES  -------------- #

# $(MAKE) fclean
all:
	$(MAKE) readline
	$(MAKE) mini
	touch $@

readline:
	tar -xzf ./readline-8.2.tar.gz
	cd readline-8.2 && sh ./configure --prefix="$$(cd .. && pwd)" && make && make install && touch $@
	touch $@

### make minishell, without re-make of readline ###
mini: $(NAME)

bonus: $(SRCS) $(HEADER)
	@$(MAKE) BONUS=1 all
	touch $@


$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -I$(INCLUDES) -I$(INCLUDE_READLINE) $(OBJS) -o $@ $(READLINE_OPT) 

# $(BONUS): $(BO_OBJS)
# 	$(CC) $(CFLAGS) $(BO_OBJS) -o $@

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDES) -I$(INCLUDE_READLINE) -c $< -o $@
# $(CC) $(CFLAGS) -I$(INCLUDES) -I$(INCLUDE_READLINE) -c $< -o $@

norm:
	norm ${SRCS} ${BO_SRCS} ${INCLUDES}

clean:
	rm -rf tmp
	rm -rf readline-8.2
	rm -f readline
	rm -f bonus all readline
	rm -rf ${OBJS} ${BO_OBJS}

fclean:
	$(MAKE) clean
	rm -rf bin include lib share
	rm -rf ${NAME}

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re bonus mini readline
