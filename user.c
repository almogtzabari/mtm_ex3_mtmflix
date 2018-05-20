#include <malloc.h>

#include <assert.h>
#include <mem.h>
#include "user.h"


struct user_t{
  const char* username;
  int age;
  Set user_friends_list;
  Set user_favorite_series;
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
        return NULL;
    }
    new_user->username=usernameCopy(username);
    if(!new_user->username){
        free(new_user);
        return NULL;
    }
    new_user->age=age;
    new_user->user_friends_list= setCreate(userCopySetElement,
                                           userDestroySetElement,
                                           userCompareSetElements);
    if(!new_user->user_friends_list){
        free((char*)new_user->username);
        free(new_user);
        return NULL;
    }
    new_user->user_favorite_series=setCreate(copySeriesSetElements,
                                             freeSeriesSetElements,
                                             compareSeriesSetElements);
}


SetElement userCopySetElement(SetElement element){
    return userCopyAux((User)element);
}

/**
 ***** Function: userCopyAux *****
 * Description: Copies a given user.
 *
 * @param user - A user to copy.
 * @return
 * A new user or NULL in case of failure.
 */
User userCopyAux (User user){
    assert(user);
    User new_user=userCreate(user->username,user->age);
    if(!new_user){
        return NULL;
    }
    new_user->user_friends_list=setCopy(user->user_friends_list);
    if(!new_user->user_friends_list){
        free((char*)new_user->username);
        return NULL;
    }
    new_user->user_favorite_series=setCopy(user->user_favorite_series);
    if(!new_user->user_favorite_series){
        free((char*)new_user->username);
        setDestroy(new_user->user_friends_list);
        return NULL;
    }
    return new_user;
}

const char* usernameCopy (const char* username){
    char* username_copy=malloc(sizeof(username)+1);
    if(!username_copy){
        return NULL;
    }
    strcpy(username_copy,username);
    return username_copy;
}

void userDestroySetElement(SetElement element){
    return (userDestroyAux((User)element));
}

/**
 ***** Function: userDestroyAux *****
 * Description: Deallocates an existing user.
 *
 * @param user - A user to destroy.
 */
void userDestroyAux (User user){
    free((char*)user->username);
    setDestroy(user->user_friends_list);
    setDestroy(user->user_favorite_series);
    free(user);
}

/**
 ***** Function: userCompare *****
 * Description: casts void* to User and calls user compare function.
 * The casting is done in order to use the set functions.
 * @param element1 -
 * @param element2
 * @return
 */
int userCompareSetElements(SetElement element1, SetElement element2){
    return (userCompareAux((User)element1,(User)element2));
}

/**
  ***** Function: userCompareAux *****
 * Description:
 * @param user1 -
 * @param user2
 * @return
 */
int userCompareAux (User user1, User user2){
    return strcmp(user1->username,user2->username);
}