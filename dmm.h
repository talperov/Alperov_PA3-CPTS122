/*
Name: Tony Alperov
Date: 9/5/25
Assignment: PA3
Description: The goal of this assignment is to create somewhat compiler that organizes
our digital music and gives us options as to what we want to with it. The options that will be provided
such as load, store, insert and more.
We will be gathering data from .csv files to converting them to strings which will help us satisfy the
requirments.
*/
#ifndef DMM_H
#define DMM_H

#define _CRT_SECURE_NO_WARNINGS
#include "testFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct duration // struct for Duration of music
{
	int minutes;
	int seconds;
}Duration;


typedef struct record // struct for music playlist and everything needed to know about music
{
	char artist[60];
	char album_title[60];
	char song_title[60];
	char genre[60];
	int times_played;
	int rating;
	Duration song_length;


}Record;


typedef struct node Node; // Node to move foward or previous on songs data

typedef struct node
{
	Record data;
	Node* next;
	Node* prev;
}Node;


Node* makeNode(Record newData); // Node that gathers new data to our DMM

int insertFront(Node** pHead, Record newData); // inserts a new Node that adds on to the New Data provided for DMM

void freeList(Node* pHead); // frees memory space

void printList(Node* pHead); // A printed list of the records of the songs.

int load_music_data(Node** pHead); // Will load up music data from the .csv file

int store_music_data(Node* pHead); // Will store up music data gathered from the .csv file

void display_command(Node* pHead); // Displays records with printList and searches specific records

Node* search_by_artist(Node* pHead, char* artist); // Searches by artist so itll display in display_command

void edit_command(Node* pHead); // Edit command for diplay to change visuals

void rate_command(Node* pHead); // Rate command that changes rating for artist.

void play_command(Node* pHead); // Function that plays everything together.

void exit_command(Node* pHead); // Simply just exits out the function when user requests

// EVERYTHING ELSE BELOW IS PART 2 of PA2 AKA PA3 assingment

void insert_command(Node** pHead); // Command that inserts a new song that a user wants to put in DMM

void delete_command(Node** pHead); // Command that deletes a song from the list that user requests

int sort_command(Node* pHead); // Ask user which sorting style they would like

void sort_by_artist(Node* pHead); // Sorts the stying by artist A-Z

void sort_by_album(Node* pHead); // Sort by Album Title (A-Z)

void sort_by_rating(Node* pHead); // Sort by Rating (1-5 ascending)

void sort_by_times_played(Node* pHead); // Sort by Times Played (largest first)

void shuffle_command(Node* pHead); // Shuffles the playlist in randomized order


#endif


