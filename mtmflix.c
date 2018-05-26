#include <stdlib.h>
#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"

#define ILLEGAL_VALUE -1

//-----------------------------------------------------------------------//
//                MTMFLIX: STATIC FUNCTIONS DECLARATIONS                 //
//-----------------------------------------------------------------------//

static bool userCanWatchSeries(MtmFlix mtmflix, User user,
                               Series series,SeriesResult* status);

static bool nameIsValid(const char *name);

static MtmFlixResult usersExist(MtmFlix mtmflix, const char* username1,
                                const char* username2);

static MtmFlixResult userAndSeriesExist(MtmFlix mtmflix,
                                        const char *username,
                                        const char *seriesName);

static User getUserByUsername(char* username,MtmFlixResult* result,
                              Set users_set);

static void rankSeriesAndAddToRankedSeriesSet(Set users_set,Set series_set,
  User user,Series series, Genre genre, MtmFlixResult* function_status,
                                                    Set ranked_series_set);

static double doubleAbs (double number);

static MtmFlixResult rankAllSeriesForUser(MtmFlix mtmflix,User user,
                                  Set ranked_series_set,FILE* outputStream,
                                   int count,Set series_set);

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
    /* New mtmflix successfully created. */
    return flix;
}

/** Rows: 5
 ***** Function: mtmFlixDestroy *****
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

/** Rows: 21
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
    if(!nameIsValid(username)){
        /* Invalid username. Only numbers and letters are allowed. */
        userDestroy(temp_user);
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    if(age<MTM_MIN_AGE || age>MTM_MAX_AGE){
        /* User does not meet age requirements.  */
        userDestroy(temp_user);
        return MTMFLIX_ILLEGAL_AGE;
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
    /* If we got here then user added successfully to mtmflix. */
    userDestroy(temp_user);
    return MTMFLIX_SUCCESS;
}

/** Rows: 13
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
        removeFromList(current_user, (char*)username,FRIENDS_LIST);
    }
    /* User removed successfully from mtmflix. */
    return MTMFLIX_SUCCESS;
}

/** Rows: 8
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
    /* Username is valid (still need to check if in use).*/
    return true;
}

/** Rows: 24
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
    Series temp_series = seriesCreate((char*)name,episodesNum,genre,ages,
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
    if(episodesNum<1){
        /* Number of episodes is 0 or less. */
        seriesDestroy(temp_series);
        return MTMFLIX_ILLEGAL_EPISODES_NUM;
    }
    if(episodesDuration<0 || episodesDuration==0){
        /* Episode average duration <= 0 . */
        seriesDestroy(temp_series);
        return MTMFLIX_ILLEGAL_EPISODES_DURATION;
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
    Series temp_series = seriesCreate((char*)name,1,DRAMA,NULL,1);
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
        removeFromList(current_user,(char*)name,FAVORITE_SERIES_LIST);
    }
    /* Series removed successully. */
    return MTMFLIX_SUCCESS;
}

/** Rows: 29
 ***** Function: mtmFlixReportSeries *****
 * Description: Prints name and genre of series in MtmFlix to a file. Only
 * the 'seriesNum' first from each genre will be printed.
 *
 * @param mtmflix - MtmFlix to print the series from.
 * @param seriesNum - Number of series from a genre to be printed.
 * @param outputStream - A file to print to.
 *
 * @return
 * MTMFLIX_NULL_ARGUMENT - At least one of the given arguments is NULL.
 * MTMFLIX_NO_SERIES - No series in mtmtflix.
 * MTMFLIX_OUT_OF_MEMORY - Any memory error.
 * MTMFLIX_SUCCESS - Users report successfully printed.
 */
