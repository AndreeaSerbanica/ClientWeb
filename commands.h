#ifndef COMMANDS_H
#define COMMANDS_H

char *register_response(char *host, int port, char **cookies, int cookies_count, char *payload_type);
char *login_response(char *host, int port, char **cookies, int cookies_count, char *payload_type);
char *access_to_library(char *host, int port, char **cookies, int cookies_count);
const char *get_books(char *host, int port, char **cookies, int cookies_count, char *token);
char *get_one_book(char *host, int port, char **cookies, int cookies_count, char *token);
const char *add_book(char *host, int port, char **cookies, int cookies_count, char *token);
const char *delete_book(char *host, int port, char **cookies, int cookies_count, char *token);
const char *logout(char *host, int port, char **cookies, int cookies_count, char *token) ;

#endif