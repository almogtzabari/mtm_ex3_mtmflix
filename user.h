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
    USER_ALREADY_EXISTS
} UserResult;

typedef struct user_t* User;


User userCreate(const char* username, int age);


User userCopy (User user);
void userDestroy (User user);
int userCompare (User user1, User user2);


char* usernameCopy(User user);
void destroyUsername (char* friend_username);


void removeUsernameFromFriendslist(User user,const char* username);
void seriesRemoveFromFavoriteSeriesLists(User user,
                                         const char *series_name);
UserResult printUserDetailsToFile(User current_user,
                                  FILE* outputStream);

MtmFlixResult userAddSeriesToSeriesList(User user,
                                        const char *seriesName);
int userGetAge (User user);

#endif //MTM_EX3_MTMFLIX_USER_H

