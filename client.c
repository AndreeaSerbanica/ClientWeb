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


int main(int argc, char *argv[])
{
    char *host = "34.246.184.49";
    int port = 8080;
    char *payload_type = "application/json";


    char command[50];
    char **cookies = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++) {
        cookies[i] = (char *)malloc(2 * 10 * sizeof(char));
    }

    int cookies_count = 0;
    char *token = NULL;

    
    while (1) {
        printf("Enter command: ");
        scanf("%s", command);

        if (strcmp(command, "register") == 0) {

            if (already_logged_in(cookies, 0) == 1) {
                continue;
            }
            
            char *response = register_response(host, port, cookies, cookies_count, payload_type);

            char *json_response = basic_extract_json_response(response);
            if (json_response == NULL) {
                printf("Utilizator înregistrat cu succes!\n");
            } else {
                JSON_Value *val_ret = json_parse_string(json_response);
                JSON_Object *json_ret = json_value_get_object(val_ret);
                const char *json_msg = json_object_get_string(json_ret, "error");
                printf("Error: %s\n", json_msg);
            }      


        } else if (strcmp(command, "login") == 0) {
            if (already_logged_in(cookies, cookies_count) == 1) {
                continue;
            }

            char *response = login_response(host, port, cookies, cookies_count, payload_type);

            //save the cookie
            int isCookieReal = 0;
            char *cookie;
            char *p = strstr(response, "Set-Cookie: ");
            if (p) {
                cookie =strtok(p + strlen("Set-Cookie: "), "\n");
                isCookieReal = 1;
                printf("Cookie: %s\n", cookie);
                printf("Utilizatorul a fost logat cu succes\n");
            }

            if (isCookieReal == 0) {
                char *json_response = basic_extract_json_response(response);
                if (json_response == NULL) {
                    printf("Utilizatorul a fost logat cu succes\n");
                } else {
                    JSON_Value *val_ret = json_parse_string(json_response);
                    JSON_Object *json_ret = json_value_get_object(val_ret);
                    const char *json_msg = json_object_get_string(json_ret, "error");
                    printf("Error: %s\n", json_msg);
                }
            }
            
            if(!strstr(response, "error")) {

                if (strstr(cookie, "connect.sid") != NULL) {
                    strcpy(cookies[cookies_count++], strtok(strdup(cookie), ";"));
                }
            }

        } else if (strcmp(command, "enter_library") == 0) {

            char *acces_response = access_to_library(host, port, cookies, cookies_count);
            
            JSON_Value *val_ret = json_parse_string(acces_response);
            JSON_Object *json_ret = json_value_get_object(val_ret);


            if(json_object_get_string(json_ret, "token") == NULL) {
                const char *json_msg = json_object_get_string(json_ret, "error");
                printf("Error: %s\n", json_msg);
            } else {
                token = malloc(1000 * sizeof(char));
                strcpy(token, json_object_get_string(json_ret, "token"));
                printf("Utilizatorul are acces la biblioteca\n");
            }


        } else if (strcmp(command, "get_books") == 0) {

            const char *get_books_msg = get_books(host, port, cookies, cookies_count, token);
            printf("Books: %s\n", get_books_msg);


        } else if (strcmp(command, "get_book") == 0) {
            char *get_book_msg = get_one_book(host, port, cookies, cookies_count, token);

            JSON_Value *val_ret = json_parse_string(get_book_msg);
            JSON_Object *json_ret = json_value_get_object(val_ret);

            if (json_object_get_string(json_ret, "error") != NULL) {
                const char *json_msg = json_object_get_string(json_ret, "error");
                printf("Error: %s\n", json_msg);
            } else {
                printf("Book: %s\n", get_book_msg);
            }


        } else if (strcmp(command, "add_book") == 0) {

            const char *add_book_msg = add_book(host, port, cookies, cookies_count, token);
            printf("%s\n", add_book_msg);
            
        } else if (strcmp(command, "delete_book") == 0) {

            const char *delete_book_msg = delete_book(host, port, cookies, cookies_count, token);
            printf("%s\n", delete_book_msg);
            
        } else if (strcmp(command, "logout") == 0) {

            if (not_logged_in(cookies, cookies_count)) {
                continue;
            }

            const char *logout_msg = logout(host, port, cookies, cookies_count, token);
            printf("%s\n", logout_msg);

            //delete the cookie
            cookies_count = 0;

        } else if (strcmp(command, "exit") == 0) {
            break;

        } else {
            printf("Invalid command\n");
        }
    }

        

    return 0;
}
