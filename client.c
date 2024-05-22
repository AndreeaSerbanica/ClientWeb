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
    int sockfd;
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
                const char *json_msg = json_object_get_string(json_ret, "error");
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

            printf("Post message: %s\n", post_message);


            send_to_server(sockfd, post_message);

            char *response = receive_from_server(sockfd);

            // printf("Response: %s\n", response);

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

            if (strstr(cookie, "connect.sid") != NULL) {
                strcpy(cookies[cookies_count++], strtok(strdup(cookie), ";"));
            }
            
            json_free_serialized_string(json_string);
            free(post_message);

            close_connection(sockfd);
            
            free(response);

        } else if (strcmp(command, "enter_library") == 0) {

            char *acces_response = access_to_library(host, port, cookies, cookies_count);
            
            JSON_Value *val_ret = json_parse_string(acces_response);
            JSON_Object *json_ret = json_value_get_object(val_ret);


            if(json_object_get_string(json_ret, "token") == NULL) {
                const char *json_msg = json_object_get_string(json_ret, "error");
                printf("Just a silly Error: %s\n", json_msg);
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
            
        }else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Invalid command\n");
        }
    }

        

    return 0;
}
