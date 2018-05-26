#include <stdlib.h>
#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"

//-----------------------------------------------------------------------//
//                MTMFLIX: STATIC FUNCTIONS DECLARATIONS                 //
//-----------------------------------------------------------------------//

static bool userCanWatchSeries(MtmFlix mtmflix, User user,
                               Series series);
static bool nameIsValid(const char *name);
static MtmFlixResult usersExist(MtmFlix mtmflix, const char* username1,
                                const char* username2);
static MtmFlixResult userAndSeriesExist(MtmFlix mtmflix,
                                        const char *username,
                                        const char *seriesName);
static User getUserByUsername(char* username,MtmFlixResult* result,
                              Set users_set);
static void rankSeriesAndAddToRankedSeriesSet(Set users_set,Set series_set,
                                              User user,Series series,
                                              Genre genre,
                                              MtmFlixResult* function_status,
                                              Set ranked_series_set);

static bool seriesShouldBeRecommended(Series series,User user,
                                      MtmFlix mtmflix,
                                      MtmFlixResult* result);
static int rankSeries(Set users_set,User user,
                      char* series_name,Series series,Set series_set,
                      Genre genre,MtmFlixResult* function_status);

//-----------------------------------------------------------------------//
//                       MTMFLIX: STRUCT                                 //
//-----------------------------------------------------------------------//

struct mtmFlix_t{
    Set users;
    Set series;
};


//-----------------------------------------------------------------------//
//                       MTMFLIX: FUNCTIONS                              //
//-----------------------------------------------------------------------//

/** Rows: 14
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
 ***** Function: *****
 * Description: Frees all allocated memory of a given MtmFlix.
 *
 * @param mtmflix - MtmFlix we want to destroy.
 */
void mtmFlixDestroy(MtmFlix mtmflix){
    if(!mtmflix){
        return;
    }
    setDestroy(mtmflix->series);
    setDestroy(mtmflix->users);
    free(mtmflix);
}

/** Rows: 19
 ***** Function: mtmFlixAddUser *****
 * Description: Adds a username to the MtmFlix if the user doesn't already
 * exist and the given age is legal.
 *
 * @param mtmflix - A mtmflix to add the user to.
 * @param username - The username of the user.
 * @param age - The age of the user.
 *
 * @return
 * MTMFLIX_SUCCESS - User added successfully.
 * MTMFLIX_NULL_ARGUMENT - At least one of the given arguments is NULL.
 * MTMFLIX_OUT_OF_MEMORY - Any memory allocation fail.
 * MTMFLIX_USERNAME_ALREADY_USED - Given username is already in use.
 * MTMFLIX_ILLEGAL_USERNAME - Given username is an empty string or contains
 * forbidden characters.
 * MTMFLIX_ILLEGAL_AGE - User does not meet age requirements.
 */
