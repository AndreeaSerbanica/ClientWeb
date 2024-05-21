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
#include "commands.h"

int aready_logged_in(char **cookies, int cookies_count) {
    for (int i = 0; i < cookies_count; i++) {
        if (strstr(cookies[i], "connect.sid") != NULL) {
            printf("User is already logged in\n");
            return 1;
        }
    }

    return 0;
}

char *json_with_credentials(char *username, char *password) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    char *json_string = json_serialize_to_string_pretty(root_value);

    json_value_free(root_value);

    return json_string;
}

const char* user_register_login(char *host, int port, char *payload_type, char *access_route) {
    char username[50],  password[50];

    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    printf("username = ");
    scanf("%s", username);
    printf("password = ");
    scanf("%s", password);

    JSON_Value *value = json_value_init_object();
    JSON_Object *json = json_value_get_object(value);
    json_object_set_string(json, "username", username);
    json_object_set_string(json, "password", password);
    char *payload = json_serialize_to_string_pretty(value);

    char* message = compute_post_request(host, access_route, payload_type, payload, NULL, 0, NULL);

    send_to_server(sockfd, message);
    char *response = receive_from_server(sockfd);
    close_connection(sockfd);

    char *cookie;
    char *p = strstr(response, "Set-Cookie: ");
    if (p) {
        cookie = strtok(p + strlen("Set-Cookie: "), "\n");
        return cookie;
    }

    char *json_response = basic_extract_json_response(response);
    if (json_response == NULL) {
        return "You have been successfully registered!";

    } else {
        JSON_Value *val_ret = json_parse_string(json_response);
        JSON_Object *json_ret = json_value_get_object(val_ret);
        return json_object_get_string(json_ret, "error");
    }
}