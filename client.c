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

int main(int argc, char *argv[])
{
    char *host = "34.246.184.49";
    int port = 8080;
    char *payload_type = "application/json";


    char command[50];
    int sockfd;
    char **cookies = NULL;
    int cookies_count = 0;

    give_cookies_space(cookies, 10);

    while (1) {
        printf("Enter command: ");
        scanf("%s", command);

        if (strcmp(command, "register") == 0) {
            char username[LINELEN], password[LINELEN];

            if (aready_logged_in(cookies, 0) == 1) {
                continue;
            }

            sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                perror("open_connection");
                return 1;
            }

            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);

            char *json_string = json_with_credentials(username, password);


            char *post_message = compute_post_request(host, "/api/v1/tema/auth/register", payload_type, json_string, NULL, 0, NULL);


            send_to_server(sockfd, post_message);
            free(post_message);


            char *response = receive_from_server(sockfd);

            char *json_response = basic_extract_json_response(response);
            if (json_response == NULL) {
                printf("Utilizator înregistrat cu succes!\n");
            } else {
                JSON_Value *val_ret = json_parse_string(json_response);
                JSON_Object *json_ret = json_value_get_object(val_ret);
                char *json_msg = json_object_get_string(json_ret, "error");
                printf("Error: %s\n", json_msg);
            }

            json_free_serialized_string(json_string);
            close_connection(sockfd);
            free(response);


        } else if (strcmp(command, "login") == 0) {
            char username[LINELEN], password[LINELEN];

            if (aready_logged_in(cookies, 0) == 1) {
                continue;
            }

            sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                perror("open_connection");
                return 1;
            }

            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);

        
            char *json_string = json_with_credentials(username, password);

            char *post_message = compute_post_request(host, "/api/v1/tema/auth/login", payload_type, json_string, NULL, 0, NULL);


            send_to_server(sockfd, post_message);

            char *response = receive_from_server(sockfd);
             char *json_response = basic_extract_json_response(response);
            if (json_response == NULL) {
                printf("Utilizatorul a fost logat cu succes\n");
            } else {
                JSON_Value *val_ret = json_parse_string(json_response);
                JSON_Object *json_ret = json_value_get_object(val_ret);
                char *json_msg = json_object_get_string(json_ret, "error");
                printf("Error: %s\n", json_msg);
            }

            json_free_serialized_string(json_string);
            free(post_message);

            close_connection(sockfd);
            
            free(response);

        } else if (strcmp(command, "exit") == 0) {
            printf("Inchidere program\n");
            break;
        } else {
            printf("Invalid command\n");
        }
    }

        

    return 0;
}
