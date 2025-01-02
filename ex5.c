/******************
Name: Ariel Mizrahi
ID:
Assignment: ex2
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Song {
    char *title;
    char *artist;
    int year;
    char *lyrics;
    int streams;
} Song;

typedef struct Playlist {
    char *name;
    Song **songs;
    int songsNum;
} Playlist;

void freeSong(Song *song);

void freePlaylist(Playlist playlist);

void printPlaylistsMenu();

void printPlaylists(Playlist *playlists, int playlistCounter);

Playlist *addPlaylist(Playlist *playlists, int playlistCounter);

char *readAndAllocate();

void playlistsActions(Playlist *playlists, int playlistNum, int playlistCounter);

void watchPlaylists(Playlist *playlists, int playlistsCounter);

void showChosenPlaylist(Playlist playlist);

void playSong(Playlist playlist);

void addSong(Playlist *playlist);

void play(Playlist playlist);

void sort(Playlist *playlist);

void sortByYear(Playlist *playlist, int swaps);

void sortBySteamsUp(Playlist *playlist, int swaps);

void sortBySteamsDown(Playlist *playlist, int swaps);

void sortAlphabetically(Playlist *playlist, int swaps);

void deleteSong(Playlist *playlist, int deleteIndex);

Playlist *removePlaylist(Playlist *playlists, int deleteIndex, int playlistCounter);


int main() {
    int cases;
    int playlistCounter = 0;
    // playlists is the playlists array that is initialized to null
    Playlist *playlists = NULL;
    // This while loop stops only when case 4 is chosen
    do {
        // printplaylistmenu is called and right after that the buffer is cleared for future inputs
        printPlaylistsMenu();
        scanf("%d", &cases);
        scanf("%*[^\n]");
        scanf("%*c");
        switch (cases) {
            case 1: {
                watchPlaylists(playlists, playlistCounter);
                break;
            }
            case 2: {
                playlists = addPlaylist(playlists, playlistCounter);
                playlistCounter++;
                break;
            }
            case 3: {
                int deleteIndex;
                printf("Choose a playlist:\n");
                printPlaylists(playlists, playlistCounter);
                scanf("%d", &deleteIndex);
                // if the user chose back to menu
                if (deleteIndex == playlistCounter + 1) {
                    break;
                }
                playlists = removePlaylist(playlists, deleteIndex - 1, playlistCounter);
                playlistCounter--;
                break;
            }

            case 4: {
                break;
            }
            default: {
                printf("Invalid option\n");
                break;
            }
        }
    } while (cases != 4);
    for (int i = 0; i < playlistCounter; i++) {
        freePlaylist(playlists[i]);
    }
    free(playlists);
    printf("Goodbye!");
}

void deleteSong(Playlist *playlist, int deleteIndex) {
    // we use free song the chosen song
    freeSong(playlist->songs[deleteIndex]);
    // then we want to move all the songs that come after the song we deleted one place to the left
    for (int i = deleteIndex; i < playlist->songsNum - 1; i++) {
        playlist->songs[i] = playlist->songs[i + 1];
    }
    // decrease songsNum which is the number of song the playlist had by 1
    playlist->songsNum--;
    //then allocate the playlist songs array with the new songNum times the size of song* of memory size
    playlist->songs = realloc(playlist->songs, playlist->songsNum * sizeof(Song *));
    /* we check if the realloc failed but ignore when it fails
    after we deleted the last song(the pointer is supposed to point to null*/
    if (playlist->songs == NULL && playlist->songsNum > 0) {
        printf("Memory reallocation failed.\n");
        exit(1);
    }
    printf("Song deleted successfully.\n");
}

