#include "Team.h"

static void		init_func_ptr(Team *);

static int 		get_nb_player_max(Team *);
static void		set_nb_player_max(Team *, int);
static int 		get_nb_player_actu(Team *);
static void		set_nb_player_actu(Team *, int);

int				init_team(Team *this, int player_max)
{
	this->nb_player_max = player_max;
	this->nb_player_actu = 0;
	init_func_ptr(this);
	return (0);
}

int 			destroy_team(Team *this)
{
	(void)this;
	return (0);
}

static void		init_func_ptr(Team *this)
{
	this->get_nb_player_max = &get_nb_player_max;
	this->set_nb_player_max = &set_nb_player_max;
	this->get_nb_player_actu = &get_nb_player_actu;
	this->set_nb_player_actu = &set_nb_player_actu;
}

static int 		get_nb_player_max(Team *this)
{
	return (this->nb_player_max);
}

static void		set_nb_player_max(Team *this, int new_val)
{
	this->nb_player_max = new_val;
}

static int 		get_nb_player_actu(Team *this)
{
	return (this->nb_player_max);
}

static void		set_nb_player_actu(Team *this, int new_val)
{
	this->nb_player_actu = new_val;
}