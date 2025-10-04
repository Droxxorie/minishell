/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:42:52 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:29:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/** 
 * @brief Libère et réinitialise l’état global du shell entre deux exécutions.
 * Cette fonction nettoie toutes les ressources dynamiques allouées
 * (environnement, tokens, commandes, redirections, pipes, etc.)
 * et ferme les descripteurs de fichiers ouverts.
 *
 * @param data Pointeur vers la structure principale contenant l’état du shell.
*/
void	cleanup_shell_state(t_data *data)
{
	int	i;

	if (!data)
		return ;
	free_env_list(&data->env_copy);
	free_env_list(&data->export);
	free_tokens(data);
	free_commands(data);
	free_redirections(data);
	free(data->line);
	data->line = NULL;
	i = 0;
	while (i < 256)
	{
		if (data->heredoc_fds[i] > 2)
			close(data->heredoc_fds[i]);
		data->heredoc_fds[i++] = -1;
	}
	if (data->pipes)
	{
		free(data->pipes->fds);
		free(data->pipes->pids);
		free(data->pipes);
	}
	data->pipes = NULL;
}

/**
 * @brief Gère les lignes vides ou non significatives saisies par l’utilisateur
 * Vérifie si la ligne est vide, ne contient que des espaces, ou correspond à
 * certaines entrées spéciales (":" ou "!"), puis la libère si nécessaire.
 *
 * @param data Pointeur vers la structure principale contenant la ligne lue.
 *
 * @return 1 si la ligne doit être ignorée (et libérée), 0 sinon.
*/
static int	empty_line_handler(t_data *data)
{
	int	i;

	i = 0;
	if (!data->line[0])
		return (free(data->line), 1);
	while (ft_iswhitespace(data->line[i]))
		i++;
	if (!data->line[i])
		return (free(data->line), 1);
	if (data->line[0] == ':' && !data->line[1])
		return (free(data->line), 1);
	if (data->line[0] == '!' && !data->line[1])
	{
		data->exit_status = 1;
		return (free(data->line), 1);
	}
	return (0);
}

/// @brief Termine proprement l’exécution du minishell.
/// Ferme les descripteurs de fichiers ouverts, libère les ressources
/// associées aux pipes et à l’état global, puis quitte le programme.
///
/// @param data Pointeur vers la structure principale contenant l’état du shell.
/// @param exit_status Code de sortie à retourner au système.
void	exit_minishell(t_data *data, int exit_status)
{
	close_fds_from(3);
	if (data->pipes->fds)
	{
		free(data->pipes->fds);
		data->pipes->fds = NULL;
	}
	if (data->pipes->pids)
	{
		free(data->pipes->pids);
		data->pipes->pids = NULL;
	}
	printf("exit\n");
	cleanup_shell_state(data);
	clear_history();
	exit(exit_status);
}

static void	launch_minishell(t_data *data)
{
	while (1)
	{
		signals_handler();
		g_waiting = 0;
		data->line = readline("minishell$ ");
		if (g_waiting == 1)
			data->exit_status = 130;
		else if (g_waiting == 3)
			data->exit_status = 131;
		if (data->line == NULL)
			exit_minishell(data, data->exit_status);
		if (empty_line_handler(data))
			continue ;
		if (syntax_error_handler(data))
		{
			reset_command_context(data);
			continue ;
		}
		if (ft_strlen(data->line))
			add_history(data->line);
		if (lexer(data) == EXIT_FAILURE || parser(data) == EXIT_FAILURE
			|| expander(data) == EXIT_FAILURE || executor(data) == EXIT_FAILURE)
		{
			reset_command_context(data);
			continue ;
		}
		reset_command_context(data);
	}
}

/// @brief Point d’entrée principal du programme minishell.
/// Initialise la structure principale, vérifie l’environnement
/// et lance la boucle d’exécution du shell.
///
/// @param argc Nombre d’arguments passés au programme.
/// @param argv Tableau des arguments passés au programme.
/// @param envp Tableau de chaînes représentant l’environnement système.
///
/// @return EXIT_SUCCESS en cas de succès,
/// ou EXIT_FAILURE en cas d’erreur critique.
int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (argc != 1)
		return (1);
	if (!isatty(STDIN_FILENO))
	{
		ft_putstr_fd("minishell: no child process allowed\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_memset(&data, 0, sizeof(t_data));
	if (init(&data, envp) == EXIT_FAILURE)
		exit_minishell(&data, EXIT_FAILURE);
	launch_minishell(&data);
	return (EXIT_SUCCESS);
}