Playlist *removePlaylist(Playlist *playlists, int deleteIndex, int playlistCounter) {
    // we free the chosen playlist
    freePlaylist(playlists[deleteIndex]);
    // then we want to move all the playlists that come after the song we deleted one place to the left
    for (int i = deleteIndex; i < playlistCounter - 1; i++) {
        playlists[i] = playlists[i + 1];
    }
    //then allocate the playlist array with the new playlistCounter -1 times the size of playlist of memory size
    playlists = realloc(playlists,(playlistCounter-1) * sizeof(Playlist));
    /* we check if the realloc failed but ignore when it fails
    after we deleted the last playlist(the pointer is supposed to point to null*/
    if (playlists == NULL && playlistCounter > 0) {
        printf("Memory reallocation failed.\n");
        exit(1);
    }
    printf("Playlist deleted.\n");
    // we return playlists to give the new playlists array to the one we are using
    return playlists;
}

// in this function we free all the pointers that use memory space for that song
void freeSong(Song *song) {
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);
}

// in this function we free all the pointers that use memory space for that playlist
void freePlaylist(Playlist playlist) {
    for (int i = 0; i < playlist.songsNum; i++) {
        freeSong(playlist.songs[i]);
    }
    free(playlist.songs);
    free(playlist.name);
}

