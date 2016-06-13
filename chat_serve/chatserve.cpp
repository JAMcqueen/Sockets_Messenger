#include"chat_helper.hpp"

//#include<stdio.h>
//#include<cstring>
//
//#include<sys/types.h>
////#include<sys/socket.h>
////#include<netdb.h>
//
//#include <netinet/in.h>

// 1. chat serves starts
// 2. waits on port given via command line
// 3. 



int main(int argc, char** argv)
{
	int recv_status, send_status;// , serv_socket;// , accepted_socket;
	//char rec_buffer[RECV_BUFF_SIZE + 1];
	char *msg_buffer;
	char *message;
	char *user_name;
	char *port_str;
	char leave_chat[LEAVE_CHAT_LEN];
	

	struct addrinfo hintServInfo;
	struct addrinfo* servInfo;
	struct sockaddr_storage client_addr;
	socklen_t cl_addr_size;

	//const char message[] = "I can't do that, Dave";
	recv_status = send_status = 0;// = serv_socket = 0;// accepted_socket = 0;
	strcpy(leave_chat, LEAVE_CHAT);
	leave_chat[LEAVE_CHAT_LEN] = '\0';

	signal(SIGINT, sig_handler);

	servInfo = (struct addrinfo*)malloc(sizeof(struct addrinfo));
	user_name = (char*)malloc(sizeof(char)*(HANDLE_SIZE + 1));
	msg_buffer = (char*)malloc(sizeof(char)*(MSG_BUFF_SIZE + 1));
	message = (char*)malloc(sizeof(char)*(MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1));

	// CHECK COMMAND LINE ARGS
	check_args(argc, argv);
	port_str = argv[1];

	// GET USER NAME
	user_name = get_user_name(user_name, HANDLE_SIZE);

	// GET SERVER INFO
	printf("getting server info...\n");
	servInfo = setup_server_info(
		AF_UNSPEC, SOCK_STREAM, AI_PASSIVE,
		NULL, port_str,
		&hintServInfo, servInfo);

	// SETUP CONNECTION
	printf("setting up connection...\n");
	serv_socket = setup_connection(servInfo, SERVER);
	printf("connection set up\n");

	// OUTER ACCEPT LOOP
	while (true)
	{
		// ACCEPT CONNECTION
		cl_addr_size = sizeof client_addr;
		printf("waiting to accept incoming connection...\n");
		accepted_socket = accept(serv_socket, (struct sockaddr*)&client_addr,
			&cl_addr_size);
		if (accepted_socket == -1)
			print_error(ACCEPT_FAIL, NULL, true);
		printf("connected to client\n");

		// INNER MESSAGE LOOP
		while (true)
		{
			// RECEIVE
			printf("waiting to receive message from client...\n");
			recv_status = recv(accepted_socket, message,
				MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1, 0);

			if (recv_status == -1)
				print_error(RECV_FAIL, NULL, true);

			if (JAM_DEBUG)
				printf("message received: %d bytes received\n", recv_status);

			if (JAM_DEBUG)
				printf("message from client: %s\n", message);
			else
				printf("%s\n", message);

			if (strcmp(message, LEAVE_CHAT_CLIENT) == 0)
				break;			

			// SEND
			printf("enter message to send to client:\n%s> ", user_name);
			msg_buffer = get_chars(msg_buffer, MSG_BUFF_SIZE + 1);

			strcpy(message, user_name);
			strcat(message, MSG_SPACE);
			strcat(message, msg_buffer);

			if (strcmp(msg_buffer, "\\quit") == 0)
			{
				send_status = send(accepted_socket, LEAVE_CHAT, LEAVE_CHAT_LEN, 0);
				if (send_status == -1)
					print_error(SEND_FAIL, NULL, true);
				break;
			}

			if (JAM_DEBUG)
			{
				printf("message to send: %s\n", message);
				printf("strlen(message): %i\n", (int)strlen(message));
				printf(" MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1:%i\n",
					(MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1));
				printf("sending message to client...\n");
			}

			send_status = send(accepted_socket, message, strlen(message) + 1, 0);
			if (send_status == -1)
				print_error(SEND_FAIL, NULL, true);

			if (JAM_DEBUG)
				printf("message sent: %d bytes sent\n", send_status);
		}

		printf("closing socket\n");
		close(accepted_socket);
	}


	if (JAM_DEBUG)
		printf("freeing servInfo\n");
	freeaddrinfo(servInfo);

	if (JAM_DEBUG)
		printf("freeing user_name\n");
	free(user_name);

	if (JAM_DEBUG)
		printf("freeing msg_buffer\n");
	free(msg_buffer);

	if (JAM_DEBUG)
		printf("freeing message\n");
	free(message);

	return 0;
}