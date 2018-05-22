#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "user.h"



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
    assert(user);
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
 *
 * @param user - A friend's name to copy.
 * @return
 * A copy of the given friend's name.
 */
char* usernameCopy(User user){
    assert(user);
    char* username_copy=malloc(strlen(user->username)+1);
    if(!username_copy){
        /* Memory allocation failed */
        return NULL;
    }
    strcpy(username_copy,user->username);
    return username_copy;
}

/**
 ***** Function: destroyUsername *****
 * Description: Deallocates the memory used for a friend's name.
 *
 * @param user - A friend's name to destroy.
 */
void destroyUsername (char* friend_username){
    free(friend_username);
}


/**
 ***** Function: removeUsernameFromFriendslist *****
 * Description: Gets a set of users and a username.
 * The function removes the given username from all the friend-lists of
 * each user in the set.
 *
 * @param users_set - Set of users to remove from their friend-lists.
 * @param username - Username to remove from their friend-list.
 * @return
 * LIST_NULL_ARGUMENT - If one or more of the arguments is NULL.
 * LIST_SUCCESS - Otherwise.
 */
void removeUsernameFromFriendslist(User user,const char* username){
    assert(user);
    assert(username);
    LIST_FOREACH(ListElement,iterator,user->user_friends_list){
        if(!strcmp((char*)iterator,username)){
            listRemoveCurrent(user->user_friends_list);
            break;
        }
    }
}


/**
 ***** Function: seriesRemoveFromFavoriteSeriesLists *****
 * Description: Removes a series from the user favorite series list.
 *
 * @param user - The user to remove from.
 * @param series_name - The series to remove.
 */
void seriesRemoveFromFavoriteSeriesLists(User user,
                                         const char *series_name){
    assert(user);
    assert(series_name);
    LIST_FOREACH(ListElement,iterator,user->user_favorite_series){
        if(!strcmp((char*)iterator,series_name)){
            listRemoveCurrent(user->user_favorite_series);
            break;
        }
    }
}