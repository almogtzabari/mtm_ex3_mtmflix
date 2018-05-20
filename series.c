#include "series.h"
#include "mtm_ex3.h"




//-----------------------------------------------------------------------//
//                   STATIC FUNCTIONS DECALARATIONS                      //
//-----------------------------------------------------------------------//

static int* seriesInsertAgeLimit(int *ages, SeriesResult *status);

//-----------------------------------------------------------------------//
//                            STRUCT SERIES                              //
//-----------------------------------------------------------------------//

struct series_t{
    char* series_name;
    int number_of_episodes;
    Genre genre;
    int* ages;
    double episode_duration;
};

//-----------------------------------------------------------------------//
//                       STRUCT SERIES FUNCTIONS                         //
//-----------------------------------------------------------------------//

/**
 ***** Function: seriesCreate *****
 * Description: Creates a new series.
 * @param series_name - Name of the series.
 * @param number_of_episodes - Number of episodes of the series.
 * @param genre - Genre of the series.
 * @param ages - Array of age limitations. The size of the array is 2. The
 * First cell is the minimum age and the second cell is maximum age.
 * If ages is NULL there is no age limitaions.
 * @param episode_duration - Average duration of episode in series.
 * @return - New series or NULL in case of fail.
 */
Series seriesCreate(const char* series_name, int number_of_episodes,
                    Genre genre, int* ages, double episode_duration){

    Series series = malloc(sizeof(*series));
    if(!series){
        /* Series memory allocation failed. */
        return NULL;
    }
    char* name = malloc(strlen(series_name)+1);
    if(!name){
        /* Name memory allocation failed. */
        free(series);
        return NULL;
    }
    SeriesResult status;
    int* series_age_limit = seriesInsertAgeLimit(ages, &status);
    if(status != SERIES_SUCCESS){
        /* Couldn't allocate memory for ages array of series. */
        free(name);
        free(series);
        return NULL;
    }
    series->ages = series_age_limit;
    series->series_name = name;
    series->genre = genre;
    series->episode_duration = episode_duration;
    series->number_of_episodes = number_of_episodes;
    return series;
}

/**
 ***** Function: copySeries *****
 * Description: Creates a copy of the given series.
 * @param series - A series to create a copy of.
 * @return - A copy of the given series or NULL in case of failure.
 */
Series copySeries(Series series){
    if(!series){
        /* NULL argument.*/
        return NULL;
    }
    Series series_copy = seriesCreate(series->series_name,
            series->number_of_episodes,series->genre,series->ages,
                                      series->episode_duration);

    if(!series_copy){
        /* Series create failed. */
        return NULL;
    }
    return series_copy;
}



char* copySeriesName(char* name){
    if(!name){
        /* NULL argument. */
        return NULL;
    }
    char* name_copy = malloc(sizeof(strlen(name)+1));
    if(!name_copy){
        /* Name copy memory allocation failed. */
        return NULL;
    }
    return strcpy(name_copy,name);
}

/**
 ***** Function: seriesDestroy *****
 * Description: Free all allocated memory of a given series.
 * @param series - Series we want to destroy.
 */
void seriesDestroy(Series series){
    free(series->series_name);
    free(series->ages);
    free(series);
}

int compareSeries(Series series1, Series series2){

}

/** //todo: what about bad min/max input?
 ***** Function: seriesSetAgeLimit *****
 * Description: This function will be used at seriesCreate.
 * The function gets an array of ages or NULL. If NULL, there are no age
 * limitations. Else, the first cell will contain minimum age limit and the
 * second cell will contain maximum age limit. In case not NULL, the
 * function will duplicate the array with a slight change: If the maximum
 * age in the array is higher than MtmFlix maximum age then the new array
 * will contain MtmFlix maximum age. If the minimum age in the array is
 * lower than MtmFlix minimum age then the new array will contain MtmFlix
 * minimum age.
 * @param ages - Array of age limitations. If NULL there are no limitaions.
 * @return - A
 */
static int* seriesInsertAgeLimit(int *ages, SeriesResult *status) {
    if (ages) {
        /* There is an age limitations */
        int *series_ages = malloc(sizeof(ages));
        if (!series_ages) {
            /* Ages array memory allocation failed. */
            *status = SERIES_MEMORY_ALLOCATION_FAILED;
            return NULL;
        }
        if (ages[0] < MTM_MIN_AGE) {
            /* Minimum age of series is lower then MtmFlix minimum age. */
            series_ages[0] = MTM_MIN_AGE;
        } else {
            series_ages[0] = ages[0];
        }
        if (ages[1] > MTM_MAX_AGE) {
            /* Maximum age of series is higher then MtmFlix maximum age. */
            series_ages[1] = MTM_MAX_AGE;
        } else {
            series_ages[1] = ages[1];
        }
        *status = SERIES_SUCCESS;
        return series_ages;
    }
    /* There is no age limitation. */
    *status = SERIES_SUCCESS;
    return NULL;
}


