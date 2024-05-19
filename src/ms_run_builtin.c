/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_run_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 09:52:54 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/19 18:08:49 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_run_builtin(t_var *mshell, t_list *args)
{
	if (str_cmp(args->val, "export") == 0)
		return (ms_export(mshell, args));
	else if (str_cmp(args->val, "unset") == 0)
		return (ms_unset(mshell, args));
	else if (str_cmp(args->val, "echo") == 0)
		return (ms_echo(mshell, args));
	else if (str_cmp(args->val, "cd") == 0)
		return (ms_cd(mshell, args));
	else if (str_cmp(args->val, "pwd") == 0)
		return (ms_pwd(mshell, args), OK);
	else if (str_cmp(args->val, "env") == 0)
		return (ms_env(mshell, args), OK);
	else if (str_cmp(args->val, "history") == 0)
		return (ms_history(mshell, args), OK);
	else if (str_cmp(args->val, "exit") == 0)
		return (ms_exit(mshell, args));
	else
	{
		if (args)
			ft_perror(PROGNAME": Command not found: %s\n", args->val);
		return (KO);
	}
	return (OK);
}

bool	syntax_check(t_list *args)
{
	const char	*known_tokens[] = {"<", "<<", ">", ">>", "|", NULL};
	int			i;

	while (args)
	{
		if (args->type == A_TOKEN)
		{
			i = 0;
			while (known_tokens[i] && str_cmp(args->val, known_tokens[i]) != 0)
				i++;
			if (i == 5)
				return (false);
			if ((args->prev && args->prev->type == A_TOKEN)
				|| (args->next && args->next->type == A_TOKEN))
				return (false);
			if (!str_cmp("<<", args->val) && !args->next)
				return (false);
		}
		args = args->next;
	}
	return (true);
}

void	parse_and_exec(t_var *mshell, char *input)
{
	t_list	*cmd_list;
	bool	syntax;

	cmd_list = cmd_parse(mshell, input);
	syntax = syntax_check(cmd_list);
	if (!syntax)
		ft_perror("Syntax error\n");
	if (syntax && cmd_list != NULL)
		ms_run_builtin(mshell, cmd_list);
	lst_free(&cmd_list, free);
}
