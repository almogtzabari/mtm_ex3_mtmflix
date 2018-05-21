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


SetElement userCopy (SetElement user_to_copy);
void userDestroy (SetElement user_to_destroy);
int userCompare (SetElement element1, SetElement element2);


ListElement copyFriendName (ListElement friend_name);
void destroyFriendName (ListElement friend_name);


#endif //MTM_EX3_MTMFLIX_USER_H