void printPlaylistsMenu() {
    printf("Please Choose:\n");
    printf("\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
}

void printPlaylists(Playlist *playlists, int playlistsCounter) {
    // if there are no playlists we print the only option which is to return to main menu
    if (playlists == NULL) {
        printf("\t1. Back to main menu\n");
        return;
    }
    // if there are playlists we print each playlist name
    for (int i = 0; i < playlistsCounter; i++) {
        printf("\t%d. %s\n", i + 1, playlists[i].name);
    }
    printf("\t%d. Back to main menu\n", playlistsCounter + 1);
}

Playlist *addPlaylist(Playlist *playlists, int playlistCounter) {
    /*newPlaylist will get the values from the user and will be added to the playlist array,
    we initialize the pointer to the songs array to null and the number of songs to 0*/
    Playlist newPlaylist;
    newPlaylist.songs = NULL;
    newPlaylist.songsNum = 0;
    printf("Enter playlist's name: \n");
    // tempName gets the name that the user inputs
    char *tempName = readAndAllocate();
    /* then we can allocate memory to the name string in the new playlist
    with the size of tempName +1 for the null terminator*/
    newPlaylist.name = malloc(strlen(tempName) + 1);
    // We check if the malloc has failed
    if (newPlaylist.name == NULL) {
        printf("Memory allocation failed\n");
        free(tempName);
        exit(1);
    }
    // after allocating the right size for the name in new playlist we can copy it from the tempName and free him
    strcpy(newPlaylist.name, tempName);
    free(tempName);
    // now we increase the size of playlist by 1 * size of playlist
    playlists = realloc(playlists, (playlistCounter + 1) * sizeof(Playlist));
    // check if the realloc failed
    if (playlists == NULL) {
        printf("Memory reallocation failed\n");
        exit(1);
    }
    // and insert the new playlist to the playlist array
    playlists[playlistCounter] = newPlaylist;
    return playlists;
}

/* readAndAllocate is called every time we want to get a string from the
user without knowing its size and then allocate the exact amount of memory for that string*/
char *readAndAllocate() {
    /* first give the string size of 1 char (1) then we want to read a new char from
    the user's input and increase the memory size of the string by 1*/
    char *tempString = malloc(sizeof(char));
    // Check if the malloc failed
    if (tempString == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // tempChar will get the one char from the user's string each time
    char tempChar;
    /* stringSize is the counts the times we get 1 char from
    the user's string ending up with the value of the string size*/
    int stringSize = 0;
    // this loop will never stop unless we break from the inside which we do when we reach the end of the string
    while (1) {
        // read one char
        scanf("%c", &tempChar);
        // Stop on newline
        if (tempChar == '\n') {
            break;
        }
        /* we store the char into tempstring in the stringsize index which was initialized at 0
         and grows by 1 each time we enter a char*/
        tempString[stringSize] = tempChar;
        stringSize++;
        // create a new char array called temp so in the case that the realloc fails mid-string we won't lose the values
        char *temp = realloc(tempString, (stringSize + 1) * sizeof(char));
        // we check if the realloc failed
        if (temp == NULL) {
            printf("Memory reallocation failed\n");
            exit(1);
        }
        // and after the realloc worked we can give tempString the value of temp
        tempString = temp;
    }
    // null-terminate the string and return it
    tempString[stringSize-1] = '\0';
    return tempString;
}

// playlistActions print all the playlist Actions and calls the wanted functions for each case the user choose
void playlistsActions(Playlist *playlists, int playlistNum, int playlistCounter) {
    int cases;
    do {
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        scanf("%d", &cases);
        switch (cases) {
            /* we call the functions with -1 the number that the user chose,
            this is the playlist we want to work on*/
            case 1: {
                showChosenPlaylist(playlists[playlistNum - 1]);
                playSong(playlists[playlistNum - 1]);
                break;
            }
            case 2: {
                addSong(&playlists[playlistNum - 1]);
                break;
            }
            case 3: {
                int deleteIndex;
                showChosenPlaylist(playlists[playlistNum - 1]);
                printf("choose a song to delete, or 0 to quit:\n");
                scanf("%d", &deleteIndex);
                if (deleteIndex == 0) {
                    break;
                }
                deleteSong(&playlists[playlistNum - 1], deleteIndex - 1);
                break;
            }
            case 4: {
                sort(&playlists[playlistNum - 1]);
                break;
            }
            case 5: {
                play(playlists[playlistNum - 1]);
                break;
            }
            case 6: {
                watchPlaylists(playlists, playlistCounter);
                break;
            }
            default: {
                printf("Invalid option\n");
                break;
            }
        }
    } while (cases != 6);
}

// watchplaylists prints all the playlists and calls playlistsactions with the playlist the user chose
void watchPlaylists(Playlist *playlists, int playlistCounter) {
    printf("Choose a playlist:\n");
    // playlistNum is the index of playlist the user chose
    int playlistNum;
    printPlaylists(playlists, playlistCounter);
    scanf("%d", &playlistNum);
    // if this statement true that means that the user chose the last option which is back to main menu so we return
    if (playlistNum == playlistCounter + 1) {
        return;
    }
    if (playlistNum <= 0 || playlistNum > playlistCounter) {
        printf("Invalid option\n");
    }
    // the playlist index is -1 the number that the user chose so we print playlists in the playlistNum - 1
    printf("playlist %s:\n", playlists[playlistNum - 1].name);
    playlistsActions(playlists, playlistNum, playlistCounter);
}

// showChosenPlaylist gets a single playlist and prints all the songs in that playlist
void showChosenPlaylist(Playlist playlist) {
    if (playlist.songsNum == 0) {
        printf("playlist %s\n", playlist.name);
        return;
    }
    for (int i = 0; i < playlist.songsNum; i++) {
        printf("%d.  Title: %s\n", i + 1, playlist.songs[i]->title);
        printf("    Artist: %s\n", playlist.songs[i]->artist);
        printf("    Released: %d\n", playlist.songs[i]->year);
        printf("    Streams: %d\n", playlist.songs[i]->streams);
    }
}

/* playSong prints the lyrics of the chosen song and adds 1 steam each play,
the function will keep looping until the user inputs 0*/
void playSong(Playlist playlist) {
    int songIndex;
    while (1) {
        printf("choose a song to play, or 0 to quit:\n");
        scanf("%d", &songIndex);
        if (songIndex == 0 || songIndex > playlist.songsNum) {
            break;
        }
        playlist.songs[songIndex - 1]->streams++;
        printf("Now playing %s:\n", playlist.songs[songIndex - 1]->title);
        printf("$ %s $\n", playlist.songs[songIndex - 1]->lyrics);
    }
}

void addSong(Playlist *playlist) {
    // newSong is the song we will input the values that the user enters and then add it to this playlist songs
    Song newSong;
    // clear the buffer before scanning so we won't scan any values left in the buffer
    scanf("%*[^\n]");
    scanf("%*c");
    printf("Enter song's details\n");
    printf("Title: \n");
    // tempTitle gets the song title from the user
    char *tempTitle = readAndAllocate();

    printf("Artist: \n");
    // tempArtist gets the song artist from the user
    char *tempArtist = readAndAllocate();

    printf("Year of release: \n");
    // tempArtist gets the song release year from the user
    int tempYear;
    scanf("%d", &tempYear);
    scanf("%*[^\n]");
    scanf("%*c");

    printf("Lyrics: \n");
    // tempLyrics gets the song lyrics from the user
    char *tempLyrics = readAndAllocate();
    // Then we give the newSong all the values from the user and initialize the streams to 0
    newSong.title = tempTitle;
    newSong.artist = tempArtist;
    newSong.year = tempYear;
    newSong.lyrics = tempLyrics;
    newSong.streams = 0;
    playlist->songs = realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song *));
    if (playlist->songs == NULL) {
        printf("Memory allocation failed\n");
        free(tempLyrics);
        free(tempArtist);
        free(tempTitle);
        exit(1);
    }
    playlist->songs[playlist->songsNum] = malloc(sizeof(Song));
    if (playlist->songs[playlist->songsNum] == NULL) {
        printf("Memory allocation failed\n");
        free(tempLyrics);
        free(tempArtist);
        free(tempTitle);
        exit(1);
    }

    *playlist->songs[playlist->songsNum] = newSong;
    playlist->songsNum++;
}

