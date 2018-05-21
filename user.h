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
int userCompare (User user1, User user2);


char* usernameCopy(User user);
void destroyUsername (User user);









SetElement userCopySetElement (SetElement element1){
    return userCopy((User)element1);
}

void userDestroySetElememnt (SetElement element1){
    return userDestroy((User)element1);
}

int userCompareSetElement (SetElement element1, SetElement element2){
    return userCompare((User)element1,(User)element2);
}

ListElement copyFriendUsername (ListElement element){
    return usernameCopy((User)element);
}

void destroyFriendUsername (ListElement element){
    return destroyUsername((User)element);
}
#endif //MTM_EX3_MTMFLIX_USER_H

