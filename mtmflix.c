#include "mtmflix.h"
#include "map.h"
#include "series.h"
#include "utilities.h"
#include "user.h"

static bool usernameCheck (const char* username);

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
    //todo: See todo's above usernameCheck function.
    if(!usernameCheck(username)){
        return MTMFLIX_ILLEGAL_USERNAME;
    }
    if(age<MTM_MIN_AGE || age>MTM_MAX_AGE){
        return MTMFLIX_ILLEGAL_AGE;
    }

}

//todo: Add comments!
//todo: Change function name to usernameIsValid (because the answer is yes or no).
//todo: Check if username is already exist in this mtmFlix.
/**
 ***** Function: usernameIsValid *****
 * @param username
 * @return
 */
static bool usernameCheck (const char* username){

    bool contain_letter=false;
    while(*username){
        if(*username<'0' || (*username>'9' && *username<'A') ||
                (*username>'Z' && *username<'a') || *username>'z'){
            return false;
        }
        if ((*username>'A' && *username<'Z') ||
            (*username>'a' && *username<'z')){
            contain_letter=true;
        }
        username++;
    }
    return contain_letter;
}

MtmFlixResult mtmFlixRemoveUser(MtmFlix mtmflix, const char* username){
    
}


