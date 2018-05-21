#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"

static bool usernameIsValid (const char* username);
static void seriesRemoveFromUsersLists(List user_favorite_series,
                                       const char* series_name);

struct mtmFlix_t{
    Set users;
    Set series;
};


/**
 ***** mtmFlixCreate *****
 * Description: Creates a new mtmFlix.
 *
 * @return
 * new mtmFlix in case of success.
 * Null in case of failure.
 */
MtmFlix mtmFlixCreate(){
    MtmFlix flix = malloc(sizeof(*flix));
    if(!flix){
        /* Failed to allocate memory. */
        return NULL;
    }
    flix->series = setCreate(seriesCopySetElement,seriesDestroySetElement,
            seriesCompareSetElements);
    if(!flix->series){
        /* Failed to allocate memory for the series set. */
        free(flix);
        return NULL;
    }
    flix->users = setCreate(userCopySetElement,userDestroySetElememnt,
            userCompareSetElements);
    if(!flix->users){
        /* Failed to allocate memory for the users set. */
        setDestroy(flix->series);
        free(flix);
    }
    return flix;
}

MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix,
                             const char* username, int age){
    if(!mtmflix || !username){
        return MTMFLIX_NULL_ARGUMENT;
    }
    User temp_user=userCreate(username,age);
    if(!temp_user){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(setIsIn(mtmflix->users,temp_user)){
        userDestroy(temp_user);
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    if(!usernameIsValid(username)){
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    if(age<MTM_MIN_AGE || age>MTM_MAX_AGE){
        return MTMFLIX_ILLEGAL_AGE;
    }
    setAdd(mtmflix->users,temp_user);
    userDestroy(temp_user);
    return MTMFLIX_SUCCESS;
}


/**
 ***** Function: usernameIsValid *****
 * @param username - Username to check.
 *
 * @return
 * True if the username contains at least one legal character
 * (letter or number) else false.
 */
static bool usernameIsValid (const char* username){
    assert(username);
    if(!(*username)){
        /*Username is an empty string */
        return false;
    }
    while(*username){
        if(*username<'0' || (*username>'9' && *username<'A') ||
                (*username>'Z' && *username<'a') || *username>'z'){
            /* Username contains an illegal character */
            return false;
        }
        username++;
    }
    return true;
}

MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name){
    if (!mtmflix || !name){
        return MTMFLIX_NULL_ARGUMENT;
    }
    Series temp_series=seriesCreate(name,1,DRAMA,NULL,1);
    if(!setIsIn(mtmflix->series,temp_series)){
        seriesDestroy(temp_series);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    setRemove(mtmflix->series,temp_series);
    seriesDestroy(temp_series);
    SET_FOREACH(User,current_user,mtmflix->users){
        /*Removing the series from each user's favorite series list  */
        seriesRemoveFromUsersLists(
                userGetFavoriteSeriesList(current_user),name);
    }
    return MTMFLIX_SUCCESS;
}

static void seriesRemoveFromUsersLists(List user_favorite_series,
                                const char* series_name){
    LIST_FOREACH(ListElement,iterator,user_favorite_series){
        if(!strcmp((char*)iterator,series_name)){
            listRemoveCurrent(user_favorite_series);
            break;
        }
    }
}


/**
 ***** Function: mtmFlixRemoveUser *****
 * Description: Removes a given user from the given MtmFlix.
 * @param mtmflix - The MtmFlix we want to remove the user from.
 * @param username - Username we want to remove.
 * @return
 * MTMFLIX_NULL_ARGUMENT - If one or more of the given arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - If the user does not exist in the MtmFlix.
 * MTMFLIX_SUCCESS - If user removed from MtmFlix.
 *
 */
//todo: what if the mtmflix->users is NULL?
MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username){
    if(!mtmflix || !username){
        MTMFLIX_NULL_ARGUMENT;
    }
    /* Creates a dummy user for compariosn. */
    User dummy_user = userCreate(username,MTM_MIN_AGE);
    if(!dummy_user){
        /* Failed to create dummy user. */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->users,dummy_user)){
        /* User does not exist. */
        userDestroy(dummy_user);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    /* If we got here then the user exist in the users-set and need to be
     * removed from there.*/
    setRemove(mtmflix->users,dummy_user); // Removing user from users set.
    /* Now we need to remove this username from every user's friendlist. */
    removeUsernameFromAllFriendlists(mtmflix->users,username);
    userDestroy(dummy_user);
    return MTMFLIX_SUCCESS;
}


