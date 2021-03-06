/*
** cmd_functions.c for cmd_functions.c in /home/david_c/B4/systemUnix/psu_2013_zappy/server/srcs
**
** Made by Flavien David
** Login   <david_c@epitech.net>
**
** Started on  sam. juil.  12 18:06:37 2014 Flavien David
** Last update sam. juil.  12 18:06:37 2014 Flavien David
*/

#include "cmd_functions.h"

void	player_socket_problem(t_player *, t_server *);
void	do_action(t_cmd **, t_server *, t_cmd *);

double		get_min_time(t_cmd *list, double max_timer)
{
  t_cmd		*tmp;
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

int	set_new_timer(t_cmd **list, t_server *s, double timer)
{
  t_cmd	*tmp;
  int	num_cmd;

  num_cmd = -2;
  tmp = (*list);
  while (tmp)
    {
      (num_cmd == -2 && tmp->num_cmd != -1) ? (num_cmd = tmp->num_cmd) : 0;
      if (tmp->num_cmd == num_cmd || tmp->num_cmd == -1)
	{
	  tmp->time -= timer;
	  if (tmp->time <= 0.0)
	    do_action(list, s, tmp);
	}
      else
	return (TRUE);
      tmp = tmp->next;
    }
  return (TRUE);
}

void	do_action(t_cmd **list, t_server *s, t_cmd *tmp)
{
  if (tmp->func != NULL && tmp->team_name == NULL)
    (*tmp->func)(((void*)(s)), tmp->owner, tmp->cmd[1]);
  else if (tmp->func != NULL && tmp->team_name != NULL)
    {
      (*tmp->func)(((void*)(s)), tmp->owner, tmp->team_name);
      tmp->owner->nb_request++;
    }
  else
    {
      add_str_in_buffer(&tmp->owner->buffer_circular, "ko\n");
      tmp->owner->mode = WRITE;
    }
  del_cmd_in_list(list, tmp);
  tmp->owner->nb_request--;
}

void	del_cmd_of_player(t_cmd **list, t_player *p)
{
  t_cmd	*tmp;

  tmp = (*list);
  while (tmp)
    {
      if (tmp->owner == p && tmp->team_name == NULL)
	del_cmd_in_list(list, tmp);
      tmp = tmp->next;
    }
}

void		update_life(t_player **player, int timer)
{
  t_player	*tmp;

  tmp = (*player);
  while (tmp)
    {
      if (tmp->intro == FALSE)
	{
	  tmp->time -= timer;
	  if (tmp->time <= 0.0)
	    {
	      add_str_in_buffer(&tmp->buffer_circular, "mort\n");
	      tmp->mode = WRITE;
	      tmp->is_alive = FALSE;
	    }
	}
      tmp = tmp->next;
    }
}
