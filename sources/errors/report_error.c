/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   report_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 17:28:55 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:14:35 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 *  @brief Affiche un message d’erreur formaté sur la sortie d’erreur standard
 * et met à jour le code de sortie du shell si nécessaire.
 *
 * @param data       Pointeur vers la structure principale du shell.
 * @param message    Message d’erreur à afficher (peut être NULL ou vide).
 * @param exit_code  Code de sortie à affecter à data->exit_status
 *                   (ignorer si égal à -1).
 */
void	report_error(t_data *data, const char *message, int exit_code)
{
	if (message && *message)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)message, 2);
		ft_putstr_fd("\n", 2);
	}
	if (exit_code != -1)
		data->exit_status = exit_code;
}

/**
 * @brief Affiche un message d’erreur composé de deux parties.
 *
 * Écrit sur la sortie d’erreur standard un message formaté avec
 * un préfixe "minishell: " suivi de deux segments textuels.
 *
 * @param message_1 Première partie du message (ex : commande).
 * @param message_2 Seconde partie du message (ex : description).
 */
void	report_error2(const char *message_1, const char *message_2)
{
	ft_putstr_fd("minishell: ", 2);
	if (message_1 && *message_1)
		ft_putstr_fd((char *)message_1, 2);
	if (message_2 && *message_2)
		ft_putstr_fd((char *)message_2, 2);
	ft_putstr_fd("\n", 2);
}

/**
 * @brief Affiche un message d’erreur composé de trois parties.
 * Permet d’afficher sur la sortie d’erreur standard un message
 * formaté avec jusqu’à trois segments textuels.
 *
 * @param message_1 Première partie du message (ex : commande).
 * @param message_2 Seconde partie du message (ex : argument).
 * @param message_3 Troisième partie du message (ex : erreur).
 */
void	report_error3(const char *message_1, const char *message_2,
		const char *message_3)
{
	ft_putstr_fd("minishell: ", 2);
	if (message_1 && *message_1)
		ft_putstr_fd((char *)message_1, 2);
	if (message_2 && *message_2)
		ft_putstr_fd((char *)message_2, 2);
	if (message_3 && *message_3)
		ft_putstr_fd((char *)message_3, 2);
	ft_putstr_fd("\n", 2);
}
