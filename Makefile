NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = builtins_export.c builtins_export_utils.c builtins_cd.c builtins_general.c builtins_unset_env_exit.c builtins_unset_utils.c \
       execute_cmd.c execute_find_path.c execute_pipeline.c execute_pipeline_utils.c execute_single.c execute_redirection.c \
       expansion.c expansion_utils.c expansion_after_lexer.c\
       lexer.c lexer_words.c lexer_special.c lexer_utils.c remove_token.c \
       parser.c parser_utils.c parser_utils_two.c init_ellibash.c \
       free.c cleaner.c heredo.c loop.c main.c signal.c utils.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft/
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(LIBFT_DIR) -lft -lreadline

$(LIBFT_A):
		@make -C $(LIBFT_DIR)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -f $(OBJS)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)
	

re: fclean all

.PHONY: all clean fclean re