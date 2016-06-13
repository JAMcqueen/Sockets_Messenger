#include"chat_helper.h"

struct addrinfo* setup_server_info(
	int ipv4_or_ipv6, int socket_type, int ip,
	char* host_name, char* port,
struct addrinfo *hintServInfo, struct addrinfo *servInfo)
{
	int status = 0;

	memset(hintServInfo, 0, sizeof *hintServInfo);
	hintServInfo->ai_family = ipv4_or_ipv6;     // AF_INET / AF_INET6 / AF_UNSPEC
	hintServInfo->ai_socktype = socket_type;    // SOCK_STREAM / SOCK_DGRAM
	hintServInfo->ai_flags = ip;                // fill in IP automatically

	status = getaddrinfo(host_name, port, hintServInfo, &servInfo);

	if (status != 0)
		print_error(GET_ADDR_FAIL, NULL, TRUE);
	
	return servInfo;
}

int setup_connection(struct addrinfo *servInfo, int servOrClient)
{
	int socket_fd, bind_status, connect_status, listen_status;
	socket_fd = bind_status = connect_status = listen_status = 0;


	socket_fd = socket(servInfo->ai_family,
		servInfo->ai_socktype, servInfo->ai_protocol);
	if (socket_fd == -1)
		print_error(GET_SOCKET_FAIL, NULL, TRUE);

	if (servOrClient == SERVER)
	{
		bind_status = bind(socket_fd, servInfo->ai_addr, servInfo->ai_addrlen);
		if (bind_status == -1)
			print_error(BIND_FAIL, NULL, TRUE);

		listen_status = listen(socket_fd, 10);
		if (listen_status == -1)
			print_error(LISTEN_FAIL, NULL, TRUE);
	}

	if (servOrClient == CLIENT)
	{
		connect_status = connect(socket_fd, servInfo->ai_addr,
			servInfo->ai_addrlen);
	}
	
	return socket_fd;
}

char* get_user_name(char* name_str, int max_len)
{
	printf("Please enter your handle name (up to %i characters):\n", max_len);
	name_str = get_chars(name_str, max_len);
	return name_str;
}

char* get_chars(char* str, int max_len)
{
	int i = 0;
	char c = 'x';

	while (TRUE)
	{
		c = getchar();
		if (c == '\n')
		{
			if (i == 0)
			{
				printf("no input entered; please type up to %i characters:\n", max_len);
				continue;
			}
			else
				break;
		}
		str[i] = c;
		i++;
		if (i == max_len)
			break;
	}

	str[i] = '\0';
	if (i >= max_len)
	{
		while ((c = getchar()) != '\n' && c != EOF);
	}
	return str;
}

int is_number(char* str, int str_len)
{
	int i = 0;
	for (i = 0; i < str_len; i++)
	{
		if (!isdigit(str[i]))
			return 0;
	}
	return 1;
}

void check_args(int arg_c, char** arg_v)
{
	int port_num;

	if (arg_c != CLIENT_ARGS)
	{
		print_error(BAD_ARGS, NULL, TRUE);
	}

	if (!is_number(arg_v[1], strlen(arg_v[1])))
	{
		print_error(BAD_PORT, NULL, TRUE);
	}

	port_num = strtol(arg_v[1], NULL, 10);
	if (port_num > MAX_PORT || port_num < MIN_PORT)
	{
		print_error(BAD_PORT, NULL, TRUE);
	}
}

void print_error(int error_code, char* message, bool quit_prog)
{
	switch (error_code)
	{
	case BAD_ARGS:
		printf("error: incorrect arguments format\n");
		break;
	case BAD_PORT:
		printf("error: invalid port number\n");
		break;
	case GET_ADDR_FAIL:
		printf("error: call to getaddrinfo() failed\n");
		break;
	case BIND_FAIL:
		printf("error: call to bind() failed\n");
		break;
	case LISTEN_FAIL:
		printf("error: call to listen() failed\n");
		break;
	case ACCEPT_FAIL:
		printf("error: call to accept() failed\n");
		break;
	case SEND_FAIL:
		printf("error: call to send() failed\n");
		break;
	case RECV_FAIL:
		printf("error: call to recv() failed\n");
		break;
	}

	if (quit_prog)
		exit_prog();
}

void exit_prog()
{
	printf("exiting program...\n");
	exit(1);
}
