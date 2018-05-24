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


UserResult userPrintDetailsToFile(User current_user,
                                  FILE *outputStream);


int userGetAge (User user);
int howManyFriendsLovedThisShow(Set users_set,User user,char* series_name);
int userHowManySeriesWithGenre(Set series_set, User user, Genre genre);
double userGetAverageEpisodeDuration(User user, Set series_set,
                                     UserResult* function_status);

MtmFlixResult userAddFavoriteSeries(User user,
                                    const char *seriesName);
void userRemoveFriend(User user, const char *username);
void userRemoveFavoriteSeries(User user,
                              const char *series_name);

void removeFromList(User user,const char* name,int list_type);
MtmFlixResult userAddToList(User user,const char* name,int list_type);
#endif //MTM_EX3_MTMFLIX_USER_H

