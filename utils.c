#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "helpers.h"
#include "parson.h"

int aready_logged_in(char **cookies, int cookies_count) {
    for (int i = 0; i < cookies_count; i++) {
        if (strstr(cookies[i], "connect.sid") != NULL) {
            printf("User is already logged in\n");
            return 1;
        }
    }

    return 0;
}

void give_cookies_space(char **cookies, int n) {
    cookies = (char **)malloc(n * sizeof(char *));
    for (int i = 0; i < n; i++) {
        cookies[i] = (char *)malloc(2 * n * sizeof(char));
    }
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