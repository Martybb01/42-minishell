/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_expand_clear.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 14:24:26 by odudniak          #+#    #+#             */
/*   Updated: 2024/06/22 21:02:14 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

t_list	*args_clearquotes(t_var *mshell, t_list *args)
{
	t_list	*arg;

	arg = args;
	while (arg)
	{
		dbg_printf(CCYAN"[expand_clear]\t|%s|:\n"CR, arg->val);
		if (arg->val)
		{
			if (!str_clearquotes((char **)&arg->val))
				return (lst_free(&args, free), cleanup(mshell, true, KO), NULL);
		}
		dbg_printf(CCYAN"\tresult|%s|\n"CR, arg->val);
		arg = arg->next;
	}
	return (args);
}
