#ifndef MTM_EX3_MTMFLIX_USER_H
#define MTM_EX3_MTMFLIX_USER_H

#include "set.h"
#include "series.h"

typedef enum {
    USER_SUCCESS,
    USER_OUT_OF_MEMORY,
    USER_NULL_ARGUMENT,
    USER_ILLEGAL_VALUE,
} UserResult;

typedef struct user_t* User;


User userCreate(const char* username, int age);
User userCopyAux (User user);
void userDestroyAux (User user);
void userDestroySetElement(SetElement element);
int userCompareAux (User user1, User user2);
int userCompareSetElements(SetElement element1, SetElement element2);
SetElement userCopySetElement(SetElement element);
const char* usernameCopy (const char* username);


#endif //MTM_EX3_MTMFLIX_USER_H
