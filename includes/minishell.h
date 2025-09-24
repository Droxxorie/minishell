/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 15:24:16 by eraad             #+#    #+#             */
/*   Updated: 2025/09/24 20:52:13 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# define SUPPORTED_OPERATORS "<>|" //* opérateurs unitaires supportés
# define FD_STATE_NONE -1        //* aucun FD particulier à laisser ouvert
# define FD_STATE_RO 0           //* seule l'extrémité pipe READ reste ouverte
# define FD_STATE_WO 1           //* seule l'extrémité pipe WRITE reste ouverte
# define FD_STATE_RW 2           //* les deux extrémités pipe restent ouvertes

//* ----------------- Globals ----------------------- *
# ifndef GLOBALS_H
#  define GLOBALS_H

extern int				g_waiting;

# endif
//* indique si le shell attend la fin d'un processus enfant

//* ----------------- Enums ----------------------- *
typedef enum e_quote
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}						t_quote;

typedef enum e_type
{
	ARG,
	CMD,
	FLAG,
	PIPE,         //* |
	REDIR_IN,     //* <
	REDIR_OUT,    //* >
	REDIR_APPEND, //* >>
	HEREDOC,      //* <<
	LIMITER,      //* limiter for heredoc
	FILE_NAME,   //* filename for redirections
}						t_type;

//* ----------------- Structs ----------------------- *
//*  Liste simplement chaînée (utilisée pour flags/args)
typedef struct s_list
{
	char				*content;
	t_quote				quote;
	struct s_list		*next;
}						t_list;

//*  Liste doublement chaînée (utilisée pour les tokens)
typedef struct s_token
{
	char				*value;
	t_type				type;
	t_quote				quote;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

//* Nœud de commande agrégée par le parser
typedef struct s_command
{
	char *command;      //* nom brut de la commande avant path resolution
	char **final_group; //* tableau final des arguments (command + args)
	t_list *args;       //* liste chaînée des arguments
	t_list *flags;      //* liste chaînée des flags
	t_quote quote;      //* type de quote si command est entre quotes
	struct s_command	*next;
}						t_command;

//* Redirections élémentaires (utiles pendant le parsing)
typedef struct s_redirection
{
	int					fd;
	char *value;   //* nom de fichier ou limiter
	t_type type;   //* type de redirection
	t_quote quote; //* type de quote si value est entre quotes
}						t_redirection;

//* Représentation de l'environnement (clé/valeur)
typedef struct s_env
{
	char				*key;
	char				equal;
	char				*value;
	struct s_env		*next;
}						t_env;

//* Regroupe la topologie des pipes / PIDs d’un pipeline
typedef struct s_pipes
{
	int nb;           //* nombre de pipes dans le pipeline
	int **fds;        //* tableau des FDs des pipes
	pid_t *pids;      //* tableau des PIDs des processus enfants
	int first_fds[2]; //* sauvegarde des FDs originaux stdin et stdout
}						t_pipes;

typedef struct s_data
{
	//* etat parsing/lexing
	char				which_quote;
	//* indique le type de quote manquante en cas d'erreur de parsing/lexing
	char				**env;
	//* environnement initial (copie de l'env passé en paramètre à main)
	char **parsed_env; //* environnement modifié pour execve
	char *expanded_str; //* écrasement mémoire(sert a stocker $user etc)

	//* PATH resolution
	char **parsed_path; //* tableau des chemins complets (PATH+ / + command)
	char *path;         //* tableau des chemins extraits de la variable PATH

	//* ligne et statuts
	char *line;      //* ligne lue par readline
	int exit_status; //* statut de sortie de la dernière commande exécutée

	//* heredoc
	int heredoc_fds[256]; //* tableau des FDs des heredocs
	//* int nb_heredocs; //* nombre de heredocs dans la ligne //TODO a faire

	//* listes principales
	t_env				*env_copy;
	//* copie de l'environnement sous forme de liste chaînée
	t_env *export;       //* liste chaînée pour export (variables triées)
	t_command *commands; //* liste chaînée des commandes
	t_token *tokens;     //* liste doublement chaînée des tokens
	t_redirection		input;
	t_redirection		output;
	t_pipes *pipes; //* structure contenant les infos sur les pipes
}						t_data;

//* ----------------- Functions ----------------------- *

//* UTILS
long long	ft_atoll(const char *str);
void		free_char_array(char **array);
char		*ft_append_char(char *str, char c);

//* INIT
t_env	*env_last_var(t_env *env);
int		add_var(t_data *data, t_env **env, char *key, char *value);
int		init_env(t_data *data, char **envp);

//* SIGNALS
void	signals_handler(void);

//* ERRORS
int		syntax_error_handler(t_data *data);
void	print_syntax_error(char error, int code);

//* FREE
void	free_tokens(t_data *data);

//* LEXING
int 	lexer(t_data *data);
int		handle_no_quote(t_data *data, t_quote *quote_state, char **token_buffer, int *command_boundary);
int		handle_single_quoted(t_data *data, t_quote *quote_state, char **token_buffer);
int		handle_double_quoted(t_data *data, t_quote *quote_state, char **token_buffer);
t_token	*create_token(char *start, char *end, t_type type, t_quote quote);
t_token	*add_classified_token(t_data *data, char *token_buffer, int *command_boundary);
t_token	*add_back_token(t_data *data, t_token *new_token);
t_token	*classify_heredoc_delimiters(t_token *tokens);
t_token	*classify_input_redirections(t_token *tokens);
t_token	*normalize_exit_echo_args(t_token *tokens);
t_token	*normalize_redirection_args(t_token *tokens);
int		validate_pipe_syntax(t_data *data);

#endif