// aya abdelrahman fares 1222654 sec 1 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for the undo stack
typedef struct Undostack {
    struct Undostack* next;// Pointer to the next element in the stack
    char* addedstr; // The string that was added
    int index; // The index at which the string was added
    int status; // 0 -> insert, 1 -> deleted
} Undostack; // Name of the structure

// same for redo stack
typedef struct Redostack {
    struct Redostack* next;
    char* addedstr;
    int index;
    int status; // 0 -> insert, 1 -> deleted
} Redostack;

typedef struct queue {
    struct queue* next;
    char* str;
} queue;

typedef struct node {
    char data[100];
    struct node* next;
} node;

node* head = NULL; // Linked list for the text

queue* addQueue = NULL; // Add Queue for insertions

Undostack* undoStack = NULL; // Undo stack

Redostack* redoStack = NULL; // Redo stack

// Function to add a string to the linked list
void enqueue(queue* q, char* data) {
    queue* newNode = (queue*)malloc(sizeof(queue));//allocate memory for the new node
    newNode->str = strdup(data);// copy the data to the new node using strdup that
    // returns a pointer to a null-terminated byte string, which is a duplicate of the string pointed to by str
    newNode->next = NULL;// set the next pointer to null

    if (q == NULL) {// if the queue is empty
        q = newNode;// set the queue to the new node
    } else { // if the queue is not empty
        queue* temp = q; // create a temporary node and set it to the queue
        while (temp->next != NULL) {// while the next pointer of the temporary node is not null
            temp = temp->next; // set the temporary node to the next node
        }
        temp->next = newNode; // set the next pointer of the temporary node to the new node
    }
}

void add_to_linkedList(char* data) { // Function to add a string to the linked list
    node* newNode = (node*)malloc(sizeof(node));
    strcpy(newNode->data, data); // Copy the data to the new node
    newNode->next = NULL; // Set the next pointer to null

    if (head == NULL) { // If the linked list is empty
        head = newNode; // Set the head to the new node
    } else {
        node* temp = head; // Create a temporary node and set it to the head
        while (temp->next != NULL) { // While the next pointer of the temporary node is not null
            temp = temp->next; // Set the temporary node to the next node
        }
        temp->next = newNode; // Set the next pointer of the temporary node to the new node
    }
}

void printList() { // Function to print the linked list
    node* print = head;
    while (print != NULL) {
        printf("%s ", print->data);
        print = print->next;
    }
    printf("\n");
}

void add_to_undo_stack(char* data, int index, int status) { // Function to add an operation to the undo stack
    Undostack* undoNode = (Undostack*)malloc(sizeof(Undostack));
    undoNode->addedstr = strdup(data); // Copy the data to the new node
    undoNode->index = index; // Store the index of the operation
    undoNode->status = status;// Store the status of the operation

    undoNode->next = undoStack; // Set the next pointer of the new node to the current top of the stack
    undoStack = undoNode;// Set the top of the stack to the new node
}


void insertStringAtIndex(char* str, int index) { // Function to insert a string at a given index


    if (index == -1) { // If the index is -1, insert at the end
            add_to_linkedList(str); // Add the string to the linked list
            printf("%s inserted at the end\n", str);

            // Log the insertion in the Undo Stack
            add_to_undo_stack(str, -1, 0); // 0 for insertion

        } else {// If the index is not -1
            node* temp = head; // Create a temporary node and set it to the head
            node* prev = NULL; // Create a previous node and set it to null
            int i = 0;

            while (temp != NULL && i < index) { // Traverse the linked list to find the node at the given index
                prev = temp;
                temp = temp->next;
                i++;
            }

            if (temp == NULL) { // If the index is out of bounds
                printf("Index out of bounds.\n");
                return;
            }

            node* newNode = (node*)malloc(sizeof(node)); // Create a new node
            strcpy(newNode->data, str); // Copy the data to the new node

            if (prev == NULL) { //  If the index is 0, insert at the beginning
                newNode->next = head; //
                head = newNode;
            } else { // If the index is not 0
                newNode->next = temp;
                prev->next = newNode;
            }

            printf("%s inserted at index %d\n", str, index);

            // Log the insertion in the Undo Stack
            add_to_undo_stack(str, index, 0); // 0 for insertion

        }
    }
void removeString(char* str) {// Function to remove a string from the linked list
    node* rv = head; // Create a temporary node and set it to the head
    node* prev = NULL; // Create a previous node and set it to null
    int index = 0;

    while (rv != NULL) { // Traverse the linked list to find the node with the given string
        if (strcmp(rv->data, str) == 0) { // If the string is found
            if (prev == NULL) {
                head = rv->next;
            } else {
                prev->next = rv->next;
            }
            printf("%s removed at index %d\n", str, index);

            // Log the removal in the Undo Stack
            add_to_undo_stack(str, index, 1); // 1 for removal

            free(rv);// Free the memory allocated for the node
            return;
        }
        prev = rv; // Set the previous node to the current node
        rv = rv->next; // Set the current node to the next node
        index++; // Increment the index
    }
    printf("String not found: %s\n", str);
}

