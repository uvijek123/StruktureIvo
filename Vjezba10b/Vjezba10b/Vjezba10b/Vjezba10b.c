#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 30

struct _list;
typedef struct _list* listPosition;
typedef struct _list {
	char city[MAX_LINE];
	int number_pop;
	listPosition next;
}list;

struct _tree;
typedef struct _tree* treePosition;
typedef struct _tree {
	char country[MAX_LINE];
	listPosition cities;
	treePosition left;
	treePosition right;
}tree;

int InsertInTreeFromFile(treePosition head, char* fileName);
int InsertInListFromFile(listPosition head, char* fileName);
int SortListWhileInserting(listPosition head, listPosition newPerson);
int InsertAfter(listPosition who, listPosition where);
listPosition CreateListElement(char* city, int number_pop);
int PrintList(listPosition first);
int PrintTreeInOrder(treePosition current);
treePosition SortTreeWhileInserting(treePosition current, treePosition newElement);
treePosition CreateTreeElement(char* country);


int main() {

	tree tCountries = { .country = "",.cities = NULL,.left = NULL,.right = NULL };
	treePosition treeCountries = &tCountries;

	char countries[MAX_LINE] = { 0 };
	printf("Name your file \n:");
	scanf(" %[^\n]", countries);
	InsertInTreeFromFile(treeCountries, countries);
	PrintTreeInOrder(treeCountries);

	return EXIT_SUCCESS;

}


int InsertInTreeFromFile(treePosition head, char* fileName) {

	FILE* dat = NULL;
	dat = fopen(fileName, "r");
	char country[MAX_LINE] = { 0 }, datCities[MAX_LINE];

	tree temp1 = { .country = "",.cities = NULL,.left = NULL,.right = NULL };
	treePosition temp2 = &temp1;

	if (!dat) {
		printf("Can't allocate memory!\n");
		return EXIT_FAILURE;
	}

	while (!feof(dat)) {

		fscanf(dat, "%s %s", country, datCities);
		//temp->cities
		temp2 = CreateTreeElement(country);
		//temp -> cities -> city, -II- -> number_pop);
		InsertInListFromFile(temp2->cities, datCities);
		head = SortTreeWhileInserting(head, temp2);
	}

	fclose(dat);
	return EXIT_SUCCESS;
}

int InsertInListFromFile(listPosition head, char* fileName) {

	FILE* dat = NULL;
	dat = fopen(fileName, "r");

	char city[MAX_LINE] = { 0 };
	int number_pop = 0;

	list temp1 = { .city = "",.number_pop = 0,.next = NULL };
	listPosition temp2 = &temp1;

	if (!dat) {
		printf("Can't allocate memory!\n");
		return EXIT_FAILURE;
	}

	while (!feof(dat)) {
		fscanf(dat, "%s %d", city, &number_pop);
		temp2 = CreateListElement(city, number_pop);
		//country -> cities -> city, -II- number_pop
		SortListWhileInserting(head, temp2);

	}
	fclose(dat);

	return EXIT_SUCCESS;

}


int SortListWhileInserting(listPosition head, listPosition newElement) {


	while (head->next != NULL && head->next->number_pop > newElement->number_pop) {
		head = head->next;
	}
	InsertAfter(newElement, head);
	return EXIT_FAILURE;
}


int InsertAfter(listPosition who, listPosition where) {

	who->next = where->next;
	where->next = who;

	return EXIT_SUCCESS;
}

listPosition CreateListElement(char* city, int number_pop) {
	listPosition newElement = NULL;
	newElement = (listPosition)malloc(sizeof(list));

	if (!newElement) {
		perror("\nCan't allocate memory!");
		return NULL;
	}

	strcpy(newElement->city, city);
	newElement->number_pop = number_pop;
	newElement->next = NULL;

	return newElement;
}

int PrintList(listPosition first) {

	if (first == NULL) return 0;
	listPosition temp = first->next;

	while (temp) {
		printf("\n\t\tCity %s with %d population", temp->city, temp->number_pop);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int PrintTreeInOrder(treePosition current) {
	if (current == NULL)
		return EXIT_SUCCESS;

	PrintTreeInOrder(current->left);
	printf("\nCountry: %s", current->country);
	PrintList(current->cities);
	PrintTreeInOrder(current->right);

	return EXIT_SUCCESS;
}

treePosition SortTreeWhileInserting(treePosition current, treePosition newElement) {

	if (!current)
		return newElement;

	if (strcmp(current->country, newElement->country) > 0) {
		current->right = SortTreeWhileInserting(current->right, newElement);
	}

	else if (strcmp(current->country, newElement->country) < 0) {
		current->left = SortTreeWhileInserting(current->left, newElement);
	}
	else free(newElement);

	return current;
}

treePosition CreateTreeElement(char* country) {
	treePosition newElement = NULL;
	newElement = (treePosition)malloc(sizeof(tree));

	if (!newElement) {
		perror("\nCan't allocate memory.");
		return NULL;
	}

	strcpy(newElement->country, country);
	newElement->cities = (treePosition)malloc(sizeof(list));
	if (!newElement->cities) {
		perror("Can't allocate!");
		free(newElement);
		return NULL;
	}
	newElement->cities->next = NULL;
	newElement->right = NULL;
	newElement->left = NULL;

	//newElement -> cities -> number_pop, newElement -> country, newElement -> left -> country

	return newElement;
}