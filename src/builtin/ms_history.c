/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_history.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odudniak <odudniak@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 17:11:16 by marboccu          #+#    #+#             */
/*   Updated: 2024/05/16 18:49:26 by odudniak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	ms_history(t_var *mshell)
{
	t_list	*current;
	int		i;

	i = 1;
	current = mshell->history;
	// FIXME Serve?
	//if (!current)
	//{
	//	ft_perror("history: No history\n");
	//	*mshell->status_code = 1;
	//	return (KO);
	//}
	while (current != NULL)
	{
		ft_printf("%d %s\n", i, (char *)current->val);
		current = current->next;
		i++;
	}
	*mshell->status_code = 0;
	return (OK);
}