void undo() { // Function to perform the Undo operation
    if (undoStack == NULL) {
        printf("Undo stack is empty.\n");
        return;
    }
    // Find the top element of the undo stack
    Undostack* current = undoStack; // Create a temporary node and set it to the top of the stack

    if (current->status == 0) { // If the operation was an insertion
        // Undo the last insertion
        removeString(current->addedstr); // Remove the last inserted string using removeString function
        printf("Undo insertion of: %s\n", current->addedstr); // Print the string that was removed

        // Remove the undone operation from the undo stack
        undoStack = current->next;
        // Move the undone operation to the redo stack
        current->next = redoStack;
        redoStack = current;

    } else {
        // Undo a removal
        insertStringAtIndex(current->addedstr, current->index); // Use the original index
        printf("Undo removal of: %s\n", current->addedstr);

        // Move the undone operation to the redo stack
        current->next = redoStack;
        // The top of the redo stack is the current operation
        redoStack = current;
        // Remove the undone operation from the undo stack
        undoStack = current->next;
    }
    // Check if the undo stack is empty after undoing all insertions
    if (undoStack == NULL) {
        printf("Undo stack is empty.\n");
    }
}

// Function to perform the Redo operation
void redo() {
    if (redoStack == NULL) {// If the redo stack is empty
        printf("Redo stack is empty.\n");
        return;
    }
    // Find the top element of the redo stack
    Redostack* current = redoStack; // Create a temporary node and set it to the top of the stack
//
    if (current->status == 0) {
        // Redo an insertion that was undone
        insertStringAtIndex(current->addedstr, current->index); //  Use the original index
        printf("Redo insertion of: %s\n", current->addedstr);

    } else { // If the operation was a removal
        // Redo a removal
        removeString(current->addedstr);
        printf("Redo removal of: %s\n", current->addedstr);
    }

    // Move the redone operation to the undo stack
    current->next = undoStack;
    undoStack = current;

    // Remove the redone operation from the redo stack
    redoStack = current->next;

    // Check if the redo stack is empty after redoing all operations
    if (redoStack == NULL) {
        printf("Redo stack is empty.\n");
    }
}
// Function to print the Undo Stack only the top
void printTopUndoStack() {
    if (undoStack == NULL) {
        printf("Undo stack is empty.\n");
        return;
    }
    printf("Top of Undo Stack: %s\n", undoStack->addedstr); // Print the top element of the undo stack
}


void printTopRedoStack() {
    if (redoStack == NULL) {
        printf("Redo stack is empty.\n");
        return;
    }
    printf("Top of Redo Stack: %s\n", redoStack->addedstr); // Print the top element of the redo stack
}

void loadInitialText() {
    FILE* in = fopen("input.txt", "r"); // Open the input file in read mode
    if (in == NULL) {
        printf("Error opening file.\n");
        return;
    }
    // Read the strings from the file and insert them into the linked list
    char str[100];
    while (fscanf(in, "%99s", str) == 1) {
        insertStringAtIndex(str, -1); // -1 for end index
    }

    fclose(in);
    printf("Initial text loaded from file.\n");
}

void saveToFile() {
    FILE* out = fopen("output.txt", "w"); // Open the output file in write mode
    if (out == NULL) {
        printf(" Error opening file.\n");
        return;
    }
    // Write the strings from the linked list to the file
    node* temp = head;
    while (temp != NULL) {
        fprintf(out, "%s ", temp->data);
        temp = temp->next;
    }

    fclose(out);
}
// Function to empty the Undo Stack
void makeEmptyUndoStack() {
    while (undoStack != NULL) {
        Undostack* empty = undoStack; // Create a temporary node and set it to the top of the stack
        undoStack = undoStack->next;
        free(empty->addedstr);
        free(empty);
    }

}
// Function to empty the Redo Stack
void makeEmptyRedoStack() {
    while (redoStack != NULL) {
        Redostack* empty = redoStack;
        redoStack = redoStack->next;
        free(empty->addedstr);
        free(empty);
    }
}


int main() {
    int op, index;
    char str[100];

    do {
        printf("\nMenu of the application:\n");
        printf("1. Load the input file which contains the initial text.\n");
        printf("2. Print the loaded text.\n");
        printf("3. Insert strings to the text.\n");
        printf("4. Remove strings from the text.\n");
        printf("5. Perform Undo operation\n");
        printf("6. Perform Redo operation\n");
        printf("7. Print the Undo Stack and the Redo stack\n");
        printf("8. Save the updated text to the output file.\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                loadInitialText();
                break;
            case 2:
                printList();
                break;
            case 3:
                printf("Enter the sentence to insert :\n");
                while(getchar() != '\n'); // Clear the input buffer
                scanf("%99[^\n]", str); // Read the entire line including spaces
                printf("Enter the index to insert at (-1 for end): ");
                scanf("%d", &index);
                insertStringAtIndex(str, index);
                break;
            case 4:
                printf("Enter the string to remove: ");
                scanf("%99s", str);
                removeString(str);
                break;
            case 5:
                undo();
                break;
            case 6:
                redo();
                break;
            case 7:
                printTopUndoStack();
                printTopRedoStack();
                break;
            case 8:
                saveToFile();
                makeEmptyUndoStack();
                makeEmptyRedoStack();
                break;
            case 9:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    } while (op != 9);

    return 0;
}