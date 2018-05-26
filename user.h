#ifndef MTM_EX3_MTMFLIX_USER_H
#define MTM_EX3_MTMFLIX_USER_H

#include "set.h"
#include "series.h"
#include "utilities.h"

typedef enum {
    FRIENDS_LIST,
    FAVORITE_SERIES_LIST
} UserList;

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


char* usernameCopy(char* user);
void destroyUsername (char* friend_username);


UserResult userPrintDetailsToFile(User current_user,
                                  FILE *outputStream);


int userGetAge (User user);
int howManyFriendsLovedThisSeries(Set users_set, User user,
                                  char *series_name);
int userHowManySeriesWithGenre(Set series_set, User user, Genre genre);
double userGetAverageEpisodeDuration(User user, Set series_set,
                                     MtmFlixResult* function_status);

bool isInUsersFavoriteSeriesList(User user,char* series_name);
void removeFromList(User user,char* name,UserList list_type);
MtmFlixResult AddToList(User user,char *name,UserList list_type);

#endif //MTM_EX3_MTMFLIX_USER_H

