#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
    int coefficient;
    int exponent;
    Position next;
} Element;


Position createElement(int coefficient, int exponent) {
    Position element = (Position)malloc(sizeof(Element));
    if (element != NULL) {
        element->coefficient = coefficient;
        element->exponent = exponent;
        element->next = NULL;
    }
    return element;
}

void insertElement(Position* polynomial, int coefficient, int exponent) {
    Position newElement = createElement(coefficient, exponent);

    if (*polynomial == NULL) {
        *polynomial = newElement;
    }
    else {
        Position current = *polynomial;
        Position prev = NULL;

        while (current != NULL && current->exponent < exponent) {
            prev = current;
            current = current->next;
        }

        if (current == NULL) {
            prev->next = newElement;
        }
        else if (current->exponent == exponent) {
            current->coefficient += coefficient;
            free(newElement);
        }
        else {
            if (prev == NULL) {
                newElement->next = *polynomial;
                *polynomial = newElement;
            }
            else {
                prev->next = newElement;
                newElement->next = current;
            }
        }
    }
}

void parsePolynomial(Position* polynomial, const char* input) {
    int coefficient, exponent;
    const char* token = strtok((char*)input, " ");
    while (token != NULL) {
        coefficient = atoi(token);
        token = strtok(NULL, " ");
        if (token != NULL) {
            exponent = atoi(token);
            insertElement(polynomial, coefficient, exponent);
        }
        token = strtok(NULL, " ");
    }
}


void displayPolynomial(Position polynomial) {
    while (polynomial != NULL) {
        printf("%dx^%d", polynomial->coefficient, polynomial->exponent);
        polynomial = polynomial->next;
        if (polynomial != NULL) {
            printf(" + ");
        }
    }
    printf("\n");
}


Position addPolynomials(Position poly1, Position poly2) {
    Position result = NULL;

    while (poly1 != NULL || poly2 != NULL) {
        if (poly1 == NULL || (poly2 != NULL && poly1->exponent > poly2->exponent)) {
            insertElement(&result, poly2->coefficient, poly2->exponent);
            poly2 = poly2->next;
        }
        else if (poly2 == NULL || (poly1 != NULL && poly1->exponent < poly2->exponent)) {
            insertElement(&result, poly1->coefficient, poly1->exponent);
            poly1 = poly1->next;
        }
        else {
            insertElement(&result, poly1->coefficient + poly2->coefficient, poly1->exponent);
            poly1 = poly1->next;
            poly2 = poly2->next;
        }
    }

    return result;
}


Position multiplyPolynomials(Position poly1, Position poly2) {
    Position result = NULL;

    while (poly1 != NULL) {
        Position current = poly2;

        while (current != NULL) {
            int coefficient = poly1->coefficient * current->coefficient;
            int exponent = poly1->exponent + current->exponent;

            Position term = result;
            Position prev = NULL;
            while (term != NULL && term->exponent > exponent) {
                prev = term;
                term = term->next;
            }

            if (term != NULL && term->exponent == exponent) {
                term->coefficient += coefficient;
                if (term->coefficient == 0) {
                    if (prev == NULL) {
                        result = term->next;
                    }
                    else {
                        prev->next = term->next;
                    }
                    free(term);
                }
            }
            else {
                Position newTerm = createElement(coefficient, exponent);
                if (prev == NULL) {
                    newTerm->next = result;
                    result = newTerm;
                }
                else {
                    prev->next = newTerm;
                    newTerm->next = term;
                }
            }

            current = current->next;
        }

        poly1 = poly1->next;
    }

    return result;
}


int main() {
    Position polynomial1 = NULL;
    Position polynomial2 = NULL;


    FILE* file = fopen("polynomials.txt", "r");
    if (file == NULL) {
        printf("Failed to open the file: polynomials.txt\n");
        return 1;
    }

    char line[100];


    if (fgets(line, sizeof(line), file) != NULL) {
        parsePolynomial(&polynomial1, line);
    }


    if (fgets(line, sizeof(line), file) != NULL) {
        parsePolynomial(&polynomial2, line);
    }

    fclose(file);

    printf("Polynomial 1: ");
    displayPolynomial(polynomial1);

    printf("Polynomial 2: ");
    displayPolynomial(polynomial2);


    Position sum = addPolynomials(polynomial1, polynomial2);
    printf("Sum of polynomials: ");
    displayPolynomial(sum);


    Position product = multiplyPolynomials(polynomial1, polynomial2);
    printf("Product of polynomials: ");
    displayPolynomial(product);


    while (polynomial1 != NULL) {
        Position temp = polynomial1;
        polynomial1 = polynomial1->next;
        free(temp);
    }

    while (polynomial2 != NULL) {
        Position temp = polynomial2;
        polynomial2 = polynomial2->next;
        free(temp);
    }

    while (sum != NULL) {
        Position temp = sum;
        sum = sum->next;
        free(temp);
    }

    while (product != NULL) {
        Position temp = product;
        product = product->next;
        free(temp);
    }

    return 0;
}