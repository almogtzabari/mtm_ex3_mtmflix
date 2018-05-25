#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "user.h"


static void userRemoveFromList(List list, const char *username);
static MtmFlixResult addToList(List list, const char *name);
static bool friendLikedTheSeries(Set users_set, char *friend_name,
                                 char *series_name);
static bool checkIfUserLikedSeries (List favorite_series_list,
                                    char* series_name);

struct user_t{
    const char* username;
    int age;
    List user_friends_list;
    List user_favorite_series;
};

/**
 ***** Function: userCreate *****
 * Description: Creates a new user.
 *
 * @param username - String with the user's name.
 * @param age - Age of the user.
 * @return
 * A pointer to a new user or NULL in case of memory error or illegal age.
 */
User userCreate(const char* username, int age){
    User new_user = malloc(sizeof(*new_user));
    if(!new_user){
        /* User memory allocation failed */
        return NULL;
    }
    char* username_copy=malloc(strlen(username)+1);
    if(!username_copy){
        /* Username memory allocation failed */
        free(new_user);
        return NULL;
    }
    strcpy(username_copy,username);
    new_user->username=(const char*)username_copy;
    new_user->age=age;
    new_user->user_friends_list= listCreate(copyFriendUsername,
            destroyFriendUsername);
    if(!new_user->user_friends_list){
        /* Friend list creation failed */
        free((char*)new_user->username);
        free(new_user);
        return NULL;
    }
    new_user->user_favorite_series= listCreate(copyFavoriteSeriesName,
            destroyFavoriteSeriesName);
    if(!new_user->user_favorite_series){
        /* User favorite series list creation failed */
        free((char*)new_user->username);
        listDestroy(new_user->user_friends_list);
        free(new_user);
        return NULL;
    }
    return new_user;
}


/**
 ***** Function: userCopy *****
 * Description: Copies a given user.
 *
 * @param user - A user to copy.
 * @return
 * A new user or NULL in case of failure.
 */
User userCopy (User user){
    assert(user);
    User new_user=userCreate(user->username,user->age);
    if(!new_user){
        /* User creation failed */
        return NULL;
    }
    new_user->user_friends_list=listCopy(user->user_friends_list);
    if(!new_user->user_friends_list){
        /* List copying failed */
        free((char*)new_user->username);
        return NULL;
    }
    new_user->user_favorite_series=listCopy(user->user_favorite_series);
    if(!new_user->user_favorite_series){
        /* List copying failed */
        free((char*)new_user->username);
        listDestroy(new_user->user_friends_list);
        return NULL;
    }
    return new_user;
}



/**
 ***** Function: userDestroy *****
 * Description: Deallocates an existing user.
 *
 * @param user - A user to destroy.
 */
void userDestroy (User user){
    if(!user){
        return;
    }
    free((char*)user->username);
    listDestroy(user->user_friends_list);
    listDestroy(user->user_favorite_series);
    free(user);
}

/**
  ***** Function: userCompare *****
 * Description: Compares between two user's usernames.
 * @param user1 - First user.
 * @param user2 - Second user.
 *
 * @return
 * A negative integer if user1's name is less than user2's name, 0 if the
 * names are equal and a positive integer if user1's name is bigger.
 *
 */
int userCompare (User user1, User user2){
    assert(user1);
    assert(user2);
    return strcmp(user1->username,user2->username);
}

/**
 ***** Function: usernameCopy *****
 * Description: Copying a friend's name.
 * @param username - A friend's name to copy.
 *
 * @return
 * A copy of the given friend's name.
 */
char* usernameCopy(char* username){
    assert(username);
    char* username_copy=malloc(strlen(username)+1);
    if(!username_copy){
        /* Memory allocation failed */
        return NULL;
    }
    strcpy(username_copy,username);
    return username_copy;
}

/**
 ***** Function: destroyUsername *****
 * Description: Deallocates the memory used for a friend's name.
 * @param user - A friend's name to destroy.
 */
void destroyUsername (char* friend_username){
    free(friend_username);
}


void removeFromList(User user,const char* name,UserList list_type){
    assert(user);
    assert(name);
    if(list_type==FRIENDS_LIST){
        userRemoveFromList(user->user_friends_list,name);
    }
    else{
        userRemoveFromList(user->user_favorite_series,name);
    }
}

static void userRemoveFromList(List list, const char *username){
    LIST_FOREACH(ListElement,iterator,list){
        if(!strcmp((char*)iterator,username)){
            listRemoveCurrent(list);
            break;
        }
    }
}


MtmFlixResult AddToList(User user,const char *name, UserList list_type){
    assert(user);
    assert(name);
    MtmFlixResult result;
    if(list_type==FRIENDS_LIST){
        result=addToList(user->user_friends_list,name);
    }
    else{
        result=addToList(user->user_favorite_series,name);
    }
    return result;
}


