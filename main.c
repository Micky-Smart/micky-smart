#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// Define constants
#define PI 3.14159265358979323846
#define E  2.71828182845904523536
#define HEADER_WIDTH 50 // Width for centering the header

// Function prototypes
double evaluate_expression(const char* expression);
double parse_term(const char** expression);
double parse_factor(const char** expression);
double handle_function(char* func, double value);
void store_memory(double value);
double recall_memory(int index);
void clear_memory();
void print_centered(const char* str);
void display_header();
void display_help();

// Memory storage
double memory[10];
int mem_index = 0;

// Function to display centered header
void display_header() {
    print_centered("****************************************");
    print_centered("*           Scientific Calculator      *");
    print_centered("*           Version 1.1                *");
    print_centered("*           Created by [MICKY SMART]     *");
    print_centered("*           © 2024                     *");
    print_centered("****************************************");
    printf("\n");
    print_centered("Supports basic arithmetic, trigonometric,");
    print_centered("exponential, logarithmic, and memory functions.");
    printf("\n\n");
    print_centered("Type 'help' for a list of commands.");
    printf("\n\n");
}

// Function to print a string centered
void print_centered(const char* str) {
    int len = strlen(str);
    int spaces = (HEADER_WIDTH - len) / 2;
    if (spaces > 0) {
        printf("%*s%s%*s\n", spaces, "", str, spaces, "");
    } else {
        printf("%s\n", str);
    }
}

// Function to display help information
void display_help() {
    printf("Available commands and functions:\n");
    printf("  - Basic arithmetic: +, -, *, /\n");
    printf("  - Trigonometric functions: sin(x), cos(x), tan(x)\n");
    printf("  - Inverse trigonometric functions: asin(x), acos(x), atan(x)\n");
    printf("  - Logarithmic functions: log(x) (natural), log10(x) (base 10)\n");
    printf("  - Exponential function: exp(x)\n");
    printf("  - Square root: sqrt(x)\n");
    printf("  - Absolute value: abs(x)\n");
    printf("  - Constants: PI, E\n");
    printf("  - Memory: store(value), recall(index), clear\n");
    printf("  - To quit, type 'exit'\n");
    printf("\n");
}

// Main function
int main() {
    char expression[256];
    double result;

    // Display the header
    display_header();

    while (1) {
        printf("Enter an expression (or type 'help' for assistance): ");
        fgets(expression, sizeof(expression), stdin);

        // Remove newline character from the end of input
        expression[strcspn(expression, "\n")] = '\0';

        // Check if the user wants to exit
        if (strcmp(expression, "exit") == 0) {
            break;
        }

        // Check if the user needs help
        if (strcmp(expression, "help") == 0) {
            display_help();
            continue;
        }

        // Check for memory commands
        if (strncmp(expression, "store(", 6) == 0) {
            double value = evaluate_expression(expression + 6);
            store_memory(value);
            continue;
        }
        if (strncmp(expression, "recall(", 7) == 0) {
            int index = atoi(expression + 7);
            double value = recall_memory(index);
            printf("Recalled value: %.10lf\n", value);
            continue;
        }
        if (strcmp(expression, "clear") == 0) {
            clear_memory();
            continue;
        }

        // Replace "pi" and "e" with PI and E in the expression
        char* replace_ptr = NULL;
        while ((replace_ptr = strstr(expression, "pi")) != NULL) {
            memmove(replace_ptr + 2, replace_ptr + 2, strlen(replace_ptr + 2) + 1);
            memcpy(replace_ptr, "PI", 2);
        }
        while ((replace_ptr = strstr(expression, "e")) != NULL) {
            memmove(replace_ptr + 1, replace_ptr + 1, strlen(replace_ptr + 1) + 1);
            memcpy(replace_ptr, "E", 1);
        }

        // Evaluate the expression
        result = evaluate_expression(expression);

        // Print the result
        if (isfinite(result)) {
            printf("Result: %.10lf\n", result);
        } else {
            printf("Error: Invalid result.\n");
        }
    }

    return 0;
}

// Function to evaluate an expression
double evaluate_expression(const char* expression) {
    const char* exp_ptr = expression;
    return parse_term(&exp_ptr);
}

// Function to parse a term (handles + and -)
double parse_term(const char** expression) {
    double value = parse_factor(expression);

    while (**expression == '+' || **expression == '-') {
        char operator = **expression;
        (*expression)++;
        double next_value = parse_factor(expression);
        if (operator == '+') {
            value += next_value;
        } else {
            value -= next_value;
        }
    }

    return value;
}

// Function to parse a factor (handles *, /, parentheses, trigonometry, and exponentiation)
double parse_factor(const char** expression) {
    double value = 0;
    char func[10];  
    int i = 0;

    // Skip whitespace
    while (isspace(**expression)) {
        (*expression)++;
    }

    if (**expression == '(') {
        (*expression)++;
        value = parse_term(expression);
        if (**expression == ')') {
            (*expression)++;
        }
    } else if (isalpha(**expression)) {
        // Parse trigonometric function or constant
        while (isalpha(**expression)) {
            func[i++] = **expression;
            (*expression)++;
        }
        func[i] = '\0';

        if (**expression == '(') {
            (*expression)++;
            value = parse_term(expression);
            if (**expression == ')') {
                (*expression)++;
            }
        }

        value = handle_function(func, value);
    } else {
        // Parse number
        value = strtod(*expression, (char**)expression);
    }

    // Handle *, /, and exponentiation ^
    while (**expression == '*' || **expression == '/' || **expression == '^') {
        char operator = **expression;
        (*expression)++;
        double next_value = parse_factor(expression);
        if (operator == '*') {
            value *= next_value;
        } else if (operator == '/') {
            if (next_value == 0) {
                printf("Error: Division by zero.\n");
                return NAN; // Return Not-A-Number to indicate an error
            }
            value /= next_value;
        } else if (operator == '^') {
            value = pow(value, next_value);
        }
    }

    return value;
}

// Function to handle trigonometric and other scientific functions
double handle_function(char* func, double value) {
    if (strcmp(func, "sin") == 0) {
        return sin(value);
    } else if (strcmp(func, "cos") == 0) {
        return cos(value);
    } else if (strcmp(func, "tan") == 0) {
        return tan(value);
    } else if (strcmp(func, "asin") == 0) {
        return asin(value);
    } else if (strcmp(func, "acos") == 0) {
        return acos(value);
    } else if (strcmp(func, "atan") == 0) {
        return atan(value);
    } else if (strcmp(func, "log") == 0) {
        return log(value);
    } else if (strcmp(func, "log10") == 0) {
        return log10(value);
    } else if (strcmp(func, "sqrt") == 0) {
        return sqrt(value);
    } else if (strcmp(func, "exp") == 0) {
        return exp(value);
    } else if (strcmp(func, "abs") == 0) {
        return fabs(value);
    } else if (strcmp(func, "PI") == 0) {
        return PI;
    } else if (strcmp(func, "E") == 0) {
        return E;
    } else {
        printf("Error: Invalid function.\n");
        return NAN;
    }
}

// Function to store a value in memory
void store_memory(double value) {
    if (mem_index < 10) {
        memory[mem_index++] = value;
        printf("Value stored in memory.\n");
    } else {
        printf("Memory is full.\n");
    }
}

// Function to recall a value from memory
double recall_memory(int index) {
    if (index >= 0 && index < mem_index) {
        return memory[index];
    } else {
        printf("Invalid memory index.\n");
        return NAN;
    }
}

// Function to clear memory
void clear_memory() {
    mem_index = 0;
    printf("Memory cleared.\n");
}
