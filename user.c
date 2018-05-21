#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "user.h"



struct user_t{
    char* username;
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
    new_user->username=malloc(strlen(username)+1);
    if(!new_user->username){
        /* Username memory allocation failed */
        free(new_user);
        return NULL;
    }
    strcpy(new_user->username,username);
    new_user->age=age;
    new_user->user_friends_list= listCreate(copyFriendUsername,
            destroyFriendUsername);
    if(!new_user->user_friends_list){
        /* Friend list creation failed */
        free(new_user->username);
        free(new_user);
        return NULL;
    }
    new_user->user_favorite_series= listCreate(copyFavoriteSeriesName,
            destroyFavoriteSeriesName);
    if(!new_user->user_favorite_series){
        /* User favorite series list creation failed */
        free(new_user->username);
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



/**
 ***** Function: userDestroy *****
 * Description: Deallocates an existing user.
 *
 * @param user - A user to destroy.
 */
void userDestroy (User user){
    assert(user);
    free(user->username);
    listDestroy(user->user_friends_list);
    listDestroy(user->user_favorite_series);
    free(user);
}

/**
  ***** Function: userCompare *****
 * Description: Compares between two user's usernames.
 * @param username1 - First username.
 * @param username2 - Second username.
 *
 * @return
 * A negative integer if username1 is less than username2, 0 if the
 * names are equal and a positive integer if username1 name is bigger.
 *
 */
int userCompare (char* username1, char* username2){
    assert(username1);
    assert(username2);
    return strcmp(username1,username2);
}




/**
 ***** Function: usernameCopy *****
 * Description: Copying a friend's name.
 * @param user - A friend's name to copy.
 *
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
 * @param user - A friend's name to destroy.
 */
void destroyUsername (char* friend_username){
    free(friend_username);
}

