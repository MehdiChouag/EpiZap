#include "Graphic.hpp"
#include "MainUI.hpp"

Graphic::Graphic(MainUI *parent)
{
	char windowid[64];
	sprintf(windowid, "SDL_WINDOWID=0x%llx", winId());
	SDL_putenv(windowid);
	setAttribute(Qt::WA_NoSystemBackground);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setFocusPolicy(Qt::StrongFocus);
	setAttribute(Qt::WA_InputMethodEnabled);
	_viewy = 0;
	_viewx = 0;
	_xhud = 0;
	_yhud = 0;
	_parent = parent;
	_mouseClick = false;
	_realUpdate = false;
	_mouseDrag = false;
	_selectedPlayer = NULL;
	initSDL();
}

Graphic::~Graphic()
{	
	for (int i(0); i != 3; i++)
		SDL_FreeSurface(_bot[(DIR)i]);
	SDL_FreeSurface(_screen);
	Mix_FreeMusic(_music);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

void	Graphic::initSDL()
{
	Lib::xSDL_Init(SDL_INIT_VIDEO | SDL_DOUBLEBUF);
	_screen = Lib::xSDL_SetVideoMode(1408, 704, 32, SDL_HWSURFACE); 
	SDL_WM_SetCaption("Zappy Viewer", NULL);
	Lib::xTTF_Init();
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	_music = Mix_LoadMUS("./music/main.mp3");
	Mix_PlayMusic(_music, -1);
	loader();
}

void Graphic::mousePressEvent(QMouseEvent *e)
{
	_lastPointPress = e->pos();
	_mouseClick = true;
}

void Graphic::mouseReleaseEvent (QMouseEvent *e)
{
	_lastPointReleased = e->pos();
	_mouseReleased = true;
}

void 	Graphic::caseClicked()
{
	QString data;

	if (_lastPointPress.x() > 0 && _lastPointPress.x() < (_map->width * 64)
		&& _lastPointPress.y() > 0 && _lastPointPress.y() < (_map->height * 64))
	{
		data = "Position: ";
		data += QString::number(_lastPointPress.x()/64);
		data += " - ";
		data += QString::number(_lastPointPress.y()/64);
		_parent->addData(data, true);
		data = "Block type: ";
		data += QString::number(_map->map[_lastPointPress.y()/64]
			[_lastPointPress.x()/64].square_type);
		_parent->addData(data, false);
		updateHud(_lastPointPress.x()/64, _lastPointPress.y()/64);
	}
}

void	Graphic::updateHud(const int x, const int y)
{
	_xhud = y;
	_yhud = x;
}

void 	Graphic::mouseMoveEvent(QMouseEvent *e)
{
	_currentPos = e->pos();
}

void 	Graphic::dragMouse()
{
	if ((_lastPointPress.x()/64 - _currentPos.x()/64) != 0 
		|| (_lastPointPress.y()/64 - _currentPos.y()/64) != 0)
	{
		if (((_lastPointPress.x() - _currentPos.x())/64) >= 1)
			_viewy -= 1;
		else if (((_lastPointPress.x() - _currentPos.x())/64) <= -1)
			_viewy += 1;
		if (((_lastPointPress.y() - _currentPos.y())/64) >= 1)
			_viewx -= 1;
		else if (((_lastPointPress.y() - _currentPos.y())/64) <= -1)
			_viewx += 1;
		if (_viewy + FIELD_X >= _map->width)
			_viewy = _map->width - FIELD_X;
		if (_viewx + FIELD_Y >= _map->height)
			_viewx = _map->height - FIELD_Y;
		if (_viewx < 0)
			_viewx = 0;
		if (_viewy < 0)
			_viewy = 0;
	}
}

bool 	Graphic::update()
{
	if (_realUpdate)
	{
		if (_mouseReleased)
		{
			_mouseDrag = false;
			_mouseReleased = false;
		}
		if (_mouseDrag)
			dragMouse();
		if (_mouseClick)
		{
			_mouseDrag = true;
			_mouseClick = false;
			caseClicked();
		}
		draw();
	}
	return (true);
}

void 	Graphic::apply_floor()
{	
	int x = 0;
	int y = 0;

	for (x = 0; x < FIELD_Y && x < _map->height; x++)
	{
		for (y = 0; y < FIELD_X && y < _map->width; y++)
		{
			Lib::applySurface(y * SP_SIZE, x * SP_SIZE, 
				_floor[_map->map[x + _viewx][y + _viewy].square_type], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, LINEMATE))
				Lib::applySurface((y * SP_SIZE) + 15, (x * SP_SIZE) + 10, _ressource[LINEMATE], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, DERAUMERE))
				Lib::applySurface((y * SP_SIZE) + 20, (x * SP_SIZE) + 15, _ressource[DERAUMERE], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, SIBUR))
				Lib::applySurface((y * SP_SIZE) + 30, (x * SP_SIZE) + 25, _ressource[SIBUR], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, MENDIANE))
				Lib::applySurface((y * SP_SIZE) + 40, (x * SP_SIZE) + 10, _ressource[MENDIANE], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, PHIRAS))
				Lib::applySurface((y * SP_SIZE) + 30, (x * SP_SIZE) + 10, _ressource[PHIRAS], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, THYSTAME))
				Lib::applySurface((y * SP_SIZE) + 15, (x * SP_SIZE) + 30, _ressource[THYSTAME], _screen);
			if (_map->map[x + _viewx][y + _viewy].inventory->get_object(_map->map[x + _viewx][y + _viewy].inventory, FOOD))
				Lib::applySurface((y * SP_SIZE) + 15, (x * SP_SIZE) + 45, _ressource[FOOD], _screen);
		}
	}
	displayPlayers();
}

