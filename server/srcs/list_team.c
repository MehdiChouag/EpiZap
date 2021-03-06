/*
** list_team.c for list_team.c in /home/david_c/B4/systemUnix/psu_2013_zappy/server/srcs
**
** Made by Flavien David
** Login   <david_c@epitech.net>
**
** Started on  sam. juil.  12 18:07:46 2014 Flavien David
** Last update sam. juil.  12 18:07:46 2014 Flavien David
*/

#include "list_team.h"

int		add_elem_in_team(t_team **front_ptr, char *name, int nb)
{
  t_team	*node;
  t_team	*tmp;

  node = malloc(sizeof(t_team));
  if (node == NULL)
    return (FALSE);
  init_team(node, name, nb);
  node->next = NULL;
  tmp = *front_ptr;
  if (tmp == NULL)
    *front_ptr = node;
  else
    {
      while (tmp->next != NULL)
	tmp = tmp->next;
      tmp->next = node;
    }
  return (TRUE);
}

int		del_elem_in_team(t_team **front_ptr, char *name)
{
  t_team	*tmp;

  tmp = *(front_ptr);
  if (tmp && strcmp(name, tmp->name) == 0)
    {
      *front_ptr = tmp->next;
      free(tmp);
      return (TRUE);
    }
  while (tmp)
    {
      if (tmp->next && strcmp(name, tmp->next->name) == 0)
	{
	  tmp->next = tmp->next->next;
	  return (TRUE);
	}
      tmp = tmp->next;
    }
  return (FALSE);
}