MtmFlixResult mtmFlixAddUser(MtmFlix mtmflix,
                             const char* username, int age){
    if(!mtmflix || !username){
        /* At least one of the arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!nameIsValid(username)){
        /* Invalid username. Only numbers and letters are allowed. */
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    if(age<=MTM_MIN_AGE || age>=MTM_MAX_AGE){
        /* User does not meet age requirements.  */
        return MTMFLIX_ILLEGAL_AGE;
    }
    /* In order to add a user to the users set and in order to check
     * whether a user with that name already exist we need to use the
     * userCompare function. Since the userCompare function has to get two
     * users we need to create a temporary user. */
    User temp_user = userCreate(username,age);
    if(!temp_user){
        /* Failed to allocate memory for the temporary user.  */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(setIsIn(mtmflix->users,temp_user)){
        /* User is already exist in the system. */
        userDestroy(temp_user);
        return MTMFLIX_USERNAME_ALREADY_USED;
    }
    /* If we got here then the user isn't in the system yet and he meets
     * all the requirements. Now we'll add him. */
    SetResult result;
    result = setAdd(mtmflix->users,temp_user);
    if(result!=SET_SUCCESS){
        /* Failed to add user to users set. */
        userDestroy(temp_user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    /* If we got here then user added successfully. */
    userDestroy(temp_user);
    return MTMFLIX_SUCCESS;
}

/** Rows: 14
 ***** Function: mtmFlixRemoveUser *****
 * Description: Removes a given user from the given MtmFlix.
 *
 * @param mtmflix - MtmFlix we want to remove the user from.
 * @param username - Username we want to remove.
 *
 * @return
 * MTMFLIX_NULL_ARGUMENT - At least one of the given arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - User does not exist in the MtmFlix.
 * MTMFLIX_SUCCESS - User removed successfully.
 *
 */
MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username){
    if(!mtmflix || !username){
        /* At least one of the arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    /* Creates a dummy user for compariosn. */
    User dummy_user = userCreate(username,MTM_MIN_AGE+1);
    if(!dummy_user){
        /* Failed to create dummy user. */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    assert(mtmflix->users);
    if(!setIsIn(mtmflix->users,dummy_user)){
        /* User does not exist. */
        userDestroy(dummy_user);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    /* If we got here then the user exist in the users set and need to be
     * removed.*/
    setRemove(mtmflix->users,dummy_user); // Removes user from users set.
    userDestroy(dummy_user);
    /* Now we need to remove this username from every user's friendlist. */
    SET_FOREACH(User,current_user,mtmflix->users){
        /*Removing the username from each user's friend list  */
        removeFromList(current_user, username,FRIENDS_LIST);
    }
    return MTMFLIX_SUCCESS;
}

/** Rows: 9
 ***** Function: nameIsValid *****
 * Description: Checks if the given name contains only letters or numbers
 * and is not an empty string.
 *
 * @param name - Name to check.
 *
 * @return
 * True - Name is valid.
 * False - Name is invalid.
 */
static bool nameIsValid(const char *name){
    assert(name);
    if(!(*name)){
        /* Username is an empty string. */
        return false;
    }
    while(*name){
        if(*name<'0' || (*name>'9' && *name<'A') ||
                (*name>'Z' && *name<'a') || *name>'z'){
            /* Username contains an illegal character. */
            return false;
        }
        name++;
    }
    return true;
}

/** Rows: 22
 ***** Function: mtmFlixAddSeries *****
 * Description: Adds a series to MtmFlix.
 *
 * @param mtmflix - A MtmFlix to add the series to.
 * @param name - Name of the series.
 * @param episodesNum - Number of episodes of the series.
 * @param genre - Genre of the series.
 * @param ages - Age limitations of the series. If NULL there are no
 * age limitations.
 * @param episodeDuration - Average length of episode of the series.
 *
 * @return
 * MTMFLIX_SUCCESS - Series added successfully.
 * MTMFLIX_NULL_ARGUMENT - At least one of the given arguments is NULL.
 * MTMFLIX_OUT_OF_MEMORY - Any memory allocation error.
 * MTMFLIX_ILLEGAL_SERIES_NAME - Given series' name is an empty string
 * or contains forbidden characters (only letters and numbers are allowed).
 * MTMFLIX_SERIES_ALREADY_EXISTS - Given series already exists.
 * MTMFLIX_ILLEGAL_EPISODES_NUM - Number of episodes <= 0.
 * MTMFLIX_ILLEGAL_EPISODES_DURATION - Average duration of an episode <= 0.
 *
 */
MtmFlixResult mtmFlixAddSeries(MtmFlix mtmflix, const char* name,
                               int episodesNum, Genre genre, int* ages,
                               int episodesDuration){
    if(!mtmflix || !name){
        /* At least one of the given arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(!nameIsValid(name)){
        /* Given series name is not valid */
        return MTMFLIX_ILLEGAL_SERIES_NAME;
    }
    if(episodesNum<1){
        /* Number of episodes is 0 or less. */
        return MTMFLIX_ILLEGAL_EPISODES_NUM;
    }
    if(episodesDuration<0 || episodesDuration==0){
        /* Episode average duration <= 0 . */
        return MTMFLIX_ILLEGAL_EPISODES_DURATION;
    }
    Series temp_series = seriesCreate(name,episodesNum,genre,ages,
            episodesDuration);
    if(!temp_series){
        /* failed to allocate memory for the temporary series. */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(setIsIn(mtmflix->series,(SetElement)temp_series)){
        /* The series already exists */
        seriesDestroy(temp_series);
        return MTMFLIX_SERIES_ALREADY_EXISTS;
    }
    /* If we got here then the series doesn't exist yet and also meets all
     * the requirements. Now we'll add it. */
    SetResult result;
    result = setAdd(mtmflix->series,temp_series); // Adds series.
    if(result!=SET_SUCCESS){
        /* Adding series failed */
        seriesDestroy(temp_series);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    /* Series addded successfully. */
    seriesDestroy(temp_series);
    return MTMFLIX_SUCCESS;
}

/** Rows: 13
 ***** Function: mtmFlixRemoveSeries *****
 * Description: Removes a given series from the given MtmFlix.
 *
 * @param mtmflix - MtmFlix we want to remove the series from.
 * @param name - Name of the series we want to remove.
 *
 * @return
 * MTMFLIX_NULL_ARGUMENT - At least one of the given arguments is NULL.
 * MTMFLIX_OUT_OF_MEMORY - Any memory allocation error.
 * MTMFLIX_USER_DOES_NOT_EXIST - Series does not exist in the MtmFlix.
 * MTMFLIX_SUCCESS - Series removed successfully.
 */
MtmFlixResult mtmFlixRemoveSeries(MtmFlix mtmflix, const char* name){
    if (!mtmflix || !name){
        /* At least one of the given arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    Series temp_series = seriesCreate(name,1,DRAMA,NULL,1);
    if(!temp_series){
        /* Failed to allocate memory for temporary series. */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->series,temp_series)){
        /* Given series does not exist */
        seriesDestroy(temp_series);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    /* Series exist and should be removed. */
    setRemove(mtmflix->series,temp_series); // Removes series from system.
    seriesDestroy(temp_series);
    SET_FOREACH(User,current_user,mtmflix->users){
        /*Removing the series from each user's favorite series list  */
        removeFromList(current_user,name,FAVORITE_SERIES_LIST);
    }
    return MTMFLIX_SUCCESS;
}

/** Rows:
 ***** Function: mtmFlixReportSeries *****
 * Description: Prints name and genre of series in MtmFlix to a file. Only
 * the 'seriesNum' first from each genre will be printed.
 *
 * @param mtmflix - MtmFlix to print the series from.
 * @param seriesNum - Number of series from a genre to be printed.
 * @param outputStream - A file to print to.
 *
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
MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username,
                                const char* seriesName){
    if(!mtmflix || !username || !seriesName){
        return MTMFLIX_NULL_ARGUMENT;
    }
    MtmFlixResult result;
    /*Checks if both user and series exist in mtmflix */
    result= userAndSeriesExist(mtmflix,username,seriesName);
    if(result!=MTMFLIX_SUCCESS){
        return result;
    }
    /*If we got here both user and series exist in mtmflix */
    User dummy_user = userCreate(username,MTM_MIN_AGE+1);
    if(!dummy_user){
        /* Memory allocation failed  */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    Series dummy_series = seriesCreate(seriesName,1,HORROR,NULL,5);
    if(!dummy_series){
        /* Memory allocation failed  */
        userDestroy(dummy_user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!userCanWatchSeries(mtmflix, dummy_user, dummy_series)){
        /*If we got here user can't add the series because of age
          limitations */
        userDestroy(dummy_user);
        seriesDestroy(dummy_series);
        return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
    }
    seriesDestroy(dummy_series);
    SET_FOREACH(SetElement,current_user,mtmflix->users){
         //todo:Check if equal age is allowed!
         if(userCompare(dummy_user,current_user)==0){
             /* We found the user with the given name so we need to add the
              * series to this user.*/
             result = AddToList((User)current_user,seriesName,
                     FAVORITE_SERIES_LIST);
             if (result!=MTMFLIX_SUCCESS) {
                 userDestroy(dummy_user);
                 return MTMFLIX_OUT_OF_MEMORY;
             }
         }
     }
    /* If we got here we couldn't find the user in the users set.
    * Shouldn't happen! */
    userDestroy(dummy_user);
    return MTMFLIX_SUCCESS;
}

/**
 ***** Static function : userCanWatchSeries *****
 * Description: Checks if a user can add a series to his favorite series
 * list by the series age limitations.
 * @param mtmflix - The system of MtmFlix.
 * @param user - The user that want to add the show.
 * @param series - The show that the user wants to add.
 *
 * @return
 * True - If the series has no age limitations or the user's age is in
 * range of the age limitations of the series, else false.
 */
static bool userCanWatchSeries(MtmFlix mtmflix, User user,
                               Series series) {
    int series_max_age = -1;
    int series_min_age = -1;
    SET_FOREACH(SetElement, current_series, mtmflix->series) {
        if (seriesCompare(series,current_series) == 0) {
            /* We found the series with the given name. */
            if (seriesHasAgeRestrictions(current_series)) {
                /* Series has age restrictions */
                series_max_age = seriesGetMaxAge(current_series);
                series_min_age = seriesGetMinAge(current_series);
                break;
            }
            else {
                /*The series has no age limitations and the user can add
                  it*/
                return true;
            }
        }
    }
    /*If we got here the series has age limitations and we need to check
      if the user can add it to his favorite series list */
    SET_FOREACH(SetElement,current_user,mtmflix->users) {
        //todo:Check if equal age is allowed!
        if (userCompare(user,current_user) == 0) {
            /* We found the user with the given name.*/
            if (series_max_age < userGetAge(current_user) ||
                (series_min_age > userGetAge(current_user))) {
                /* User's age is not in rage */
                return false;
            }
        }
    }
    /*If we got here the user can add the series to his favorite series
      list*/
    return true;
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
    MtmFlixResult result;
    /* Checks if both user and series exist in mtmflix. */
    result= userAndSeriesExist(mtmflix, username, seriesName);
    if(result!=MTMFLIX_SUCCESS){
        /*We get here in case the user or the series doesn't exist in the
          mtmflix or in case of memory allocation error*/
        return result;
    }
    User temp_user=userCreate(username,14);
    if(!temp_user){
        /*User creation failed because of memory allocation error */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SET_FOREACH(SetElement,user,mtmflix->users){
        if(userCompare(temp_user,user)==0){
            /*User to remove from found */
            removeFromList((User)user,seriesName,FAVORITE_SERIES_LIST);
            break;
        }
    }
    userDestroy(temp_user);
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixAddFriend *****
 * @param mtmflix - The system we are working on.
 * @param username1 - The user to add to his friend list.
 * @param username2 - The user to add to.
 *
 * @return
 * MTMFLIX_NULL_ARGUMENT - At least one of the arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - At least one of the users doesn't exist.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_SUCCESS= - Adding friend succeeded.
 */
MtmFlixResult mtmFlixAddFriend(MtmFlix mtmflix, const char* username1,
                               const char* username2){
    if(!mtmflix || !username1 || !username2){
        return MTMFLIX_NULL_ARGUMENT;
    }
    MtmFlixResult result;
    /*Checks if both users exist in the mtmflix */
    result=usersExist(mtmflix,username1,username2);
    if(result!=MTMFLIX_SUCCESS){
        return result;
    }
    /*If we got here both users exist in mtmflix */
    User dummy_user = userCreate(username1,MTM_MIN_AGE+1);
    if(!dummy_user){
        /* Memory allocation failed  */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SET_FOREACH(SetElement,current_user,mtmflix->users){
        if(userCompare(dummy_user,current_user)==0){
            /* We found the user with the given name so we need to add
             * username2 to this user's friend list.*/
            result = AddToList((User)current_user,username2,FRIENDS_LIST);
            if (result!=MTMFLIX_SUCCESS) {
                userDestroy(dummy_user);
                return result;
            }
        }
    }
    userDestroy(dummy_user);
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
 * MTMFLIX_NULL_ARGUMENT - At least one of the arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - At least one of the usernames doesn't
 * exist in the system.
 */
MtmFlixResult mtmFlixRemoveFriend(MtmFlix mtmflix, const char* username1,
                                  const char* username2){
    if(!mtmflix || !username1 || !username2){
        return MTMFLIX_NULL_ARGUMENT;
    }
    MtmFlixResult result;
    /*Checks if both users exist in the mtmflix */
    result=usersExist(mtmflix,username1,username2);
    if(result!=MTMFLIX_SUCCESS){
        return result;
    }
    /* If we got here both users exist. */
    User dummy_user1 = userCreate(username1,MTM_MIN_AGE+1);
    SET_FOREACH(SetElement,current_user,mtmflix->users){
        if(userCompare(dummy_user1,current_user)==0){
            /* We found user1. */
            removeFromList(current_user,username2,FRIENDS_LIST);

        }
    }
    userDestroy(dummy_user1);
    return MTMFLIX_SUCCESS;
}

//todo:31 lines
MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix, const char* username,
                                        int count, FILE* outputStream){
    if(!mtmflix || !username || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(count<0){
        return MTMFLIX_ILLEGAL_NUMBER;
    }
    MtmFlixResult result;
    Set series_set_copy = setCopy(mtmflix->series);
    if(!series_set_copy){
        /* Memory allocation failed. */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    User user = getUserByUsername((char*)username,&result,mtmflix->users);
    if(result!=MTMFLIX_SUCCESS){
        /*We get here in case of memory allocation error or in case the
         * user with the given username doesn't exist */
        setDestroy(series_set_copy);
        return result;
    }
    Set ranked_series_set=setCreate(rankedSeriesCopySetElement,
                                    rankedSeriesDestroySetElement,
                                    rankedSeriesCompareSetElement);
    if(!ranked_series_set){
        setDestroy(series_set_copy);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SET_FOREACH(SetElement,series,series_set_copy){
        if(!seriesShouldBeRecommended(series,user,mtmflix,&result)) {
            if(result!=MTMFLIX_SUCCESS) {
                setDestroy(series_set_copy);
                return MTMFLIX_OUT_OF_MEMORY;
            }
            continue;
        }
        setDestroy(series_set_copy);
        if(result!=MTMFLIX_SUCCESS) {
            return MTMFLIX_OUT_OF_MEMORY;
        }
    /*If we got here the current series should be added to the set of
     * recommended series */
        rankSeriesAndAddToRankedSeriesSet(mtmflix->users,mtmflix->series,
                                        user,series,seriesGetGenre(series),
                                        &result,ranked_series_set);
        if(result!=MTMFLIX_SUCCESS){
            setDestroy(ranked_series_set);
            return MTMFLIX_OUT_OF_MEMORY;
        }
    }
    rankedSeriesPrintToFile(count,ranked_series_set,outputStream,&result);
    setDestroy(ranked_series_set);
    if(result!=MTMFLIX_SUCCESS){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_SUCCESS;
}

static bool seriesShouldBeRecommended(Series series,User user,
                                      MtmFlix mtmflix,
                                      MtmFlixResult* result) {
    char *series_name = seriesGetName(series);
    if (!series_name) {
        *result=MTMFLIX_OUT_OF_MEMORY;
        return false;
    }
    if ((isInUsersFavoriteSeriesList(user, series_name)) ||
        (!userCanWatchSeries(mtmflix, user, series))) {
        /*If we get here the series is already in the user's favorite
          series list or the user's age is not in the range of age
          limitations of the series. This series shouldn't be
          recommended*/
        free(series_name);
        *result=MTMFLIX_SUCCESS;
        return false;
    }
    *result=MTMFLIX_SUCCESS;
    return true;
}



static int rankSeries(Set users_set,User user,
                      char* series_name,Series series,Set series_set,
                      Genre genre,MtmFlixResult* function_status){
    int same_genre=userHowManySeriesWithGenre(series_set,user,genre);
    if(same_genre==-1){
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return -1;
    }
    Set series_set_copy = setCopy(series_set);
    if(!series_set_copy){
        return -1;
    }
    double average_list_episode_duration=
            userGetAverageEpisodeDuration(user,series_set,function_status);
    setDestroy(series_set_copy);
    if(*function_status!=MTMFLIX_SUCCESS){
        return -1;
    }
    int number_of_friends_loved_this_series=
            howManyFriendsLovedThisSeries(users_set,user,series_name);
    int current_series_episode_duration=seriesGetEpisodeDuration(series);
    double rank=((double)same_genre*number_of_friends_loved_this_series);
    // todo: rank should be int.
    rank/=(1+abs(current_series_episode_duration-(int)
            (average_list_episode_duration)));
    return (int)rank;
}


static void rankSeriesAndAddToRankedSeriesSet(Set users_set,Set series_set,
                                              User user,Series series,
                                              Genre genre,
                                              MtmFlixResult* function_status,
                                              Set ranked_series_set){
    char* series_name=seriesGetName(series);
    if(!series_name){
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return;
    }
    char* series_genre_string=getGenreNameByEnum(seriesGetGenre(series));
    if(!series_genre_string){
        free(series_name);
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return;
    }
    int rank=rankSeries(users_set,user,series_name,series,series_set,
            genre,function_status);
    if(*function_status!=MTMFLIX_SUCCESS){
        free(series_name);
        free(series_genre_string);
        return;
    }
    RankedSeries new_ranked_series=rankedSeriesCreate
            (rank,series_name,series_genre_string);
    free(series_name);
    free(series_genre_string);
    if(!new_ranked_series){
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return;
    }
    SetResult result=setAdd(ranked_series_set,new_ranked_series);
    rankedSeriesDestroy(new_ranked_series);
    if(result!=SET_SUCCESS){
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return;
    }
}


/**
 ***** Function: usersExist *****
 * Description: Checks if the users exist in mtmflix.
 * @param mtmflix - The mtmflix to check in.
 * @param username1 - A username to check.
 * @param username2 - A username to check.
 *
 * @return
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation failure.
 * MTMFLIX_USER_DOES_NOT_EXIST - If one of the users doesn't exist in
 * the mtmflix.
 * MTMFLIX_SUCCESS - If both of the users exist in the mtmflix.
 */
static MtmFlixResult usersExist(MtmFlix mtmflix, const char* username1,
                              const char* username2){
    User dummy_user1 = userCreate(username1,MTM_MIN_AGE+1);
    if(!dummy_user1){
        return MTMFLIX_OUT_OF_MEMORY;;
    }
    User dummy_user2 = userCreate(username2,MTM_MIN_AGE+1);
    if(!dummy_user2){
        userDestroy(dummy_user1);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->users,dummy_user1) ||
       !setIsIn(mtmflix->users,dummy_user2)){
        /* At least one user doesn't exist in the system. */
        userDestroy(dummy_user1);
        userDestroy(dummy_user2);
        return MTMFLIX_USER_DOES_NOT_EXIST;

    }
    userDestroy(dummy_user1);
    userDestroy(dummy_user2);
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: userAndSeiesExist *****
 * @param mtmflix - The mtmflix to check in.
 * @param username - A username to check.
 * @param seriesName - A series name to check.
 *
 * @return
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation failure.
 * MTMFLIX_USER_DOES_NOT_EXIST - If the user doesn't exist in
 * the mtmflix.
 * MTMFLIX_SERIES_DOES_NOT_EXIST - If the series doesn't exist in
 * the mtmflix.
 * MTMFLIX_SUCCESS -If both user and series exist in the mtmflix.
 */
static MtmFlixResult userAndSeriesExist(MtmFlix mtmflix,
                                        const char *username,
                                        const char *seriesName){
    User dummy_user = userCreate(username,MTM_MIN_AGE+1);
    if(!dummy_user){
        /* Memory allocation failed  */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->users,(SetElement)dummy_user)){
        /* User doesn't exist. */
        userDestroy(dummy_user);
        return MTMFLIX_USER_DOES_NOT_EXIST;
    }
    Series dummy_series = seriesCreate(seriesName,1,HORROR,NULL,5);
    if(!dummy_series){
        /* Memory allocation failed  */
        userDestroy(dummy_user);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    if(!setIsIn(mtmflix->series,(SetElement)dummy_series)){
        /* Series doesn't exist. */
        userDestroy(dummy_user);
        seriesDestroy(dummy_series);
        return MTMFLIX_SERIES_DOES_NOT_EXIST;
    }
    userDestroy(dummy_user);
    seriesDestroy(dummy_series);
    return MTMFLIX_SUCCESS;
}

static User getUserByUsername(char* username,MtmFlixResult* result,
                              Set users_set){
    User temp_user=userCreate(username,MTM_MIN_AGE+1);
    if(!temp_user){
        *result=MTMFLIX_OUT_OF_MEMORY;
        return NULL;
    }
    SET_FOREACH(SetElement,user,users_set){
        if(userCompare(user,temp_user)==0){
            userDestroy(temp_user);
            *result=MTMFLIX_SUCCESS;
            return (User)user;
        }
    }
    userDestroy(temp_user);
    *result=MTMFLIX_USER_DOES_NOT_EXIST;
    return NULL;
}

