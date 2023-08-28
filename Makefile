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
		$(EXPANSION)file_expansion_match.c		\
		$(EXPANSION)file_expansion_split.c		\
		$(EXPANSION)file_expansion_pattern.c	\
		$(EXPANSION)ft_qsort.c					\
		$(EXPANSION)env_substitution.c			\
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
		$(BUILTIN_DIR)cd.c

CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g2
# ----------- BONUS SRCS  ----------- #
# BO_SRCS = ./minishell_bonus.c
HEADER = \
	$(INCLUDES)minishell.h	\
	$(INCLUDES)executor.h	\
	$(INCLUDES)builtin.h
# BO_HEADER = minishell_bonus.h

# -------------- OBJS  -------------- #
OBJS := $(SRCS:.c=.o)
BO_OBJS := $(BO_SRCS:.c=.o)
# BO_INCLUDES = ./bonus/includes/

# -------------- BONUS -------------- #
ifdef BONUS
    SRCS := $(BO_SRCS)
    OBJS := $(BO_OBJS)
    HEADER := $(BO_HEADER)
    # INCLUDES := $(BO_INCLUDES)
endif

# ------------- RULES  -------------- #

# $(MAKE) fclean
all:
	$(MAKE) readline
	$(MAKE) mini

readline:
	tar -xzf ./readline-8.2.tar.gz
	cd readline-8.2 && sh ./configure --prefix="$$(cd .. && pwd)" && make && make install
	touch $@

### make minishell, without re-make of readline ###
mini: $(NAME)

bonus: $(BO_SRCS) $(INCLUDES)$(BO_HEADER)
	@$(MAKE) BONUS=1 all

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
	rm -rf ${OBJS} ${BO_OBJS}

fclean:
	$(MAKE) clean
	rm -rf bin include/readline lib share
	rm -rf ${NAME}

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re mini readline
