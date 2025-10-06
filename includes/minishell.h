/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 15:24:16 by eraad             #+#    #+#             */
/*   Updated: 2025/10/06 03:14:20 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L

//* ----------------- Libraries ----------------------- *
# include "../libft/include/libft.h"
# include <errno.h>             //* for errno
# include <fcntl.h>             //* for open
# include <limits.h>            //* for PATH_MAX
# include <readline/history.h>  //* for add_history
# include <readline/readline.h> //* for readline
# include <signal.h>            //* for signal, sigaction
# include <stdbool.h>           //* for bool, true, false
# include <stdio.h>             //* for printf, perror
# include <stdlib.h>            //* for malloc, free, exit, getenv, perror
# include <string.h>            //* for strerror
# include <sys/stat.h>          //* for struct stat, stat, fstat
# include <sys/wait.h>          //* for wait, waitpid
# include <unistd.h>            //* for fork, execve, pipe, dup2, chdir, getcwd,
//* close, write, read, access

//* ----------------- Macros ----------------------- *
# define SUPPORTED_OPERATORS "<>|"
# define FD_STATE_NONE -1
# define FD_STATE_RO 0
# define FD_STATE_WO 1
# define FD_STATE_RW 2

//* ----------------- Globals ----------------------- *

/*
 * - g_waiting : indicateur d’attente de signal (interruption, heredoc, etc.)
 */
# ifndef GLOBALS_H
#  define GLOBALS_H

// extern int				g_waiting;
extern volatile sig_atomic_t	g_waiting;

# endif

//* ----------------- Enums ----------------------- *

/*
 * Énumération représentant les différents types de quoting
 * rencontrés dans une ligne de commande. Utilisée pour déterminer
 * le comportement d’expansion et d’interprétation des caractères spéciaux.
 *
 * - NQ / NO_QUOTE       : aucun quoting, expansion autorisée
 * - SQ / SINGLE_QUOTE   : guillemets simples (' '), aucune expansion
 * - DQ / DOUBLE_QUOTE   : guillemets doubles (" "), expansion partielle
 */
typedef enum e_quote
{
	NQ,
	NO_QUOTE = NQ,
	SQ,
	SINGLE_QUOTE = SQ,
	DQ,
	DOUBLE_QUOTE = DQ,
}								t_quote;

/*
 * Énumération représentant les différents types de tokens
 * rencontrés lors de l’analyse lexicale d’une ligne de commande.
 * Chaque valeur permet d’identifier la nature syntaxique d’un élément.
 *
 * - ARG          : argument d’une commande
 * - CMD          : nom de commande
 * - FLAG         : option ou paramètre commençant par '-'
 * - PIPE         : opérateur de pipe '|'
 * - REDIR_IN     : redirection d’entrée '<'
 * - REDIR_OUT    : redirection de sortie '>'
 * - REDIR_APPEND : redirection de sortie avec ajout '>>'
 * - HEREDOC      : redirection de type '<<'
 * - LIMITER      : mot-clé limitant un heredoc
 * - FILE_NAME    : nom de fichier cible d’une redirection
 */
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

//* ----------------- Structs ----------------------- *

/*
 * Structure représentant une liste simplement chaînée générique.
 * Utilisée pour stocker les arguments et les options d’une commande
 * avant leur conversion en tableau (argv).
 *
 * - content : chaîne de caractères stockée (argument ou flag)
 * - quote   : type de quoting appliqué à cet élément
 * - next    : pointeur vers l’élément suivant de la liste
 */
typedef struct s_minilist
{
	char						*content;
	t_quote						quote;
	struct s_minilist			*next;
}								t_minilist;

/*
 * Structure représentant une liste doublement chaînée de tokens.
 * Utilisée par le lexer et le parser pour manipuler les éléments
 * lexicaux d’une ligne de commande.
 *
 * - value : contenu textuel du token (mot, opérateur, symbole, etc.)
 * - type  : type syntaxique du token (commande, opérateur, redirection, etc.)
 * - quote : type de quoting appliqué au token
 * - next  : pointeur vers le token suivant
 * - prev  : pointeur vers le token précédent
 */