static MtmFlixResult addToList(List list, const char *name){
    ListResult result;
    LIST_FOREACH(ListElement,iterator,list) {
        if (!strcmp((char*) iterator, name)) {
            /* The name is already in the list */
            return MTMFLIX_SUCCESS;
        }
    }
    /*If we got here we need to add the name to the list */
    result=listInsertFirst(list,(ListElement)name);
    if(result!=LIST_SUCCESS){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    result=listSort(list,genericStrcmp);
    if(result!=LIST_SUCCESS){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_SUCCESS;
}


/**
 ***** Function: printUserDetailsToFile *****
 * Description: Gets a user and prints its details to a given file.
 * @param current_user - The user which his details will be printed to the
 * file.
 * @param outputStream - A file to print to.
 *
 * @return
 * USER_OUT_OF_MEMORY - In case of memory allocation error.
 * USER_SUCCESS - Printing to file succeeded.
 */
UserResult userPrintDetailsToFile(User current_user,
                                  FILE *outputStream) {
    const char *user_details = mtmPrintUser(current_user->username,
                                            current_user->age,
                                            current_user->user_friends_list
            ,current_user->user_favorite_series);
    if (!user_details) {
        return USER_OUT_OF_MEMORY;
    }
    if(fprintf(outputStream, "%s\n", user_details)<0){
        return USER_OUT_OF_MEMORY;
    }
    return USER_SUCCESS;
}

int userGetAge (User user){
    assert(user);
    return user->age;
}





/**
 ***** Function: howManyFriendsLovedThisSeries *****
 * @param users_set - Set which contains all users in mtmflix.
 * @param user - A user to recommend to.
 * @param series_name - A series name to search on friends lists.
 *
 * @return
 * The number of friends that loved the series.
 */
int howManyFriendsLovedThisSeries(Set users_set, User user,
                                  char *series_name){
    int how_many_loved_this_series=0;
    LIST_FOREACH(ListElement,friend_name,user->user_friends_list){
        /*Checks each friend series list */
        if(friendLikedTheSeries(users_set, (char *) friend_name,
                                series_name)){
            how_many_loved_this_series++;
        }
    }
    return how_many_loved_this_series;
}

/**
 ***** Function: friendLikedTheSeries *****
 * Description: searches for the friend in the users list and checks
 * if he liked the series.
 * @param users_set - Set which contains all users in mtmflix.
 * @param friend_name - a friend's name to look in the users set.
 * @param series_name - A series name to search on this friend's list.
 * @return
 * True if the friend liked the series, else false.
 */
static bool friendLikedTheSeries(Set users_set, char *friend_name,
                                 char *series_name){
    User friend=userCreate(friend_name,MTM_MIN_AGE+1);
    SET_FOREACH(User,current_user,users_set){
        /*Searching for the friend in users set */
        if(userCompare(friend,current_user)==0){
           /*Found the friend in users list */
            if(checkIfUserLikedSeries(current_user->user_favorite_series,
                                      series_name)){
                userDestroy(friend);
                return true;
            }
        }
    }
    /*Should get here */
    userDestroy(friend);
    return false;
}

/**
 ***** Function: checkIfUserLikedSeries *****
 * @param favorite_series_list - Favorite series list of the friend.
 * @param series_name - A series name to search on this friend's list.
 * @return
 * True if the series is found in this friend's favorite series list,
 * else false.
 */
static bool checkIfUserLikedSeries (List favorite_series_list,char* series_name){
    /*Checks if the series exists in friend's favorite series list */
    LIST_FOREACH(ListElement,friend_series,favorite_series_list){
        if(strcmp(series_name,(char*)friend_series)==0){
            return true;
        }
    }
    return false;
}

/**
 ***** Function: userHowManySeriesWithGenre *****
 * Description: Gets a user and a genre and returns the number of series in
 * user's favorite-series-list with the same genre.
 * @param user - The user we want to check his favorite-series-list.
 * @param genre - The genre we are looking for.
 * @param series_set - Set of all the series in the mtmflix.
 * @return
 * If succeeded - Number of series with same genre in user's
 * favorite-series-list.
 * If fails - Will return -1.
 */
int userHowManySeriesWithGenre(Set series_set, User user, Genre genre){
    Genre current_genre;
    int count=0;
    SeriesResult status;
    LIST_FOREACH(ListElement,current_series_name,
                 user->user_favorite_series){
        current_genre = seriesGetGenreByName((char*)current_series_name,
                                             series_set,&status);
        if(status != SERIES_SUCCESS){
            /* There was an error with seriesGetGenreByName. */
            return -1;
        }
        if(current_genre == genre){
            /* Current series has the same genre as the given genre. */
            count++;
        }
    }
    return count;
}

/**
 ***** Function: userGetAverageEpisodeDuration *****
 * Description: Gets a user, a status and a set of all the series in the
 * system. The function returns the average duration of episodes of user's
 * favorite series.
 *
 * @param user - The user we want to check his favorite series for
 * the calulation.
 * @param series_set - Set of all the series in the system.
 * @param function_status - Will hold information of success/failure of the
 * function.
 * @return
 * If succeeded - Average episode duration of all series in user's
 * favorite-series-list.
 * If fails - returns -1.
 */
double userGetAverageEpisodeDuration(User user, Set series_set,
                                     MtmFlixResult* function_status){
    int episode_duration=0;
    int number_of_series=0;
    SeriesResult series_status;
    LIST_FOREACH(ListElement,current_series_name,
                 user->user_favorite_series){
        episode_duration+= seriesGetDurationByName(current_series_name,
                                                   series_set,
                                                   &series_status);
        if(series_status != SERIES_SUCCESS){
            /* Error in seriesGetDurationByName. */
            *function_status = MTMFLIX_OUT_OF_MEMORY;
            return -1; // This value won't be checked.
        }
        number_of_series++;
    }
    *function_status = MTMFLIX_SUCCESS;
    if(number_of_series == 0){
        /* User doesn't have any series in his favorite list. */
        return 0;
    }
    return ((double)episode_duration)/((double)number_of_series);
}

bool isInUsersFavoriteSeriesList(User user,char* series_name){
    LIST_FOREACH(ListElement,series,user->user_favorite_series){
        if(strcmp(series,series_name)==0){
            /*If we get here series is found in user's favorite series
              list*/
            return true;
        }
    }
    return false;
}







