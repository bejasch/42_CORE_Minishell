/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:56:31 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 12:38:50 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static size_t	cmd_len(char *str, char in_sgl, char in_dbl);
static char		*token_write(char const *str, size_t len);

// Writes all tokens into the array
char	write_all_tokens(char *str, char **tokens_out, size_t tokens)
{
	char	**writer;
	size_t	len;

	writer = tokens_out;
	while (tokens--)
	{
		while (*str == ' ')
			str++;
		if (is_operator(str))
			len = is_operator(str);
		else
			len = cmd_len(str, 0, 0);
		*writer = token_write(str, len);
		if (!(*(writer++)))
			return (ft_split_free(tokens_out), 0);
		str += len;
	}
	*writer = 0;
	return (1);
}

static size_t	cmd_len(char *str, char in_sgl, char in_dbl)
{
	size_t	len;

	len = 0;
	while (str[len])
	{
		if (in_sgl || in_dbl)
		{
			if (in_sgl && str[len] == '\'')
				in_sgl = 0;
			else if (in_dbl && str[len] == '"')
				in_dbl = 0;
		}
		else if (str[len] == '\'')
			in_sgl = 1;
		else if (str[len] == '"')
			in_dbl = 1;
		else if (is_operator(str + len))
			break ;
		len++;
	}
	while (str[len - 1] == ' ')
		len--;
	return (len);
}

// Writes one token into a new string with length len
static char	*token_write(char const *str, size_t len)
{
	char	*out;
	char	*writer;

	out = (char *)malloc((len + 1) * sizeof(char));
	if (!out)
		return (NULL);
	writer = out;
	while (len--)
		*(writer++) = *(str++);
	*writer = '\0';
	return (out);
}
