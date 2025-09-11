/*
Name: Tony Alperov
Date: 9/5/25
Assignment: PA3
Description: Main file for Digital Music Manager (DMM)
*/

#include "dmm.h"

int main(void)
{
    Node* pHead = NULL;
    int choice = 0;

    do
    {
        printf("\n=== Welcome to Digital Music Manager ===\n");
        printf("1. Load\n");
        printf("2. Store\n");
        printf("3. Display\n");
        printf("4. Edit\n");
        printf("5. Rate\n");
        printf("6. Play\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
        {
            if (load_music_data(&pHead))
            {
                printf("File loaded successfully.\n");
            }
            else
            {
                printf("Failed to load file.\n");
            }
        }
        else if (choice == 2)
        {
            if (store_music_data(pHead))
            {
                printf("Data stored successfully.\n");
            }
            else
            {
                printf("Error storing data.\n");
            }
        }
        else if (choice == 3)
        {
            display_command(pHead);
        }
        else if (choice == 4)
        {
            edit_command(pHead);
        }
        else if (choice == 5)
        {
            rate_command(pHead);
        }
        else if (choice == 6)
        {
            play_command(pHead);
        }
        else if (choice == 7)
        {
            exit_command(pHead);
        }
        else
        {
            printf("Invalid choice! Please select 1-7.\n");
        }

    } while (choice != 7);

    freeList(pHead);

    return 0;
}