MtmFlixResult mtmFlixReportSeries(MtmFlix mtmflix, int seriesNum,
                                  FILE* outputStream){
    if(!mtmflix || !outputStream){
        /* At least one of the given arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(setGetSize(mtmflix->series)==0){
        /* No series in mtmtflix. */
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
/** Rows: 10
 ***** Function: mtmFlixReportUsers *****
 * Description: Prints all the details of all the users to a file.
 *
 * @param mtmflix - The mtmflix to print the series list from.
 * @param outputStream -A file to print to.
 *
 * @return
 * MTMFLIX_NULL_ARGUMENT - At least one of the given arguments is NULL.
 * MTMFLIX_NO_USERS - No users in mtmtflix.
 * MTMFLIX_OUT_OF_MEMORY - In case of memory allocation error.
 * MTMFLIX_SUCCESS - Printing has succeeded.
 */
MtmFlixResult mtmFlixReportUsers(MtmFlix mtmflix, FILE* outputStream){
    if (!mtmflix || !outputStream){
        /* At least one of the given arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    if(setGetSize(mtmflix->users)==0){
        /* No users in mtmtflix. */
        return MTMFLIX_NO_USERS;
    }
    UserResult result;
    SET_FOREACH(SetElement,current_user,mtmflix->users){
        result= userPrintDetailsToFile(current_user, outputStream);
        if(result!=USER_SUCCESS){
            /* Failed to print. */
            return MTMFLIX_OUT_OF_MEMORY;
        }
    }
    /* Users printed successfully. */
    return MTMFLIX_SUCCESS;
}

/** Rows: 30
 ***** Function: mtmFlixSeriesJoin *****
 * Description: Gets a username and a series name and put the series in
 * user's favorite-series-list.
 *
 * @param mtmflix - The system of MtmFlix.
 * @param username - The user to add the series to his favorite series.
 * @param seriesName - The name of the series we want to add.
 *
 * @return
 * MTMFLIX_SUCCESS - Adding succeeded.
 * MTMFLIX_NULL_ARGUMENT - At lease one of the given arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - User doesn't exist in the given system.
 * MTMFLIX_SERIES_DOES_NOT_EXIST - Series doesn't exist in the given
 * system.
 * MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE - User does not meet age restrictions.
 */
MtmFlixResult mtmFlixSeriesJoin(MtmFlix mtmflix, const char* username,
                                const char* seriesName){
    if(!mtmflix || !username || !seriesName){
        /* At lease one of the given arguments is NULL. */
        return MTMFLIX_NULL_ARGUMENT;
    }
    /* Checks if both user and series exist in mtmflix. */
    MtmFlixResult result= userAndSeriesExist(mtmflix,username,seriesName);
    if(result!=MTMFLIX_SUCCESS){
        return result;
    }
    /*If we got here both user and series exist in mtmflix */
    User dummy_user = userCreate(username,MTM_MIN_AGE+1);
    Series dummy_series = seriesCreate((char*)seriesName,1,HORROR,NULL,5);
    if(!dummy_series || !dummy_user){
        /* Memory allocation failed  */
        userDestroy(dummy_user);
        seriesDestroy(dummy_series);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SeriesResult status;
    if(!userCanWatchSeries(mtmflix, dummy_user, dummy_series,&status)){
        /* User can't watch series because of age limitations or
         * userCanWatchSeries had a memory fail. */
        if(status!=SERIES_SUCCESS){
            /* Memory fail: userCanWatchSeries memory fail. */
            userDestroy(dummy_user);
            seriesDestroy(dummy_series);
            return MTMFLIX_OUT_OF_MEMORY;
        }
        /*If we got here user can't add the series because of age
          limitations */
        userDestroy(dummy_user);
        seriesDestroy(dummy_series);
        return MTMFLIX_USER_NOT_IN_THE_RIGHT_AGE;
    }
    /* If we got here then the user can add the series to his list.  */
    seriesDestroy(dummy_series);
    SET_FOREACH(SetElement,current_user,mtmflix->users){
         if(userCompare(dummy_user,current_user)==0){
             /* We found the user with the given name so we need to add the
              * series to this user.*/
             result = AddToList((User)current_user,(char*)seriesName,
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

/** Rows: 24
 ***** Static function : userCanWatchSeries *****
 * Description: Checks if a user can add a series to his favorite series
 * list by the series age limitations.
 *
 * Notice: Will not damage the iterator.
 *
 * @param mtmflix - The system of MtmFlix.
 * @param user - The user that want to add the show.
 * @param series - The show that the user wants to add.
 *
 * @return
 * True - If the series has no age limitations or the user's age is in
 * range of the age limitations of the series, else false.
 */
static bool userCanWatchSeries(MtmFlix mtmflix, User user,
                               Series series,SeriesResult* status) {
    Set series_set_copy = setCopy(mtmflix->series);
    if(!series_set_copy){
        *status = SERIES_MEMORY_ALLOCATION_FAILED;
        return false;
    }
    int series_max_age = -1;
    int series_min_age = -1;
    SET_FOREACH(SetElement, current_series, series_set_copy) {
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
                *status = SERIES_SUCCESS;
                setDestroy(series_set_copy);
                return true;
            }
        }
    }
    setDestroy(series_set_copy);
    /*If we got here the series has age limitations and we need to check
      if the user can add it to his favorite series list */
    SET_FOREACH(SetElement,current_user,mtmflix->users) {
        //todo:Check if equal age is allowed!
        if (userCompare(user,current_user) == 0) {
            /* We found the user with the given name.*/
            if (series_max_age < userGetAge(current_user) ||
                (series_min_age > userGetAge(current_user))) {
                /* User's age is not in rage */
                *status = SERIES_SUCCESS;
                return false;
            }
        }
    }
    /*If we got here the user can add the series to his favorite series
      list*/
    *status = SERIES_SUCCESS;
    return true;
}

/** Rows: 16
 ***** Function: mtmFlixSeriesLeave *****
 * Description: Gets a mtmflix system, username and series name.
 * The function removes the series from the given user's favorite list.
 *
 * @param mtmflix - The system we are working on.
 * @param username - The user we want to remove from.
 * @param seriesName - The series we want to remove.
 *
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
    User temp_user=userCreate(username,MTM_MIN_AGE);
    if(!temp_user){
        /*User creation failed because of memory allocation error */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SET_FOREACH(SetElement,user,mtmflix->users){
        if(userCompare(temp_user,user)==0){
            /* Found the user with the given username. Now we'll remove
             * the series from his list. */
            removeFromList((User)user,(char*)seriesName,
                                                  FAVORITE_SERIES_LIST);
            break;
        }
    }
    /* Series successfully removed from user's favorite list. */
    userDestroy(temp_user);
    return MTMFLIX_SUCCESS;
}

/** Rows: 20
 ***** Function: mtmFlixAddFriend *****
 * Description: Adds username2 to the friend list of username1.
 *
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
    if(strcmp(username1,username2)==0){
        /* The user is trying to add himself */
        return MTMFLIX_SUCCESS;
    }
    /*If we got here both users exist in mtmflix */
    User dummy_user = userCreate(username1,MTM_MIN_AGE);
    if(!dummy_user){
        /* Memory allocation failed  */
        return MTMFLIX_OUT_OF_MEMORY;
    }
    SET_FOREACH(SetElement,current_user,mtmflix->users){
        if(userCompare(dummy_user,current_user)==0){
            /* We found the user with the given name so we need to add
             * username2 to this user's friend list.*/
            result = AddToList((User)current_user,(char*)username2,
                                                        FRIENDS_LIST);
            if (result!=MTMFLIX_SUCCESS) {
                /* Failed to add to usernames1's friend list. */
                userDestroy(dummy_user);
                return result;
            }
        }
    }
    /* Username2 successfully added to username1's friend list. */
    userDestroy(dummy_user);
    return MTMFLIX_SUCCESS;
}

/** Rows: 12
 ***** Function: mtmFlixRemoveFriend *****
 * Description: Removes username2 from the friend
 * list of username1.
 *
 * @param mtmflix - The system we are working on.
 * @param username1 - The username we want to remove a friend from.
 * @param username2 - The username we want to remove.
 *
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
            removeFromList(current_user,(char*)username2,FRIENDS_LIST);
        }
    }
    /* Username2 sucessfully removed from username1's friend list. */
    userDestroy(dummy_user1);
    return MTMFLIX_SUCCESS;
}

/** Rows: 25
 ***** Function: mtmFlixGetRecommendations *****
 * Description: Prints recommendations of series for the given user.
 * Recommendations will be printed into the given file.
 *
 * @param mtmflix - Mtmflix we are working on.
 * @param username - The username we want to print recommendations for.
 * @param count - How many series to recommend from each genre.
 * @param outputStream - File to print to.
 *
 * @return
 * MTMFLIX_SUCCESS - Successfully printed the recommendations.
 * MTMFLIX_NULL_ARGUMENT - At least one of the given  arguments is NULL.
 * MTMFLIX_USER_DOES_NOT_EXIST - User with given username doesn't exist.
 * MTMFLIX_ILLEGAL_NUMBER - A negative numer was inserted.
 */
MtmFlixResult mtmFlixGetRecommendations(MtmFlix mtmflix,
                    const char* username, int count, FILE* outputStream){
    if(!mtmflix || !username || !outputStream){
        return MTMFLIX_NULL_ARGUMENT;
    }
    MtmFlixResult result;
    User user = getUserByUsername((char*)username,&result,mtmflix->users);
    if(result!=MTMFLIX_SUCCESS){
        /*We get here in case of memory allocation error or in case the
         * user with the given username doesn't exist */
        return result;
    }
    if(count<0){
        return MTMFLIX_ILLEGAL_NUMBER;
    }

    Set series_set_copy = setCopy(mtmflix->series);
    if(!series_set_copy){
        /* Memory allocation failed. */
        return MTMFLIX_OUT_OF_MEMORY;
    }

    Set ranked_series_set=setCreate(rankedSeriesCopySetElement,
            rankedSeriesDestroySetElement, rankedSeriesCompareSetElement);
    if(!ranked_series_set){
        setDestroy(series_set_copy);
        return MTMFLIX_OUT_OF_MEMORY;
    }
    /* rankAllSeriesForUser will rank the relevant series and print them
     * to the given file. */
    result=rankAllSeriesForUser(mtmflix,user,ranked_series_set,
            outputStream,count,series_set_copy);
            if(result!=MTMFLIX_SUCCESS) {
                /* Failed to print. */
                setDestroy(series_set_copy);
                setDestroy(ranked_series_set);
                return MTMFLIX_OUT_OF_MEMORY;
    }
    /* Recommendations printed successfully.*/
    setDestroy(series_set_copy);
    setDestroy(ranked_series_set);
    return MTMFLIX_SUCCESS;
}


//-----------------------------------------------------------------------//
//                       MTMFLIX: STATIC FUNCTIONS                       //
//-----------------------------------------------------------------------//

/** Rows: 12
 ***** Static function: getUserByUsername *****
 * Description: Gets a name of user and a set of users.
 * Returns the user with that name in the given users set.
 *
 * Notice: This resets the iterator of given set.
 *
 * @param username - Name of user we want to get.
 * @param result - Success/failure of the function.
 * @param users_set - Users set we are looking in.
 *
 * @return
 * MTMFLIX_OUT_OF_MEMORY - Any memory error.
 * MTMFLIX_SUCCESS - Sucess.
 * MTMFLIX_USER_DOES_NOT_EXIST - User does not exist in the given set.
 */
static User getUserByUsername(char* username,MtmFlixResult* result,
                              Set users_set){
    /* Creating a temp user for comparison. */
    User temp_user = userCreate(username,MTM_MIN_AGE+1);
    if(!temp_user){
        /* Couldn't create temp user. */
        *result=MTMFLIX_OUT_OF_MEMORY;
        return NULL;
    }
    SET_FOREACH(SetElement,user,users_set){
        if(userCompare(user,temp_user)==0){
            /* We found the user with the given name. */
            userDestroy(temp_user);
            *result=MTMFLIX_SUCCESS;
            return (User)user;
        }
    }
    /* User with the given username does not exist in the given set. */
    userDestroy(temp_user);
    *result=MTMFLIX_USER_DOES_NOT_EXIST;
    return NULL;
}

/** Rows: 17
 ***** Static function: userAndSeriesExist *****
 * Description: Gets a mtmflix, a name of a user and a name of a series
 * and returns whether or not they exist in the given mtmflix.
 * First the function checks the user and then the series.
 *
 * @param mtmflix - The mtmflix to check in.
 * @param username - A username to check.
 * @param seriesName - A series name to check.
 *
 * @return
 * MTMFLIX_OUT_OF_MEMORY - Any memory error.
 * MTMFLIX_USER_DOES_NOT_EXIST - User doesn't exist in the mtmflix.
 * MTMFLIX_SERIES_DOES_NOT_EXIST - Series doesn't exist in
 * the mtmflix.
 * MTMFLIX_SUCCESS - Both user and series exist in the mtmflix.
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
    Series dummy_series = seriesCreate((char*)seriesName,1,HORROR,NULL,5);
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
    /* Both user and series exist. */
    userDestroy(dummy_user);
    seriesDestroy(dummy_series);
    return MTMFLIX_SUCCESS;
}

