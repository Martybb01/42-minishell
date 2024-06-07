/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marboccu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/06/07 17:46:41 by marboccu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_run_builtin(t_var *mshell, t_command *command)
{
	t_list	*args;

	args = command->args;
	if (str_equals(args->val, "export"))
		return (ms_export(mshell, args));
	else if (str_equals(args->val, "unset"))
		return (ms_unset(mshell, args));
	else if (str_equals(args->val, "echo"))
		return (ms_echo(mshell, args));
	else if (str_equals(args->val, "cd"))
		return (ms_cd(mshell, args));
	else if (str_equals(args->val, "pwd"))
		return (ms_pwd(mshell, args), OK);
	else if (str_equals(args->val, "env"))
		return (ms_env(mshell, args), OK);
	else if (str_equals(args->val, "history"))
		return (ms_history(mshell, args), OK);
	else if (str_equals(args->val, "exit"))
		return (ms_exit(mshell, args));
	return (KO);
}

/// TODO refactor: need to handle the redirects/heredocs
static int	ms_exec_command(t_var *mshell, t_command *command, bool tofork)
{
	pid_t	pid;


	ms_heredoc(mshell, command);
	//ms_rediout(mshell, command);
	if (ms_is_builtin(command->args->val) && !tofork)
		ms_run_builtin(mshell, command);
	else if (ms_is_builtin(command->args->val) && tofork)
	{
		pid = fork();
		if (pid < 0)
			return (pf_errcode(ERR_FORK), KO);
		else if (!pid)
		{
			ms_run_builtin(mshell, command);
			cleanup(mshell, true, *mshell->status_code);
		}
	}
	else
		ms_exec_cmd(mshell, command->args);
	return (OK);
}

static void	ms_wrap_cleanup(t_var *mshell, t_list *cmds_without_command)
{
	t_list	*next;
	t_list	*cmd;

	if (cmds_without_command->prev)
		cmds_without_command->prev->next = NULL;
	freeallcmds(mshell->all_cmds, true);
	while (cmds_without_command)
	{
		cmd = cmds_without_command->val;
		next = cmds_without_command->next;
		lst_free(&cmd, free);
		free(cmds_without_command);
		cmds_without_command = next->prev;
	}
}

static bool	ms_wrap_commands(t_var *mshell)
{
	t_list		*cmds;
	t_command	*cmd_container;

	cmds = mshell->all_cmds;
	while (cmds)
	{
		cmd_container = ft_calloc(1, sizeof(t_command));
		if (!cmd_container)
			return (ms_wrap_cleanup(mshell, cmds), false);
		cmd_container->args = cmds->val;
		cmds->val = cmd_container;
		cmds = cmds->next;
	}
	return (true);
}

void	*ms_exec_commands(t_var *mshell, t_list *all)
{
	t_list		*cmds_list;
	int			size;
	t_command	*command;

	mshell->all_cmds = lst_split_bystrval(all, "|");
	if (!ms_wrap_commands(mshell) || !mshell->all_cmds)
		return (NULL);
	cmds_list = mshell->all_cmds;
	size = lst_size(cmds_list);
	dbg_printf(CCYAN"TOTAL COMMANDS TO EXECUTE: %d\n"CR, size);
	while (cmds_list)
	{
		command = cmds_list->val;
		ms_exec_command(mshell, command, size > 1);
		cmds_list = cmds_list->next;
	}
	return (freeallcmds(mshell->all_cmds, true), NULL);
}
