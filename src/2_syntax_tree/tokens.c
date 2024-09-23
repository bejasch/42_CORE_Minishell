/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:33 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 22:31:03 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static size_t	count_tokens(char *str);
static void		move_to_next_token(char **str, char in_sgl, char in_dbl);
static char		clean_tokens(char **tok, char **tmp, char **check, char *copy);
static char		*has_space(char *str, char in_sgl, char in_dbl);

// Allocates (with malloc(3)) and returns an array of token-strings 
// obtained by splitting on operators: ( ) | || && < << > >> . 
// The array ends with a NULL pointer.
char	**split_tokens(char *str)
{
	char	**tokens_out;
	size_t	tokens;

	if (!str)
		return (NULL);
	tokens = count_tokens(str);
	tokens_out = (char **)malloc((tokens + 1) * sizeof(char *));
	if (!tokens_out)
		return (NULL);
	if (!write_all_tokens(str, tokens_out, tokens))
		return (NULL);
	if (!clean_tokens(tokens_out, tokens_out, NULL, NULL))
		return (ft_split_free(tokens_out), NULL);
	return (tokens_out);
}

static size_t	count_tokens(char *str)
{
	size_t	amount;

	amount = 0;
	while (*str)
	{
		if (*str == ' ')
			str++;
		else if (is_operator(str))
		{
			amount++;
			str += is_operator(str);
		}
		else
		{
			amount++;
			move_to_next_token(&str, 0, 0);
		}
	}
	return (amount);
}

static void	move_to_next_token(char **str, char in_sgl, char in_dbl)
{
	while (**str)
	{
		if (in_sgl || in_dbl)
		{
			if (in_sgl && **str == '\'')
				in_sgl = 0;
			else if (in_dbl && **str == '"')
				in_dbl = 0;
		}
		else if (**str == '\'')
			in_sgl = 1;
		else if (**str == '"')
			in_dbl = 1;
		else if (is_operator(*str))
			break ;
		(*str)++;
	}
}

// moves something behind redirection operators in front of them
static char	clean_tokens(char **tok, char **tmp, char **check, char *copy)
{
	while (*tmp)
	{
		if (is_redirection(*tmp) && has_space(*(tmp + 1), 0, 0))
		{
			check = tmp - 1;
			while (check >= tok)
			{
				if (!is_operator(*check)
					&& (check == tok || !is_redirection(*(check - 1))))
				{
					copy = *check;
					*check = ft_strjoin(*check, has_space(*(tmp + 1), 0, 0));
					if (!*check)
						return (0);
					free(copy);
					*has_space(*(tmp + 1), 0, 0) = '\0';
					break ;
				}
				check--;
			}
		}
		tmp++;
	}
	return (1);
}

// returns the address of the first space outside of quotes
static char	*has_space(char *str, char in_sgl, char in_dbl)
{
	while (*str)
	{
		if (in_sgl || in_dbl)
		{
			if ((in_sgl && *str == '\'') || (in_dbl && *str == '"'))
			{
				in_sgl = 0;
				in_dbl = 0;
			}
		}
		else if (*str == '\'' || *str == '"')
		{
			if (*str == '\'')
				in_sgl = 1;
			else if (*str == '"')
				in_dbl = 1;
		}
		else if (*str == ' ')
			return (str);
		str++;
	}
	return (NULL);
}
