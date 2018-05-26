#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "user.h"



//-----------------------------------------------------------------------//
//                USER: STATIC FUNCTIONS DECLARATIONS                    //
//-----------------------------------------------------------------------//

static void userRemoveFromList(List list,char *name);

static MtmFlixResult addNameToAList(List list, char *name);

static bool friendLikedTheSeries(Set users_set, char *friend_name,
                                 char *series_name);

static bool checkIfUserLikedSeries (List favorite_series_list,
                                    char* series_name);

//-----------------------------------------------------------------------//
//                        USER: STRUCT                                   //
//-----------------------------------------------------------------------//

struct user_t{
    char* username;
    int age;
    List user_friends_list;
    List user_favorite_series;
};


//-----------------------------------------------------------------------//
//                       USER: FUNCTIONS                                 //
//-----------------------------------------------------------------------//

/** Rows: 25
 ***** Function: userCreate *****
 * Description: Creates a new user.
 *
 * @param username - String with the user's name.
 * @param age - Age of the user.
 *
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
        userDestroy(new_user);
        return NULL;
    }
    strcpy(username_copy,username);
    List friend_list=listCreate(copyFriendUsername,destroyFriendUsername);
    if(!friend_list){
        /* Friend list creation failed */
        free(username_copy);
        userDestroy(new_user);
        return NULL;
    }
    List favorite_series_list = listCreate(copyFavoriteSeriesName,
                                           destroyFavoriteSeriesName);
    if(!favorite_series_list){
        /* User favorite series list creation failed */
        free(username_copy);
        listDestroy(friend_list);
        userDestroy(new_user);
        return NULL;
    }
    new_user->username=username_copy;
    new_user->age=age;
    new_user->user_friends_list=friend_list;
    new_user->user_favorite_series=favorite_series_list;

    return new_user;
}


/** Rows: 15
 ***** Function: userCopy *****
 * Description: Copies a given user.
 *
 * @param user - A user to copy.
 *
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
    /* User create has created an extra of the lists.*/
    listDestroy(new_user->user_favorite_series);
    listDestroy(new_user->user_friends_list);
    new_user->user_friends_list=listCopy(user->user_friends_list);
    if(!new_user->user_friends_list){
        /* List copying failed */
        free(new_user->username);
        return NULL;
    }
    new_user->user_favorite_series=listCopy(user->user_favorite_series);
    if(!new_user->user_favorite_series){
        /* List copying failed */
        free(new_user->username);
        listDestroy(new_user->user_friends_list);
        return NULL;
    }
    return new_user;
}

/** Rows: 6
 ***** Function: userDestroy *****
 * Description: Deallocates an existing user.
 *
 * @param user - A user to destroy.
 */
void userDestroy (User user){
    if(!user){
        return;
    }
    free((user->username));
    listDestroy(user->user_friends_list);
    listDestroy(user->user_favorite_series);
    free(user);
}

/** Rows: 1
  ***** Function: userCompare *****
 * Description: Compares between two user's usernames.
 *
 * @param user1 - First user.
 * @param user2 - Second user.
 *
 * @return
 * Positive Integer - User1's name is bigger than user2's name.
 * Negative Integer - User2's name is bigger than user1's name.
 * Zero - Both names are equal.
 */
int userCompare (User user1, User user2){
    assert(user1);
    assert(user2);
    return strcmp(user1->username,user2->username);
}

/** Rows: 5
 ***** Function: usernameCopy *****
 * Description: Copying a friend's name.
 *
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

/** Rows: 1
 ***** Function: destroyUsername *****
 * Description: Deallocates the memory used for a friend's name.
 *
 * @param user - A friend's name to destroy.
 */
void destroyUsername (char* friend_username){
    free(friend_username);
}

/** Rows: 3
 ***** Function: removeFromList *****
 * Description: Removes the given name from the specified list of given
 * user.
 *
 * @param user - User we want to remove from one of his list.
 * @param name - Name to remove from list.
 * @param list_type - Which of given user's lists to remove from.
 */
void removeFromList(User user,char* name,UserList list_type){
    assert(user);
    assert(name);
    if(list_type==FRIENDS_LIST){
        userRemoveFromList(user->user_friends_list,name);
    }
    else{
        userRemoveFromList(user->user_favorite_series,name);
    }
}

/** Rows: 5
 ***** Function: addNameToUsersList *****
 * Description: Adds a given name to a specified list of a given user.
 *
 * @param user - User we want to add to one of his lists.
 * @param name - Name we want to add to list.
 * @param list_type - Which of given user's lists to add to.
 * @return
 */
