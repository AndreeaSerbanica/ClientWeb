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
    char command[50];
    int sockfd;
    char **cookies = NULL;

    char *host = "34.246.184.49";
    int port = 8080;
    char *payload_type = "application/json";

    while (1) {
        printf("Enter command: ");
        scanf("%s", command);

        if (strcmp(command, "register") == 0) {
            char username[LINELEN], password[LINELEN];

            sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                perror("open_connection");
                return 1;
            }

            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);

            JSON_Value *root_value = json_value_init_object();
            JSON_Object *root_object = json_value_get_object(root_value);
            json_object_set_string(root_object, "username", username);
            json_object_set_string(root_object, "password", password);
            char *json_string = json_serialize_to_string_pretty(root_value);


            char *post_message = compute_post_request(host, "/register", payload_type, json_string, NULL, 0);
            send_to_server(sockfd, post_message);
            free(post_message);
            

            char *response = receive_from_server(sockfd);
            // printf("here\n");
            printf("Response:\n%s\n", response);

            json_free_serialized_string(json_string);
            json_value_free(root_value);

            close_connection(sockfd);
            free(response);
        }
    }

        

    return 0;
}
