#ifndef MTM_EX3_MTMFLIX_RANKED_SERIES_H
#define MTM_EX3_MTMFLIX_RANKED_SERIES_H

#include <assert.h>
#include "set.h"
#include "mtmflix.h"

//-----------------------------------------------------------------------//
//                 RANKED SERIES: TYPEDEFS AND DEFINES                   //
//-----------------------------------------------------------------------//


typedef struct ranked_series_t* RankedSeries;


//-----------------------------------------------------------------------//
//                     RANKED SERIES: FUNCTIONS                          //
//-----------------------------------------------------------------------//

/**
 ***** Function : rankedSeriesCreate *****
 * Description : Creates a new ranked series.
 *
 * @param rank - Series rank.
 * @param series_name - Series name.
 * @param series_genre - Series genre.
 *
 * @return
 * NULL in case of memory allocation error, else a pointer to a new ranked
 * series.
 */
RankedSeries rankedSeriesCreate (int rank,char* series_name,
                                 char* series_genre);

/**
 ***** Function : rankedSeriesCopy *****
 * Description: Creates a copy of a given ranked series.
 *
 * @param ranked_series - Ranked series to create a copy of.
 *
 * @return
 * NULL - Any memory error.
 * Copy of given ranked series - Else.
 */
RankedSeries rankedSeriesCopy (RankedSeries ranked_series);

/**
 ***** Function : rankedSeriesDestroy *****
 * Description: Deallocates all recources of a ranked series.
 *
 * @param ranked_series - Ranked series to destroy.
 */
void rankedSeriesDestroy (RankedSeries ranked_series);

/**
 ***** Function : rankedSeriesCompare *****
 * Description : Compares between to ranked series by their rank, if the
 * rank is equal, the comparison is made by their names.
 *
 * @param ranked_series1 - A ranked series to compare.
 * @param ranked_series2 - A ranked series to compare.
 *
 * @return
 * If the ranks are different, return a positive integer if the rank of
 * ranked series 2 is higher else a negative integer. If the ranks are
 * equal the comparison is made by strcmp (returns a positive integer if
 * the name of ranked series 1 has a bigger value).
 */
int rankedSeriesCompare (RankedSeries ranked_series1,
                         RankedSeries ranked_series2);

/**
 ***** Function : rankedSeriesPrintToFile *****
 * Description: Prints ranked series to file.
 *
 * @param number_of_series_to_print - The number of series to print to
 * file from each genre. If the number is 0 we need to print all the Ranked
 * series.
 * @param ranked_series_set - Set of all ranked series.
 * @param outputStream - A file to print to.
 * @param result - The status of the function, in case of memory allocation
 * error will be updated to MTMFLIX_OUT_OF_MEMORY.
 */
void rankedSeriesPrintToFile(int number_of_series_to_print,
                             Set ranked_series_set,
                             FILE* outputStream,MtmFlixResult* result);

#endif //MTM_EX3_MTMFLIX_RANKED_SERIES_H