/** Rows: 15
 ***** Static function: usersExist *****
 * Description: Gets two usernames and checks if the users exist in
 * the mtmflix.
 *
 * @param mtmflix - The mtmflix to check in.
 * @param username1 - A username to check.
 * @param username2 - A username to check.
 *
 * @return
 * MTMFLIX_OUT_OF_MEMORY - Any memory error.
 * MTMFLIX_USER_DOES_NOT_EXIST - At least one users doesn't exist.
 * MTMFLIX_SUCCESS - Both of the users exist in the mtmflix.
 */
static MtmFlixResult usersExist(MtmFlix mtmflix, const char* username1,
                                const char* username2){
    /* Creating temporary users for comparison. */
    User dummy_user1 = userCreate(username1,MTM_MIN_AGE);
    if(!dummy_user1){
        return MTMFLIX_OUT_OF_MEMORY;;
    }
    User dummy_user2 = userCreate(username2,MTM_MIN_AGE);
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
    /* Both users exist. */
    userDestroy(dummy_user1);
    userDestroy(dummy_user2);
    return MTMFLIX_SUCCESS;
}

/** rows: 27
 ***** Static function: rankSeriesAndAddToRankedSeriesSet *****
 * Description: Ranks the given series (single series) and inserts it to a
 * ranked series set.
 *
 * Notice: This resets the iterator of the series set and users set.
 *
 * @param users_set - Set of all the users in the mtmflix.
 * @param series_set - Set of all the series in the mtmflix.
 * @param user - User we want to rank according to.
 * @param series - Series we want to rank.
 * @param genre
 * @param function_status
 * @param ranked_series_set
 */
