/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 21:16:28 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 17:44:29 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// helper function to print out the elements of a linked list
void	print_args(char *str, t_list *linked_args)
{
	ft_printf("\t%s:\n", str);
	while (linked_args)
	{
		ft_printf("CONTENT: %s\n", linked_args->content);
		linked_args = linked_args->next;
	}
}

// for debugging, print the AST structure
void	print_ast_level(t_ast *node, int level)
{
	int	i;

	i = 0;
	if (node)
	{
		while (i++ < level)
			ft_printf("    ");
		ft_printf("%s\n", node->value);
		print_ast_level(node->left, level + 1);
		print_ast_level(node->right, level + 1);
	}
}

// Print the AST structure
void	print_ast(t_ast *root)
{
	if (!root)
		return ;
	ft_printf("%s\n", root->value);
	print_ast_level(root->left, 1);
	print_ast_level(root->right, 1);
}

// Prints out the argv array
void	print_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		ft_printf("ARG: -->%s<--\n", argv[i++]);
}

// static void	print_tokens(char **tokens)
// {
// 	ft_printf("Tokens:\n");
// 	while (*tokens)
// 	{
// 		ft_printf("%s\n", *tokens);
// 		tokens++;
// 	}
// }
