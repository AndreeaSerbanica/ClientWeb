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

int already_logged_in(char **cookies, int cookies_count) {
    for (int i = 0; i < cookies_count; i++) {
        if (strstr(cookies[i], "connect.sid") != NULL) {
            printf("User is already logged in\n");
            return 1;
        }
    }
    return 0;
}

int not_logged_in(char **cookies, int cookies_count) {
    for (int i = 0; i < cookies_count; i++) {
        if (strstr(cookies[i], "connect.sid") != NULL) {
            return 0;
        }
    }
    printf("Trebuie sa dai login!\n");
    return 1;
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

void strip_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}