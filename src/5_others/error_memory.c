/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:50:50 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/23 16:50:39 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

//Function to free array of strings
void	free_array(char ***array)
{
	size_t	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
		free((*array)[i++]);
	free(*array);
	*array = NULL;
}

// Function to print error messages
void	error_message(char *cmd, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

// This function recursively frees the left and right nodes
void	free_ast(t_ast **node)
{
	if (!node || !*node)
		return ;
	if ((*node)->left)
		free_ast(&(*node)->left);
	if ((*node)->right)
		free_ast(&(*node)->right);
	free(*node);
	*node = NULL;
}

// Free function for the data struct
int	ft_free(t_data *data, int exit)
{
	if (data->input)
		free(data->input);
	if (data->tokens)
		ft_split_free(data->tokens);
	if (data->bin_paths)
		ft_split_free(data->bin_paths);
	ft_lstclear(&data->linked_args, free);
	ft_lstclear(&data->files_list, free);
	free_array(&data->argv);
	free_ast(&data->ast_root);
	ft_lstclear(&data->export_list, free);
	ft_lstclear(&data->redir_wordlist, free);
	if (data->status_str)
		free(data->status_str);
	free_array(&data->list_envs);
	free_waitlist(&data->child_pids);
	free(data->shell_name);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	rl_clear_history();
	return (exit);
}

// Error message for wrong syntax
void	spec_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}