// play gets a playlist and plays all the songs, increasing each song stream by 1
void play(Playlist playlist) {
    if (playlist.songsNum == 0) {
        return;
    }
    for (int i = 0; i < playlist.songsNum; i++) {
        playlist.songs[i]->streams++;
        printf("Now playing %s:\n", playlist.songs[i]->title);
        printf("$ %s $\n", playlist.songs[i]->lyrics);
    }
}

// sort gets a playlist and prints a menu for the user to chose what sort to pick
void sort(Playlist *playlist) {
    int cases;
    printf("choose:\n"
        "1. sort by year\n"
        "2. sort by streams - ascending order\n"
        "3. sort by streams - descending order\n"
        "4. sort alphabetically\n");
    scanf("%d", &cases);
    switch (cases) {
        case 1: {
            sortByYear(playlist, 0);
            break;
        }
        case 2: {
            sortBySteamsUp(playlist, 0);
            break;
        }
        case 3: {
            sortBySteamsDown(playlist, 0);
            break;
        }
        case 4: {
            sortAlphabetically(playlist, 0);
            break;
        }
        default: {
            sortAlphabetically(playlist, 0);
            break;
        }
    }
    printf("sorted\n");
}

/* a recursive function that sorts the playlist songs array by years,
swaps is the number of swaps we've preformed starting at 0*/
void sortByYear(Playlist *playlist, int swaps) {
    // first we check if the number of swaps is equal the number of songs in the playlist, if so we've finished sorting
    if (swaps >= playlist->songsNum) {
        return;
    }
    // firstSong is the song at the swaps index
    Song *firstSong = playlist->songs[swaps];
    // minYear is the year we want to check against other songs release years initialized to the  song in the swaps index
    int minYear = playlist->songs[swaps]->year;
    // minYearIndex will be the index of the song with the earliest release date (from the swaps index to the last song)
    int minYearIndex = 0;
    /* in this loop we go from the song in swaps index to the last song and check if we've found a song with an earlier
    release year, if so we give the minYear his year number so we can check it
    against all the other songs, and save his index*/
    for (int i = swaps; i < playlist->songsNum; i++) {
        if (playlist->songs[i]->year <= minYear) {
            minYear = playlist->songs[i]->year;
            minYearIndex = i;
        }
    }
    // now that we've got the index of the song with the earliest release year we can swap him with song the in the swaps index
    playlist->songs[swaps] = playlist->songs[minYearIndex];
    playlist->songs[minYearIndex] = firstSong;
    // and then move on to the next index
    sortByYear(playlist, swaps + 1);
}

