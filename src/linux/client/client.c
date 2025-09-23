#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <signal.h> 
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>

#define TITLE "omsg client\n"
#define INTERRUPT_MSG "\nCaught interrupt\n"
#define DOMAIN AF_INET // IPv4
#define TYPE SOCK_STREAM // TCP
#define PROTOCOL 0 // default
#define PORT 6667 // default
#define MAX_LEN 512
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
char input[MAX_LEN];
char output[MAX_LEN];
char password[MAX_LEN];
char nickname[MAX_LEN];
char username[MAX_LEN];
char realname[MAX_LEN];
struct pollfd fds[2];

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
    char out_msg[strlen(msg) + 2];
    snprintf(
        out_msg,
        strlen(msg) + 2,
        "%s\r\n",
        msg
    );
    write(
        sock,
        out_msg,
        strlen(out_msg)
    );
    printf("%s> %s\n", NICKNAME, msg);
}

void out_b(char * msg) {
    char out_msg[strlen(msg) + 2];
    snprintf(
        out_msg,
        strlen(msg) + 2,
        "%s\n",
        msg
    );
    write(
        sock,
        out_msg,
        strlen(out_msg)
    );
    printf("%s> %s", NICKNAME, msg);
    fflush(stdout);
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
    
    char psw_msg[strlen(password) + 6];
    char nck_msg[strlen(nickname) + 6];
    char usr_msg[strlen(username) + strlen(realname)+ 12];
    snprintf(
        psw_msg,
        strlen(password) + 6,
        "PASS %s",
        password
    );
    out(psw_msg);
    
    snprintf(
        nck_msg,
        strlen(nickname) + 6,
        "NICK %s",
        nickname
    );
    out(nck_msg);

    snprintf(
        usr_msg,
        strlen(username) + strlen(realname) + 12,
        "USER %s 0 * :%s",
        username,
        realname
    );
    out(usr_msg);
}

void make_connection(void) {
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
    
    fds[1].fd = sock;
    fds[1].events = POLLIN;
}

void handle_ping(void) {
    char * ping_val = strstr(output, "PING :");
    if (ping_val != NULL) {
        char pong_cmd[MAX_LEN];
        snprintf(pong_cmd, MAX_LEN, "PONG :%s", ping_val + 6);
        out(pong_cmd);
    }
}

void handle_incoming(void) {
    read(
        sock,
        output,
        MAX_LEN
    );

    printf("%s", output);
    handle_ping();
}

bool check_feeds(void) {
    bool test = (poll(fds, 2, 500) > 0);
    // if (!test) printf("test failed - %b\n", test);
    return test;
}

void handle_outgoing(void) {
    char msg_out[MAX_LEN];
    fgets(msg_out, MAX_LEN, stdin);
    out_b(msg_out);
    // fflush(stdout);
}

void setup_terminal(void) {
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
}

int main(int argc, char const *argv[])
{
    printf(TITLE);
    
    printf("Set up signal handler\n");
    signal(SIGINT, handle_interrupt);
    setup_terminal();
    make_connection();
    authenticate();
    
    while(1) {
        // Check if new data in feeds
        if (check_feeds()) {
            // New incoming data
            if(fds[1].revents & POLLIN) {
                handle_incoming();
                continue;
            }
            else  if(fds[0].revents & POLLIN) {
                handle_outgoing();
            }
            else { 
                printf("An unprecedented event has occured\n");
            }
        }
        //printf("\n.");
    }
}
