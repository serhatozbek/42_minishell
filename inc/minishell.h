/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:53:15 by ocubukcu          #+#    #+#             */
/*   Updated: 2025/08/21 04:00:33 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdbool.h>
# include <sys/types.h>
# include <stddef.h>

# define EXIT_SHELL 256

extern volatile sig_atomic_t	g_sigint_recived;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_LESS,
	T_GREAT,
	T_DGREAT,
	T_DLESS,
}								t_token_type;

typedef struct s_token
{
	char						*value;
	t_token_type				type;
	char						quote;
	bool						has_space_after;
	struct s_token				*next;
}								t_token;

typedef struct s_shell
{
	char						**env;
	char						**lines;
	char						*noenvpath;
	int							exit_status;
}								t_shell;

typedef struct s_redir
{
	t_token_type				type;
	char						*filename;
	bool						expand_heredoc;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						**args;
	t_redir						*redirs;
	int							type;
	int							in_fd;
	int							out_fd;
	struct s_cmd				*next;
}								t_cmd;

typedef struct s_exec_data
{
	int							pipefd[2];
	int							in_fd;
	struct s_shell				*shell;
	struct s_cmd				*cmds;
}								t_exec_data;

int								is_builtin(char *cmd);
int								exec_builtin(t_cmd *cmd, t_shell *shell,
									int child_flag);
int								just_space_check(char *str);
int								do_single_builtin(t_cmd *cmd, t_shell *shell);

int								builtin_export(char **args, t_shell *shell);
int								print_sorted_env(t_shell *shell);
int								is_valid_identifier(char *str);

int								builtin_unset(char **args, t_shell *shell);
int								builtin_env(t_shell *shell);
int								builtin_exit(char **args, t_shell *shell,
									int child_flag);
int								builtin_cd(char **args, t_shell *shell);

int								set_env(t_shell *shell, char *assignment);
int								unset_env(t_shell *shell, char *name);
int								update_existing_env(t_shell *shell,
									char *assignment, int len);
int								add_new_env(t_shell *shell, char *assignment);

int								execute_pipeline(t_cmd *cmd_list,
									t_shell *shell);
int								wait_for_children(pid_t last_pid);
void							status_check(int last_status);

int								open_redirs(t_cmd *cmd, t_exec_data *data);
void							exec_cmd(t_cmd *cmd, t_exec_data *data);
char							*find_in_path(char *cmd, t_exec_data *data);

bool							is_assignment_list(char **args);
int								execute_single_command(t_cmd *cmd,
									t_shell *shell);

int								expand_tokens(t_token *tokens, t_shell *shell);
char							*expand_line(const char *line, t_shell *shell);
void							expand_lexer(t_token **tokens, t_shell *shell);
void							remove_empty_tokens(t_token **head);

char							*expand_token(char *token, t_shell *shell,
									char quote);
char							*expand_variables(char *str, t_shell *shell,
									char quote);
char							*expand_variablesi(char *str, t_shell *shell);
char							*get_env_value(char *key, char **env);
char							*expand_single_var(char *str, int *pos,
									t_shell *shell);

void							free_tokens(t_token *tokens);
void							free_cmds(t_cmd *cmds);
void							shell_final_cleanup(t_shell *shell);
void							ft_free_split(char **split);
void							ft_cleaner(t_exec_data *data);
void							fd_clean(t_cmd *tmp);

int								heredo_c_syntax(t_token *token, t_shell *shell);
int								setup_heredoc_pipe(t_redir *redir,
									t_exec_data *data);
void							heredoc_clean_exit(t_exec_data *data,
									int write_fd);

t_token							*lexer(char *input, t_shell *shell);
t_token							*lex_word(char **input);
bool							is_special_char(char c);
bool							is_whitespace(char c);
void							join_adjacent_words(t_token *tokens);
t_token							*lex_special(char **input);
t_token							*new_token(t_token_type type, char *value,
									char quote);

t_cmd							*parser(t_token *tokens, t_shell *shell);
t_cmd							*new_cmd(void);
int								add_args(t_cmd *cmd, char *arg);
int								parse_redir(t_token **curr, t_cmd *cmd);
void							error_exit(int status, t_shell *shell);
int								syntax_control(t_token *curr, t_token *tmp);

void							set_signals(void);
void							heredoc_sigint_handler(int sig);
void							set_execution_signals(void);
void							set_default_signals(void);
char							**dup_env(char **env);

char							*strjoin_three(const char *s1, const char *s2,
									const char *s3);
char							*ft_strjoin_free(char *s1, char *s2);
char							**dup_env(char **env);
void							data_packet_create(t_exec_data *data,
									t_cmd *cmd, t_shell *shell);

int								init_ellibash(t_shell *shell, char **env);
int								bash_loop(t_shell *shell);

#endif
