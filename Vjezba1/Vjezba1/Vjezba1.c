﻿#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define MAX_POINTS 15

typedef struct _student
{
    char name[MAX];
    char surname[MAX];
    int points;
} student;

int main() {
    int brstud = 0;
    char ch[MAX] = { 0 };
    FILE* file = fopen("studenti.txt", "r");

    if (file == NULL) {
        printf("ERROR -1\n");
        return -1;
    }

    while (fgets(ch, MAX, file) != NULL) {
        brstud++;
    }

    // Vratimo se na početak datoteke
    rewind(file);

    student* students = NULL;
    students = (student*)malloc(brstud * sizeof(student));

    if (students == NULL) {
        printf("ERROR -2\n");
        return -2;
    }

    for (int i = 0; i < brstud; i++) {
        fscanf(file, "%s %s %d", students[i].name, students[i].surname, &students[i].points);
    }

    printf("students: \n");
    for (int i = 0; i < brstud; i++) {
        printf("Name: %s, Surname: %s, Points: %d, Relative points: %.2f%%\n",
            students[i].name, students[i].surname, students[i].points,
            (float)students[i].points / MAX_POINTS * 100);
    }
    fclose(file);

    free(students);

    return 0;
}