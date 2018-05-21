#include "utilities.h"


//-----------------------------------------------------------------------//
//                       SERIES SET FUNCTIONS                            //
//-----------------------------------------------------------------------//

SetElement seriesCopySetElement(SetElement element){
    return seriesCopy((Series)element);
}

int seriesCompareSetElements(SetElement element1, SetElement element2){
    return seriesCompare((Series)element1,(Series)element2);
}

void seriesDestroySetElement(SetElement element){
    seriesDestroy((Series)element);
}

//-----------------------------------------------------------------------//
//                       SERIES LIST FUNCTIONS                           //
//-----------------------------------------------------------------------//

ListElement copyFavoriteSeriesName(ListElement element){
    return seriesCopyName((char *)element);
}

void destroyFavoriteSeriesName(ListElement element){
    seriesDestroyName((char*)element);
}


//-----------------------------------------------------------------------//
//                       USERS SET FUNCTIONS                             //
//-----------------------------------------------------------------------//

SetElement userCopySetElement(SetElement element1){
    return userCopy((User)element1);
}

void userDestroySetElememnt (SetElement element1){
    return userDestroy((User)element1);
}

int userCompareSetElements(SetElement element1, SetElement element2){
    return userCompare((User)element1,(User)element2);
}


//-----------------------------------------------------------------------//
//                       USERS LIST FUNCTIONS                            //
//-----------------------------------------------------------------------//

ListElement copyFriendUsername(ListElement element){
    return usernameCopy((User)element);
}

void destroyFriendUsername(ListElement element){
    return destroyUsername((char*)element);
}

/**
 ***** Function: removeUsernameFromAllFriendlists *****
 * Description: Gets a set of users and a username.
 * The function removes the given username from all the friend-lists of
 * each user in the set.
 * @param users_set - Set of users to remove from their friend-lists.
 * @param username - Username to remove from their friend-list.
 * @return
 * LIST_NULL_ARGUMENT - If one or more of the arguments is NULL.
 * LIST_SUCCESS - Otherwise.
 */
ListResult removeUsernameFromAllFriendlists(Set users_set, char* username){
    if(!users_set || !username){
        /* One or more of the arguments is NULL. */
        return LIST_NULL_ARGUMENT;
    }
    List user_friend_list;
    SET_FOREACH(SetElement,iterator,users_set){
        user_friend_list = userGetFriendList((User)iterator);
        removeUserFromFriendList(user_friend_list,(ListElement)username);
    }
    return LIST_SUCCESS;
}

/**
 ***** Funcion: removeUserFromFriendList *****
 * Description: Gets a friend list of specific user and a username of a
 * friend and removes that friend from the friend list.
 * @param user_friend_list - The Friend list that we want to remove from.
 * @param username - The username we want to remove from the list.
 */
void removeUserFromFriendList(List user_friend_list, ListElement username){
    if(!user_friend_list){
        /* Possible empty friend list. */
        return ;
    }
    LIST_FOREACH(ListElement,iterator,user_friend_list){
        if(strcpy((char*)iterator,username)==0){
            /* We found the username in this user_friend_list. */
            listRemoveCurrent(user_friend_list);
        }
    }
}