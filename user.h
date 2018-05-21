#ifndef MTM_EX3_MTMFLIX_USER_H
#define MTM_EX3_MTMFLIX_USER_H

#include "set.h"
#include "series.h"
#include "utilities.h"

typedef enum {
    USER_SUCCESS,
    USER_OUT_OF_MEMORY,
    USER_NULL_ARGUMENT,
    USER_ILLEGAL_VALUE,
} UserResult;

typedef struct user_t* User;


User userCreate(const char* username, int age);


User userCopy (User user);
void userDestroy (User user);
int userCompare (char* username1, char* username2);


char* usernameCopy(User user);
void destroyUsername (char* friend_username);


#endif //MTM_EX3_MTMFLIX_USER_H

