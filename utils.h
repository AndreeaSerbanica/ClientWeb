#ifndef UTILS_H 
#define UTILS_H

//verif if the user is logged in
int already_logged_in(char **cookies, int cookies_count);
int not_logged_in(char **cookies, int cookies_count);
//creates a json string with the credentials
char *json_with_credentials(char *username, char *password);
void strip_newline(char *str);

#endif