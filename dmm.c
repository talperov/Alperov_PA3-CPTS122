/*
Name: Tony Alperov
Date: 9/5/25
Assignment: PA3
Description: The goal of this assignment is to create somewhat compiler that organizes
our digital music and gives us options as to what we want to with it. The options that will be provided
such as load, store, insert and more.
We will be gathering data from .csv files to converting them to strings which will help us satisfy the
requirements.
*/

#include "dmm.h"

// Node that gathers new data to our DMM
Node* makeNode(Record newData)
{
    Node* pMem = (Node*)malloc(sizeof(Node));

    if (pMem != NULL)
    {
        pMem->data = newData;
        pMem->next = NULL;
        pMem->prev = NULL;
    }

    return pMem;
}

// Inserts a new Node at the front of the list
int insertFront(Node** pHead, Record newData)
{
    Node* pMem = makeNode(newData);
    if (pMem == NULL)
    {
        return 0;
    }

    pMem->next = *pHead;
    pMem->prev = NULL;

    if (*pHead != NULL)
    {
        (*pHead)->prev = pMem;
    }

    *pHead = pMem;

    return 1;
}

// Prints the full list of records
void printList(Node* pHead)
{
    if (pHead == NULL)
    {
        printf("List is empty.\n");
        return;
    }

    Node* pCur = pHead;
    while (pCur != NULL)
    {
        printf("Artist: %s\n", pCur->data.artist);
        printf("Album: %s\n", pCur->data.album_title);
        printf("Song: %s\n", pCur->data.song_title);
        printf("Genre: %s\n", pCur->data.genre);
        printf("Times Played: %d\n", pCur->data.times_played);
        printf("Duration: %d:%02d\n", pCur->data.song_length.minutes, pCur->data.song_length.seconds);
        printf("Rating: %d\n\n", pCur->data.rating);

        pCur = pCur->next;
    }
}

// Frees the memory of the entire list
void freeList(Node* pHead)
{
    Node* pCur = pHead;
    while (pCur != NULL)
    {
        Node* temp = pCur;
        pCur = pCur->next;
        free(temp);
    }
}

// Load music data from CSV into list (beginner-friendly)
int load_music_data(Node** pHead)
{
    FILE* infile = fopen("musicPlayList.csv", "r");
    if (infile == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    char record[200];
    Record song;

    while (fgets(record, sizeof(record), infile))
    {
        // Remove newline at the end
        record[strcspn(record, "\n")] = 0;

        char* ptr = record;
        char* token;

        // Parse artist (quoted or unquoted)
        if (*ptr == '"')
        {
            ptr++; // skip initial quote
            char* end_quote = strchr(ptr, '"');
            if (end_quote)
            {
                int len = (int)(end_quote - ptr);

                strncpy(song.artist, ptr, len);

                song.artist[len] = '\0';

                ptr = end_quote + 2; // skip quote and comma
            }
            else
            {
                strcpy(song.artist, ""); // fallback
            }
        }
        else
        {
            token = strtok(ptr, ",");
            if (token != NULL)
            {
                strcpy(song.artist, token);
            }
            else
            {
                strcpy(song.artist, "");
            }
            ptr = NULL; // for subsequent strtok calls
        }

        // Album title
        token = strtok(ptr, ",");
        if (token != NULL)
        {
            strcpy(song.album_title, token);
        }
        else
        {
            strcpy(song.album_title, "");
        }

        // Song title
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strcpy(song.song_title, token);
        }
        else
        {
            strcpy(song.song_title, "");
        }

        // Genre
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            strcpy(song.genre, token);
        }
        else
        {
            strcpy(song.genre, "");
        }

        // Duration (minutes:seconds)
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            char* colon = strchr(token, ':');
            if (colon != NULL)
            {
                *colon = '\0';
                song.song_length.minutes = atoi(token);
                song.song_length.seconds = atoi(colon + 1);
            }
            else
            {
                song.song_length.minutes = 0;
                song.song_length.seconds = 0;
            }
        }
        else
        {
            song.song_length.minutes = 0;
            song.song_length.seconds = 0;
        }

        // Times played
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            song.times_played = atoi(token);
        }
        else
        {
            song.times_played = 0;
        }

        // Rating
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            song.rating = atoi(token);
        }
        else
        {
            song.rating = 0;
        }

        // Insert at the front of the list
        insertFront(pHead, song);
    }

    fclose(infile);
    return 1;
}

