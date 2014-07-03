#include 	"Player.h"
#include 	"xfunction.h"

static void			init_func_ptr(Player *);
static void			set_inventory(Player *, Inventory *);
static Inventory 	*get_inventory(Player *);

int		init_player(Player *this, int fd, int width, int height)
{
	this->fd = fd;
	this->nb_request = 0;
	this->x = rand() % width;
	this->y = rand() % height;
	this->time = 0;
	this->lvl = 0;
	this->mode = WRITE;
	this->intro = TRUE;
	this->buffer_circular = NULL;
	if (!(this->inventory = xmalloc(sizeof(Inventory))))
		return (-1);
	init_inventory(this->inventory, NULL, 0);
	init_func_ptr(this);
	if (!(this->buffer_circular = xmalloc(sizeof(CircularBuffer))))
		return (-1);
	if (create_circular_buffer(&this->buffer_circular) == FALSE)
		return (-1);
	add_str_in_buffer(&this->buffer_circular, "BIENVENUE\n");
	display_circular_buffer(this->buffer_circular, 1);
	return (0);
}

int 	destroy_player(Player *this)
{
	free(this->inventory);
	//clear_circular_buffer(&this->buffer_circular);
	return (0);
}

static void		init_func_ptr(Player *this)
{
	this->get_inventory = &get_inventory;
	this->set_inventory = &set_inventory;
	this->fct_read = &fct_read;
}

static void		set_inventory(Player *this, Inventory *i)
{
	this->inventory = i;
}

static Inventory 	*get_inventory(Player *this)
{
	return (this->inventory);
}