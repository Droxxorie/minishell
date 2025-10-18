/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:29:25 by eraad             #+#    #+#             */
/*   Updated: 2025/10/18 19:31:06 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

//****************************************************************************/
//*                                 INCLUDES                                 */
//****************************************************************************/

# include "../libft/include/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

//****************************************************************************
//*                                  MACROS                                  *
//****************************************************************************

# define SUPPORTED_OPERATORS "<>|"
# define FD_STATE_NONE -1
# define FD_STATE_RO 0
# define FD_STATE_WO 1
# define FD_STATE_RW 2

//****************************************************************************
//*                                 GLOBALS                                  *
//****************************************************************************

extern volatile sig_atomic_t	g_waiting;

//****************************************************************************
//*                                  TYPES                                   *
//****************************************************************************

typedef enum e_quote
{
	NQ,
	NO_QUOTE = NQ,
	SQ,
	SINGLE_QUOTE = SQ,
	DQ,
	DOUBLE_QUOTE = DQ,
}	t_quote;

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
}	t_type;

//****************************************************************************
//*                                 STRUCTS                                  *
//****************************************************************************

typedef struct s_minilist
{
	char				*content;
	t_quote				quote;
	struct s_minilist	*next;
}						t_minilist;

typedef struct s_token
{
	char			*value;
	t_type			type;
	t_quote			quote;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct s_redir
{
	int				fd;
	char			*value;
	t_type			type;
	t_quote			quote;
	struct s_redir	*next;
}					t_redir;

typedef struct s_command
{
	char				*command;
	char				**argv;
	t_minilist			*args;
	t_minilist			*flags;
	t_quote				quote;
	t_redir				*redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_env
{
	char			*key;
	char			equal;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_pipes
{
	int		nb;
	int		*fds;
	pid_t	*pids;
	int		saved_stdio[2];
}			t_pipes;

typedef struct s_data
{
	char		which_quote;
	char		**env;
	char		*line;
	char		*line_base;
	int			exit_status;
	char		*path;
	t_env		*env_copy;
	t_env		*export;
	t_command	*commands;
	t_token		*tokens;
	t_quote		current_quote;
	t_pipes		*pipes;
	t_bool		saw_empty_word_as_command;
	t_bool		emit_empty_word;
	t_bool		tmp_in_redir_context;
	t_bool		tmp_have_content_before;
	int			tmp_dollar_index;
} t_data;

//****************************************************************************
//*                               PROTOTYPES                                 *
//****************************************************************************

//* LAUNCH */
void		launch_minishell(t_data *data);

//*------------------------------ UTILS -------------------------------------*

long long	ft_atoll(const char *str);
int			ft_append_char(char **str, char c);
int			append_join(char **out, char *chunk);
int			ft_strcmp(const char *s1, const char *s2);
char		*strjoin_free_left(char *left, char *right);
char		*strjoin_free_both(char *left, char *right);

void		close_fds_from(int start_fd);
int			safe_putstr_fd(char *s, int fd);
int			safe_putchar_fd(char c, int fd);

t_minilist	*minilist_last(t_minilist *list);
void		minilist_clear(t_minilist **list);
void		minilist_add_back(t_minilist **list, t_minilist *new_node);

void		swap_env_fields(t_env *a, t_env *b);
char		*find_env_value(t_data *data, const char *key);

//*------------------------------ INIT --------------------------------------*

int			init(t_data *data, char **envp);
int			init_env(t_data *data, char **envp);

t_env		*env_last_var(t_env *env);
int			add_var(t_data *data, t_env **env, char *key, char *value);
int			extract_key_value(const char *entry, char **key, char **value,
				int length);

int			add_shlvl(t_data *data);
int			update_shlvl(t_data *data, int shlvl);
int			create_shlvl(t_data *data, int shlvl);

//*----------------------------- SIGNALS ------------------------------------*

int			sig_event(void);
void		signals_handler(void);
void		setup_child_signals(void);
void		setup_heredoc_signals(void);
void		disable_readline_catch_signals(void);

//*------------------------------ ERRORS ------------------------------------*

int			syntax_error_handler(t_data *data);
void		print_syntax_error(char error, int code);
void		report_error2(const char *m1, const char *m2);
void		report_error3(const char *m1, const char *m2, const char *m3);
void		report_error(t_data *data, const char *message, int exit_code);

//*------------------------------- FREE -------------------------------------*

void		cleanup_shell_state(t_data *data);
void		reset_command_context(t_data *data);
void		cleanup_and_exit(t_data *data, int exit_status);

void		free_tokens(t_data *data);
void		free_env_list(t_env **env);
void		free_commands(t_data *data);
void		free_pipes_all(t_data *data);
void		free_char_array(char **array);

//*----------------------------- EXPANDING ----------------------------------*

int			expander(t_data *data);
t_bool		need_expansion(char *str);
t_quote		quote_state(char *line, size_t index);
char		*extract_var_name(char *str, size_t *i);
t_bool		env_var_exists(t_data *data, char *variable);
char		*get_env_value(t_data *data, char *variable);

//*------------------------------ LEXING ------------------------------------*

int			lexer(t_data *data);
int			add_operator_token(t_data *data, char operator,
				int *command_boundary);

int			validate_pipe_syntax(t_data *data);
t_token		*classify_input_redirections(t_data *data, t_token *tokens);
t_token		*classify_output_redirections(t_data *data, t_token *tokens);
int			lexer_postprocess(t_data *data, t_quote quote_state,
				char **token_buffer, int command_boundary);

void		add_back_token(t_data *data, t_token *new_token);
t_token		*create_token(char *start, char *end, t_type type, t_quote quote);
t_token		*add_classified_token(t_data *data, char *token_buffer,
				int *command_boundary);

int			handle_single_quoted(t_data *data, t_quote *qs, char **tokbuf);
int			handle_double_quoted(t_data *data, t_quote *qs, char **tokbuf);
int			handle_no_quote(t_data *data, t_quote *qs, char **tokbuf,
				int *command_boundary);
int			process_whitespace(t_data *data, char **token_buffer,
				int *command_boundary);
int			process_operator(t_data *data, char **token_buffer,
				int *command_boundary);

t_token		*normalize_exit_echo_args(t_token *tokens);
t_token		*normalize_redirection_args(t_token *tokens);
t_token		*classify_heredoc_delimiters(t_data *data, t_token *tokens);
t_token		*classify_input_redirections(t_data *data, t_token *tokens);

//*------------------------------ PARSING -----------------------------------*

int			parser(t_data *data);

t_command	*append_command_node(t_data *data);

int			add_command_arg(t_command *current, t_token *tok);
int			add_command_flag(t_command *current, t_token *tok);
int			set_command_name(t_command *current, t_token *tok);

size_t		count_command_elements(t_command *command);
int			append_args_to_argv(t_command *current, size_t *i);
int			append_flags_to_argv(t_command *current, size_t *i);
int			append_command_to_argv(t_command *current, size_t *i);

int			setup_heredoc_node(t_data *data, t_redir *redir);
char		*expand_heredoc_line(t_data *data, const char *s);
int			push_redir(t_data *data, t_command *current, t_token *tok);

int			build_command_argv(t_data *data);
int			handle_redirection_token(t_data *data, t_command **current,
				t_token *token);
int			dispatch_command_tokens(t_data *data, t_command **current,
				t_token *token, int *command_boundary);

//*-------------------------- EXEC/ REDIRECTIONS ---------------------------*

int			safe_close_fd(int *fd);
int			save_stdio(int *saved_stdio);
void		close_pipe_fds(int *fds, int count);
int			restore_saved_stdio(int *saved_stdio);

int			dup_apply_fd(t_data *data, int fd, int target);
int			apply_pipe_in(t_data *data, int *fds, int index);
int			apply_pipe_out(t_data *data, int *fds, int index);

void		redir_push_back(t_redir **list, t_redir *node);
int			check_redir_operator(t_data *data, t_redir *redir);
t_redir		*new_redir(t_type type, t_quote quote, const char *value);
int			child_setup_io(t_data *data, t_command *node, int *fds,
				int index);
int			apply_redirections_in_parent(t_data *data, t_command *node,
				int saved[2]);
int			open_redirs_for_command(t_data *data, t_command *node,
				int *in_fd, int *out_fd);

int			prepare_pipes(t_data *data, int number_of_commands);
int			wait_and_cleanup_pipeline(t_data *data, pid_t *pids, int count,
				int *fds);

int			executor(t_data *data);
int			launch_pipeline(t_data *data);

//*------------------------------ BUILTINS ----------------------------------*

int			is_builtin_command(t_command *node);
int			dispatch_builtin(t_data *data, t_command *node);
int			handle_builtin_command(t_data *data, t_command *node);
pid_t		launch_builtin_child(t_data *data, t_command *node, int *fds,
				int index);

//* echo */
int			execute_builtin_echo(char **argv);

//* cd */
int			execute_builtin_cd(t_data *data, t_command *node);
//* pwd */
int			execute_builtin_pwd(t_data *data, t_command *node);

//* export */
char		*extract_env_key(char *arg);
int			error_arg_handler(char *arg);
t_bool		key_is_valid(const char *key);
long long	parse_shlvl(const char *value);
void		init_export_list(t_data *data);
int			error_key_handler(char *key, char *arg);
int			env_index_of_key(t_env *env, char *key);
int			check_event_expansion_arg(const char *arg);
void		export_add_key(t_data *data, const char *key);
int			execute_builtin_export(t_data *data, char **argv, int fd);
int			process_export_args(t_data *data, char *arg, t_bool do_mutate);
void		env_update_value(t_data *data, const char *arg, int key_index);
void		handle_export_arg(t_data *data, char *key, char *arg, int key_idx);
void		export_update_value(t_data *data, const char *key, const char *arg);
t_env		*sort_export_list(t_env *head, int (*cmp)(const char *,
					const char *));

//* unset */
int			execute_builtin_unset(t_data *data, char **argv);

//* env */
int			execute_builtin_env(t_data *data, char **argv, int fd_out);

//* exit */
int			execute_builtin_exit(t_data *data, char **argv);
void		exit_minishell(t_data *data, int exit_status);

//*------------------------------ EXTERNALS ---------------------------------*

int			compute_n_cmds(t_data *data);
char		**env_list_to_array(const t_env *env);
char		**build_exec_search_paths(t_data *data, const t_env *env);

void		parent_close_after_fork(int *fds, int index, int n_cmds);
int			handle_external_command(t_data *data, int *fds, int index,
				pid_t *pid);

t_bool		command_path_is_valid(t_data *data, t_command *node,
				char **command_path);

#endif
