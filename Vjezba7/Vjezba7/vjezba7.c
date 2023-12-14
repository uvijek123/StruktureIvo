#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _directory {
    char name[50];
    struct _directory* subdirectories;
    struct _directory* next;
} Directory;


typedef struct _stack {
    Directory* currentDirectory;
    struct _stack* nextStack;
} Stack;


Directory* createDirectory(char name[]) {
    Directory* newDirectory = (Directory*)malloc(sizeof(Directory));
    strcpy(newDirectory->name, name);
    newDirectory->subdirectories = NULL;
    newDirectory->next = NULL;
    return newDirectory;
}


void addSubdirectory(Directory* parent, Directory* subdirectory) {
    subdirectory->next = parent->subdirectories;
    parent->subdirectories = subdirectory;
}


void printDirectory(Directory* currentDirectory) {
    printf("\nContents of directory %s:\n", currentDirectory->name);

    Directory* currentSubdirectory = currentDirectory->subdirectories;
    while (currentSubdirectory != NULL) {
        printf("%s\n", currentSubdirectory->name);
        currentSubdirectory = currentSubdirectory->next;
    }
}


Directory* changeDirectory(Directory* currentDirectory, char name[]) {
    Directory* tempDir = currentDirectory->subdirectories;
    while (tempDir != NULL) {
        if (strcmp(tempDir->name, name) == 0) {
            return tempDir;
        }
        tempDir = tempDir->next;
    }
    return currentDirectory;
}


Directory* goBack(Directory* currentDirectory, Directory* rootDirectory) {
    if (currentDirectory != rootDirectory) {
        return rootDirectory;
    }
    else {
        printf("Already in the root directory.\n");
        return currentDirectory;
    }
}

int main() {
    Directory* rootDirectory = createDirectory("C:");
    Directory* currentDirectory = rootDirectory;

    int choice;
    char userInput[100]; 

    do {
        printf("\nMenu:\n");
        printf("1. md (make directory)\n");
        printf("2. cd dir (change directory)\n");
        printf("3. cd.. (go back to the parent directory)\n");
        printf("4. dir (list contents of the current directory)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        fgets(userInput, sizeof(userInput), stdin);
        sscanf(userInput, "%d", &choice);

        switch (choice) {
        case 1:
        {
            char dirName[50];
            printf("Enter directory name: ");
            fgets(dirName, sizeof(dirName), stdin);
            dirName[strcspn(dirName, "\n")] = '\0'; 

            Directory* newDir = createDirectory(dirName);
            addSubdirectory(currentDirectory, newDir);

            printf("Directory %s created.\n", dirName);
        }
        break;
        case 2:
        {
            char dirName[50];
            printf("Enter directory name: ");
            fgets(dirName, sizeof(dirName), stdin);
            dirName[strcspn(dirName, "\n")] = '\0'; 

            currentDirectory = changeDirectory(currentDirectory, dirName);
        }
        break;
        case 3:
            currentDirectory = goBack(currentDirectory, rootDirectory);
            break;
        case 4:
            printDirectory(currentDirectory);
            break;
        case 5:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }

    } while (choice != 5);

    return 0;
}