MtmFlixResult addNameToUsersList(User user,char *name,UserList list_type){
    assert(user);
    assert(name);
    MtmFlixResult result;
    if(list_type==FRIENDS_LIST){
        result= addNameToAList(user->user_friends_list, name);
    }
    else{
        result= addNameToAList(user->user_favorite_series, name);
    }
    return result;
}


/** Rows: 8
 ***** Function: userPrintDetailsToFile *****
 * Description: Gets a user and prints its details to a given file.
 *
 * @param current_user - The user which his details will be printed to the
 * file.
 * @param outputStream - A file to print to.
 *
 * @return
 * USER_OUT_OF_MEMORY - Any memory error.
 * USER_SUCCESS - Successfully printed.
 */
UserResult userPrintDetailsToFile(User current_user, FILE *outputStream) {

    const char *user_details = mtmPrintUser(current_user->username,
                       current_user->age, current_user->user_friends_list
                                     ,current_user->user_favorite_series);
    if (!user_details) {
        return USER_OUT_OF_MEMORY;
    }
    if(fprintf(outputStream, "%s", user_details)<0){
        return USER_OUT_OF_MEMORY;
    }
    return USER_SUCCESS;
}

/** Rows: 1
 ***** Function: userGetAge *****
 * Description: Returns the age of given user.
 *
 * @param user - User to get its age.
 *
 * @return
 * Age of given user.
 */
int userGetAge (User user){
    assert(user);
    return user->age;
}

/** Rows: 5
 ***** Function: howManyFriendsLovedThisSeries *****
 * Description: Returns how many friends of a given user loves a given
 * series.
 *
 * @param users_set - Set which contains all users in mtmflix.
 * @param user - A user to check with his friends.
 * @param series_name - A series name to search on friends' favorite lists.
 *
 * @return
 * The number of friends that loved the given series.
 */
int howManyFriendsLovedThisSeries(Set users_set, User user,
                                                        char *series_name){
    int how_many_loved_this_series=0;
    LIST_FOREACH(ListElement,friend_name,user->user_friends_list){
        /*Checks each friend series list */
        if(friendLikedTheSeries(users_set,(char*)friend_name,series_name)){
            how_many_loved_this_series++;
        }
    }
    return how_many_loved_this_series;
}

/** Rows: 12
 ***** Function: userHowManySeriesWithGenre *****
 * Description: Returns the number of series in user's favorite-series-list
 * with the same genre as the given genre.
 *
 * @param user - The user we want to check his favorite-series-list.
 * @param genre - The genre we are looking for.
 * @param series_set - Set of all the series in the mtmflix.
 *
 * @return
 * If succeeded - Number of series with same genre in user's
 * favorite-series-list.
 * If fails - Will return ILLEGAL_VALUE.
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
            return ILLEGAL_VALUE;
        }
        if(current_genre == genre){
            /* Current series has the same genre as the given genre. */
            count++;
        }
    }
    return count;
}

/** Rows: 16
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
 *
 * @return
 * If succeeded - Average episode duration of all series in user's
 * favorite-series-list.
 * If fails - returns ILLEGAL_VALUE.
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
            return ILLEGAL_VALUE; // This value won't be checked.
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

/** Rows: 4
 ***** Function: isInUsersFavoriteSeriesList *****
 * Description: Returns whether or not a given series name is in given
 * user's favorite series list.
 *
 * @param user - A user to check in his favorite series list.
 * @param series_name - Series name to check for.
 *
 * @return
 * True - Series name does exist in given user's favorite series list.
 * False - Series name doesn't exist in given user's favorite series list.
 */
bool isInUsersFavoriteSeriesList(User user,char* series_name){
    LIST_FOREACH(ListElement,series,user->user_favorite_series){
        if(strcmp(series,series_name)==0){
            /* series is found in user's favorite series list*/
            return true;
        }
    }
    return false;
}


//-----------------------------------------------------------------------//
//                       USER: STATIC FUNCTIONS                          //
//-----------------------------------------------------------------------//

/** Rows: 11
 ***** Static function: addNameToAList *****
 * Description: Add a given name to a given list.
 *
 * @param list - List to add to.
 * @param name - Name to add to the given list.
 *
 * @return
 * MTMFLIX_SUCCESS - Successfully added.
 * MTMFLIX_OUT_OF_MEMORY - Any memory error.
 *
 */
static MtmFlixResult addNameToAList(List list, char *name){
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

/** Rows: 4
 ***** Static function: userRemoveFromList *****
 * Description: remove a given name from a given list.
 *
 * @param list - List to remove from.
 * @param name - Name to remove from list.
 */
static void userRemoveFromList(List list, char *name){
    LIST_FOREACH(ListElement,iterator,list){
        if(!strcmp((char*)iterator,name)){
            listRemoveCurrent(list);
            break;
        }
    }
}

/** Rows: 9
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

/** Rows: 4
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



