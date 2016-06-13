#include"chat_helper.h"

//#include<stdio.h>
//#include<string.h>
//
//#include<sys/types.h>
////#include<sys/socket.h>
////#include<netdb.h>

//#include <netinet/in.h>

// 1. chat serves starts
// 2. waits on port given via command line
// 3. 


int main(int argc, char** argv)
{
	int recv_status, send_status, serv_socket, accepted_socket;
	char *msg_buffer;
	char *message;
	char *user_name;
	char *port_str;
	char *host_str;
	char leave_chat[LEAVE_CHAT_LEN];

	struct addrinfo hintServInfo;
	struct addrinfo* servInfo;

	//const char message[] = "open the bay doors, HAL!!!";
	recv_status = send_status = serv_socket = accepted_socket = 0;
	strcpy(leave_chat, LEAVE_CHAT);
	leave_chat[LEAVE_CHAT_LEN] = '\0';

	servInfo = (struct addrinfo*)malloc(sizeof(struct addrinfo));
	user_name = (char*)malloc(sizeof(char)*(HANDLE_SIZE + 1));
	msg_buffer = (char*)malloc(sizeof(char)*(MSG_BUFF_SIZE + 1));
	message = (char*)malloc(sizeof(char)*(MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1));


	if (argc != 3)
		print_error(BAD_ARGS, NULL, TRUE);

	// CHECK COMMAND LINE ARGS
	check_args(argc, argv);
	port_str = argv[1];
	host_str = argv[2];

	// GET USER NAME
	user_name = get_user_name(user_name, HANDLE_SIZE);

	// GET SERVER INFO
	printf("getting server info...\n");
	servInfo = setup_server_info(
		AF_UNSPEC, SOCK_STREAM, AI_PASSIVE,
		host_str, port_str,
		&hintServInfo, servInfo);

	// SETUP CONNECTION
	printf("setting up connection...\n");
	serv_socket = setup_connection(servInfo, CLIENT);
	printf("connection set up\n");

	while (TRUE)
	{
		// SEND
		while (TRUE)
		{
			printf("enter message to send to server:\n%s> ", user_name);
			msg_buffer = get_chars(msg_buffer, MSG_BUFF_SIZE + 1);
			strcpy(message, user_name);
			strcat(message, MSG_SPACE);
			strcat(message, msg_buffer);

			if (strlen(message) >= MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1)
			{
				if (JAM_DEBUG)
				{
					printf("strlen(message): %i\n", (int)strlen(message));
					printf(" MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1:%i\n",
						(MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE));
				}
				printf("message too long! max message length is 500 characters\n");
			}
			else
				break;
		}


		if (strcmp(msg_buffer, "\\quit") == 0)
		{
			send_status = send(serv_socket, LEAVE_CHAT, LEAVE_CHAT_LEN, 0);
			if (send_status == -1)
				print_error(SEND_FAIL, NULL, TRUE);
			break;
		}
			
		if (JAM_DEBUG)
		{
			printf("message to send: %s\n", message);
			printf("strlen(message): %i\n", (int)strlen(message));
			printf(" MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1:%i\n",
				(MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1));
			printf("sending message to server...\n");
		}

		send_status = send(serv_socket, message, strlen(message) + 1, 0);
		if (send_status == -1)
			print_error(SEND_FAIL, NULL, TRUE);

		if (JAM_DEBUG)
			printf("message sent: %d bytes sent\n", send_status);

		// RECEIVE
		printf("waiting to receive message from server...\n");
		recv_status = recv(serv_socket, message,
			MSG_BUFF_SIZE + HANDLE_SIZE + SPACE_SIZE + 1, 0);
		if (recv_status == -1)
			print_error(RECV_FAIL, NULL, TRUE);

		if (JAM_DEBUG)
		{
			printf("message received: %d bytes received\n", recv_status);
			printf("server message: %s\n", message);
		}
		else
			printf("%s\n", message);

		if (strcmp(message, LEAVE_CHAT_SERVER) == 0)
			break;
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