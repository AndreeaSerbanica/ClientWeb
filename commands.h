#ifndef COMMANDS_H
#define COMMANDS_H

char *access_to_library(char *host, int port, char **cookies, int cookies_count);
char *get_books(char *host, int port, char **cookies, int cookies_count, char *token);

#endif