#include	"Server.h"
#include	"List.h"

int 	init_bits_fields(Server *this, fd_set *readfds, fd_set *writefds)
{
	FD_ZERO(readfds);
	FD_ZERO(writefds);
	FD_SET(this->socket, readfds);
	init_fd(this->player, readfds);
	return (0);
}

int						accept_socket(Server *s)
{
	struct sockaddr_in  client_sin; 
	socklen_t 			len;
	int 				fd;

	len = sizeof(client_sin);
	if ((fd = xaccept(s->socket, &(client_sin), &len)) == FALSE)
		return (-1);
	if (add_elem(&s->player, fd) != 0)
		return (-1);
	else
		sprintf(s->msg, "<font color=\"Green\">*** CLIENT ADD IN LIST ***</font><br />");
	if (s->max_fd < fd)
		s->max_fd = fd;
		s->n_client++;
	sprintf(s->msg, "%s<font color=\"Green\">*** NEW CONNECTION FROM IP %s ON PORT %d AND FD %d ***</font>", s->msg, inet_ntoa(client_sin.sin_addr), s->port, fd);
	if (write(fd, "--- SUCCESSLY CONNECT ---\n", 26) <= 0)
		return (-1);
	return (0);
}