static void rankSeriesAndAddToRankedSeriesSet(Set users_set,Set series_set,
     User user,Series series, Genre genre, MtmFlixResult* function_status,
                                                    Set ranked_series_set){
    char* series_name = seriesGetName(series);
    if(!series_name){
        /* Failed to copy name. */
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
                        genre,function_status); // Ranking the series.
    if(*function_status!=MTMFLIX_SUCCESS){
        /* Failed to rank. */
        free(series_name);
        free(series_genre_string);
        return;
    }
    /* Creating a ranked series and inserts it to the set
     * of ranked series.*/
    RankedSeries new_ranked_series = rankedSeriesCreate
                                 (rank,series_name,series_genre_string);
    free(series_name);
    free(series_genre_string);
    if(!new_ranked_series){
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return;
    }
    SetResult result = setAdd(ranked_series_set,new_ranked_series);
    rankedSeriesDestroy(new_ranked_series); // Destroying copy.
    if(result!=SET_SUCCESS){
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return;
    }
}

/** rows: 3
 ***** Static function: doubleAbs *****
 * Description: Gets a number (double) and returns its distance from zero
 * AKA absolute value.
 *
 * @param number - Number we check.
 *
 * @return
 * Distance of the number from zero.
 */
static double doubleAbs (double number){
    if(number<0){
        return (-number);
    }
    return number;
}

