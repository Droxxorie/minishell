/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:29:25 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 03:59:20 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

//* ----------------- Libraries ----------------------- */
# include "../libft/include/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

//* ----------------- Macros ----------------------- */
# define SUPPORTED_OPERATORS "<>|"
# define FD_STATE_NONE -1
# define FD_STATE_RO 0
# define FD_STATE_WO 1
# define FD_STATE_RW 2

//* ----------------- Globals ----------------------- */
# ifndef GLOBALS_H
#  define GLOBALS_H
extern volatile sig_atomic_t	g_waiting;
# endif

//* ----------------- Enums ----------------------- */

typedef enum e_quote
{
	NQ,
	NO_QUOTE = NQ,
	SQ,
	SINGLE_QUOTE = SQ,
	DQ,
	DOUBLE_QUOTE = DQ,
}								t_quote;

typedef enum e_type
{
	ARG,
	CMD,
	FLAG,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	LIMITER,
	FILE_NAME,
}								t_type;

//* ----------------- Structs ----------------------- */

typedef struct s_minilist
{
	char						*content;
	t_quote						quote;
	struct s_minilist			*next;
}								t_minilist;

typedef struct s_token
{
	char						*value;
	t_type						type;
	t_quote						quote;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

typedef struct s_redir
{
	int							fd;
	char						*value;
	t_type						type;
	t_quote						quote;
	struct s_redir				*next;
}								t_redir;

typedef struct s_command
{
	char						*command;
	char						**argv;
	t_minilist					*args;
	t_minilist					*flags;
	t_quote						quote;
	t_redir						*redirs;
	struct s_command			*next;
}								t_command;

typedef struct s_env
{
	char						*key;
	char						equal;
	char						*value;
	struct s_env				*next;
}								t_env;

typedef struct s_pipes
{
	int							nb;
	int							*fds;
	pid_t						*pids;
	int							saved_stdio[2];
}								t_pipes;

typedef struct s_data
{
	char						which_quote;
	char						**env;
	char						*line;
	int							exit_status;
	char						*path;
	t_env						*env_copy;
	t_env						*export;
	t_command					*commands;
	t_token						*tokens;
	t_quote						current_quote;
	t_pipes						*pipes;
}								t_data;

//* ----------------- Functions ----------------------- */

void							launch_minishell(t_data *data);

//* UTILS */
long long						ft_atoll(const char *str);
void							close_fds_from(int start_fd);
int								safe_putstr_fd(char *s, int fd);
int								safe_putchar_fd(char c, int fd);
t_minilist						*minilist_last(t_minilist *list);
int								ft_append_char(char **str, char c);
void							swap_env_fields(t_env *a, t_env *b);
int								ft_strcmp(const char *s1, const char *s2);
void							minilist_add_back(t_minilist **list,
									t_minilist *new_node);
char							*find_env_value(t_data *data, const char *key);

//* INIT */
t_env							*env_last_var(t_env *env);
int								init(t_data *data, char **envp);
int								init_env(t_data *data, char **envp);
int								update_shlvl(t_data *data, int shlvl);
int								create_shlvl(t_data *data, int shlvl);
int								add_shlvl(t_data *data);
int								add_var(t_data *data, t_env **env, char *key,
									char *value);
int								extract_key_value(const char *entry, char **key,
									char **value, int length);

//* SIGNALS */
int								sig_event(void);
void							signals_handler(void);
void							setup_child_signals(void);
void							setup_heredoc_signals(void);

//* ERRORS */
int								syntax_error_handler(t_data *data);
void							print_syntax_error(char error, int code);
void							report_error(t_data *data, const char *message,
									int exit_code);
void							report_error2(const char *message_1,
									const char *message_2);
void							report_error3(const char *message_1,
									const char *message_2,
									const char *message_3);

//* FREE */
void							cleanup_shell_state(t_data *data);
void							free_tokens(t_data *data);
void							free_env_list(t_env **env);
void							free_commands(t_data *data);
void							free_pipes_all(t_data *data);
void							free_char_array(char **array);
void							minilist_clear(t_minilist **list);
void							reset_command_context(t_data *data);

//* EXPANDING */
int								expander(t_data *data);
t_bool							need_expansion(char *str);
t_quote							quote_state(char *line, size_t index);
char							*extract_var_name(char *str, size_t *i);
t_bool							env_var_exists(t_data *data, char *variable);
char							*get_env_value(t_data *data, char *variable);

//* LEXING */
int								add_operator_token(t_data *data, char operator,
									int * command_boundary);
int								lexer(t_data *data);
int								validate_pipe_syntax(t_data *data);
t_token							*normalize_exit_echo_args(t_token *tokens);
t_token							*normalize_redirection_args(t_token *tokens);
t_token							*classify_input_redirections(t_token *tokens);
t_token							*classify_heredoc_delimiters(t_token *tokens);
void							add_back_token(t_data *data,
									t_token *new_token);
t_token							*create_token(char *start, char *end,
									t_type type, t_quote quote);
int								handle_single_quoted(t_data *data,
									t_quote *quote_state, char **token_buffer);
int								handle_double_quoted(t_data *data,
									t_quote *quote_state, char **token_buffer);
t_token							*add_classified_token(t_data *data,
									char *token_buffer, int *command_boundary);
int								handle_no_quote(t_data *data,
									t_quote *quote_state, char **token_buffer,
									int *command_boundary);

//* PARSING */
int								push_redir(t_data *data, t_command *current,
									t_token *token);
int								setup_heredoc_node(t_data *data,
									t_redir *redir);
int								parser(t_data *data);
t_command						*append_command_node(t_data *data);
size_t							count_command_elements(t_command *command);
int								append_args_to_argv(t_command *current,
									size_t *i);
int								append_flags_to_argv(t_command *current,
									size_t *i);
int								append_command_to_argv(t_command *current,
									size_t *i);
int								add_command_arg(t_command *current_command,
									t_token *current_token);
int								set_command_name(t_command *current_command,
									t_token *current_token);
int								add_command_flag(t_command *current_command,
									t_token *current_token);

//* EXECUTING / REDIRECTIONS */
int								dup_apply_fd(t_data *data, int fd, int target);
int								apply_pipe_in(t_data *data, int *fds,
									int index);
int								apply_pipe_out(t_data *data, int *fds,
									int index);
void							redir_push_back(t_redir **list, t_redir *node);
t_redir							*new_redir(t_type type, t_quote quote,
									const char *value);
int								open_redirs_for_command(t_data *data,
									t_command *node, int *in_fd, int *out_fd);
int								child_setup_io(t_data *data, t_command *node,
									int *fds, int index);
// int *fds, int index, int n);
int								apply_redirections_in_parent(t_data *data,
									t_command *node, int saved[2]);
int								wait_and_cleanup_pipeline(t_data *data,
									pid_t *pids, int count, int *fds);
int								executor(t_data *data);
int								launch_pipeline(t_data *data);
int								prepare_pipes(t_data *data,
									int number_of_commands);

//* BUILTINS */
int								dispatch_builtin(t_data *data, t_command *node);
int								is_builtin_command(t_command *node);
int								handle_builtin_command(t_data *data,
									t_command *node);
pid_t							launch_builtin_child(t_data *data,
									t_command *node, int *fds, int index);
int								safe_close_fd(int *fd);
int								save_stdio(int *saved_stdio);
void							close_pipe_fds(int *fds, int count);
int								restore_saved_stdio(int *saved_stdio);

/* ECHO */
int								execute_builtin_echo(char **argv);

/* CD */
int								execute_builtin_cd(t_data *data,
									t_command *node);

/* PWD */
int								execute_builtin_pwd(t_data *data,
									t_command *node);

/* EXPORT */
void							export_add_key(t_data *data, const char *key);
int								check_event_expansion_arg(const char *arg);
int								error_key_handler(char *key, char *arg);
int								error_arg_handler(char *arg);
t_bool							key_is_valid(const char *key);
long long						parse_shlvl(const char *value);
int								process_export_args(t_data *data, char *arg,
									t_bool do_mutate);
char							*extract_env_key(char *arg);
void							init_export_list(t_data *data);
int								env_index_of_key(t_env *env, char *key);
int								execute_builtin_export(t_data *data,
									char **argv, int fd);
void							env_update_value(t_data *data, const char *arg,
									int key_index);
void							env_add_from_arg(t_data *data, t_env *env,
									char *key, char *arg);
void							handle_export_arg(t_data *data, char *key,
									char *arg, int key_index);
t_env							*sort_export_list(t_env *head,
									int (*cmp)(const char *, const char *));

/* UNSET */
int								execute_builtin_unset(t_data *data,
									char **argv);

/* ENV */
int								execute_builtin_env(t_data *data, char **argv,
									int fd_out);

/* EXIT */
void							exit_minishell(t_data *data, int exit_status);
int								execute_builtin_exit(t_data *data, char **argv);

//* EXTERNALS */
// int	path_is_dir(const char *path);
// int has_slash(const char *str);
// char	*path_join(const char *dir, const char *cmd);
void							parent_close_after_fork(int *fds, int index,
									int n_cmds);
int								compute_n_cmds(t_data *data);
char							**env_list_to_array(const t_env *env);
char							**build_exec_search_paths(t_data *data,
									const t_env *env);
void							handle_external_command(t_data *data, int *fds,
									int index, pid_t *pid);
t_bool							command_path_is_valid(t_data *data,
									t_command *node, char **command_path);
int								dispatch_command_tokens(t_data *data,
									t_command **current, t_token *token,
									int *command_boundary);
int								handle_redirection_token(t_data *data,
									t_command **current, t_token *token);
int								build_command_argv(t_data *data);

#endif