// Stores the music data to a CSV file
int store_music_data(Node* pHead)
{
    FILE* outfile = fopen("musicPlayList.csv", "w");
    if (outfile == NULL)
    {
        printf("Failed to open file for writing.\n");
        return 0;
    }

    Node* pCur = pHead;

    while (pCur != NULL)
    {
        if (strchr(pCur->data.artist, ',') != NULL)
        {
            fprintf(outfile, "\"%s\",", pCur->data.artist);
        }
        else
        {
            fprintf(outfile, "%s,", pCur->data.artist);
        }

        fprintf(outfile, "%s,", pCur->data.album_title);
        fprintf(outfile, "%s,", pCur->data.song_title);
        fprintf(outfile, "%s,", pCur->data.genre);

        fprintf(outfile, "%d:%02d,", pCur->data.song_length.minutes, pCur->data.song_length.seconds);

        fprintf(outfile, "%d,%d\n", pCur->data.times_played, pCur->data.rating);

        pCur = pCur->next;
    }

    fclose(outfile);
    return 1;
}

// Search by artist
Node* search_by_artist(Node* pHead, char* artist)
{
    Node* pCur = pHead;
    while (pCur != NULL)
    {
        if (strcmp(pCur->data.artist, artist) == 0)
        {
            return pCur;
        }
        pCur = pCur->next;
    }

    return NULL;
}

