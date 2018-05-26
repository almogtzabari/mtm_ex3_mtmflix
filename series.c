#include "series.h"
#include "mtm_ex3.h"


#define ILLEGAL_VALUE -1

//-----------------------------------------------------------------------//
//                SERIES: STATIC FUNCTIONS DECLARATIONS                  //
//-----------------------------------------------------------------------//

static int* seriesInsertAgeLimit(int *ages, SeriesResult *status);
static int getGenrePosition(Genre genre);

//-----------------------------------------------------------------------//
//                            SERIES: STRUCT                             //
//-----------------------------------------------------------------------//

struct series_t{
    char* series_name;
    int number_of_episodes;
    Genre genre;
    int* ages;
    int episode_duration;
};

//-----------------------------------------------------------------------//
//                       SERIES: FUNCTIONS                               //
//-----------------------------------------------------------------------//

/** Rows: 20
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
                    Genre genre, int* ages, int episode_duration){

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
    strcpy(name,series_name);
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

/** Rows: 8
 ***** Function: seriesCopy *****
 * Description: Creates a copy of the given series.
 *
 * @param series - A series to create a copy of.
 * @return - A copy of the given series or NULL in case of failure.
 */
Series seriesCopy(Series series){
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

/** Rows: 7
 ***** Function: seriesCopyName *****
 * Description: Creates a copy of the given series name.
 *
 * @param name - Name we want to copy.
 *
 * @return
 * If success - A copy of the given name.
 * Else - NULL.
 */
char* seriesCopyName(char *name){
    if(!name){
        /* NULL argument. */
        return NULL;
    }
    char* name_copy = malloc(strlen(name)+1);
    if(!name_copy){
        /* Name copy memory allocation failed. */
        return NULL;
    }
    strcpy(name_copy,name);
    return name_copy;
}

/** Rows: 5
 ***** Function: seriesDestroy *****
 * Description: Free all allocated memory of a given series.
 *
 * @param series - Series we want to destroy.
 */
void seriesDestroy(Series series){
    if(!series){
        return;
    }
    free(series->series_name);
    free(series->ages);
    free(series);
}

/** Rows: 9
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
int seriesCompare(Series series1, Series series2){
    if(!strcmp(series1->series_name,series2->series_name)){
        /* Series has the same name. This is in order to check if a series
         * exist in a set using its name only. */
        return 0;
    }
    assert(series1 && series2);
    int genre1_position = getGenrePosition(series1->genre);
    int genre2_position = getGenrePosition(series2->genre);
    int genre_diff = genre1_position-genre2_position;
    if(genre_diff){
        /* The series has different genres. */
        return genre_diff;
    }
    /* Two series has the same genre. */
    return strcmp(series1->series_name,series2->series_name);
}

/** Rows: 1
 ***** Function: seriesDestroyName *****
 * Description: Free all allocated memory for the name of a series.
 *
 * @param name - Name we want to destroy.
 */
void seriesDestroyName(char* name){
    free(name);
}

/** Rows: 3
 ***** Function: seriesHasAgeRestrictions *****
 * Description: Returns whether or not a series has age restrictions.
 *
 * @param series - Series we want to check if has age restrictions.
 *
 * @return
 * True - Has age restrictions.
 * False - Doesn't have age restrictions.
 */
bool seriesHasAgeRestrictions(Series series){
    if(!series->ages){
        return false;
    }
    return true;
}

/** Rows: 1
 ***** Function: seriesGetMaxAge *****
 * Description: Returns the maximum age allowed of given series.
 *
 * @param series - Series we want to get its max age restrictions.
 *
 * @return
 * Integer - Maximum age allowed for given series.
 */
int seriesGetMaxAge(Series series){
    assert(series);
    assert(series->ages);
    return series->ages[1];
}

/** Rows: 1
 ***** Function: seriesGetMinAge *****
 * Description: Returns the minimum age allowed of given series.
 *
 * @param series - Series we want to get its min age restrictions.
 *
 * @return
 * Integer - Minimum age allowed for given series.
 */
int seriesGetMinAge(Series series){
    assert(series);
    assert(series->ages);
    return series->ages[0];
}


/** Rows: 2
 ***** Function: seriesGetEpisodeDuration *****
 * Description: returns the average episode duration of a given series.
 *
 * @param series - Series we want to get its average episode duration.
 *
 * @return
 * Average episode duration of given series.
 */
int seriesGetEpisodeDuration (Series series){
    assert(series);
    return series->episode_duration;
}

/** Rows: 2
 ***** Function: seriesGetGenre *****
 * Description: returns the genre of a given series.
 *
 * @param series - Series we want to get its genre.
 *
 * @return
 * Genre of given series.
 */
Genre seriesGetGenre (Series series){
    assert(series);
    return series->genre;
}