/* a recursive function that sorts the playlist songs array by streams from the lowest to the highest,
swaps is the number of swaps we've preformed starting at 0*/
void sortBySteamsUp(Playlist *playlist, int swaps) {
    // first we check if the number of swaps is equal the number of songs in the playlist, if so we've finished sorting
    if (swaps >= playlist->songsNum) {
        return;
    }
    // firstSong is the song at the swaps index
    Song *firstSong = playlist->songs[swaps];
    // minStreams is the streams we want to check against other songs streams initialized to the song in the swaps index streams
    int minStreams = playlist->songs[swaps]->streams;
    // minStreamsIndex will be the index of the song with the least streams (from the swaps index to the last song)
    int minStreamsIndex = 0;
    /* in this loop we go from the song in swaps index to the last song and check if we've found a song with the least
    streams, if so we give the minStreams his streams so we can check it
    against all the other songs, and save his index*/
    for (int i = swaps; i < playlist->songsNum; i++) {
        if (playlist->songs[i]->streams <= minStreams) {
            minStreams = playlist->songs[i]->streams;
            minStreamsIndex = i;
        }
    }
    // now that we've got the index of the song with the least streams we can swap him with the song in the swaps index
    playlist->songs[swaps] = playlist->songs[minStreamsIndex];
    playlist->songs[minStreamsIndex] = firstSong;
    // and then move on to the next index
    sortBySteamsUp(playlist, swaps + 1);
}

/* a recursive function that sorts the playlist songs array by streams from the highest to the lowest,
swaps is the number of swaps we've preformed starting at 0*/
void sortBySteamsDown(Playlist *playlist, int swaps) {
    // first we check if the number of swaps is equal the number of songs in the playlist, if so we've finished sorting
    if (swaps >= playlist->songsNum) {
        return;
    }
    // firstSong is the song at the swaps index
    Song *firstSong = playlist->songs[swaps];
    // maxStreams is the streams we want to check against other songs streams initialized to the song in the swaps index streams
    int maxStreams = playlist->songs[swaps]->streams;
    // maxStreamsIndex will be the index of the song with the most streams (from the swaps index to the last song)
    int maxStreamsIndex = 0;
    /* in this loop we go from the song in swaps index to the last song and check if we've found a song with the most
    streams, if so we give the maxStreams his streams so we can check it
    against all the other songs, and save his index*/
    for (int i = swaps; i < playlist->songsNum; i++) {
        if (playlist->songs[i]->streams >= maxStreams) {
            maxStreams = playlist->songs[i]->streams;
            maxStreamsIndex = i;
        }
    }
    // now that we've got the index of the song with the most streams we can swap him with the song in the swaps index
    playlist->songs[swaps] = playlist->songs[maxStreamsIndex];
    playlist->songs[maxStreamsIndex] = firstSong;
    // and then move on to the next index
    sortBySteamsDown(playlist, swaps + 1);
}

/* a recursive function that sorts the playlist songs array alphabetically,
swaps is the number of swaps we've preformed starting at 0*/
void sortAlphabetically(Playlist *playlist, int swaps) {
    // first we check if the number of swaps is equal the number of songs in the playlist, if so we've finished sorting
    if (swaps >= playlist->songsNum) {
        return;
    }
    // firstSong is the song at the swaps index
    Song *firstSong = playlist->songs[swaps];
    // biggestDiff is the difference between string that we want to check against other differences initialized to 0
    int biggestDiff = 0;
    /* biggestDiffIndex will be the index of the song with the biggest difference when compared to the song
    in the swaps index (from the swaps index to the last song)*/
    int biggestDiffIndex = 0;
    /* in this loop we go from the song in swaps index to the last song and check if we've found a song that when
     compared to the song in the swaps index gives us the biggest difference, if so we give the biggestDiff
     that value so we can check it against all the other songs, and save his index*/
    for (int i = swaps; i < playlist->songsNum; i++) {
        if (strcmp(playlist->songs[swaps]->title, playlist->songs[i]->title) >= biggestDiff) {
            biggestDiff = strcmp(playlist->songs[swaps]->title, playlist->songs[i]->title);
            biggestDiffIndex = i;
        }
    }
    /* now that we've got the index of the song with the biggest difference of string
    we can swap him with the song in the swaps index*/
    playlist->songs[swaps] = playlist->songs[biggestDiffIndex];
    playlist->songs[biggestDiffIndex] = firstSong;
    // and then move on to the next index
    sortAlphabetically(playlist, swaps + 1);
}
