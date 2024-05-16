#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "utils.h"
#include "parson.h"

char *access_to_library(char *host, int port, char **cookies, int cookies_count) {
    char *response = NULL;
    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    char *message = compute_get_request(host, "/api/v1/tema/library/access", NULL, cookies, cookies_count, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);


    return basic_extract_json_response(response);

}