typedef struct s_token
{
	char						*value;
	t_type						type;
	t_quote						quote;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

/*
 * Structure représentant une commande unique du shell.
 * Utilisée pour stocker toutes les informations nécessaires à son exécution
 * (nom, arguments, options, type de quoting et chaînage pour les pipelines).
 *
 * - command : nom brut de la commande avant résolution du PATH
 * - argv    : tableau final des arguments à passer à execve()
 * - args    : liste chaînée des arguments (hors flags)
 * - flags   : liste chaînée des options (ex: "-l", "-a")
 * - quote   : type de quoting appliqué à la commande
 * - next    : pointeur vers la commande suivante du pipeline
 */
typedef struct s_command
{
	char						*command;
	char						**argv;
	t_minilist					*args;
	t_minilist					*flags;
	t_quote						quote;
	struct s_command			*next;
}								t_command;

/*
 * Structure représentant une redirection de flux dans une commande.
 * Utilisée pour gérer les redirections d’entrée, de sortie et les heredocs.
 *
 * - fd     : descripteur de fichier concerné (entrée ou sortie)
 * - value  : nom du fichier ou limiteur du heredoc
 * - type   : type de redirection (REDIR_IN, REDIR_OUT, REDIR_APPEND, HEREDOC)
 * - quote  : type de quoting appliqué à la valeur (utile pour les limiters)
 */
typedef struct s_redirection
{
	int							fd;
	char						*value;
	t_type						type;
	t_quote						quote;
}								t_redirection;

/*
 * Structure représentant une variable d’environnement.
 * Utilisée pour stocker et manipuler les paires clé/valeur de l’environnement.
 *
 * - key    : nom de la variable d’environnement
 * - equal  : indicateur de présence du signe '=' (0 ou 1)
 * - value  : valeur associée à la clé (peut être NULL si non définie)
 * - next   : pointeur vers l’élément suivant de la liste chaînée
 */
typedef struct s_env
{
	char						*key;
	char						equal;
	char						*value;
	struct s_env				*next;
}								t_env;

/*
 * Structure représentant la gestion des pipes dans un pipeline de commandes.
 * Utilisée par l’executor pour créer, relier et fermer correctement les flux
 * entre processus lors de l’exécution de commandes enchaînées.
 *
 * - nb           : nombre total de pipes dans le pipeline
 * - fds          : tableau contenant les fds de chaque pipe ([read, write])
 * - pids         : tableau des PIDs des processus enfants exécutés
 * - saved_stdio  : sauvegarde des descripteurs standards (stdin, stdout)
 */
typedef struct s_pipes
{
	int							nb;
	// int					**fds;
	int							*fds;
	pid_t						*pids;
	int							saved_stdio[2];
}								t_pipes;

/*
 * Structure principale du shell contenant l’ensemble de l’état global.
 * Elle centralise les informations nécessaires aux différentes étapes :
 * lexing, parsing, expansion, exécution et gestion des redirections.
 *
 * - which_quote : indique l’état de quoting courant lors du parsing
 * - env         : copie du tableau d’environnement initial (char **envp)
 * - line        : ligne de commande saisie par l’utilisateur (readline)
 * - exit_status : code de sortie de la dernière commande exécutée
 * - path        : variable PATH extraite de l’environnement
 * - heredoc_fds : tableau des descripteurs de fichiers pour les heredocs
 * - env_copy    : environnement stocké sous forme de liste chaînée
 * - export      : liste triée des variables exportées
 * - commands    : liste chaînée des commandes à exécuter
 * - tokens      : liste doublement chaînée des tokens lexicaux
 * - input       : structure de redirection d’entrée active
 * - output      : structure de redirection de sortie active
 * - pipes       : structure de gestion des pipes dans le pipeline
 */
typedef struct s_data
{
	char						which_quote;
	char						**env;
	char						*line;
	int							exit_status;
	// char			**parsed_path;
	char						*path;
	int							heredoc_fds[256];
	t_env						*env_copy;
	t_env						*export;
	t_command					*commands;
	t_token						*tokens;
	t_quote						current_quote;
	t_redirection				input;
	t_redirection				output;
	t_pipes						*pipes;
}								t_data;

//* ----------------- Functions ----------------------- *

//* UTILS *//
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

//* INIT *//

t_env							*env_last_var(t_env *env);
int								init(t_data *data, char **envp);
int								init_env(t_data *data, char **envp);
int								update_shlvl(t_data *data, int shlvl);
int								create_shlvl(t_data *data, int shlvl);
int								add_var(t_data *data, t_env **env, char *key,
									char *value);
int								extract_key_value(const char *entry, char **key,
									char **value, int length);

//* SIGNALS *//
int								sig_event(void);
void							signals_handler(void);
void							setup_child_signals(void);
void							setup_heredoc_signals(void);

//* ERRORS *//
int								syntax_error_handler(t_data *data);
void							print_syntax_error(char error, int code);
void							report_error(t_data *data, const char *message,
									int exit_code);
void							report_error2(const char *message_1,
									const char *message_2);
void							report_error3(const char *message_1,
									const char *message_2,
									const char *message_3);

//* FREE *//
void							free_tokens(t_data *data);
void							free_env_list(t_env **env);
void							free_commands(t_data *data);
void							free_pipes_all(t_data *data);
void							free_char_array(char **array);
void							free_redirections(t_data *data);
void							minilist_clear(t_minilist **list);
void							reset_command_context(t_data *data);

//* EXPANDING *//
int								expander(t_data *data);
t_bool							need_expansion(char *str);
t_quote							quote_state(char *line, size_t index);
char							*extract_var_name(char *str, size_t *i);
t_bool							env_var_exists(t_data *data, char *variable);
char							*get_env_value(t_data *data, char *variable);

//* LEXING *//
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

//* PARSING *//
int								parser(t_data *data);
t_command						*append_command_node(t_data *data);
int								setup_heredoc(t_data *data, char *limiter);
size_t							count_command_elements(t_command *command);
int								append_args_to_argv(t_command *current,
									size_t *i);
int								append_flags_to_argv(t_command *current,
									size_t *i);
int								append_command_to_argv(t_command *current,
									size_t *i);
int								is_redirection_value(t_data *data,
									t_token *current_token);
int								add_command_arg(t_command *current_command,
									t_token *current_token);
int								set_command_name(t_command *current_command,
									t_token *current_token);
int								add_command_flag(t_command *current_command,
									t_token *current_token);
int								handle_redirection_fd(t_redirection *redir,
									t_token *token, int flags);

//* EXECUTING *//
int								wait_and_cleanup_pipeline(t_data *data, pid_t *pids, int count,
		int *fds);
int								child_dup_fds(t_data *data, int *fds, int index,
									int number_of_commands);
int								executor(t_data *data);
int								launch_pipeline(t_data *data);
int								prepare_pipes(t_data *data, int number_of_commands);

//* BUILTINS
int								dispatch_builtin(t_data *data, t_command *node);
int								safe_close_fd(int *fd);
int								save_stdio(int *saved_stdio);
t_bool							key_is_valid(const char *key);
int								is_builtin_command(t_command *node);
void							close_pipe_fds(int *fds, int count);
int								restore_saved_stdio(int *saved_stdio);
char							*find_env_value(t_data *data, const char *key);
int								handle_builtin_command(t_data *data, int *fds,
									int index, t_command *node);

//* ECHO
int								execute_builtin_echo(char **argv);

//* CD
int								execute_builtin_cd(t_data *data,
									t_command *node);

//* ENV
int								execute_builtin_pwd(t_data *data,
									t_command *node);

//* EXPORT
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

//* UNSET
int								execute_builtin_unset(t_data *data,
									char **argv);

//* ENV
int								execute_builtin_env(t_data *data, char **argv,
									int fd_out);

//* EXIT
void							exit_minishell(t_data *data, int exit_status);
int								execute_builtin_exit(t_data *data, char **argv);

//* EXTERNALS *//
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

#endif