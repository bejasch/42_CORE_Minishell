/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:19 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 19:39:12 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static t_ast	*create_node(char *value);
static t_ast	*and_or_node(char **token_start, char **token_end, char par);
static t_ast	*pipe_node(char **token_start, char **token_end, char par);
static t_ast	*redirect_node(char **token_start, char **token_end, char par);

// Creates an AST out of the tokens array
t_ast	*create_ast(char **token_start, char **token_end)
{
	t_ast	*node;

	if (token_start > token_end)
		return (NULL);
	else if (token_start == token_end)
		return (create_node(*token_start));
	node = and_or_node(token_start, token_end, 0);
	if (node)
		return (node);
	node = pipe_node(token_start, token_end, 0);
	if (node)
		return (node);
	node = redirect_node(token_start, token_end, 0);
	if (node)
		return (node);
	return (create_ast(token_start + 1, token_end - 1));
}

// Function to create a new AST node
static t_ast	*create_node(char *value)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->value = value;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

// checks for AND && or OR || outside of parenthesis
static t_ast	*and_or_node(char **token_start, char **token_end, char par)
{
	t_ast	*node;
	char	**token_search;

	token_search = token_start;
	while (token_search <= token_end)
	{
		if (ft_strncmp(*token_search, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_search, ")", 1) == 0)
			par--;
		else if (!par && (!ft_strncmp(*token_search, "&&", 2)
				|| !ft_strncmp(*token_search, "||", 2)))
		{
			node = create_node(*token_search);
			node->left = create_ast(token_start, token_search - 1);
			node->right = create_ast(token_search + 1, token_end);
			return (node);
		}
		token_search++;
	}
	return (NULL);
}

// checks for a pipe | outside of parenthesis
static t_ast	*pipe_node(char **token_start, char **token_end, char par)
{
	t_ast	*node;
	char	**token_search;

	token_search = token_start;
	while (token_search <= token_end)
	{
		if (ft_strncmp(*token_search, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_search, ")", 1) == 0)
			par--;
		else if (!par && !ft_strncmp(*token_search, "|", 1))
		{
			node = create_node(*token_search);
			node->left = create_ast(token_start, token_search - 1);
			node->right = create_ast(token_search + 1, token_end);
			return (node);
		}
		token_search++;
	}
	return (NULL);
}

// checks for redirections >, >>, < or << outside of parenthesis
static t_ast	*redirect_node(char **token_start, char **token_end, char par)
{
	t_ast	*node;
	char	**token_search;

	token_search = token_start;
	while (token_search <= token_end)
	{
		if (ft_strncmp(*token_search, "(", 1) == 0)
			par++;
		else if (ft_strncmp(*token_search, ")", 1) == 0)
			par--;
		else if (!par && (!ft_strncmp(*token_search, ">", 1)
				|| !ft_strncmp(*token_search, ">>", 2)
				|| !ft_strncmp(*token_search, "<", 1)
				|| !ft_strncmp(*token_search, "<<", 2)))
		{
			node = create_node(*token_search);
			node->left = create_ast(token_start, token_search - 1);
			node->right = create_ast(token_search + 1, token_end);
			return (node);
		}
		token_search++;
	}
	return (NULL);
}
