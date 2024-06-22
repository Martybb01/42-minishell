/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:04:40 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/22 10:54:32 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static bool	export_cleanup(t_var *mshell, t_list *args, char **split)
{
	lst_free(&args, free);
	str_freemtx(split);
	pf_errcode(E_MALLOC);
	cleanup(mshell, true, 1);
	return (false);
}

static void	export_print_everything(t_var *mshell)
{
	t_list	*lst;
	char	*val;
	bool	addquote;

	lst = mshell->env;
	while (lst)
	{
		ft_printf("declare -x %s", (char *)lst->key);
		val = (char *)lst->val;
		addquote = !val || !val[0] || str_includesset(val, "\" '\t\n\v\r*;$@");
		if (!lst->_hidden && addquote)
			ft_printf("='%s'", val);
		else if (!lst->_hidden && !addquote)
			ft_printf("=%s", val);
		ft_printf("\n");
		lst = lst->next;
	}
}

static bool	export_upsert_variable(t_var *mshell, char **split,
	t_list *arg, t_list *variable)
{
	const bool	new_declaration = variable == NULL;

	if (!variable)
		variable = lst_new(split[1], split[0]);
	if (!variable)
		return (false);
	if (!split[1] && new_declaration)
		variable->_hidden = true;
	if (!new_declaration)
		free(split[0]);
	if (!new_declaration && str_idxofstr(arg->val, "+=") != -1)
	{
		variable->val = str_freejoin(variable->val, split[1]);
		variable->_hidden = false;
		free(split[1]);
	}
	else if (!new_declaration && str_idxofchar(arg->val, '=') != -1)
	{
		free(variable->val);
		variable->val = split[1];
	}
	if (new_declaration && !lst_addnode_tail(&mshell->env, variable))
		return (free(split), lst_free(&variable, free), false);
	return (free(split), true);
}

static bool	export_handle_arg(t_var *mshell, t_list *arg, bool *status)
{
	const bool	append = str_idxofstr(arg->val, "+=") != -1;
	t_list		*env_node;
	char		**split;

	if (append)
		split = str_split_firststr(arg->val, "+=");
	else
		split = str_split_first(arg->val, '=');
	if (!split)
		return (KO);
	if (!str_isvariable(split[0]))
	{
		*status = false;
		return (ft_perror("export: `%s`: not a valid identifier\n", split[0]),
			str_freemtx(split), false);
	}
	env_node = lst_findbykey_str(mshell->env, split[0]);
	if (!export_upsert_variable(mshell, split, arg, env_node))
		return (false);
	return (true);
}

t_state	ms_export(t_var *mshell, t_list *args)
{
	t_list			*arg;
	bool			res;

	if (ft_isdebug())
	{
		dbg_printf(CCYAN"[export]\tDEBUG:\n"CR);
		lst_printstr(args);
	}
	res = true;
	if (lst_size(args) == 1)
		return (export_print_everything(mshell), OK);
	arg = args->next;
	while (arg)
	{
		if (!export_handle_arg(mshell, arg, &res))
			return (export_cleanup(mshell, args, NULL), KO);
		arg = arg->next;
	}
	return (res);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_var		mshell;
// 	t_list		*args;

// 	(void)av;
// 	mshell = (t_var){0};
// 	if (ac != 1)
// 		return (pf_errcode(E_INVALID_ARGC), cleanup(&mshell, true, 1), 1);
// 	mshell._main.envp = envp;
// 	ms_init(&mshell);

// 	//args = cmd_parse(mshell, "env bla");
// 	//ms_env(&mshell, args);
// 	//lst_free(&args, free);

// 	//args = cmd_parse(mshell, "export c1= c_ _c");
// 	//ms_export(&mshell, args);
// 	//lst_free(&args, free);

// 	//args = cmd_parse(mshell, "export 1c1= .c_ x55 !_c");
// 	//ms_export(&mshell, args);
// 	//lst_free(&args, free);

// 	//args = cmd_parse(mshell, "export");
// 	//ms_export(&mshell, args);
// 	//lst_free(&args, free);

// 	return (cleanup(&mshell, true, 0));
// }
