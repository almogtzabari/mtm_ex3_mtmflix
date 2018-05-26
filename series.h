
#ifndef MTM_EX3_MTMFLIX_SERIES_H
#define MTM_EX3_MTMFLIX_SERIES_H

#include "map.h"
#include <malloc.h>
#include <string.h>
#include "set.h"
#include "mtm_ex3.h"
#include <assert.h>

//-----------------------------------------------------------------------//
//                    SERIES: TYPEDEFS AND DEFINES                       //
//-----------------------------------------------------------------------//
#define NUMBER_OF_GENRES 8
#define ILLEGAL_VALUE -1

typedef struct series_t* Series;

typedef enum SeriesResult_t {
    SERIES_MEMORY_ALLOCATION_FAILED,
    SERIES_SUCCESS,
    SERIES_NULL_ARGUMENT,
    SERIES_ALREADY_EXISTS
} SeriesResult;


//-----------------------------------------------------------------------//
//                   SERIES: FUNCTIONS DECLARATIONS                      //
//-----------------------------------------------------------------------//

/**
 ***** Function: seriesCopy *****
 * Description: Creates a copy of the given series.
 *
 * @param series - A series to create a copy of.
 * @return - A copy of the given series or NULL in case of failure.
 */
Series seriesCopy(Series series);

/**
 ***** Function: seriesCopyName *****
 * Description: Creates a copy of the given series name.
 *
 * @param name - Name we want to copy.
 *
 * @return
 * If success - A copy of the given name.
 * Else - NULL.
 */
char* seriesCopyName(char *name);

/**
 ***** Function: seriesDestroy *****
 * Description: Free all allocated memory of a given series.
 *
 * @param series - Series we want to destroy.
 */
void seriesDestroy(Series series);

/**
 ***** Function: seriesCompare *****
 * Description: Compare between two series.
 *
 * @param series1 - First series to compare.
 * @param series2 - second series to compare.
 *
 * @return
 * Postive Number - Series1 is bigger then series2.
 * Negative Number - Series2 is bigger then series1.
 * Zero - Series are equal.
 */
int seriesCompare(Series series1, Series series2);

/**
 ***** Function: seriesCreate *****
 * Description: Creates a new series.
 *
 * @param series_name - Name of the series.
 * @param number_of_episodes - Number of episodes of the series.
 * @param genre - Genre of the series.
 * @param ages - Array of age limitations. The size of the array is 2. The
 * First cell is the minimum age and the second cell is maximum age.
 * If ages is NULL there is no age limitaions.
 * @param episode_duration - Average duration of episode in series.
 *
 * @return - New series or NULL in case of fail.
 */
Series seriesCreate(char* series_name, int number_of_episodes,
                    Genre genre, int* ages, int episode_duration);

/**
 ***** Function: seriesDestroyName *****
 * Description: Free all allocated memory for the name of a series.
 *
 * @param name - Name we want to destroy.
 */
void seriesDestroyName(char* name);

/**
 ***** Function: seriesGetGenre *****
 * Description: returns the genre of a given series.
 *
 * @param series - Series we want to get its genre.
 *
 * @return
 * Genre of given series.
 */
Genre seriesGetGenre (Series series);

/**
 ***** Function: printSeriesDetailsToFile *****
 * Description: Prints a series name and genre to a file.
 *
 * @param series - Series we want to print to the given file.
 * @param outputStream - The file we want to print into.
 *
 * @return
 * SERIES_MEMORY_ALLOCATION_FAILED - Any memory error.
 * SERIES_SUCCESS - Successfully printed.
 */
SeriesResult printSeriesDetailsToFile(Series series,
                                      FILE* outputStream);

/**
 ***** Function: getGenreNameByEnum *****
 * Description: Converts genre to the string that represents the genre.
 *
 * @param genre - A number that represents a genre.
 * @return
 * NULL in case of memory allocation error, else the string represents the
 * number of genre.
 */
char* getGenreNameByEnum(Genre genre);

/**
 ***** Function: seriesHasAgeRestrictions *****
 * Description: Returns whether or not a series has age restrictions.
 *
 * @param series - Series we want to check if has age restrictions.
 *
 * @return
 * True - Has age restrictions.
 * False - Doesn't have age restrictions.
 */
bool seriesHasAgeRestrictions(Series series);

/**
 ***** Function: seriesGetMaxAge *****
 * Description: Returns the maximum age allowed of given series.
 *
 * @param series - Series we want to get its max age restrictions.
 *
 * @return
 * Integer - Maximum age allowed for given series.
 */
int seriesGetMaxAge(Series series);

/**
 ***** Function: seriesGetMinAge *****
 * Description: Returns the minimum age allowed of given series.
 *
 * @param series - Series we want to get its min age restrictions.
 *
 * @return
 * Integer - Minimum age allowed for given series.
 */
int seriesGetMinAge(Series series);

/**
 ***** Function: seriesGetName *****
 * Description: Returns the name of a given series.
 * @param series - Series we want to get its name.
 * @return
 * If success - Name of given series.
 * Else - NULL.
 */
char* seriesGetName (Series series);

/**
 ***** Function: seriesGetGenreByName *****
 * Description: Gets a series name, a set of all the series in the mtmflix
 * and a status. The function returns the genre of the series with that
 * name.
 *
 * @param series_name - The name of the series we want its genre.
 * @param series_set - Set of all the series in the system.
 * @param status - A status that will be updated at the end of the
 * function indicates if there was a problem.
 * @return
 * Genre of the series with the given name.
 */
Genre seriesGetGenreByName(char* series_name, Set series_set,
                           SeriesResult* status);

/**
 ***** Function: seriesGetDurationByName *****
 * Description: Gets series name, a status and a set of all the series in
 * the system and returns the episode duration of the given series.
 *
 * @param series_name - The series we want to get its episode duration.
 * @param series_set - Set of all the series in the system.
 * @param status - Will hold information of success/failure of the
 * function.
 * @return
 * If succeeded - The episode duration of the series with the given series
 * name.
 * If fails - Returns ILLEGAL_VALUE.
 */
int seriesGetDurationByName(char *series_name, Set series_set,
                            SeriesResult *status);

/**
 ***** Function: seriesGetEpisodeDuration *****
 * Description: returns the average episode duration of a given series.
 *
 * @param series - Series we want to get its average episode duration.
 *
 * @return
 * Average episode duration of given series.
 */
int seriesGetEpisodeDuration (Series series);

#endif //MTM_EX3_MTMFLIX_SERIES_H

