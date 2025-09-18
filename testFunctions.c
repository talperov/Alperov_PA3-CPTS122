#include "testFunctions.h"

void test_insert_feature(void)
{
    Node* pHead = NULL;
    Record Song;

    strcpy(Song.artist, "Perry, Katy");
    strcpy(Song.album_title, "Witness");
    strcpy(Song.song_title, "Chained to the Rhythm");
    strcpy(Song.genre, "pop");

    Song.song_length.minutes = 4;
    Song.song_length.seconds = 36;

    Song.times_played = -1; // invalid
    Song.rating = 6;        // invalid

    // Fix invalid values
    if (Song.times_played < 0)
    {
        Song.times_played = 0;
    }
    if (Song.rating < 1 || Song.rating > 5)
    {
        Song.rating = 5;
    }

    insertFront(&pHead, Song);

    printf("\nInsert Test Result:\n");
    printList(pHead);

    freeList(pHead);
}

void test_delete_feature(void)
{
    Node* pHead = NULL;
    Record Song;

    strcpy(Song.artist, "Perry, Katy");
    strcpy(Song.album_title, "Witness");
    strcpy(Song.song_title, "Chained to the Rhythm");
    strcpy(Song.genre, "pop");

    Song.song_length.minutes = 4;
    Song.song_length.seconds = 36;

    Song.times_played = 3;
    Song.rating = 5;

    insertFront(&pHead, Song);

    printf("\nDelete Test Before:\n");
    printList(pHead);

    delete_command(&pHead);

    printf("\nDelete Test After:\n");
    printList(pHead);

    freeList(pHead);
}

void test_shuffle_feature(void)
{
    Node* pHead = NULL;

    Record song1 = { "Artist1", "Album1", "Song1", "Pop", 1, 5, {3, 10} };
    Record song2 = { "Artist2", "Album2", "Song2", "Pop", 2, 4, {2, 50} };
    Record song3 = { "Artist3", "Album3", "Song3", "Pop", 3, 3, {4, 0} };

    insertFront(&pHead, song3);
    insertFront(&pHead, song2);
    insertFront(&pHead, song1);

    printf("\nShuffle Test:\n");
    shuffle_command(pHead);

    freeList(pHead);
}
