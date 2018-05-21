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



