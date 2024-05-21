#ifndef UTILS_H 
#define UTILS_H

//verif if the user is logged in
int aready_logged_in(char **cookies, int cookies_count);
//creates a json string with the credentials
char *json_with_credentials(char *username, char *password);

const char* user_register_login(char *host, int port, char *payload_type, char *access_route);

#endif