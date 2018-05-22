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
 * NULL in case of failure.
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
 ***** Function: mtmFlixDestroy *****
 * Description: Gets a MtmFlix and free all resources of it.
 * @param mtmflix - The MtmFlix we want to destroy.
 */
void mtmFlixDestroy(MtmFlix mtmflix){
    setDestroy(mtmflix->users);
    setDestroy(mtmflix->series);
    free(mtmflix);
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
        removeUsernameFromFriendslist(current_user,username);
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
    if(setIsIn(mtmflix->series,temp_series)){
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
        seriesRemoveFromFavoriteSeriesLists(current_user, name);
    }
    return MTMFLIX_SUCCESS;
}

/**
 ***** Function: mtmFlixReportSeries *****
 * Description: Prints series names and genres to a file.
 * The function will print 'seriesNum' series from each genre. The series
 * will be sorted by genre and in each genre sorted by name.
 * @param mtmflix - The mtmflix to print the series from.
 * @param seriesNum - Number of series to print from each genre.
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
        /*Should print all the series found in mtmflix. */
        SET_FOREACH(SetElement,current_series,mtmflix->series){
            // todo: Did you open the file for writing? Did you close it?
            result = printSeriesDetailsToFile(current_series,outputStream);
            if(result != SERIES_SUCCESS){
                /* Print has failed because of memory allocation error. */
                return MTMFLIX_OUT_OF_MEMORY;
            }
        }
    }
    else {
        int prints_left = seriesNum; // Initializes the number of prints.
        /* current_genre will store the genre of the series that are
         * currently being printed. Each time we finished to print a
         * specific genre the current genre will get the next genre in the
         * set. */
        Genre current_genre = seriesGetGenre((Series)setGetFirst
                (mtmflix->users)); // Initializes to genre of first series.
        SET_FOREACH(SetElement,current_series,mtmflix->series){
            if(seriesGetGenre((Series)current_series) == current_genre){
                /* Current series has the same genre as current genre. */
                if (prints_left>0){
                    /* Still need to print series from that genre. */
                    result = printSeriesDetailsToFile(current_series,
                            outputStream);
                    if(result!=SERIES_SUCCESS){
                        /*Print has failed becuase of memory allocation
                         * error. */
                        return MTMFLIX_OUT_OF_MEMORY;
                    }
                    prints_left--;
                }
                else {
                    /* Current series shouldn't be printed because we
                     * already printed enough from that genre. */
                    continue;
                }
            }
            else{
                /* Current series has a new genre. */
                prints_left = seriesNum; // Initializes the number of prints.
                /* Updating the current genre to new genre. */
                current_genre = seriesGetGenre((Series)current_series);
                result = printSeriesDetailsToFile(current_series,
                        outputStream);
                if(result!=SERIES_SUCCESS){
                    /* Print has failed because of memory allocation
                     * error. */
                    return MTMFLIX_OUT_OF_MEMORY;
                }
            }
        }
    }
    return MTMFLIX_SUCCESS;
}
