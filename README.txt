*******************************************************************************
Usage of Server/Client Project 
*******************************************************************************

**********************
function documentation
**********************
function information is in header files

**********************
compilation
**********************
Each program (i.e. server/client) has own makefile in its respective
directory. To compile simply type "make" (without quotation marks)

**********************
execution
**********************
For server side:
chatserve <port #>

For client side:
chatclient <port # used for chatserve socket> <chatserve hostname>

**********************
usage
**********************
1. start the server program
2. server user is prompted for handle name (10 chars max)
3. start the client program
4. client user is prompted for handle name (10 chars max)
5. client can either send message (500 char max), or exit program with \quit
6. server can send response message to client (500 char max), or \quit
7. steps 5-6 repeat until someone quits