/** Rows: 11
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
                                      FILE* outputStream){
    char* series_genre_string = getGenreNameByEnum(series->genre);
    if(!series_genre_string){
        /* Failed to get genre. */
        return SERIES_MEMORY_ALLOCATION_FAILED;
    }
    const char* series_details = mtmPrintSeries(series->series_name,
                                                series_genre_string);
    free(series_genre_string);
    if(!series_details){
        return SERIES_MEMORY_ALLOCATION_FAILED;
    }
    if(fprintf(outputStream,"%s",series_details)<0){
        return SERIES_MEMORY_ALLOCATION_FAILED;
    }
    return SERIES_SUCCESS;
}

/** Rows: 5
 ***** Function: seriesGetName *****
 * Description: Returns the name of a given series.
 * @param series - Series we want to get its name.
 * @return
 * If success - Name of given series.
 * Else - NULL.
 */
char* seriesGetName (Series series){
    char* series_name_copy=malloc(strlen(series->series_name)+1);
    if(!series_name_copy){
        return NULL;
    }
    strcpy(series_name_copy,series->series_name);
    return series_name_copy;
}

/** Rows: 6
 ***** Function: getGenreNameByEnum *****
 * Description: Converts genre to the string that represents the genre.
 *
 * @param genre - A number that represents a genre.
 * @return
 * NULL in case of memory allocation error, else the string represents the
 * number of genre.
 */
char* getGenreNameByEnum(Genre genre){
    const char* genres[] = { "SCIENCE_FICTION", "DRAMA", "COMEDY", "CRIME",
                             "MYSTERY","DOCUMENTARY", "ROMANCE", "HORROR"};
    char* genre_string = malloc(strlen(genres[genre])+1);
    if(!genre_string){
        return NULL;
    }
    strcpy(genre_string,genres[genre]);
    return genre_string;
}

/** Rows: 12
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
                           SeriesResult* status){
    assert(series_name);
    assert(series_set);
    Series dummy_series = seriesCreate(series_name,1,HORROR,NULL,4);
    if(!dummy_series){
        *status = SERIES_MEMORY_ALLOCATION_FAILED;
        return HORROR; // This value won't be checked.
    }
    SET_FOREACH(SetElement,current_series,series_set){
        if(seriesCompare(current_series,dummy_series)==0){
            /* We found the series with the given name. */
            seriesDestroy(dummy_series);
            *status = SERIES_SUCCESS;
            return ((Series)current_series)->genre;
        }
    }
    /* We shouldn't get here. */
    seriesDestroy(dummy_series);
    *status = SERIES_MEMORY_ALLOCATION_FAILED; // Series doesn't exist.
    return HORROR;

}

/** Rows: 12
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
                            SeriesResult *status){
    Series dummy_series = seriesCreate(series_name,1,HORROR,NULL,2);
    if(!dummy_series){
        /* Memory allocation failed. */
        *status = SERIES_MEMORY_ALLOCATION_FAILED;
        return ILLEGAL_VALUE; // This value won't be checked.
    }
    SET_FOREACH(SetElement,current_series,series_set){
        if(seriesCompare(current_series,dummy_series)==0){
            /* We fount the series with the given name. */
            *status = SERIES_SUCCESS;
            seriesDestroy(dummy_series);
            return ((Series)current_series)->episode_duration;
        }
    }
    /*  Shouldn't get here. */
    seriesDestroy(dummy_series);
    *status = SERIES_MEMORY_ALLOCATION_FAILED; // Series doesn't exist.
    return ILLEGAL_VALUE; // This value won't be checked.
}

//-----------------------------------------------------------------------//
//                      SERIES: STATIC FUNCTIONS                         //
//-----------------------------------------------------------------------//

/** Rows: 16
 ***** Static function: seriesSetAgeLimit *****
 * Description: This function will be used at seriesCreate.
 * The function gets an array of ages or NULL. If NULL, there are no age
 * limitations. Else, the first cell will contain minimum age limit and the
 * second cell will contain maximum age limit. In case not NULL, the
 * function will duplicate the array with a slight change: If the maximum
 * age in the array is higher than MtmFlix maximum age then the new array
 * will contain MtmFlix maximum age. If the minimum age in the array is
 * lower than MtmFlix minimum age then the new array will contain MtmFlix
 * minimum age.
 *
 * @param ages - Array of age limitations. If NULL there are no limitaions.
 * @return
 * Array of final age restrictions.
 */
static int* seriesInsertAgeLimit(int* ages, SeriesResult *status) {
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

/** Rows: 2
 ***** Static function: getGenrePosition *****
 * Description: Gets a genre and returns its lexicographical position.
 * ascii value.
 *
 * //------ Genre ------ Enum ------ Lex oreder
 * //  SCIENCE_FICTION     0            7
 * //  DRAMA               1            3
 * //  COMEDY              2            0
 * //  CRIME               3            1
 * //  MYSTERY             0            5
 * //  DOCUMENTARY         1            2
 * //  ROMANCE             2            6
 * //  HORROR              3            4
 *
 * @param genre - The genre we want to get its position.
 *
 * @return
 * Integer indicates the position of the genre.
 */
static int getGenrePosition(Genre genre){
    int genres_position[NUMBER_OF_GENRES] = {7,3,0,1,5,2,6,4};
    return genres_position[genre];
}

