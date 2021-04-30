#include <stdio.h>
#include "libft/libft.h"

int my_lstsize(t_list *lst)
{
	int i;

	i = 0;
	t_list *tmp;
	tmp = lst;
	while(tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}
int main()
{
	t_list *lst;

	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("1")));
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("2")));
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("3")));
	ft_lstadd_back(&lst, ft_lstnew(ft_strdup("4")));
	printf("%i ", my_lstsize(lst));
}