/** Rows: 19
 ***** Static function: rankSeries *****
 * Description: Ranks the given series according to the given user.
 *
 * @param users_set - Set of all users in the mtmflix.
 * @param user - User we want to rank the sereis according to.
 * @param series_name - Name of the series we want to rank. This will save
 * us the trouble of making another copy of the name of the series.
 * @param series - Series we want to rank.
 * @param series_set - Set of all the series in the mtmflix.
 * @param genre - Genre of the series we rank.
 * @param function_status - Will hold success/fail status of the function.
 *
 * @return
 * ILLEGAL_VALUE - In case of any error.
 * Else - The rank of the series.
 */
static int rankSeries(Set users_set,User user,
                      char* series_name,Series series,Set series_set,
                      Genre genre,MtmFlixResult* function_status){
    /* "G" - Checks how many series from user's favorite list has the same
     * genre as the given series.*/
    int same_genre = userHowManySeriesWithGenre(series_set,user,genre);
    if(same_genre == ILLEGAL_VALUE){
        /* Failed to check how many from the same genre. */
        *function_status=MTMFLIX_OUT_OF_MEMORY;
        return ILLEGAL_VALUE;
    }
    /* Creating a copy of the series set in order to not ruin the iterator
     * for the caller function. */
    Set series_set_copy = setCopy(series_set);
    if(!series_set_copy){
        return ILLEGAL_VALUE;
    }
    /* "L" - Checks the average episode duration of all of user's favorite
     * series. */
    double average_list_episode_duration=
       userGetAverageEpisodeDuration(user,series_set_copy,function_status);
    setDestroy(series_set_copy);
    if(*function_status!=MTMFLIX_SUCCESS){
        /* Failed to check average episode duration of all series in user's
         * favorite series list. */
        return ILLEGAL_VALUE;
    }
    /* "F" - Checks how many friends loved this series. */
    int number_of_friends_loved_this_series=
            howManyFriendsLovedThisSeries(users_set,user,series_name);
    /* "CUR" - Checks current series episode duration. */
    int current_series_episode_duration = seriesGetEpisodeDuration(series);
    double rank=(same_genre*number_of_friends_loved_this_series);
    rank/=(1+doubleAbs((double)current_series_episode_duration-
                       (average_list_episode_duration)));
    return (int)rank;
}

