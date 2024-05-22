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

const char *get_books(char *host, int port, char **cookies, int cookies_count, char *token) {
    char *response = NULL;
    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    char *message = compute_get_request(host, "/api/v1/tema/library/books", NULL, cookies, cookies_count, token);

    // printf("%s\n", message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    // printf("The response is here: %s\n", response);

    if (strstr(response, "[")) {
        return strstr(response, "[");
    } else {
        char *json_ret = basic_extract_json_response(response);
        JSON_Value *ret_val = json_parse_string(json_ret);
        JSON_Object *ret_obj = json_value_get_object(ret_val);
        const char *error_msg = json_object_get_string(ret_obj, "error");
        return error_msg;
    }


    return basic_extract_json_response(response);
}

char *get_one_book(char *host, int port, char **cookies, int cookies_count, char *token) {
    char *response = NULL;
    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    char id[10];

    printf("id = ");
    scanf("%s", id);

    char link[100]  = "/api/v1/tema/library/books/";
    strcat(link, id);

    char *message = compute_get_request(host, link, NULL, cookies, cookies_count, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);


    return basic_extract_json_response(response);
}

const char *add_book(char *host, int port, char **cookies, int cookies_count, char *token) {
    char *response = NULL;

    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    char title[100], author[100], genre[100], publisher[100], page_count[100];

    char newline[2];
    fgets(newline, 2, stdin);


    printf("title = ");
    fgets(title, 100, stdin);
    strip_newline(title);

    printf("author = ");
    fgets(author, 100, stdin);
    strip_newline(author);

    printf("genre = ");
    fgets(genre, 100, stdin);
    strip_newline(genre);

    printf("publisher = ");
    fgets(publisher, 100, stdin);
    strip_newline(publisher);

    printf("page_count = ");
    fgets(page_count, 100, stdin);


    JSON_Value *value = json_value_init_object();
    JSON_Object *json = json_value_get_object(value);

    json_object_set_string(json, "title", title);
    json_object_set_string(json, "author", author);
    json_object_set_string(json, "genre", genre);
    json_object_set_string(json, "publisher", publisher);
    json_object_set_string(json, "page_count", page_count);

    char *payload = json_serialize_to_string_pretty(value);

    char *message = compute_post_request(host, "/api/v1/tema/library/books", "application/json", payload, cookies, cookies_count, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    if (basic_extract_json_response(response) != NULL) {
        JSON_Value *ret_val = json_parse_string(response);
        JSON_Object *ret_obj = json_value_get_object(ret_val);
        const char *error_msg = json_object_get_string(ret_obj, "error");
        return error_msg;
    }

    return "Book added successfully!";
}

const char *delete_book(char *host, int port, char **cookies, int cookies_count, char *token) {
    char *response = NULL;

    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    char id[10];
    printf("id = ");
    scanf("%s", id);

    char link[100] = "/api/v1/tema/library/books/";
    strcat(link, id);

    char *message = compute_delete_request(host, link, "application/json", token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    if (basic_extract_json_response(response) != NULL) {
        JSON_Value *ret_val = json_parse_string(response);
        JSON_Object *ret_obj = json_value_get_object(ret_val);
        const char *error_msg = json_object_get_string(ret_obj, "error");
        return error_msg;
    }
    return "Book deleted successfully!";
}

const char *logout(char *host, int port, char **cookies, int cookies_count, char *token) {
    char *response = NULL;

    int sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("open_connection");
        return NULL;
    }

    char *message = compute_get_request(host, "/api/v1/tema/auth/logout", NULL, cookies, cookies_count, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    close_connection(sockfd);

    if (basic_extract_json_response(response) != NULL) {
        JSON_Value *ret_val = json_parse_string(response);
        JSON_Object *ret_obj = json_value_get_object(ret_val);
        const char *error_msg = json_object_get_string(ret_obj, "error");
        return error_msg;
    }
    return "You have been logged out!";
}