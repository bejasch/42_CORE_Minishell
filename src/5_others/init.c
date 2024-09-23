/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:45:17 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 15:25:11 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// Initializes all the struct data
int	initialize_data(t_data *data, int argc, char **envp)
{
	if (argc != 1)
		return (error_message(NULL, "main", "Too many arguments"), 0);
	data->list_envs = envs_list(envp);
	data->export_list = array_to_linkedlist(data->list_envs);
	data->shell_name = ft_strdup("minishell");
	data->stdin = dup(STDIN_FILENO);
	data->stdout = dup(STDOUT_FILENO);
	data->status = 0;
	data->status_str = ft_itoa(0);
	handle_signals(0);
	null_data_struct(data);
	return (1);
}

// Resets all data vars for a new prompt
void	null_data_struct(t_data *data)
{
	data->input = NULL;
	data->tokens = NULL;
	data->ast_root = NULL;
	data->linked_args = NULL;
	data->redir_wordlist = NULL;
	data->id = 1;
	data->signal_fd = 0;
	data->in_child = 0;
	data->ran_builtin = 0;
	data->argc = 0;
	data->argv = NULL;
	data->linked_args = NULL;
	data->files_list = NULL;
	data->bin_paths = NULL;
	data->child_pids = NULL;
}

// Restore original STDIN and STDOUT
// option 0: only STDIN, 1: only STDOUT, 2: both
// Returns 1 on success, 0 on failure
int	restore_stdin_stdout(t_data *data, char option)
{
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if (option == 0 || option == 2)
		dup2(data->stdin, STDIN_FILENO);
	if (option == 1 || option == 2)
		dup2(data->stdout, STDOUT_FILENO);
	return (1);
}

// TODO: Implement function to free the AST and linked_args
// build specific functions for freeing the data in each while loop
void	free_prompt_data(t_data *data)
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
	free_waitlist(&data->child_pids);
	ft_lstclear(&data->redir_wordlist, free);
	free_ast(&data->ast_root);
	if (data->status_str)
		free(data->status_str);
	if (g_signal)
		data->status = 130;
	g_signal = 0;
	data->status_str = ft_itoa(data->status);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	if (!restore_stdin_stdout(data, 2))
		exit (ft_free(data, 1));
	null_data_struct(data);
	handle_signals(1);
}

// splits the input into tokens and builds the AST
// returns 1 on success, 0 on empty AST
char	build_ast(t_data *data)
{
	char	**end_tokens;

	data->tokens = split_tokens(data->input);
	if (!data->tokens)
		return (0);
	end_tokens = data->tokens;
	while (*end_tokens)
		end_tokens++;
	end_tokens--;
	if (!right_parenthesis(data->tokens, end_tokens))
		return (error_message("Syntax error", NULL, "Wrong parenthesis"), 0);
	data->ast_root = create_ast(data->tokens, end_tokens);
	if (!data->ast_root)
		return (0);
	return (1);
}
