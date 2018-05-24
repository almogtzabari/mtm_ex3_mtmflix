#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"


static bool nameIsValid(const char *name);

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

/**
 ***** Function: mtmFlixAddUser *****
 * Description: Adds a username to the MtmFlix if the user doesn't already
 * exist and the given age is legal.
 * @param mtmflix - A mtmflix to add the user to.
 * @param username - The username of the user.
 * @param age - The age of the user.
 * @return
 * MTMFLIX_SUCCESS - Adding the user succeeded.
 * MTMFLIX_NULL_ARGUMENT - If one or more of the given arguments is NULL.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_USERNAME_ALREADY_USED - In case the given username is already in
 * use.
 * MTMFLIX_ILLEGAL_USERNAME - In case the given username is an empty string
 * or contains an illegal character (Not a number or a letter).
 * MTMFLIX_ILLEGAL_AGE - In case the given age is not bigger than
 * MTM_MIN_AGE and smaller than MTM_MAX_AGE.
 */
MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix,
                             const char* username, int age){
    if(!mtmflix || !username){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!nameIsValid(username)){
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    if(age<=MTM_MIN_AGE || age>=MTM_MAX_AGE){
        return MTMFLIX_ILLEGAL_AGE;
    }
    User temp_user=userCreate(username,age);
    if(!temp_user){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(setIsIn(mtmflix->users,temp_user)){
        userDestroy(temp_user);
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    SetResult result;
    result=setAdd(mtmflix->users,temp_user);
    if(result!=SET_SUCCESS){
        userDestroy(temp_user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    userDestroy(temp_user);
    return MTMFLIX_SUCCESS;
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
        return MTMFLIX_NULL_ARGUMENT;
    }
    /* Creates a dummy user for compariosn. */
    User dummy_user = userCreate(username,MTM_MIN_AGE+1);
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
    userDestroy(dummy_user);
    /* Now we need to remove this username from every user's friendlist. */
    SET_FOREACH(User,current_user,mtmflix->users){
        /*Removing the username from each user's friend list  */
        userRemoveFriend(current_user, username);
    }
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: nameIsValid *****
 * Description: Checks if the given name contains only letters or numbers
 * and at least one of them.
 * @param name - Name to check.
 *
 * @return
 * True if the name contains only letters or numbers and at least one of
 * them.
 */
static bool nameIsValid(const char *name){
    assert(name);
    if(!(*name)){
        /*Username is an empty string */
        return false;
    }
    while(*name){
        if(*name<'0' || (*name>'9' && *name<'A') ||
                (*name>'Z' && *name<'a') || *name>'z'){
            /* Username contains an illegal character */
            return false;
        }
        name++;
    }
    return true;
}
/**
 ***** Function: mtmFlixAddSeries *****
 * @param mtmflix - A mtmflix to add the series to.
 * @param name - The name of the series.
 * @param episodesNum - The number of episodes of the series.
 * @param genre - The genre of the series.
 * @param ages - The age limitations of the series. If NULL there are no
 * age limitations.
 * @param episodeDuration - The average length of an episode of the series.
 *
 * @return
 * MTMFLIX_SUCCESS - Adding the series succeeded.
 * MTMFLIX_NULL_ARGUMENT - If one or more of the given arguments is NULL.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_ILLEGAL_SERIES_NAME - In case the given series name is an empty
 * string or contains an illegal character (Not a number or a letter).
 * MTMFLIX_SERIES_ALREADY_EXISTS - In case the given series already exists
 * in the mtmflix.
 * MTMFLIX_ILLEGAL_EPISODES_NUM - In case the number of episodes is less
 * than 0.
 * MTMFLIX_ILLEGAL_EPISODES_DURATION - In case the average duration of an
 * episode is less than 0.
 *
 */
MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name,
                               int episodesNum, Genre genre, int* ages,
                               int episodesDuration){
    if(!mtmflix || !name){
        /* One of the arguments is NULL */
        return MTMFLIX_NULL_ARGUMENT;
    }

    if(!nameIsValid(name)){
        /* The given series name is not valid */
        return MTMFLIX_ILLEGAL_SERIES_NAME;
    }
    if(episodesNum<1){
        /* Number of episodes is 0 or less. */
        return MTMFLIX_ILLEGAL_EPISODES_NUM;
    }
    if(episodesDuration<0 || episodesDuration==0){
        /* Episode average duration is 0 or less. */
        return MTMFLIX_ILLEGAL_EPISODES_DURATION;
    }

    Series temp_series = seriesCreate(name,episodesNum,genre,ages,
            episodesDuration);

    if(!temp_series){
        /* Series creation failed */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(setIsIn(mtmflix->series,(SetElement)temp_series)){
        /* The series already exists */
        seriesDestroy(temp_series);
        return MTMFLIX_SERIES_ALREADY_EXISTS;
    }
    SetResult result;
    result = setAdd(mtmflix->series,temp_series);
    if(result!=SET_SUCCESS){
        /* Adding series failed */
        seriesDestroy(temp_series);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    seriesDestroy(temp_series);
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixRemoveSeries *****
 * Removes a given series from the given MtmFlix.
 * @param mtmflix - The MtmFlix we want to remove the series from.
 * @param name - The series name we want to remove.
 * @return
 * MTMFLIX_NULL_ARGUMENT - If one or more of the given arguments is NULL.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_USER_DOES_NOT_EXIST - The series does not exist in the MtmFlix.
 * MTMFLIX_SUCCESS - If the series removed successfully from MtmFlix.
 */
MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name){
    if (!mtmflix || !name){
        return MTMFLIX_NULL_ARGUMENT;
    }
    Series temp_series = seriesCreate(name,1,DRAMA,NULL,1);
    if(!temp_series){
        /* Failed to create temp_series */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->series,temp_series)){
        /*The given series does not exist */
        seriesDestroy(temp_series);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    setRemove(mtmflix->series,temp_series);
    seriesDestroy(temp_series);
    SET_FOREACH(User,current_user,mtmflix->users){
        /*Removing the series from each user's favorite series list  */
        userRemoveFavoriteSeries(current_user, name);
    }
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixReportSeries *****
 * Description: Prints all the series name and genres to a file.
 * @param mtmflix - The mtmflix to print the series list from.
 * @param seriesNum - Number of series from a genre to be printed.
 * @param outputStream - A file to print to.
 * @return
 * MTMFLIX_NULL_ARGUMENT - If one or more of the given arguments is NULL.
 * MTMFLIX_NO_SERIES - If there are no series on mtmtflix.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_SUCCESS - Printing has succeeded.
 */
MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix, int seriesNum,
                                  FILE* outputStream){
    if(!mtmflix || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(setGetSize(mtmflix->series)==0){
        return MTMFLIX_NO_SERIES;
    }
    assert(seriesNum>=0);
    SeriesResult result;
    if(seriesNum==0){
        /*Should print all the series found in mtmflix*/
        SET_FOREACH(SetElement,current_series,mtmflix->series){
            result=printSeriesDetailsToFile(current_series,outputStream);
            if(result!=SERIES_SUCCESS){
                /*Print has failed becuase of memory
                         * allocation error */
                return MTMFLIX_OUT_OF_MEMORY;
            }
        }
    }
    else {
        /*Sets the number of series from each genre should be printed */
        int number_of_series_from_genre=seriesNum;
        /*Sets the current genre to the first genre in the list */
        Genre current_genre=seriesGetGenre((Series)setGetFirst
                (mtmflix->series));
        SET_FOREACH(SetElement,current_series,mtmflix->series){
            /*Checks if the current series is still from the same genre*/
            if(seriesGetGenre((Series)current_series)==current_genre){
                /*Checks if the current series details should be printed */
                if (number_of_series_from_genre>0){
                    result=printSeriesDetailsToFile(current_series,
                            outputStream);
                    if(result!=SERIES_SUCCESS){
                        /*Print has failed becuase of memory
                         * allocation error */
                        return MTMFLIX_OUT_OF_MEMORY;
                    }
                    number_of_series_from_genre--;
                }
                else {
                    /*The current series is still from the same genre but
                     * should not be printed */
                    continue;
                }
            }
            else{
                /*If we reached here, the current series is from a
                 * different genre,so we need to print it */
                /*The current series details will be printed so we have to
                 * print series from the same genre more seriesNum-1
                 * times */
                number_of_series_from_genre=seriesNum-1;
                /*Setting the new genre to be the current genre */
                current_genre=seriesGetGenre((Series)current_series);
                result=printSeriesDetailsToFile(current_series,
                        outputStream);
                if(result!=SERIES_SUCCESS){
                    /*Print has failed becuase of memory
                         * allocation error */
                    return MTMFLIX_OUT_OF_MEMORY;
                }
            }
        }
    }
    return MTMFLIX_SUCCESS;
}
/**
 ***** Function: mtmFlixReportUsers *****
 * Description: Prints all the details of all the users to a file.
 * @param mtmflix - The mtmflix to print the series list from.
 * @param outputStream -A file to print to.
 * @return
 * MTMFLIX_NULL_ARGUMENT - If one or more of the given arguments is NULL.
 * MTMFLIX_NO_USERS - If there are no users on mtmtflix.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_SUCCESS - Printing has succeeded.
 */
MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream){
    if (!mtmflix || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(setGetSize(mtmflix->users)==0){
        return MTMFLIX_NO_USERS;
    }
    UserResult result;
    SET_FOREACH(SetElement,current_user,mtmflix->users){
        result= userPrintDetailsToFile(current_user, outputStream);
        if(result!=USER_SUCCESS){
            return MTMFLIX_OUT_OF_MEMORY;
        }
    }
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixSeriesJoin *****
 * Description: Gets a username and a series name and put the series in
 * user's favorite-series-list.
 *
 * @param mtmflix - The system of MtmFlix.
 * @param username - The user to add the series to his favorite series.
 * @param seriesName - The name of the series we want to add.
 * @return
 * MTMFLIX_SUCCESS - Adding succeeded.
 * MTMFLIX_NULL_ARGUMENT - One or more of the given arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - User doesn't exist in the given system.
 * MTMFLIX_SERIES_DOES_NOT_EXIST - Series doesn't exist in the given
 * system.
 * MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE - User does not meet age restrictions.
 */
 //todo: 42 ROWS!
MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username,
                                const char* seriesName){
    if(!mtmflix){
        return MTMFLIX_NULL_ARGUMENT;
    }
    User dummy_user = userCreate(username,MTM_MIN_AGE+1);
    if(!dummy_user){
        /* Memeory allocation failed  */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->users,(SetElement)dummy_user)){
        /* User doesn't exist. */
        userDestroy(dummy_user);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    Series dummy_series = seriesCreate(seriesName,1,HORROR,NULL,5);
    if(!dummy_series){
        /* Memeory allocation failed  */
        userDestroy(dummy_user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->series,(SetElement)dummy_series)){
        /* Series doesn't exist. */
        userDestroy(dummy_user);
        seriesDestroy(dummy_series);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    bool age_restrictions = false;
    int series_max_age = -1;
    int series_min_age = -1;
    SET_FOREACH(SetElement,current_series,mtmflix->series){
        if(seriesCompare(dummy_series,current_series)==0){
            /* We found the series with the given name. */
            if(seriesHasAgeRestrictions(current_series)){
                /* Series has age restrictions */
                age_restrictions = true;
                series_max_age = seriesGetMaxAge(current_series);
                series_min_age = seriesGetMinAge(current_series);
                break;
            }
        }
    }
    MtmFlixResult result;
    SET_FOREACH(SetElement,current_user,mtmflix->users){
         //todo:Check if equal age is allowed!
         if(userCompare(dummy_user,current_user)==0){
             /* We found the user with the given name.*/
             if(age_restrictions){
                 /* There are age restrictions, therefore we need to check
                  * user's age in order to add the series. */
                 if(series_max_age<userGetAge(current_user) ||
                    (series_min_age>userGetAge(current_user))) {
                     /* User's age is not in rage */
                     userDestroy(dummy_user);
                     seriesDestroy(dummy_series);
                     return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
                 }
             }
             /* If we got here we need to add the series to the user. */
             result = userAddFavoriteSeries((User)current_user,seriesName);
             if (result!=MTMFLIX_SUCCESS) {
                 userDestroy(dummy_user);
                 seriesDestroy(dummy_series);
                 return MTMFLIX_OUT_OF_MEMORY;
             }
         }
     }
    /* Shouldn't get here! */
    userDestroy(dummy_user);
    seriesDestroy(dummy_series);
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixSeriesLeave *****
 * Description: Gets a mtmflix system, username and series name.
 * The function removes the series from the given user's favorite list.
 *
 * @param mtmflix - The system we are working on.
 * @param username - The user we want to remove from.
 * @param seriesName - The series we want to remove.
 * @return
 * MTMFLIX_SUCCESS - Function succeeded.
 * MTMFLIX_NULL_ARGUMENT - At lease one argument is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - User doesn't exist in the system.
 * MTMFLIX_SERIES_DOES_NOT_EXIST - Series doesn't exist in the system.
 */
MtmFlixResult mtmFlixSeriesLeave(MtmFlix mtmflix, const char* username,
                                 const char* seriesName){
    if(!mtmflix || !username || !seriesName){
        return MTMFLIX_NULL_ARGUMENT;
    }
    User temp_user=userCreate(username,14);
    if(!temp_user){
        /*User creation failed becuase of memory allocation error */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->users,(temp_user))){
        userDestroy(temp_user);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    SET_FOREACH(SetElement,user,mtmflix->users){
        if(!userCompare(temp_user,user)){
            /*User to remove from found */
            userDestroy(temp_user);
            removeFromList((User)user,seriesName,2);
            break;
        }
    }
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixRemoveFriend *****
 * Description: Gets two usernames and removes username2 from the friend
 * list of username1.
 *
 * @param mtmflix - The system we are working on.
 * @param username1 - The username we want to remove a friend from.
 * @param username2 - The username we want to remove.
 * @return
 * MTMFLIX_SUCCESS - Friend removed successfully.
 * MTMFLIX_OUT_OF_MEMORY - Memory allocation error.
 * MTMFLIX_NULL_ARGUMENT - At lease one of the arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - At least one of the usernames doesn't
 * exist in the system.
 */
MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix, const char* username1,
                                  const char* username2){
    if(!mtmflix || !username1 || username2){
        return MTMFLIX_NULL_ARGUMENT;
    }
    User dummy_user1 = userCreate(username1,MTM_MIN_AGE+1);
    User dummy_user2 = userCreate(username2,MTM_MIN_AGE+1);
    if(!dummy_user1 || !dummy_user2){
        //todo: check if it's okay to combine the IF like this.
        userDestroy(dummy_user1);
        userDestroy(dummy_user2);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->users,dummy_user1) ||
                            !setIsIn(mtmflix->users,dummy_user2)){
        /* At least one user doesn't exist in the system. */
        userDestroy(dummy_user1);
        userDestroy(dummy_user2);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    /* If we got here both users exist. */
    SET_FOREACH(SetElement,current_user,mtmflix->users){
        if(userCompare(dummy_user1,current_user)==0){
            /* We found user1. */
            userRemoveFriend(current_user, username2);
            userDestroy(dummy_user1);
            userDestroy(dummy_user2);
            return MTMFLIX_SUCCESS;
        }
    }
    /* If we got here we couldn't find user1 in the users set.
     * Shouldn't happen! */
    userDestroy(dummy_user1);
    userDestroy(dummy_user2);
    return MTMFLIX_OUT_OF_MEMORY; // Shouldn't get here!
}


