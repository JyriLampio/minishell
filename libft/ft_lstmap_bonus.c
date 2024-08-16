/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 13:23:15 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/28 14:35:01 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*start;
	void	*buf;

	if (!lst || !f || !del)
		return (0);
	start = 0;
	while (lst)
	{
		buf = f(lst->content);
		newlst = ft_lstnew(buf);
		if (!newlst)
		{
			free(buf);
			ft_lstclear(&start, del);
			return (0);
		}
		ft_lstadd_back(&start, newlst);
		lst = lst->next;
	}
	return (start);
}