void 	Graphic::addPlayerHud(t_player *temp)
{
	_selectedPlayer = temp;
}

void 	Graphic::updatePlayerHud()
{
	QString data;

	data = "Position: ";
	data += QString::number(_selectedPlayer->x);
	data += " - ";
	data += QString::number(_selectedPlayer->y);
	_parent->addData2(data, true);
	data = "Player fd: ";
	data += QString::number(_selectedPlayer->fd);
	_parent->addData2(data, false);
	data = "Level : ";
	data += QString::number(_selectedPlayer->lvl + 1);
	_parent->addData2(data, false);
	_stuffPlayer[LINEMATE].second = _selectedPlayer->inventory->get_object(_selectedPlayer->inventory, LINEMATE);
	_stuffPlayer[DERAUMERE].second = _selectedPlayer->inventory->get_object(_selectedPlayer->inventory, DERAUMERE);
	_stuffPlayer[SIBUR].second = _selectedPlayer->inventory->get_object(_selectedPlayer->inventory, SIBUR);
	_stuffPlayer[MENDIANE].second = _selectedPlayer->inventory->get_object(_selectedPlayer->inventory, MENDIANE);
	_stuffPlayer[PHIRAS].second = _selectedPlayer->inventory->get_object(_selectedPlayer->inventory, PHIRAS);
	_stuffPlayer[THYSTAME].second = _selectedPlayer->inventory->get_object(_selectedPlayer->inventory, THYSTAME);
	_stuffPlayer[FOOD].second = (int)_selectedPlayer->time;
	for (std::map<obj_type, std::pair<QString, int> >::const_iterator it = _stuffPlayer.begin(); it != _stuffPlayer.end(); ++it)
		_parent->addData2((*it).second.first + QString::number((*it).second.second), false);

}

void 	Graphic::displayPlayers()
{
	t_player 	*temp = _server->player;
	bool 	check;

	check = false;
	while (temp != NULL)
	{
		if (temp->x == _yhud + _viewy && temp->y == _xhud + _viewx)
			addPlayerHud(temp);
		if (temp->x >= 0 && temp->y >= 0 && temp->x >= _viewy && temp->y >= _viewx
			&& temp->x <= (_viewy + FIELD_X) && temp->y <= (_viewx + FIELD_Y))
			Lib::applySurface(((temp->x - _viewy) * SP_SIZE + 10), ((temp->y - _viewx) * SP_SIZE), _bot[(DIR)temp->dir], _screen);
		temp = temp->next;
	}
	temp = _server->player;
	while (temp != NULL)
	{
		if (temp == _selectedPlayer)
			check = true;
		temp = temp->next;
	}
	if (check == false)
		_selectedPlayer = NULL;
	if (_selectedPlayer != NULL)
		updatePlayerHud();
	else
		_parent->addData2(QString(""), true);
}

