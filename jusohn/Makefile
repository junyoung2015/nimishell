CC = cc
CFLAGS = -Wall -Wextra -fsanitize=address -g2 #-Werror # 
READLINE_OPT = -L./lib/ -lreadline
NAME = minishell
# -------------- DIRS  -------------- #
INCLUDES = ./includes/
INCLUDE_READLINE = ./include/
EXECUTOR_DIR = ./executor/
LEXER_DIR = ./lexer/
MEMORY_DIR = ./memory/
PARSER_DIR = ./parser/
STR_DIR = ./str/
TOKENIZER_DIR = ./tokenizer/

# -------------- SRCS  -------------- #
SRCS = ./minishell.c						\
		$(EXECUTOR_DIR)executor.c			\
		$(LEXER_DIR)lexer.c					\
		$(MEMORY_DIR)mem_utils.c			\
		$(PARSER_DIR)parser.c				\
		$(STR_DIR)str_utils.c				\
		$(STR_DIR)str_utils_create.c		\
		$(TOKENIZER_DIR)tokenizer.c

# ----------- BONUS SRCS  ----------- #
# BO_SRCS = ./minishell_bonus.c

HEADER = minishell.h
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

mini: $(NAME)

bonus: $(BO_SRCS) $(INCLUDES)$(BO_HEADER)
	@$(MAKE) BONUS=1 all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(READLINE_OPT) $(OBJS) -o $@

# $(BONUS): $(BO_OBJS)
# 	$(CC) $(CFLAGS) $(BO_OBJS) -o $@

%.o: %.c $(INCLUDES)$(HEADER)
	$(CC) $(CFLAGS) -I$(INCLUDES) -I$(INCLUDE_READLINE) -c $< -o $@

# re:
# 	make fclean
# 	make all

# clean:
# 	rm -rf ${OBJS} ${BO_OBJS}

# fclean:
# 	make clean
# 	rm -rf ${NAME} ${BONUS}

norm:
	norm ${SRCS} ${BO_SRCS} ${INCLUDES}

# all: readline

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
