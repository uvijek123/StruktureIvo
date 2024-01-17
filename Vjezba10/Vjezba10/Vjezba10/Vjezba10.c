﻿#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 50

struct _city;
typedef struct _city* posCity;
typedef struct _city {
	char city_name[50];
	int population;
	posCity D;
	posCity L;
}city;

struct country;
typedef struct country* posCountry;
typedef struct country {
	char country_name[50];
	posCity City;
	posCountry next;
}country;

int addcountry(char* countryName, char* FileName, posCountry p);
int addcity(char* FileName, posCity s);
posCity newcity(char* cityName, int stanovnici, posCity s);
int printcountry(posCountry p);
int printcity(posCity s);
int deletecountry(posCountry p);
int deletecity(posCity q);

int main()
{
	posCountry head = NULL;
	head = (posCountry)malloc(sizeof(struct country));
	head->next = NULL;
	if (head == NULL)
	{
		printf("Greska\n");
		return EXIT_FAILURE;
	}

	char file_name[MAX];
	char country_name[MAX];
	char FileName[MAX];
	int pop = 0;

	printf("Unesi ime datoteke s drzavama\n");
	scanf("%s", file_name);

	FILE* fp = NULL;
	fp = fopen(file_name, "r");
	if (fp == NULL)
	{
		printf("Datoteka se ne moze otvoriti\n");
		return EXIT_FAILURE;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s %s", country_name, FileName);
		addcountry(country_name, FileName, head);
	}
	fclose(fp);

	printcountry(head->next);

	deletecountry(head->next);

	return EXIT_SUCCESS;
}

int addcountry(char* countryName, char* FileName, posCountry p)
{
	posCountry co = NULL;
	co = (posCountry)malloc(sizeof(country));
	co->next = NULL;

	posCity c = NULL;

	c = (posCity)malloc(sizeof(city));

	if (c == NULL)
	{
		printf("Greska");
		return EXIT_FAILURE;
	}

	if (co == NULL)
	{
		printf("Greska");
		return EXIT_FAILURE;
	}

	strcpy(co->country_name, countryName);

	while (p->next != NULL && strcmp(p->next->country_name, co->country_name) < 0) {
		p = p->next;
	}

	co->next = p->next;
	p->next = co;


	strcpy(c->city_name, "");
	c->population = 0;
	c->D = NULL;
	c->L = NULL;
	co->City = c;

	addcity(FileName, co->City);

	return EXIT_SUCCESS;;
}

int addcity(char* FileName, posCity c)
{
	FILE* fp = NULL;
	char cityName[50];
	int pop = 0;

	fp = fopen(FileName, "r");

	if (fp == NULL)
	{
		printf("Pogreska pri otvaranju.\n");
		return EXIT_FAILURE;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s %d", cityName, &pop);
		c = newcity(cityName, pop, c);
	}

	fclose(fp);
	return EXIT_SUCCESS;
}

posCity newcity(char* cityName, int people, posCity c)
{
	posCity ci = NULL;
	if (c == NULL)
	{
		ci = (posCity)malloc(sizeof(city));
		if (ci == NULL)
		{
			printf("Greska\n");
			return 0;
		}

		strcpy(ci->city_name, cityName);
		ci->population = people;
		ci->D = NULL;
		ci->L = NULL;

		return ci;
	}


	if (c->population < people)
		c->D = newcity(cityName, people, c->D);

	else if (c->population > people)
		c->L = newcity(cityName, people, c->L);

	else if (people == c->population)
	{
		if (strcmp(cityName, c->city_name) >= 0)
			c->D = newcity(cityName, people, c->D);

		else
			c->L = newcity(cityName, people, c->L);

	}
	return c;
}

int printcountry(posCountry p)
{
	while (p != NULL)
	{
		printf("\n%s\n", p->country_name);
		printcity(p->City);
		p = p->next;
	}
	return EXIT_SUCCESS;
}

int printcity(posCity p)
{
	if (p != NULL)
	{
		printcity(p->L);
		printf("%s\n", p->city_name);
		printcity(p->D);
	}
	return EXIT_SUCCESS;;
}
int deletecountry(posCountry p)
{
	posCountry temp = NULL;
	while (p->next != NULL)
	{
		temp = p->next;
		p->next = temp->next;
		deletecity(temp->City);
		free(temp);
	}
	return EXIT_SUCCESS;
}

int deletecity(posCity p)
{
	if (p != NULL)
	{
		deletecity(p->L);
		deletecity(p->D);
		free(p);
	}
	return EXIT_SUCCESS;
}