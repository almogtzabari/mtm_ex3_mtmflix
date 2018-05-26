#ifndef MTM_EX3_MTMFLIX_USER_H
#define MTM_EX3_MTMFLIX_USER_H

#include "set.h"
#include "series.h"
#include "utilities.h"

//-----------------------------------------------------------------------//
//                      USER: TYPEDEFS AND DEFINES                       //
//-----------------------------------------------------------------------//

#define ILLEGAL_VALUE -1

typedef enum {
    FRIENDS_LIST,
    FAVORITE_SERIES_LIST
} UserList;

typedef enum {
    USER_SUCCESS,
    USER_OUT_OF_MEMORY,
    USER_NULL_ARGUMENT,
    USER_ILLEGAL_VALUE,
    USER_ALREADY_EXISTS
} UserResult;

typedef struct user_t* User;

//-----------------------------------------------------------------------//
//                   USER: FUNCTIONS DECLARATIONS                       //
//-----------------------------------------------------------------------//

/**
 ***** Function: userCreate *****
 * Description: Creates a new user.
 *
 * @param username - String with the user's name.
 * @param age - Age of the user.
 *
 * @return
 * A pointer to a new user or NULL in case of memory error or illegal age.
 */
User userCreate(const char* username, int age);

/**
 ***** Function: userCopy *****
 * Description: Copies a given user.
 *
 * @param user - A user to copy.
 *
 * @return
 * A new user or NULL in case of failure.
 */
User userCopy (User user);

/**
 ***** Function: userDestroy *****
 * Description: Deallocates an existing user.
 *
 * @param user - A user to destroy.
 */
void userDestroy (User user);

/**
  ***** Function: userCompare *****
 * Description: Compares between two user's usernames.
 *
 * @param user1 - First user.
 * @param user2 - Second user.
 *
 * @return
 * Positive Integer - User1's name is bigger than user2's name.
 * Negative Integer - User2's name is bigger than user1's name.
 * Zero - Both names are equal.
 */
int userCompare (User user1, User user2);

/**
 ***** Function: usernameCopy *****
 * Description: Copying a friend's name.
 *
 * @param username - A friend's name to copy.
 *
 * @return
 * A copy of the given friend's name.
 */
char* usernameCopy(char* user);

/**
 ***** Function: destroyUsername *****
 * Description: Deallocates the memory used for a friend's name.
 *
 * @param user - A friend's name to destroy.
 */
void destroyUsername (char* friend_username);

/**
 ***** Function: userPrintDetailsToFile *****
 * Description: Gets a user and prints its details to a given file.
 *
 * @param current_user - The user which his details will be printed to the
 * file.
 * @param outputStream - A file to print to.
 *
 * @return
 * USER_OUT_OF_MEMORY - Any memory error.
 * USER_SUCCESS - Successfully printed.
 */
UserResult userPrintDetailsToFile(User current_user, FILE *outputStream);

/**
 ***** Function: userGetAge *****
 * Description: Returns the age of given user.
 *
 * @param user - User to get its age.
 *
 * @return
 * Age of given user.
 */
int userGetAge (User user);

/**
 ***** Function: howManyFriendsLovedThisSeries *****
 * Description: Returns how many friends of a given user loves a given
 * series.
 *
 * @param users_set - Set which contains all users in mtmflix.
 * @param user - A user to check with his friends.
 * @param series_name - A series name to search on friends' favorite lists.
 *
 * @return
 * The number of friends that loved the given series.
 */
int howManyFriendsLovedThisSeries(Set users_set, User user,
                                  char *series_name);

/**
 ***** Function: userHowManySeriesWithGenre *****
 * Description: Returns the number of series in user's favorite-series-list
 * with the same genre as the given genre.
 *
 * @param user - The user we want to check his favorite-series-list.
 * @param genre - The genre we are looking for.
 * @param series_set - Set of all the series in the mtmflix.
 *
 * @return
 * If succeeded - Number of series with same genre in user's
 * favorite-series-list.
 * If fails - Will return ILLEGAL_VALUE.
 */
int userHowManySeriesWithGenre(Set series_set, User user, Genre genre);

/**
 ***** Function: userGetAverageEpisodeDuration *****
 * Description: Gets a user, a status and a set of all the series in the
 * system. The function returns the average duration of episodes of user's
 * favorite series.
 *
 * @param user - The user we want to check his favorite series for
 * the calulation.
 * @param series_set - Set of all the series in the system.
 * @param function_status - Will hold information of success/failure of the
 * function.
 *
 * @return
 * If succeeded - Average episode duration of all series in user's
 * favorite-series-list.
 * If fails - returns ILLEGAL_VALUE.
 */
double userGetAverageEpisodeDuration(User user, Set series_set,
                                     MtmFlixResult* function_status);

/**
 ***** Function: isInUsersFavoriteSeriesList *****
 * Description: Returns whether or not a given series name is in given
 * user's favorite series list.
 *
 * @param user - A user to check in his favorite series list.
 * @param series_name - Series name to check for.
 *
 * @return
 * True - Series name does exist in given user's favorite series list.
 * False - Series name doesn't exist in given user's favorite series list.
 */
bool isInUsersFavoriteSeriesList(User user,char* series_name);

/**
 ***** Static function: userRemoveFromList *****
 * Description: remove a given name from a given list.
 *
 * @param list - List to remove from.
 * @param name - Name to remove from list.
 */
void removeFromList(User user,char* name,UserList list_type);

/**
 ***** Function: addNameToUsersList *****
 * Description: Adds a given name to a specified list of a given user.
 *
 * @param user - User we want to add to one of his lists.
 * @param name - Name we want to add to list.
 * @param list_type - Which of given user's lists to add to.
 * @return
 */
MtmFlixResult addNameToUsersList(User user,char *name,UserList list_type);

#endif //MTM_EX3_MTMFLIX_USER_H