void 	Graphic::initRealUpdate(const t_server *server)
{
	_map = server->map;
	_server = (t_server *)server;
	_realUpdate = true;
}

void 	Graphic::loopHud()
{
	QString data;

	data = "Position: ";
	data += QString::number(_yhud + _viewy);
	data += " - ";
	data += QString::number(_xhud + _viewx);
	_parent->addData(data, true);
	data = "Block type: ";
	data += QString::number(_map->map[_xhud + _viewx][_yhud + _viewy].square_type);
	_parent->addData(data, false);
	_stuff[LINEMATE].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, LINEMATE);
	_stuff[DERAUMERE].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, DERAUMERE);
	_stuff[SIBUR].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, SIBUR);
	_stuff[MENDIANE].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, MENDIANE);
	_stuff[PHIRAS].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, PHIRAS);
	_stuff[THYSTAME].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, THYSTAME);
	_stuff[FOOD].second = _map->map[_xhud + _viewx][_yhud + _viewy].inventory->get_object(_map->map[_xhud + _viewx][_yhud + _viewy].inventory, FOOD);
	for (std::map<obj_type, std::pair<QString, int> >::const_iterator it = _stuff.begin(); it != _stuff.end(); ++it)
		_parent->addData((*it).second.first + QString::number((*it).second.second), false);
}

void 	Graphic::draw()
{
	if (_realUpdate)
	{
		loopHud();
		apply_floor();
		Lib::xSDL_Flip(_screen);
	}
}

void 	Graphic::loader()
{
	_stuff[LINEMATE] = std::make_pair("<br><img src=\"./server/textures/linemate.png\"/> Linemate : ", 0);
	_stuff[DERAUMERE] = std::make_pair("<img src=\"./server/textures/deraumere.png\"/> Deraumere : ", 0);
	_stuff[SIBUR] = std::make_pair("<img src=\"./server/textures/sibur.png\"/> Sibur : ", 0);
	_stuff[MENDIANE] = std::make_pair("<img src=\"./server/textures/mendiane.png\"/> Mendiane : ", 0);
	_stuff[PHIRAS] = std::make_pair("<img src=\"./server/textures/phiras.png\"/> Phiras : ", 0);
	_stuff[THYSTAME] = std::make_pair("<img src=\"./server/textures/thystame.png\"/> Thystame : ", 0);
	_stuff[FOOD] = std::make_pair("<img src=\"./server/textures/food.png\"/> Food : ", 0);
	_stuffPlayer = _stuff;
	_floor[0] = Lib::loadImage("./server/textures/grass.png");
	_floor[1] = _floor[0];
	_floor[2] = _floor[0];
	_floor[3] = _floor[0];
	_floor[4] = _floor[0];
	_floor[5] = Lib::loadImage("./server/textures/mud.png");
	_floor[6] = Lib::loadImage("./server/textures/mud2.png");
	_floor[7] = Lib::loadImage("./server/textures/mud3.png");
	_ressource[LINEMATE] = Lib::loadImage("./server/textures/linemate.png");
	_ressource[DERAUMERE] = Lib::loadImage("./server/textures/deraumere.png");
	_ressource[SIBUR] = Lib::loadImage("./server/textures/sibur.png");
	_ressource[MENDIANE] = Lib::loadImage("./server/textures/mendiane.png");
	_ressource[PHIRAS] = Lib::loadImage("./server/textures/phiras.png");
	_ressource[THYSTAME] = Lib::loadImage("./server/textures/thystame.png");
	_ressource[FOOD] = Lib::loadImage("./server/textures/food.png");
	_bot[NORTH] = zoomSurface(Lib::loadImage("./server/textures/LinkRunU1.gif"), 2.5, 2.5, 1);
	_bot[SOUTH] = zoomSurface(Lib::loadImage("./server/textures/LinkRunShieldD1.gif"), 2.5, 2.5, 1);
	_bot[EAST] = zoomSurface(Lib::loadImage("./server/textures/LinkRunR1.gif"), 2.5, 2.5, 1);
	_bot[WEST] = zoomSurface(Lib::loadImage("./server/textures/LinkRunShieldL1.gif"), 2.5, 2.5, 1);
}
