#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct _stackElement;
typedef struct _stackElement* Position;

typedef struct _stackElement {
    double number;
    Position next;
} StackElement;


int calculatePostfixFromFile(Position head, char* fileName, double* result);
int readFile(char* fileName, char* buffer);
int parseStringIntoPostfix(Position head, char* buffer, double* result);
int checkStackAndExtractResult(Position head, double* result);
Position createStackElement(double number);
int push(Position head, Position newStackElement);
int printStack(Position first);
int pop(Position head, double* result);
int popAndPerformOperation(Position head, char operation, double* result);

int main() {
    char fileName[] = "postfix.txt";
    double result = 0.0;
    Position head = createStackElement(0.0);

    int success = calculatePostfixFromFile(head, fileName, &result);

    if (success) {
        printf("Result of the postfix expression: %.2f\n", result);
    }
    else {
        printf("Error occurred during calculation.\n");
    }

    // Clean up the stack
    while (head->next != NULL) {
        pop(head, &result);
    }
    free(head);

    return 0;
}


int calculatePostfixFromFile(Position head, char* fileName, double* result) {
    char buffer[100];
    if (!readFile(fileName, buffer)) {
        return 0;
    }

    if (!parseStringIntoPostfix(head, buffer, result)) {
        return 0;
    }

    return 1;
}


int readFile(char* fileName, char* buffer) {
    FILE* filePointer = NULL;
    filePointer = fopen(fileName, "r");
    if (filePointer == NULL) {
        printf("Error opening file: %s\n", fileName);
        return 0;
    }


    if (fgets(buffer, 100, filePointer) == NULL) {
        fclose(filePointer);
        printf("Error reading from file: %s\n", fileName);
        return 0;
    }

    fclose(filePointer);

    return 1;
}





int parseStringIntoPostfix(Position head, char* buffer, double* result) {
    int i = 0;
    while (buffer[i] != '\0') {
        if (isdigit(buffer[i])) {
            double number = 0.0;
            while (isdigit(buffer[i])) {
                number = number * 10 + (buffer[i] - '0');
                i++;
            }
            push(head, createStackElement(number));
        }
        else if (buffer[i] == '+' || buffer[i] == '-' || buffer[i] == '*' || buffer[i] == '/') {
            if (!popAndPerformOperation(head, buffer[i], result)) {
                return 0;
            }
            i++;
        }
        else {
            i++;
        }
    }

    return checkStackAndExtractResult(head, result);
}


int checkStackAndExtractResult(Position head, double* result) {
    if (head->next == NULL) {
        return 0;
    }
    else if (head->next->next != NULL) {
        return 0;
    }
    else {
        *result = head->next->number;
        return 1;
    }
}


Position createStackElement(double number) {
    Position newStackElement = NULL;
    newStackElement = (Position)malloc(sizeof(StackElement));
    if (newStackElement == NULL) {
        printf("Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }
    newStackElement->number = number;
    newStackElement->next = NULL;
    return newStackElement;
}


int push(Position head, Position newStackElement) {
    newStackElement->next = head->next;
    head->next = newStackElement;
    return 1;
}


int printStack(Position first) {
    while (first->next != NULL) {
        printf("%.2f ", first->next->number);
        first = first->next;
    }
    printf("\n");
    return 1;
}


int pop(Position head, double* result) {
    if (head->next == NULL) {
        return 0;
    }

    Position temp = head->next;
    *result = temp->number;
    head->next = temp->next;
    free(temp);

    return 1;
}


int popAndPerformOperation(Position head, char operation, double* result) {
    double operand1, operand2;
    if (!pop(head, &operand1) || !pop(head, &operand2)) {
        return 0;
    }

    switch (operation) {
    case '+':
        push(head, createStackElement(operand2 + operand1));
        break;
    case '-':
        push(head, createStackElement(operand2 - operand1));
        break;
    case '*':
        push(head, createStackElement(operand2 * operand1));
        break;
    case '/':
        if (operand1 == 0) {
            printf("Error: Division by zero.\n");
            return 0;
        }
        push(head, createStackElement(operand2 / operand1));
        break;
    default:
        printf("Error: Unknown operator.\n");
        return 0;
    }

    return 1;
}