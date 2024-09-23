/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proof.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:40:18 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 14:51:38 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	check_input_cont(char *str, char needs_string);
static char	valid_string(char *str, char in_sgl, char in_dbl);
static char	car_sgl_dbl(char **str, char *in_sgl, char *in_dbl);
static char	after_pipe(char *str);

// needs_string: 0 = no, 1 = yes (string must follow !)
char	check_input(char *str, char needs_string)
{
	while (*str == ' ')
		str++;
	if (!*str && !needs_string)
		return (1);
	else if (!*str)
		return (spec_error("newline"), 0);
	if (!ft_strncmp(str, "&&", 2))
		return (spec_error("&&"), 0);
	if (!ft_strncmp(str, "||", 2))
		return (spec_error("||"), 0);
	if (!ft_strncmp(str, "&", 1))
		return (spec_error("&"), 0);
	if (!ft_strncmp(str, "|&", 2))
		return (spec_error("|&"), 0);
	if (!ft_strncmp(str, "|", 1))
		return (spec_error("|"), 0);
	return (check_input_cont(str, needs_string));
}

static char	check_input_cont(char *str, char needs_string)
{
	if (!needs_string
		&& (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2)))
		return (check_input(str + 2, 1));
	if (!ft_strncmp(str, ">>", 2))
		return (spec_error(">>"), 0);
	if (!ft_strncmp(str, "<<", 2))
		return (spec_error("<<"), 0);
	if (!needs_string
		&& (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1)))
		return (check_input(str + 1, 1));
	if (!ft_strncmp(str, ">", 1))
		return (spec_error(">"), 0);
	if (!ft_strncmp(str, "<", 1))
		return (spec_error("<"), 0);
	return (valid_string(str, 0, 0));
}

// Check if the string is valid
static char	valid_string(char *str, char in_sgl, char in_dbl)
{
	if (*str == '&' || *str == '|' || *str == '>' || *str == '<')
		return (0);
	while (*str)
	{
		if (!car_sgl_dbl(&str, &in_sgl, &in_dbl))
			continue ;
		if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2)
			|| !ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
			return (check_input(str + 2, 1));
		else if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
			return (check_input(str + 1, 1));
		else if (!ft_strncmp(str, "|", 1))
			return (after_pipe(str + 1));
		else if (!ft_strncmp(str, "(", 1))
			return (check_input(str + 1, 0));
		else if (!ft_strncmp(str++, ")", 1))
			return (check_input(str, 0));
	}
	if (in_sgl || in_dbl)
		return (error_message(NULL, NULL,
				"syntax error: unclosed quotation mark"), 0);
	return (1);
}

static char	car_sgl_dbl(char **str, char *in_sgl, char *in_dbl)
{
	if (*in_sgl || *in_dbl)
	{
		if ((*in_sgl && **str == '\'') || (*in_dbl && **str == '"'))
		{
			*in_sgl = 0;
			*in_dbl = 0;
		}
		(*str)++;
		return (0);
	}
	else if (**str == '\'' || **str == '"')
	{
		if (**str == '\'')
			*in_sgl = 1;
		else if (**str == '"')
			*in_dbl = 1;
		(*str)++;
		return (0);
	}
	return (1);
}

static char	after_pipe(char *str)
{
	while (*str == ' ')
		str++;
	if (!*str)
		return (error_message(NULL, "syntax error",
				"pipe at end of input"), 0);
	return (check_input(str, 0));
}
