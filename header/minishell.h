/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:23:35 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 16:52:45 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

# include "../src/0_Libft_extended/include/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <curses.h>
# include <errno.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/ioctl.h>

// global var for received signals
extern volatile __sig_atomic_t	g_signal;

typedef struct s_ast			t_ast;

// struct for the Abstract syntax tree nodes
struct s_ast
{
	char	*value;
	t_ast	*left;
	t_ast	*right;
};

typedef struct s_pid			t_pid;

// struct for the Abstract syntax tree nodes
struct s_pid
{
	pid_t	id;
	t_pid	*next;
};

typedef struct s_vars			t_vars;

// struct for linked variables list to be replaced:
typedef struct s_vars
{
	char	*key_start;
	int		key_len;
	char	*value_start;
	int		len_diff;
	t_vars	*next;
}	t_vars;

// struct for the information
typedef struct s_data
{
	char	*shell_name;
	int		stdin;
	int		stdout;
	char	*input;
	char	**tokens;
	t_ast	*ast_root;
	int		status;
	char	*status_str;
	char	**list_envs;
	t_list	*export_list;
	char	in_child;
	char	ran_builtin;
	int		fd[2];
	int		signal_fd;
	pid_t	id;
	int		argc;
	char	**argv;
	t_list	*linked_args;
	t_list	*files_list;
	t_list	*redir_wordlist;
	char	**bin_paths;
	t_pid	*child_pids;
}								t_data;

// AST:
t_ast	*create_ast(char **token_start, char **token_end);
// REDIRECTIONS:
int		redirect(char *operator, char *word, t_data *data);
int		redirect_output(char *filename, t_data *data);
int		append_output(char *filename, t_data *data);
int		redirect_input(char *filename, t_data *data);
// HEREDOC:
int		heredoc(char *delimiter, t_data *data);
// EXEC:
int		execute(char *input, t_data *data);
void	update_home(t_data *data, char **argv);
// EXEC_HELPERS:
char	run_builtin(t_data *data);
char	create_argv_argc(t_data *data, t_list *linked_args);
t_list	*clean_args(t_list **args);
int		run_from_bin_path(t_data *data);
// HELPERS:
void	clean_quotations(char *str, char *writer, char in_sgl, char in_dbl);
char	right_parenthesis(char **token_start, char **token_end);
char	is_redirection(char *str);
char	is_operator(char *str);
// DEBUG_FUNCS:
void	print_args(char *str, t_list *linked_args);
void	print_ast(t_ast *root);
// INIT:
int		initialize_data(t_data *data, int argc, char **envp);
void	null_data_struct(t_data *data);
int		restore_stdin_stdout(t_data *data, char option);
void	free_prompt_data(t_data *data);
char	build_ast(t_data *data);
// PARSING:
int		parse_ast(t_ast *node, t_data *data);
// UTILS:
char	is_redirection(char *str);
//SIGNALS:
void	handle_signals(char option);
void	signal_action(int sig);
// SPLIT_CMD
t_list	*get_args(char *str);
// TOKENS:
char	**split_tokens(char *str);
// TOKENS_HELP:
char	write_all_tokens(char *str, char **tokens_out, size_t tokens);
// VAR_EXPAND:
char	expand_variables(t_list *linked_args, t_data *data);
// VAR_HELPERS:
char	replace_vars(t_list *arg, t_vars *vars, char *content_ptr);
void	free_vars(t_vars *vars);
// WILDCARD:
char	add_wildcards(t_data *data, t_list **linked_args);
// PROOF:
char	check_input(char *str, char needs_string);
// WAITING:
int		fill_child_pid(t_pid **child_pids, pid_t id);
int		wait_for_children(t_pid *child_pids);
void	free_waitlist(t_pid **child_pids);

//VARIABLES
// void store_envs(char **envp);
char	**envs_list(char **envp);
char	**update_list(char *variable, char **list_envs);
char	*return_value_env(char *variable, char **list);
char	*return_value_var(char *variable, t_list *head);
char	**delete_env(char *variable, char **list);
void	delete_var(char *var, t_data *data);
char	*search_var(char *variable, t_list *head);
char	*search_env(char *variable, char **list);
void	bubble_sort(char *arr[], int n);
char	**create_list(char **list);
char	*search(char *variable, char **list, t_list *head);

int		expand_list(char **argv, t_list *head);

//LINKED LIST UTILS
void	fill_exportlist(char *arg, t_list *export_list);
void	print_list(t_list *head);
void	sort_list(t_list *head);
void	append_node(t_list **head, const char *data);

//BUILT-INS
int		builtin_export(char **argv, int argc,
			char **list_envs, t_list *export_list);
t_list	*array_to_linkedlist(char *arr[]);
int		builtin_echo(char **argv, int argc);
int		builtin_env(char **list_envs);
int		builtin_unset(t_data *data);
int		builtin_cd(t_data *data);
int		builtin_pwd(void);
int		builtin_exit(char **argv, int argc, t_data *data);

//MEMORY
int		exit_error(t_data *data);

//UTILS
char	*ft_strcat(char *dst, const char *src);
int		with_value(char *string);
int		handle_plus_or_minus(char c);

// ERROR_MEMORY:
void	free_array(char ***array);
void	error_message(char *cmd, char *arg, char *message);
void	free_ast(t_ast **node);
int		ft_free(t_data *data, int exit);
void	spec_error(char *token);

#endif
