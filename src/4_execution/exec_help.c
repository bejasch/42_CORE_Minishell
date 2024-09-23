/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:46:53 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/23 16:50:37 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static char	try_run_path(char **path, char file_exists, t_data *data);

// run a built-in command if it is one of the following:
char	run_builtin(t_data *data)
{
	if (!ft_strncmp(*data->argv, "echo", 5))
		data->status = builtin_echo(data->argv, data->argc);
	else if (!ft_strncmp(*data->argv, "cd", 3))
		data->status = builtin_cd(data);
	else if (!ft_strncmp(*data->argv, "pwd", 4))
		data->status = builtin_pwd();
	else if (!ft_strncmp(*data->argv, "export", 7))
		data->status = builtin_export(data->argv,
				data->argc, data->list_envs, data->export_list);
	else if (!ft_strncmp(*data->argv, "unset", 6))
		data->status = builtin_unset(data);
	else if (!ft_strncmp(*data->argv, "env", 4))
		data->status = builtin_env(data->list_envs);
	else if (!ft_strncmp(*data->argv, "exit", 5))
		data->status = builtin_exit(data->argv, data->argc, data);
	else
		return (0);
	if (data->in_child)
		exit(ft_free(data, data->status));
	data->ran_builtin = 1;
	return (1);
}

// creates an array of cmd-strings from a linked list of strings
// Also frees the linked_args list
char	create_argv_argc(t_data *data, t_list *linked_args)
{
	int		size;
	char	**writer;

	size = ft_lstsize(linked_args);
	if (!size)
		return (0);
	data->argv = (char **)malloc((size + 1) * sizeof(char *));
	if (!data->argv)
		return (0);
	writer = data->argv;
	while (linked_args)
	{
		clean_quotations(linked_args->content, linked_args->content, 0, 0);
		*writer = ft_strdup(linked_args->content);
		if (!*writer)
			return (0);
		linked_args = linked_args->next;
		writer++;
	}
	*writer = NULL;
	data->argc = size;
	return (1);
}

// remove list elements containing empty/no strings
t_list	*clean_args(t_list **args)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = *args;
	prev = NULL;
	while (tmp)
	{
		if (!tmp->content || !*(char *)tmp->content)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*args = tmp->next;
			free(tmp->content);
			free(tmp);
			tmp = prev;
		}
		prev = tmp;
		if (tmp)
			tmp = tmp->next;
	}
	return (*args);
}

// Checks for binaries in PATH
int	run_from_bin_path(t_data *data)
{
	char	*path_str;

	path_str = search_env("PATH", data->list_envs);
	if (!path_str)
	{
		error_message(data->argv[0], NULL, "No such file or directory");
		return (127);
	}
	data->bin_paths = ft_split(path_str, ':');
	if (!data->bin_paths)
		return (1);
	if (try_run_path(data->bin_paths, 0, data))
	{
		error_message(data->argv[0], NULL, "Permission denied");
		exit(ft_free(data, 126));
	}
	error_message(data->argv[0], NULL, "command not found");
	exit(ft_free(data, 127));
}

// Searches all path strings for cmd; returns 1 if found without exec. rights
static char	try_run_path(char **path, char file_exists, t_data *data)
{
	char	*tmp;

	while (*path)
	{
		tmp = ft_strjoin(*path, "/");
		if (!tmp)
			exit(ft_free(data, 1));
		free(*path);
		*path = ft_strjoin(tmp, data->argv[0]);
		if (!*path)
			*path = tmp;
		else
			free(tmp);
		if (access(*path, F_OK) == 0)
			file_exists = 1;
		if (access(*path, X_OK) == 0)
			exit(ft_free(data, execve(*path, data->argv, data->list_envs)));
		path++;
	}
	return (file_exists);
}
