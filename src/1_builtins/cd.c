/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:25:57 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 23:02:19 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

//Helper for cd built-in to update PWD and OLDPWD
int	update_env_var(const char *key, const char *value, char **list_envs)
{
	char	env_var[1024];

	ft_strlcpy(env_var, key, sizeof(env_var));
	ft_strcat(env_var, value);
	update_list(env_var, list_envs);
	return (0);
}

//Helper for cd built-in for a change to home directory
int	change_to_home(char **list_envs)
{
	char	*home_dir;

	home_dir = search_env("HOME", list_envs);
	if (!home_dir)
	{
		error_message("cd", NULL, "HOME not set");
		return (1);
	}
	if (chdir(home_dir) == -1)
	{
		error_message("cd", NULL, "error changing to HOME directory");
		return (1);
	}
	return (0);
}

// TODO: Seemingly "cd -" should also work!??!
int	builtin_cd(t_data *data)
{
	char	cwd[1024];
	char	*current_dir;
	char	*previous_pwd;

	if (data->argc > 2)
	{
		error_message("cd", NULL, "too many arguments");
		return (1);
	}
	previous_pwd = search_env("PWD", data->list_envs);
	if (data->argc == 1 && change_to_home(data->list_envs))
		return (1);
	if (data->argc == 2 && !ft_strncmp(data->argv[1], "~", 1))
		update_home(data, &data->argv[1]);
	if (data->argc == 2 && chdir(data->argv[1]) == -1)
	{
		error_message("cd", data->argv[1], "No such file or directory");
		return (1);
	}
	current_dir = getcwd(cwd, sizeof(cwd));
	if (update_env_var("OLDPWD=", previous_pwd, data->list_envs) == -1
		|| update_env_var("PWD=", current_dir, data->list_envs) == -1)
		return (1);
	return (0);
}
