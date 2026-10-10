
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define MAX_COMPLAINTS 200

typedef struct {
    char id[20];
    char category[40];
    char problem[200];
    char location[100];
    char status[20];
    int priority;
} Complaint;

void registerComplaint(void);
void displayComplaints(void);
void searchComplaint(void);
void sortComplaints(void);
void updateComplaintStatus(void);
void displayPendingComplaints(void);
void complaintSummary(void);
void filterComplaints(void);
void saveComplaints(void);
void loadComplaints(void);

#endif
