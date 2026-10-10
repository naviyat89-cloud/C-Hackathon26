
#include <stdio.h>
#include <string.h>
#include "functions.h"

static Complaint complaints[MAX_COMPLAINTS];
static int complaintCount = 0;

static void readLine(const char *message, char *buffer, int size)
{
    printf("%s", message);
    if (fgets(buffer, size, stdin) != NULL)
        buffer[strcspn(buffer, "\n")] = '\0';
}

static int readNumber(const char *message, int min, int max)
{
    char input[50], extra;
    int number;

    while (1) {
        readLine(message, input, sizeof(input));

        if (sscanf(input, "%d %c", &number, &extra) == 1 &&
            number >= min && number <= max)
            return number;

        printf("Invalid input. Try again.\n");
    }
}

static const char *priorityName(int priority)
{
    switch (priority) {
        case 1: return "Critical";
        case 2: return "High";
        case 3: return "Medium";
        default: return "Low";
    }
}

static void printComplaint(const Complaint *c)
{
    printf("\nComplaint ID : %s\n", c->id);
    printf("Category     : %s\n", c->category);
    printf("Problem      : %s\n", c->problem);
    printf("Priority     : %s\n", priorityName(c->priority));
    printf("Location     : %s\n", c->location);
    printf("Status       : %s\n", c->status);
    printf("-----------------------------\n");
}

void registerComplaint(void)
{
    Complaint c;
    int i;

    if (complaintCount >= MAX_COMPLAINTS) {
        printf("Complaint storage is full!\n");
        return;
    }

    printf("\n--- Register Complaint ---\n");
    readLine("Complaint ID (e.g. C105): ", c.id, sizeof(c.id));

    if (strlen(c.id) == 0) {
        printf("ID cannot be empty.\n");
        return;
    }

    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].id, c.id) == 0) {
            printf("Complaint ID already exists!\n");
            return;
        }
    }

    readLine("Category: ", c.category, sizeof(c.category));
    readLine("Problem: ", c.problem, sizeof(c.problem));
    readLine("Location: ", c.location, sizeof(c.location));

    printf("\n1. Critical\n2. High\n3. Medium\n4. Low\n");
    c.priority = readNumber("Select priority: ", 1, 4);
    strcpy(c.status, "Pending");

    complaints[complaintCount++] = c;
    saveComplaints();

    printf("Complaint registered successfully!\n");
}

void displayComplaints(void)
{
    int i;

    if (complaintCount == 0) {
        printf("No complaints registered.\n");
        return;
    }

    for (i = 0; i < complaintCount; i++)
        printComplaint(&complaints[i]);
}

void searchComplaint(void)
{
    char id[20];
    int i;

    readLine("Enter complaint ID: ", id, sizeof(id));

    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].id, id) == 0) {
            printComplaint(&complaints[i]);
            return;
        }
    }

    printf("Complaint not found.\n");
}

void sortComplaints(void)
{
    int i, j;
    Complaint temp;

    for (i = 0; i < complaintCount - 1; i++) {
        for (j = 0; j < complaintCount - i - 1; j++) {
            if (complaints[j].priority > complaints[j + 1].priority) {
                temp = complaints[j];
                complaints[j] = complaints[j + 1];
                complaints[j + 1] = temp;
            }
        }
    }

    printf("Complaints sorted: Critical to Low.\n");
    displayComplaints();
    saveComplaints();
}

void updateComplaintStatus(void)
{
    char id[20];
    int i, choice;

    readLine("Enter complaint ID: ", id, sizeof(id));

    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].id, id) == 0) {
            printf("1. Pending\n2. In Progress\n3. Resolved\n");
            choice = readNumber("Select status: ", 1, 3);

            if (choice == 1)
                strcpy(complaints[i].status, "Pending");
            else if (choice == 2)
                strcpy(complaints[i].status, "In Progress");
            else
                strcpy(complaints[i].status, "Resolved");

            saveComplaints();
            printf("Status updated successfully!\n");
            return;
        }
    }

    printf("Complaint not found.\n");
}

void displayPendingComplaints(void)
{
    int i, found = 0;

    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].status, "Pending") == 0) {
            printComplaint(&complaints[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No pending complaints.\n");
}

void complaintSummary(void)
{
    int i, pending = 0, progress = 0, resolved = 0;
    int critical = 0, high = 0, medium = 0, low = 0;

    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].status, "Pending") == 0)
            pending++;
        else if (strcmp(complaints[i].status, "In Progress") == 0)
            progress++;
        else if (strcmp(complaints[i].status, "Resolved") == 0)
            resolved++;

        switch (complaints[i].priority) {
            case 1: critical++; break;
            case 2: high++; break;
            case 3: medium++; break;
            case 4: low++; break;
        }
    }

    printf("\n===== COMPLAINT SUMMARY =====\n");
    printf("Total complaints : %d\n", complaintCount);
    printf("Pending          : %d\n", pending);
    printf("In Progress      : %d\n", progress);
    printf("Resolved         : %d\n", resolved);
    printf("Critical         : %d\n", critical);
    printf("High             : %d\n", high);
    printf("Medium           : %d\n", medium);
    printf("Low              : %d\n", low);
}

void filterComplaints(void)
{
    char category[40];
    int i, found = 0;

    readLine("Enter category: ", category, sizeof(category));

    for (i = 0; i < complaintCount; i++) {
        if (strcmp(complaints[i].category, category) == 0) {
            printComplaint(&complaints[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No complaints found in this category.\n");
}

void saveComplaints(void)
{
    FILE *file = fopen("complaints.dat", "wb");

    if (file == NULL) {
        printf("Unable to save complaint file.\n");
        return;
    }

    fwrite(&complaintCount, sizeof(int), 1, file);
    fwrite(complaints, sizeof(Complaint), complaintCount, file);
    fclose(file);
}

void loadComplaints(void)
{
    FILE *file = fopen("complaints.dat", "rb");
    int count;

    if (file == NULL)
        return;

    if (fread(&count, sizeof(int), 1, file) == 1 &&
        count >= 0 && count <= MAX_COMPLAINTS) {
        if (fread(complaints, sizeof(Complaint), count, file) == count)
            complaintCount = count;
    }

    fclose(file);
}
