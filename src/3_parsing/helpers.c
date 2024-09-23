/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:14:13 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 22:32:33 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// removes quotations from a string (but no quotes inside quotes) 
void	clean_quotations(char *str, char *writer, char in_sgl, char in_dbl)
{
	while (*str)
	{
		if (in_sgl || in_dbl)
		{
			if ((in_sgl && *str == '\'') || (in_dbl && *str == '"'))
			{
				in_sgl = 0;
				in_dbl = 0;
				str++;
				continue ;
			}
		}
		else if (*str == '\'' || *str == '"')
		{
			if (*str == '\'')
				in_sgl = 1;
			else if (*str == '"')
				in_dbl = 1;
			str++;
			continue ;
		}
		*(writer++) = *(str++);
	}
	*writer = '\0';
}

// checks for right usage of parenthesis in token strings
char	right_parenthesis(char **token_start, char **token_end)
{
	int		par;

	par = 0;
	while (token_start <= token_end)
	{
		if (ft_strncmp(*token_start, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_start, ")", 1) == 0)
			par--;
		if (par < 0)
			return (0);
		token_start++;
	}
	if (par)
		return (0);
	return (1);
}

// returns 1 if >, >>, <, <<; else 0
char	is_redirection(char *str)
{
	if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
		return (1);
	else if (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
		return (1);
	return (0);
}

// 0 : no operator
// 1 : ( ) | < >
// 2 : && || << >>
char	is_operator(char *str)
{
	if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2))
		return (2);
	else if (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
		return (2);
	else if (!ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (1);
	else if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
		return (1);
	else if (!ft_strncmp(str, "|", 1))
		return (1);
	return (0);
}
