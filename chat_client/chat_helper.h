#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <netinet/in.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

typedef int bool;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define JAM_DEBUG 0

#define SERVER 0
#define CLIENT 1

#define	CLIENT_ARGS 3

#define MSG_BUFF_SIZE 500
#define SEND_BUFF_SIZE 500
#define RECV_BUFF_SIZE 500
#define HANDLE_SIZE 10
#define SPACE_SIZE 2
#define MSG_SPACE "> "

#define LEAVE_CHAT "client has terminated chat session\0"
#define LEAVE_CHAT_LEN 35
#define LEAVE_CHAT_SERVER "server has terminated chat session\0"

#define MIN_PORT 0
#define MAX_PORT 65535

#define BAD_ARGS 0
#define BAD_PORT 1
#define GET_ADDR_FAIL 2
#define GET_SOCKET_FAIL 3
#define BIND_FAIL 4
#define LISTEN_FAIL 5
#define ACCEPT_FAIL 6
#define SEND_FAIL 7
#define RECV_FAIL 8

// name: setup_server_info()
// purpose: gets information about server for socket setup
//
// server:
// host_name = NULL
// port =      give port to listen to
// ip   =      AI_PASSIVE - IP of host
//
// client:
// host_name = give target server host name
// port =      give target server port
//
// purpose: get server info needed to create socket
// args:
//		int ipv4_or_ipv6: hintServInfo->ai_family
//		int socket_type: hintServInfo->ai_socktype
//		int ip: hintServInfo->ai_flags
//		char* host_name: name of host server is on
//		char* port: string of port # to listen for incoming connections
//		struct addrinfo *hintServInfo: struct for getaddrinfo()
//		struct addrinfo *servInfo: struct to store result of getaddrinfo() 
// precondition(s): memory has been allocated for hintServInfo
// postcondition(s): struct result of getaddrinfo() is returned
struct addrinfo* setup_server_info(
	int ipv4_or_ipv6, int socket_type, int ip,
	char* host_name, char* port,
struct addrinfo *hintServInfo, struct addrinfo *servInfo);

// name: setup_connection()
// purpose: creates socket and calls connect() on that socket
// args:
//		struct addrinfo *servInfo: struct result of getaddrinfo()
//		int servOrClient: is calling process in the server or client program
// precondition(s): servInfo has been setup
// postcondition(s): returns socket fd of connected socket
int setup_connection(struct addrinfo *servInfo,
	int servOrClient);

// name: get_user_name()
// purpose: gets user handle from stdin
// args:
//		char* name_str: string to store name in
//		int max_len: max length of string
// precondition(s): memory has been allocated for name_str
// postcondition(s): name_str contains user set handle
char* get_user_name(char* name_str, int max_len);

// name: get_chars()
// purpose: gets stream of characters from stdin
// args:
//		char* str: string to store characters
//		int max_len: max # of characters to get
// precondition(s): max_len bytes of memory has been allocated for str
// postcondition(s): str contains characters from stdin
// source(s):
// http://stackoverflow.com/questions/7831755/what-is-the-simplest-way-of-getting-user-input-in-c
// http://stackoverflow.com/questions/9457789/clearing-stdin-buffer
char* get_chars(char* str, int max_len);

// name: is_number()
// purpose: checks if string is a number
// args:
//		char* str: string to check
//		int str_len: length of string
// precondition(s): str points to a str
// postcondition(s): returns 1 if string is a number, 0 if not
int is_number(char* str, int str_len);

// name: check_args()
// purpose: checks if command line arguments are legal
// args:
//		int arg_c: argc from main()
//		char** arg_v: argv from main()
// precondition(s): none
// postcondition(s): exits program if bad arguments were passed;
//		otherwise nothing happens
void check_args(int arg_c, char** arg_v);

// name: print_error()
// purpose: prints out error message associated with error code
// args:
//		int error_code: error code
//		char* message: supplementary message to pass (unused)
//		bool quit_prog: boolean for quitting program after printing error msg
// precondition(s): error_code is defined
// postcondition(s): prints error message and quits program
void print_error(int error_code, char* message, bool quit_prog);

// name: exit_prog()
// purpose: exits program
// args: none
// precondition(s): none
// postcondition(s): cleans up before exiting (unused)
void exit_prog();
