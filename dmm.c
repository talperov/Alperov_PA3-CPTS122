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

