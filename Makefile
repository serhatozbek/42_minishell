NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -rf
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
INC_DIR = inc
LIBFT_DIR = inc/libft
LIBFT = $(LIBFT_DIR)/libft.a

OBJ_DIR = obj
CFLAGS += -I$(INC_DIR) -I$(LIBFT_DIR)
LDFLAGS = -L$(LIBFT_DIR) -lft -lreadline
SRCS =	src/builtins/builtins_cd.c \
		src/builtins/builtins_export.c \
		src/builtins/builtins_export_utils.c \
		src/builtins/builtins_general.c \
		src/builtins/builtins_unset_env_exit.c \
		src/builtins/builtins_unset_utils.c \
		src/execute_and_pipelines/execute_cmd.c \
		src/execute_and_pipelines/execute_find_path.c \
		src/execute_and_pipelines/execute_pipeline.c \
		src/execute_and_pipelines/execute_pipeline_utils.c \
		src/execute_and_pipelines/execute_redirection.c \
		src/execute_and_pipelines/execute_single.c \
		src/execute_and_pipelines/heredo.c \
		src/expansion/expansion.c \
		src/expansion/expansion_utils.c \
		src/lexer/expansion_after_lexer.c \
		src/lexer/lexer.c \
		src/lexer/lexer_special.c \
		src/lexer/lexer_utils.c \
		src/lexer/lexer_words.c \
		src/lexer/remove_token.c \
		src/main_loop/init_ellibash.c \
		src/main_loop/loop.c \
		src/main_loop/main.c \
		src/main_loop/signal.c \
		src/parser/parser.c \
		src/parser/parser_utils.c \
		src/parser/parser_utils_two.c \
		src/utils/cleaner.c \
		src/utils/free.c \
		src/utils/utils.c

OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(YELLOW)Linking libraries and creating $(NAME)...$(DEF_COLOR)"
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(GREEN)✓ Success! \033[1;37mElli\033[1;31mbash\033[0m is ready.$(DEF_COLOR)"

$(LIBFT):
	@echo "$(BLUE)Compiling Libft...$(DEF_COLOR)"
	@$(MAKE) -sC $(LIBFT_DIR)
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(GREEN)✓ Compiled:$(DEF_COLOR) $<"

clean:
	@echo "$(YELLOW)Cleaning object files...$(DEF_COLOR)"
	@$(MAKE) -sC $(LIBFT_DIR) clean
	@$(RM) -r $(OBJ_DIR)
	@echo "$(GREEN)✓ Object files removed.$(DEF_COLOR)"

fclean:
	@echo "$(YELLOW)Full cleaning project...$(DEF_COLOR)"
	@$(MAKE) -sC $(LIBFT_DIR) fclean
	@$(RM) -r $(OBJ_DIR)
	@$(RM) $(NAME)
	@echo "$(GREEN)✓ Project fully cleaned.$(DEF_COLOR)"

re: fclean all

.PHONY: all clean fclean re

.SILENT:
.ERROR:
	@echo "$(RED)✗ Error: \033[1;37melli\033[1;31mbash\033[0m Compilation failed.$(DEF_COLOR)"