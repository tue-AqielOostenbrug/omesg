#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h> 
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TITLE "omsg client\n"
#define INTERRUPT_MSG "\nCaught interrupt\n"
#define DOMAIN AF_INET // IPv4
#define TYPE SOCK_STREAM // TCP
#define PROTOCOL 0 // default
#define PORT 6667 // default
#define IN_LEN 512 // input length
#define OUT_LEN 512 // output length
#define PASSWORD "none"
#define NICKNAME "jim"
#define USERNAME "user0"
#define REALNAME "JIMMY"
#define ADDR "127.0.0.1"

// global variables
bool interrupted = false;
struct sockaddr_in addr;
int sock = 0;
int connection = 0;
char input[IN_LEN];
char output[OUT_LEN];
char password[512];
char nickname[512];
char username[512];
char realname[512];


void handle_interrupt(int n) {
    printf(INTERRUPT_MSG);
    interrupted = true;
    shutdown(
        sock,
        SHUT_WR
    );
    exit(0);
}

void out(char * msg) {
    sprintf(
        msg,
        "%s\r\n",
        msg
    );
    write(
        sock,
        msg,
        strlen(msg)
    );
    printf("%s> %s", NICKNAME, msg);
}

void authenticate(void) {
    printf("Authenticating\n");
    printf(password);
    printf("Please enter your credentials\n");
    printf("password: ");
    scanf(" %s", &password);
    printf("nickname: ");
    scanf(" %s", &nickname);
    printf("username: ");
    scanf(" %s", &username);
    printf("realname: ");
    scanf(" %s", &realname);
    
    char psw_msg[strlen(password) + 5];
    char nck_msg[strlen(nickname) + 5];
    char usr_msg[strlen(username) + strlen(realname)+ 11];
    printf("S1\n");
    sprintf(
        psw_msg,
        "PASS %s",
        password
    );
    out(psw_msg);
    
    sprintf(
        nck_msg,
        "NICK %s",
        nickname
    );
    out(nck_msg);

    sprintf(
        usr_msg,
        "USER %s 0 * :%s",
        username,
        realname
    );
    out(usr_msg);
}

int main(int argc, char const *argv[])
{
    printf(TITLE);
    
    printf("Set up signal handler\n");
    signal(SIGINT, handle_interrupt);

    printf("Initialize socket\n");
    sock = socket(
        DOMAIN,
        TYPE,
        PROTOCOL
    );

    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    addr.sin_family = DOMAIN;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ADDR);

    printf("Connect to server\n");
    connection = connect(
        sock,
        (struct sockaddr *) &addr,
        sizeof(addr)
    );

    if (connection < 0) {
        perror("connection");
        exit(EXIT_FAILURE);
    }
    

    authenticate();

    char code[16];
    char * loc_png;
    while(1) {
        read(
            sock,
            output,
            OUT_LEN
        );
        printf("%s> %s", "server", output);
        loc_png = strstr(output, "PING :");
        if (loc_png != NULL) {
            sprintf(code, "PONG :%s", loc_png + 6, 16);
            out(code);
            loc_png = NULL;
            continue;
        }
    }
}
