/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntaxe_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:20:13 by eraad             #+#    #+#             */
/*   Updated: 2025/10/04 19:16:51 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Affiche un message d’erreur de syntaxe adapté au code fourni.
 *
 * Formate les messages « operation '…' not supported » et
 * « syntax error near unexpected token '…' », ainsi qu’un cas générique.
 *
 * @param error Caractère fautif (si applicable).
 * @param code  Code d’erreur :
 *              1 = '\n' / ';' / '\\' non supporté hors quotes
 *              2 = "||" non supporté
 *              3 = "&&" non supporté
 *              4 = token inattendu (ex. guillemet)
 *              5 = token inattendu + delimiter heredoc invalide
 *              6 = guillemet non fermé
 *              7 = token inattendu (générique)
 */
void	print_syntax_error(char error, int code)
{
	if (code < 4)
	{
		ft_putstr_fd("minishell: operation '", 2);
		if (code == 1)
			ft_putchar_fd(error, 2);
		else if (code == 2)
			ft_putstr_fd("||", 2);
		else if (code == 3)
			ft_putstr_fd("&&", 2);
		ft_putstr_fd("' not supported\n", 2);
	}
	else if (code == 4 || code == 5)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		ft_putchar_fd(error, 2);
		ft_putstr_fd("'\n", 2);
		if (code == 5)
			ft_putstr_fd("invalid heredoc delimiter\n", 2);
	}
	else if (code == 6)
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
	else if (code == 7)
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
}

/**
 * @brief Analyse la ligne pour détecter les erreurs de syntaxe simples.
 *
 * Vérifie la présence de guillemets non fermés, d’opérateurs interdits
 * ou mal placés, et de caractères spéciaux invalides.
 * Met à jour le champ which_quote de data selon l’état courant.
 *
 * @param line          Ligne de commande à analyser.
 * @param opened_quote  Indicateur d’ouverture d’un guillemet (0 ou 1).
 * @param index         Position actuelle dans la ligne.
 * @param data          Pointeur vers la structure principale du shell.
 * @return Code d’erreur non nul si une erreur est détectée, 0 sinon.
 */
static int	scan_line(char *line, int opened_quote, int index, t_data *data)
{
	while (line[index])
	{
		if (!opened_quote && (line[index] == '\'' || line[index] == '"'))
		{
			data->which_quote = line[index];
			opened_quote = 1;
		}
		else if ((line[index] == '\'' || line[index] == '"')
			&& line[index] == data->which_quote)
		{
			opened_quote = 0;
			data->which_quote = '\0';
		}
		if ((line[index] == '\n' || line[index] == ';' || line[index] == '\\')
			&& !opened_quote)
			return (print_syntax_error(line[index], 1), 1);
		if (!opened_quote && line[index] == '|' && line[index + 1] == '|')
			return (print_syntax_error('|', 2), 2);
		if ((line[index] == '&' && line[index + 1] == '&') && !opened_quote)
			return (print_syntax_error('&', 3), 3);
		index++;
	}
	if (opened_quote)
		return (print_syntax_error(data->which_quote, 4), 4);
	return (0);
}

/**
 * @brief Analyse la ligne de commande pour détecter une erreur de syntaxe.
 *
 * Réinitialise l’état de quoting, appelle le scanner lexical et met à jour
 * le code de sortie en cas d’erreur syntaxique détectée.
 *
 * @param data Pointeur vers la structure principale contenant la ligne analysée.
 * @return 1 si une erreur de syntaxe est détectée, 0 sinon.
 */
int	syntax_error_handler(t_data *data)
{
	int	errno;

	if (!data->line)
		return (0);
	data->which_quote = '\0';
	errno = scan_line(data->line, 0, 0, data);
	if (errno != 0)
	{
		data->exit_status = 2;
		return (1);
	}
	return (0);
}