// Display command
void display_command(Node* pHead)
{
    int choice = 0;
    printf("Display Options:\n");
    printf("1. Display all songs\n");
    printf("2. Display by artist\n");
    printf("Choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1)
    {
        printList(pHead);
    }
    else if (choice == 2)
    {
        char artist[60];
        printf("Enter artist name: ");
        fgets(artist, 60, stdin);
        artist[strcspn(artist, "\n")] = 0;

        Node* pCur = pHead;
        int found = 0;

        while (pCur != NULL)
        {
            if (strcmp(pCur->data.artist, artist) == 0)
            {
                printf("Artist: %s\n", pCur->data.artist);
                printf("Album: %s\n", pCur->data.album_title);
                printf("Song: %s\n", pCur->data.song_title);
                printf("Genre: %s\n", pCur->data.genre);
                printf("Times Played: %d\n", pCur->data.times_played);
                printf("Duration: %d:%02d\n", pCur->data.song_length.minutes, pCur->data.song_length.seconds);
                printf("Rating: %d\n\n", pCur->data.rating);

                found = 1;
            }
            pCur = pCur->next;
        }

        if (!found)
        {
            printf("No songs found for artist \"%s\".\n", artist);
        }
    }
}

// Edit a record
void edit_command(Node* pHead)
{
    char name[60];
    printf("Enter Artist to edit: \n");
    fgets(name, 60, stdin);
    name[strcspn(name, "\n")] = 0;

    Node* edit_record = search_by_artist(pHead, name);

    if (edit_record == NULL)
    {
        printf("No records found for artist %s.\n", name);
        return;
    }

    printf("Enter new artist name: ");
    fgets(edit_record->data.artist, 60, stdin);
    edit_record->data.artist[strcspn(edit_record->data.artist, "\n")] = 0;

    printf("Enter new album name: ");
    fgets(edit_record->data.album_title, 60, stdin);
    edit_record->data.album_title[strcspn(edit_record->data.album_title, "\n")] = 0;

    printf("Enter new song name: ");
    fgets(edit_record->data.song_title, 60, stdin);
    edit_record->data.song_title[strcspn(edit_record->data.song_title, "\n")] = 0;

    printf("Enter new genre: ");
    fgets(edit_record->data.genre, 60, stdin);
    edit_record->data.genre[strcspn(edit_record->data.genre, "\n")] = 0;

    printf("Enter duration (minutes seconds): ");
    scanf("%d %d", &edit_record->data.song_length.minutes, &edit_record->data.song_length.seconds);

    printf("Enter times played: ");
    scanf("%d", &edit_record->data.times_played);
    getchar();

    printf("Enter rating (1-5): ");
    scanf("%d", &edit_record->data.rating);
    getchar();

    printf("Record updated successfully.\n");
}

// Rate a song
void rate_command(Node* pHead)
{
    char title[60];
    int new_rating;

    printf("Enter song title to rate: ");
    fgets(title, 60, stdin);
    title[strcspn(title, "\n")] = 0;

    Node* pCur = pHead;
    int found = 0;

    while (pCur != NULL)
    {
        if (strcmp(pCur->data.song_title, title) == 0)
        {
            printf("Enter new scaling (1-5): \n");
            scanf("%d", &new_rating);
            getchar();

            if (new_rating >= 1 && new_rating <= 5)
            {
                pCur->data.rating = new_rating;
                printf("Updated rating for  \"%s\" to %d.\n", title, new_rating);
            }
            else
            {
                printf("Invalid rating. Please try again.\n");
            }

            found = 1;
            break;
        }
        pCur = pCur->next;
    }

    if (!found)
    {
        printf("No record for song \"%s\".\n", title);
    }
}

// Play songs
void play_command(Node* pHead)
{
    if (pHead == NULL)
    {
        printf("Playlist is Empty.\n");
        return;
    }

    char title[60];
    printf("Enter song to play (leave blank for all): \n");
    fgets(title, 60, stdin);
    title[strcspn(title, "\n")] = 0;

    Node* pCur = pHead;

    if (strlen(title) > 0)
    {
        while (pCur != NULL && strcmp(pCur->data.song_title, title) != 0)
        {
            pCur = pCur->next;
        }

        if (pCur == NULL)
        {
            printf("Song \"%s\" not found.\n", title);
            return;
        }
    }

    while (pCur != NULL)
    {
        printf("Now Playing:\n");
        printf("Artist: %s\nSong: %s\nAlbum: %s\nDuration: %d:%02d\nRating: %d\nTimes Played: %d\n\n",
            pCur->data.artist, pCur->data.song_title, pCur->data.album_title,
            pCur->data.song_length.minutes, pCur->data.song_length.seconds,
            pCur->data.rating, pCur->data.times_played);

        pCur->data.times_played++;

        printf("Going to Next Song.\n");
        pCur = pCur->next;
        system("pause");
    }
}

// Exit program
void exit_command(Node* pHead)
{
    if (store_music_data(pHead))
    {
        printf("Playlist saved successfully.\n");
    }
    else
    {
        printf("Error saving playlist.\n");
    }

    printf("Exiting Program. . .\n");
}

// EVERYTHING ELSE BELOW IS PART 2 of PA2 AKA PA3 assingment

void insert_command(Node** pHead) // Command that inserts a new song into DMM
{
    Record newSong;

    printf("Enter the Artist name: ");
    scanf("%s", newSong.artist);

    printf("Enter the Album title: ");
    scanf("%s", newSong.album_title);

    printf("Enter the Song title: ");
    scanf("%s", newSong.song_title);

    printf("Enter the Genre: ");
    scanf("%s", newSong.genre);

    printf("Enter the amount of times played: ");
    scanf("%d", &newSong.times_played);

    // Validate rating between 1–5
    do
    {
        printf("Enter the rating (1-5): ");
        scanf("%d", &newSong.rating);
    } while (newSong.rating < 1 || newSong.rating > 5);

    printf("Enter song length (minutes seconds): ");
    scanf("%d %d", &newSong.song_length.minutes, &newSong.song_length.seconds);

    // Insert at front using your insertFront function
    insertFront(pHead, newSong);

    printf("\nNew Information Loaded!\n");
    printf("Artist: %s\n", newSong.artist);
    printf("Song: %s\n", newSong.song_title);
    printf("Album: %s\n", newSong.album_title);
    printf("Genre: %s\n", newSong.genre);
    printf("Duration: %d:%02d\n", newSong.song_length.minutes, newSong.song_length.seconds);
    printf("Rating: %d\n", newSong.rating);
    printf("Times Played: %d\n\n", newSong.times_played);
}

void delete_command(Node** pHead)  // Command that deletes a song from the list that user requests
{
    if (*pHead == NULL)
    {
        printf("Playlist is empty. Nothing to delete.\n");
        return;
    }

    printf("\nCurrent Playlist:\n");

    printList(*pHead); // Show all songs so user can choose

    char title[60];
    printf("Enter the song title to delete: ");
    scanf("%s", title);

    Node* pCur = *pHead;

    while (pCur != NULL)
    {
        if (strcmp(pCur->data.song_title, title) == 0)
        {
            // Case: deleting the head
            if (pCur == *pHead)
            {
                *pHead = pCur->next;
            }

            // Re-link neighbors
            if (pCur->prev != NULL)
            {
                pCur->prev->next = pCur->next;
            }
            if (pCur->next != NULL)
            {
                pCur->next->prev = pCur->prev;
            }

            free(pCur);

            printf("\nSong \"%s\" deleted successfully!\n", title);

            if (*pHead == NULL)
            {
                printf("Playlist is now empty.\n");
            }

            return;
        }
        pCur = pCur->next;
    }

    printf("\nSong \"%s\" not found in playlist.\n", title);
}

int sort_command(Node* pHead) // Ask user which sorting style they would like
{
    int pick = 0;

    if (pHead == NULL)
    {
        printf("No sorting, playlist is empty!\n");
        return 0;
    }

    printf("Sorting Options are listed below!\n");
    printf("1. Sorting by Artist\n");
    printf("2. Sorting by Album\n");
    printf("3. Sorting by Rating\n");
    printf("4. Sorting by Times Played\n");
    printf("Enter your pick: \n");
    scanf("%d", &pick);

    switch (pick)
    {
    case 1:
        sort_by_artist(pHead);
        break;
    case 2:
        sort_by_album(pHead);

        break;
    case 3: 
        sort_by_rating(pHead);

        break;
    case 4: 
        sort_by_times_played(pHead);

        break;
    default:
        puts("Cancelling . . . .");
        return 0;
    }
    printf("Sorting completed!\n");
    return pick;
}

void sort_by_artist(Node* pHead) // Sorts the stying by artist A-Z
{
    if (pHead == NULL)
    {
        printf("No sorting, playlist is empty!\n");
        return;
    }
    int swap = 0;
    Node* pCur;
    Node* last = NULL;

    do
    {
        swap = 0;
        pCur = pHead;
        
        while (pCur->next != last)
        {
            if (strcmp(pCur->data.artist, pCur->next->data.artist) > 0)
            {
                Record temp = pCur->data;
                pCur->data = pCur->next->data;
                pCur->next->data = temp;

                swap = 1;
            }
            pCur = pCur->next;
        }
        last = pCur;
    } while (swap);
    printf("Playlist has been sorted.\n");
} 


void sort_by_album(Node* pHead) // Sort by Album Title (A-Z)
{
    if (pHead == NULL)
    {
        printf("Playlist is empty. Nothing to sort.\n");
        return;
    }

    int swap;
    Node* pCur;
    Node* last = NULL;

    do
    {
        swap = 0;
        pCur = pHead;

        while (pCur->next != last)
        {
            if (strcmp(pCur->data.album_title, pCur->next->data.album_title) > 0)
            {
                // Inline swap
                Record temp = pCur->data;
                pCur->data = pCur->next->data;
                pCur->next->data = temp;

                swap = 1;
            }
            pCur = pCur->next;
        }
        last = pCur;
    } while (swap);

    printf("Playlist sorted by album title (A-Z).\n");
}


void sort_by_rating(Node* pHead) // Sort by Rating (1-5 ascending)
{
    if (pHead == NULL)
    {
        printf("Playlist is empty. Nothing to sort.\n");
        return;
    }

    int swap;
    Node* pCur;
    Node* last = NULL;

    do
    {
        swap = 0;
        pCur = pHead;

        while (pCur->next != last)
        {
            if (pCur->data.rating > pCur->next->data.rating)
            {
                // Inline swap
                Record temp = pCur->data;
                pCur->data = pCur->next->data;
                pCur->next->data = temp;

                swap = 1;
            }
            pCur = pCur->next;
        }
        last = pCur;
    } while (swap);

    printf("Playlist sorted by rating (1-5).\n");
}


void sort_by_times_played(Node* pHead) // Sort by Times Played (largest first)
{
    if (pHead == NULL)
    {
        printf("Playlist is empty. Nothing to sort.\n");
        return;
    }

    int swap;
    Node* pCur;
    Node* last = NULL;

    do
    {
        swap = 0;
        pCur = pHead;

        while (pCur->next != last)
        {
            if (pCur->data.times_played < pCur->next->data.times_played) 
            {
                Record temp = pCur->data;
                pCur->data = pCur->next->data;
                pCur->next->data = temp;

                swap = 1;
            }
            pCur = pCur->next;
        }
        last = pCur;
    } while (swap);

    printf("Playlist sorted by times played (largest first).\n");
}


