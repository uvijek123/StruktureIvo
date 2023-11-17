#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH (50)
#define NO_PERSON_FOUND (NULL)
#define EMPTY_LIST (-1)
#define NO_PERSON_FOUND_INT (-2)
#define SINGLE_PERSON_IN_LIST (-3)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person* Position;

typedef struct _Person {
    char first_name[50];
    char surname[50];
    int birth_year;
    Position next;
} Person;


Position createPerson() {
    Position newPerson = (Position)malloc(sizeof(Person));

    if (newPerson == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter first name: ");
    fgets(newPerson->first_name, sizeof(newPerson->first_name), stdin);
    newPerson->first_name[strcspn(newPerson->first_name, "\n")] = '\0';

    printf("Enter surname: ");
    fgets(newPerson->surname, sizeof(newPerson->surname), stdin);
    newPerson->surname[strcspn(newPerson->surname, "\n")] = '\0';

    printf("Enter birth year: ");
    scanf("%d", &(newPerson->birth_year));
    getchar();

    newPerson->next = NULL;
    return newPerson;
}

Position addToBeginning(Position head) {
    Position newPerson = createPerson();
    newPerson->next = head;
    return newPerson;
}

void printList(Position head) {
    Position currentPerson = head;
    while (currentPerson != NULL) {
        printf("First Name: %s, Surname: %s, Birth Year: %d\n", currentPerson->first_name, currentPerson->surname, currentPerson->birth_year);
        currentPerson = currentPerson->next;
    }
}

Position addToEnd(Position head) {
    Position newPerson = createPerson();
    if (head == NULL) {
        return newPerson;
    }

    Position currentPerson = head;
    while (currentPerson->next != NULL) {
        currentPerson = currentPerson->next;
    }
    currentPerson->next = newPerson;
    return head;
}

Position findPersonBySurname(Position head, const char* surname) {
    Position currentPerson = head;
    while (currentPerson != NULL) {
        if (strcmp(currentPerson->surname, surname) == 0) {
            return currentPerson;
        }
        currentPerson = currentPerson->next;
    }
    return NULL;
}

Position deleteElement(Position head, const char* surname) {
    Position personToDelete = findPersonBySurname(head, surname);

    if (personToDelete == NULL) {
        printf("Person with surname %s not found.\n", surname);
        return head;
    }

    if (personToDelete == head) {
        head = head->next;
    }
    else {
        Position currentPerson = head;
        while (currentPerson->next != personToDelete) {
            currentPerson = currentPerson->next;
        }
        currentPerson->next = personToDelete->next;
    }

    free(personToDelete);
    return head;
}

Position addBeforePerson(Position head, const char* targetSurname) {
    Position person = findPersonBySurname(head, targetSurname);

    if (person == NULL) {
        printf("Person with surname %s not found. Cannot add before.\n", targetSurname);
        return head;
    }

    Position newPerson = createPerson();

    if (person == head) {
        newPerson->next = head;
        return newPerson;
    }
    else {
        Position currentPerson = head;
        while (currentPerson->next != person) {
            currentPerson = currentPerson->next;
        }
        newPerson->next = person;
        currentPerson->next = newPerson;
    }

    return head;
}

Position addAfterPerson(Position head, const char* targetSurname) {
    Position person = findPersonBySurname(head, targetSurname);

    if (person == NULL) {
        printf("Person with surname %s not found. Cannot add after.\n", targetSurname);
        return head;
    }

    Position newPerson = createPerson();
    newPerson->next = person->next;
    person->next = newPerson;

    return head;
}
void saveListToFile(Position head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("File opening failed");
        return;
    }

    Position currentPerson = head;
    while (currentPerson != NULL) {
        fprintf(file, "%s %s %d\n", currentPerson->first_name, currentPerson->surname, currentPerson->birth_year);
        currentPerson = currentPerson->next;
    }

    fclose(file);
}
Position readListFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("File opening failed");
        return NULL;
    }

    Position head = NULL;
    char first_name[50];
    char surname[50];
    int birth_year;

    while (fscanf(file, "%s %s %d", first_name, surname, &birth_year) == 3) {
        Position newPerson = (Position)malloc(sizeof(Person));
        if (newPerson == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strcpy(newPerson->first_name, first_name);
        strcpy(newPerson->surname, surname);
        newPerson->birth_year = birth_year;
        newPerson->next = head;
        head = newPerson;
    }

    fclose(file);
    return head;
}
void swapItems(Position first, Position second) {
    Person temp;
    strcpy(temp.first_name, first->first_name);
    strcpy(temp.surname, first->surname);
    temp.birth_year = first->birth_year;

    strcpy(first->first_name, second->first_name);
    strcpy(first->surname, second->surname);
    first->birth_year = second->birth_year;

    strcpy(second->first_name, temp.first_name);
    strcpy(second->surname, temp.surname);
    second->birth_year = temp.birth_year;
}

Position bubbleSortBySurname(Position head) {
    int swapped = 0;
    Position lastPersonRead = NULL;
    Position start = head;

    // Return if the list is empty or has only one element
    if (!head) {
        printf("Empty list!\n");
        return NULL;
    }
    else if (!head->next) {
        printf("Only single element in list!\n");
        return head;
    }

    do {
        swapped = 0;
        Position current = start;

        while (current->next != lastPersonRead) {
            if (strcmp(current->surname, current->next->surname) > 0) {
                swapItems(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        lastPersonRead = current;
    } while (swapped);

    return start;
}


int main() {
    Position head = NULL;
    int choice;

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Add to the beginning\n");
        printf("2. Print list\n");
        printf("3. Add to the end\n");
        printf("4. Delete element by surname\n");
        printf("5. Add before a person\n");
        printf("6. Add after a person\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
        case 1:
            head = addToBeginning(head);
            break;
        case 2:
            if (head == NULL) {
                printf("The list is empty.\n");
            }
            else {
                printf("List of people:\n");
                printList(head);
            }
            break;
        case 3:
            head = addToEnd(head);
            break;
        case 4:
            char surnameToDelete[50];
            printf("Enter the surname to delete: ");
            fgets(surnameToDelete, sizeof(surnameToDelete), stdin);
            surnameToDelete[strcspn(surnameToDelete, "\n")] = '\0';
            head = deleteElement(head, surnameToDelete);
            break;
        case 5:
            char targetSurnameBefore[50];
            printf("Enter the surname before which you want to add a person: ");
            fgets(targetSurnameBefore, sizeof(targetSurnameBefore), stdin);
            targetSurnameBefore[strcspn(targetSurnameBefore, "\n")] = '\0';
            head = addBeforePerson(head, targetSurnameBefore);
            break;
        case 6:
            char targetSurnameAfter[50];
            printf("Enter the surname after which you want to add a person: ");
            fgets(targetSurnameAfter, sizeof(targetSurnameAfter), stdin);
            targetSurnameAfter[strcspn(targetSurnameAfter, "\n")] = '\0';
            head = addAfterPerson(head, targetSurnameAfter);
            break;
        case 7:
            while (head != NULL) {
                Position next = head->next;
                free(head);
                head = next;
            }
            exit(EXIT_SUCCESS);
        default:
            printf("Invalid choice. Please try again.\n");
        }

        printf("-----------------------------------------------------\n");
    }
    saveListToFile(head, "people.txt");
    head = readListFromFile("people.txt");
    head = bubbleSortBySurname(head);

    return 0;
}