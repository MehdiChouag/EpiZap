#include "cmd_functions.h"

double		get_min_time(t_cmd *list, double max_timer)
{
	t_cmd	*tmp;
	double	timer;
	int		num_cmd;

	tmp = list;
	if (tmp != NULL)
		num_cmd = tmp->num_cmd;
	timer = max_timer;
	while (tmp)
	{
		if (tmp->num_cmd == num_cmd)
		{
			if (tmp->time < timer)
				timer = tmp->time;
		}
		tmp = tmp->next;
	}
	return (timer);
}

int			set_new_timer(t_cmd **list, Server *s, double timer)
{
	t_cmd	*tmp;
	int		num_cmd;

	tmp = (*list);
	if (tmp)
		num_cmd = tmp->num_cmd;
	while (tmp)
	{
		if (tmp->num_cmd == num_cmd)
		{
			tmp->time -= timer;
			if (tmp->time <= 0.0)
			{
				(*tmp->func)(((void*)(s)), tmp->owner, tmp->cmd);		
				del_cmd_in_list(list, tmp);
				tmp->owner->nb_request--;
			}
		}
		else
			return (TRUE);
		tmp = tmp->next;
	}
	return (TRUE);
}