#ifndef PLAYER_H_
# define PLAYER_H_

# include <stdlib.h>
# include "Inventory.h"
# include "CircularBuffer.h"

enum mode
{
	READ,
	WRITE,
	NONE
};

enum DIR
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

typedef struct 		Player
{
	/* Attributes */
	int				fd;
	int				lvl;
	float			time;
	int 		 	x;
	int 			y;
	Inventory		*inventory;
	int				nb_request;
	struct Player 	*next;
	CircularBuffer	*buffer_circular;
	int 			mode;
	int 			dir;
	int 			intro;
	char 			*team_name;
	/* Methods */
	Inventory 		*(*get_inventory)(struct Player *);
	void			(*set_inventory)(struct Player *, Inventory *);
	int 			(*fct_read)(struct Player *, void *);
}					Player;

int 	init_player(struct Player *, int, int, int);
int 	destroy_player(struct Player *);
int 	fct_read(Player *, void *);
void 	copy_player(Player *, Player *);

#endif 	/* PLAYER_H_ */