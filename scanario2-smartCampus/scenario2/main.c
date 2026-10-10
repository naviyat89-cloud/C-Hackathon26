
#include <stdio.h>
#include "functions.h"

int main(void)
{
    int choice;

    loadComplaints();

    do {
        printf("\n====================================\n");
        printf("   SMART CAMPUS COMPLAINT SYSTEM\n");
        printf("====================================\n");
        printf("1. Register New Complaint\n");
        printf("2. Display All Complaints\n");
        printf("3. Search Complaint\n");
        printf("4. Sort by Priority\n");
        printf("5. Update Complaint Status\n");
        printf("6. Display Pending Complaints\n");
        printf("7. Complaint Summary\n");
        printf("8. Filter by Category\n");
        printf("0. Save and Exit\n");
        printf("====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("Enter a valid number.\n");
            continue;
        }

        while (getchar() != '\n') {}

        switch (choice) {
            case 1: registerComplaint(); break;
            case 2: displayComplaints(); break;
            case 3: searchComplaint(); break;
            case 4: sortComplaints(); break;
            case 5: updateComplaintStatus(); break;
            case 6: displayPendingComplaints(); break;
            case 7: complaintSummary(); break;
            case 8: filterComplaints(); break;
            case 0:
                saveComplaints();
                printf("Data saved. Goodbye!\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}