/** Rows: 18
 ***** Static function: seriesShouldBeRecommended *****
 * Description: Returns whether or not the given series should be ranked
 * for given user. The function checks if the user meet the age
 * requirements of the series, and if so, checks if the series is already
 * in his favorite list.
 *
 * @param series - Series we check.
 * @param user - User we check according to.
 * @param mtmflix - The mtmflix in which it all happens.
 * @param result - Will hold success/fail status of the function.
 *
 * @return
 */
static bool seriesShouldBeRecommended(Series series,User user,
                                      MtmFlix mtmflix,
                                      MtmFlixResult* result) {
    char *series_name = seriesGetName(series); //todo: free series_name
    if (!series_name) {
        *result=MTMFLIX_OUT_OF_MEMORY;
        return false;
    }
    SeriesResult status;
    /* Checking age limitations of series vs user's age. */
    bool user_can_watch=userCanWatchSeries(mtmflix, user, series,&status);
    if(status!=SERIES_SUCCESS){
        /* UserCanWatch failed. */
        free(series_name);
        *result = MTMFLIX_OUT_OF_MEMORY;
        free(series_name);
        return false;
    }
    if ((isInUsersFavoriteSeriesList(user, series_name))||!user_can_watch){
        /*If we get here the series is already in the user's favorite
          series list or the user's age is not in the range of age
          limitations of the series. This series shouldn't be
          recommended*/
        free(series_name);
        *result=MTMFLIX_SUCCESS;
        return false;
    }
    /* User meets age requirements and also doesn't have the series in his
     * favorite list. This mean that the given series should be ranked. */
    free(series_name);
    *result=MTMFLIX_SUCCESS;
    return true;
}

/** Rows: 18
 ***** Static function: rankAllSeriesForUser *****
 * Description: Makes the ranking of all the relevant series for the given
 * user and prints it to the give file.
 *
 * @param mtmflix - The mtmflix we are working in.
 * @param user - User that should rank according to.
 * @param ranked_series_set - Set of ranked series.
 * @param outputStream - File to print to the ranked series.
 * @param count - How many series to print from each genre.
 * @param series_set - a copy of all the series in the mtmflix. This is
 * in order to not ruin the iterator for the caller function.
 *
 * @return
 * MTMFLIX_OUT_OF_MEMORY - Any memory error.
 * MTMFLIX_SUCCESS - All relevant series were ranked and printed
 * successfully.
 */
static MtmFlixResult rankAllSeriesForUser(MtmFlix mtmflix,User user,
       Set ranked_series_set,FILE* outputStream, int count,Set series_set){
    MtmFlixResult result;
    SET_FOREACH(SetElement,series,mtmflix->series){
        if(!seriesShouldBeRecommended(series,user,mtmflix,&result)) {
            if(result!=MTMFLIX_SUCCESS) {
                return MTMFLIX_OUT_OF_MEMORY;
            }
            /* Series shouldn't be recommended. */
            continue;
        }
        if(result!=MTMFLIX_SUCCESS) {
            return MTMFLIX_OUT_OF_MEMORY;
        }
        /*If we got here the current series should be added to the set of
         * recommended series */
        rankSeriesAndAddToRankedSeriesSet(mtmflix->users,series_set,
                                          user,series,
                                          seriesGetGenre(series),
                                          &result,ranked_series_set);
        if(result!=MTMFLIX_SUCCESS){
            return MTMFLIX_OUT_OF_MEMORY;
        }
    }
    rankedSeriesPrintToFile(count,ranked_series_set,outputStream,&result);
    if(result!=MTMFLIX_SUCCESS){
        return MTMFLIX_OUT_OF_MEMORY;
    }
    return MTMFLIX_SUCCESS;
}