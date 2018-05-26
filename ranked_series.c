#include <malloc.h>
#include <memory.h>
#include "ranked_series.h"


//-----------------------------------------------------------------------//
//                        RANKED SERIES: STRUCT                          //
//-----------------------------------------------------------------------//


struct ranked_series_t{
    int rank;
    char* series_name;
    char* series_genre;
};


//-----------------------------------------------------------------------//
//                        RANKED SERIES: FUNCTIONS                       //
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
RankedSeries rankedSeriesCreate (int rank, char* series_name,
                                 char* series_genre){
    RankedSeries new_ranked_series=malloc(sizeof(*new_ranked_series));
    if(!new_ranked_series){
        return NULL;
    }
    char* series_name_copy=malloc(strlen(series_name)+1);
    if(!series_name_copy){
        free(new_ranked_series);
        return NULL;
    }
    strcpy(series_name_copy,series_name);
    new_ranked_series->series_name=series_name_copy;
    char* series_genre_copy=malloc(strlen(series_genre)+1);
    if(!series_genre_copy){
        free(series_name_copy);
        free(new_ranked_series);
        return NULL;
    }
    strcpy(series_genre_copy,series_genre);
    new_ranked_series->series_genre=series_genre_copy;
    new_ranked_series->rank=rank;
    return new_ranked_series;
}

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
RankedSeries rankedSeriesCopy (RankedSeries ranked_series){
    RankedSeries ranked_series_copy=rankedSeriesCreate(ranked_series->rank,
            ranked_series->series_name,ranked_series->series_genre);
    if(!ranked_series_copy){
        return NULL;
    }
    return ranked_series_copy;
}

/**
 ***** Function : rankedSeriesDestroy *****
 * Description: Deallocates all recources of a ranked series.
 *
 * @param ranked_series - Ranked series to destroy.
 */
void rankedSeriesDestroy (RankedSeries ranked_series){
    if(!ranked_series){
        return;
    }
    free(ranked_series->series_genre);
    free(ranked_series->series_name);
    free(ranked_series);
}

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
                         RankedSeries ranked_series2){
    if((ranked_series1->rank)!=(ranked_series2->rank)){
        return (ranked_series2->rank)-(ranked_series1->rank);
    }
    return strcmp(ranked_series1->series_name,ranked_series2->series_name);
}

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
                             FILE* outputStream,MtmFlixResult* result){
    if(number_of_series_to_print==0){
        number_of_series_to_print=setGetSize(ranked_series_set);
    }
    SET_FOREACH(RankedSeries,ranked_series,ranked_series_set){
        if(ranked_series->rank==0){
            continue;
        }
        if(number_of_series_to_print>0){
            const char* ranked_series_details=mtmPrintSeries
                    (ranked_series->series_name,
                     ranked_series->series_genre);
            if(!ranked_series_details){
                *result=MTMFLIX_OUT_OF_MEMORY;
                return;
            }
            if((fprintf(outputStream,"%s",ranked_series_details)<0)){
                *result=MTMFLIX_OUT_OF_MEMORY;
                return;
            }
        }
        number_of_series_to_print--;
    }
}