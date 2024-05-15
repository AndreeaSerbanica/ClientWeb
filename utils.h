#ifndef UTILS_H 
#define UTILS_H

//verif if the user is logged in
int aready_logged_in(char **cookies, int cookies_count);
//allocates memory for cookies
void give_cookies_space(char **cookies, int n);
//creates a json string with the credentials
char *json_with_credentials(char *username, char *password);

#endif