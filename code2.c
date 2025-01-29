#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include<windows.h>
#include<unistd.h>

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "12345" // ????? ???? ???? ?????
#define STAFF_FILE "staff_profiles.txt"
#define MEMBER_FILE "member_profiles.txt"

void getPassword(char *password);
void adminLogin();
void staffLogin();
void memberLogin();

// Admin Menu Functions
void adminMenu();
void addStaff();
void viewStaffList();
void deleteStaff();
void viewReports();

int isUsernameTaken(const char *username);
int validateName(const char *name);
int validateNationalID(const char *nationalID);
int validatePhoneNumber(const char *phoneNumber);
int validateEmail(const char *email);
int validatePassword(const char *password);
void validPassword(char *password);

int parseDate(const char *dateStr, struct tm *date);


// Individual Reports
void reportMembersWithStatus();         // 1. List of Members with Status
void reportActiveStaff();               // 2. List of Active Staff
void reportRemovedStaff();              // 3. List of Removed Staff
void reportAllBooks();                  // 4. List of All Books
void reportAvailableBooks();            // 5. List of Available Books for Loan
void reportLoanedBooks();               // 6. List of Currently Loaned Books
void reportOverdueBooks();              // 7. List of Overdue Books
void reportLoanHistory();               // 8. Loan History (By Date Range)
void reportBookRegistrationHistory();   // 9. Book Registration History
void reportBooksByStaff();   

// Staff Menu
void staffMenu();
void staffLogin();
// Member Management
void registerNewMember();        // 1. Register a new library member
void renewMembership();          // 2. Renew membership for an existing member
void listAllMembers();           // 3. List all members (active, former, and inactive)
void removeMember();             // 4. Remove a member from the system
// Book Management
void registerNewBook();          // 5. Register a new book in the library
// Reports
void staffViewReports();              // 6. View reports (e.g., active members, available books)
// Account Settings
void accountSettings();          // 7. Update account details (e.g., username, password)
// Logout
void logout();                   // 8. Log out of the staff account


// ???? ???? ???? ???? ????? ?? ????? (*)
void getPassword(char *password) {
    char ch;
    int index = 0;

    while (1) {
        ch = getch(); // Read a single character

        if (ch == '\r' || ch == '\n') { // Enter key (End of password input)
            break;
        }

        if (ch == '\b') { // Handle Backspace
            if (index > 0) {
                index--;
                printf("\b \b"); // Erase the last character on screen
            }
        } else { 
            password[index++] = ch; // Store the character in the password array
            printf("*"); // Display '*' instead of the actual character
        }
    }
    password[index] = '\0'; // Null-terminate the string
    printf("\n");
}


// ???? ????? ??????? ???? ?? ????
int checkCredentials(const char *filename, const char *username, const char *password) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    char fileUsername[50], filePassword[50];
    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
            fclose(file);
            return 1; // ????? ???? ??
        }
    }

    fclose(file);
    return 0; // ????? ?????
}

// ???? ?????
void adminLogin() {
    char username[50], password[50];

    printf("\n--- Admin Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    getPassword(password);

    // Hardcoded admin credentials
    if (strcmp(username, "admin") == 0 && strcmp(password, "12345") == 0) {
        printf("Admin login successful! Redirecting to Admin Menu...\n");
        sleep(4);                 // Wait for 4 seconds
        system("cls");            // Clear the screen (Windows)
        adminMenu();              // Redirect to Admin Menu
    } else {
        printf("Invalid admin credentials!\n");
    }
}

// ???? ????????
void staffLogin() {
    char username[50], password[50];
    char fileUsername[50], filePassword[50];
    char buffer[300];
    int loginSuccessful = 0;

    printf("\n--- Staff Login ---\n");

    while (!loginSuccessful) {
        printf("Enter Username: ");
        scanf("%s", username);

        printf("Enter Password: ");
        getPassword(password); // Use the getPassword() function to mask input with '*'

        FILE *file = fopen("staff_profiles.txt", "r");
        if (!file) {
            printf("Error: Unable to open staff profiles file.\n");
            return;
        }

        // Check the username and password against the file
        while (fgets(buffer, sizeof(buffer), file)) {
            sscanf(buffer, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%s", fileUsername, filePassword);
            if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
                loginSuccessful = 1;
                break;
            }
        }

        fclose(file);

        if (loginSuccessful) {
            printf("Login successful! Redirecting to Staff Menu...\n");
            staffMenu(); // Redirect to staff menu
        } else {
            printf("Invalid username or password. Please try again.\n");
        }
    }
}

// ???? ????
void memberLogin() {
    char username[50], password[50];

    printf("\n--- Member Login ---\n");
    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    getPassword(password);

    if (checkCredentials(MEMBER_FILE, username, password)) {
        printf("Member login successful!\n");
    } else {
        printf("Invalid member credentials!\n");
    }
}

void addStaff() {
    char firstName[50], lastName[50], employmentDate[20];
    char mobileNumber[15], nationalID[20], email[50];
    char username[50], password[50], confirmPassword[50];

    printf("\n--- Add New Staff ---\n");

    // Input First Name (Only alphabetic characters)
    while (1) {
        printf("Enter First Name: ");
        scanf("%s", firstName);
        if (validateName(firstName)) break;
        printf("Error: First name must only contain alphabetic characters.\n");
    }

    // Input Last Name (Only alphabetic characters)
    while (1) {
        printf("Enter Last Name: ");
        scanf("%s", lastName);
        if (validateName(lastName)) break;
        printf("Error: Last name must only contain alphabetic characters.\n");
    }

    // Input Date of Employment
    printf("Enter Date of Employment (YYYY-MM-DD): ");
    scanf("%s", employmentDate);

    // Input Mobile Number
    while (1) {
        printf("Enter Mobile Number (Format: 09123456789): ");
        scanf("%s", mobileNumber);
        if (validatePhoneNumber(mobileNumber)) break;
        printf("Error: Invalid mobile number format.\n");
    }

    // Input National ID
    while (1) {
        printf("Enter National ID (10 digits): ");
        scanf("%s", nationalID);
        if (validateNationalID(nationalID)) break;
        printf("Error: National ID must be exactly 10 digits and contain no letters.\n");
    }

    // Input Email Address
    while (1) {
        printf("Enter Email Address: ");
        scanf("%s", email);
        if (validateEmail(email)) break;
        printf("Error: Invalid email format.\n");
    }

    // Check for unique username
    while (1) {
        printf("Enter Username: ");
        scanf("%s", username);
        if (isUsernameTaken(username)) {
            printf("Error: Username '%s' is already taken. Please choose a different username.\n", username);
        } else {
            break; // Username is unique
        }
    }

    // Input and confirm password
    while (1) {
        printf("Enter Password: ");
        getPassword(password);

        if (!validatePassword(password)) {
            printf("Error: Password must be at least 8 characters long, include uppercase, lowercase, a digit, and a special character.\n");
            continue;
        }

        printf("Confirm Password: ");
        validPassword(confirmPassword);

        if (strcmp(password, confirmPassword) == 0) {
            break; // Passwords match
        } else {
            printf("Error: Passwords do not match. Please try again.\n");
        }
    }

    // Save staff details in the file
    FILE *file = fopen(STAFF_FILE, "a");
    if (!file) {
        printf("Error: Unable to open staff profiles file for writing.\n");
        return;
    }

    // Write details to the file
    fprintf(file, "%s|%s|%s|%s|%s|%s|%s|%s\n",
            firstName, lastName, employmentDate,
            mobileNumber, nationalID, email,
            username, password);
    fclose(file);

    printf("New staff member '%s %s' has been successfully added!\n", firstName, lastName);
}

int validateName(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i])) return 0; // Check if all characters are alphabetic
    }
    return 1; // Valid name
}
int validateNationalID(const char *nationalID) {
    if (strlen(nationalID) != 10) return 0; // Must be exactly 10 characters
    for (int i = 0; nationalID[i] != '\0'; i++) {
        if (!isdigit(nationalID[i])) return 0; // All characters must be digits
    }
    return 1; // Valid National ID
}

int validatePhoneNumber(const char *phoneNumber) {
    if (strlen(phoneNumber) != 11) return 0; // Must be exactly 11 characters
    if (phoneNumber[0] != '0' || phoneNumber[1] != '9') return 0; // Must start with '09'
    for (int i = 0; phoneNumber[i] != '\0'; i++) {
        if (!isdigit(phoneNumber[i])) return 0; // All characters must be digits
    }
    return 1; // Valid phone number
}

int validateEmail(const char *email) {
    const char *at = strchr(email, '@'); // Find '@'
    const char *dot = strrchr(email, '.'); // Find last '.'

    // Email must have '@' and '.' after '@', and they should not be at the beginning or end
    if (at && dot && at < dot && at != email && dot[1] != '\0') {
        return 1; // Valid email
    }
    return 0; // Invalid email
}

int validatePassword(const char *password) {
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;

    if (strlen(password) < 8) return 0; // Must be at least 8 characters long

    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) hasUpper = 1;
        if (islower(password[i])) hasLower = 1;
        if (isdigit(password[i])) hasDigit = 1;
        if (ispunct(password[i])) hasSpecial = 1;
    }

    // Must include all character types
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

int isUsernameTaken(const char *username) {
    FILE *file = fopen(STAFF_FILE, "r");
    if (!file) {
        return 0; // If file does not exist, assume username is not taken
    }

    char fileUsername[50];
    char buffer[300]; // To skip other staff details

    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]", fileUsername);
        if (strcmp(fileUsername, username) == 0) {
            fclose(file);
            return 1; // Username is taken
        }
    }

    fclose(file);
    return 0; // Username is not taken
}

void validPassword(char *password) {
    char ch;
    int index = 0;

    while (1) {
        ch = getch(); // Read a single character

        if (ch == '\r' || ch == '\n') { // Enter key (End of password input)
            break;
        }

        if (ch == '\b') { // Handle Backspace
            if (index > 0) {
                index--;
                printf("\b \b"); // Erase the last character on screen
            }
        } else {
            password[index++] = ch; // Store the character in the password array
            printf("*"); // Display '*' instead of the actual character
        }
    }
    password[index] = '\0'; // Null-terminate the string
    printf("\n");
}


void viewStaffList() {
    char firstName[50], lastName[50], employmentDate[20];
    char username[50], email[50], nationalID[20];
    char buffer[300]; // To store a line of staff data

    printf("\n--- Staff List ---\n");

    FILE *file = fopen(STAFF_FILE, "r"); // Open the file in read mode
    if (!file) {
        printf("Error: Unable to open staff profiles file.\n");
        return;
    }

    int staffCount = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        // Parse and extract relevant fields (First Name, Last Name, Employment Date, National ID, Email, Username)
        sscanf(buffer, "%[^|]|%[^|]|%[^|]|%*[^|]|%[^|]|%[^|]|%[^|]|%*s",
               firstName, lastName, employmentDate, nationalID, email, username);

        // Display the extracted information
        printf("%d. Name: %s %s | Username: %s | Employment Date: %s | Email: %s | National ID: %s\n\n",
               ++staffCount, firstName, lastName, username, employmentDate, email, nationalID);
    }

    if (staffCount == 0) {
        printf("No staff members found in the system.\n");
    }

    fclose(file);
}


void deleteStaff() {
    char firstName[50], lastName[50], username[50], buffer[300];
    char selectedUsername[50];
    int found = 0;

    FILE *file = fopen(STAFF_FILE, "r");
    if (!file) {
        printf("Error: Unable to open staff profiles file.\n");
        return;
    }

    // Display current staff members
    printf("\n--- Staff List ---\n");
    int staffCount = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%*s",
               firstName, lastName, username);
        printf("%d. Name: %s %s | Username: %s\n", ++staffCount, firstName, lastName, username);
    }

    fclose(file);

    if (staffCount == 0) {
        printf("No staff members found in the system.\n");
        return;
    }

    // Ask the admin to select the staff member to delete by username
    printf("\nEnter the Username of the staff member to delete: ");
    scanf("%s", selectedUsername);

    // Open the file again to process the deletion
    file = fopen(STAFF_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile) {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    // Process the file and exclude the selected staff member
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%[^|]|%*s",
               firstName, lastName, username);

        if (strcmp(username, selectedUsername) == 0) {
            found = 1;
            printf("Are you sure you want to delete staff member '%s %s' (Username: %s)? (y/n): ", firstName, lastName, username);
            char confirmation;
            scanf(" %c", &confirmation);
            if (confirmation == 'y' || confirmation == 'Y') {
                printf("Staff member '%s %s' (Username: %s) has been successfully deleted.\n", firstName, lastName, username);
                continue; // Skip writing this staff member to the temp file
            } else {
                printf("Deletion cancelled.\n");
                fprintf(tempFile, "%s", buffer); // Keep the staff member in the file
            }
        } else {
            fprintf(tempFile, "%s", buffer); // Write other staff members to the temp file
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove(STAFF_FILE);       // Remove the original file
        rename("temp.txt", STAFF_FILE); // Replace with the updated file
    } else {
        remove("temp.txt"); // Delete temp file if no match found
        printf("Error: No staff member found with the username '%s'.\n", selectedUsername);
    }
}


void viewReports() {
    int choice;

    while (1) {
        // Display available reports
        printf("\n--- Reports Menu ---\n");
        printf("1. List of Members with Status\n");
        printf("2. List of Active Staff\n");
        printf("3. List of Removed Staff\n");
        printf("4. List of All Books\n");
        printf("5. List of Available Books for Loan\n");
        printf("6. List of Currently Loaned Books\n");
        printf("7. List of Overdue Books\n");
        printf("8. Loan History (By Date Range)\n");
        printf("9. Book Registration History\n");
        printf("10. Books Registered by Specific Staff Member\n");
        printf("11. Exit Reports Menu\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        // Handle admin's choice
        switch (choice) {
            case 1:
                reportMembersWithStatus();
                break;
            case 2:
                reportActiveStaff();
                break;
            case 3:
                reportRemovedStaff();
                break;
            case 4:
                reportAllBooks();
                break;
            case 5:
                reportAvailableBooks();
                break;
            case 6:
                reportLoanedBooks();
                break;
            case 7:
                reportOverdueBooks();
                break;
            case 8:
                reportLoanHistory();
                break;
            case 9:
                reportBookRegistrationHistory();
                break;
            case 10:
                reportBooksByStaff();
                break;
            case 11:
                printf("Exiting Reports Menu...\n");
                return; // Exit the reports menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void reportMembersWithStatus() {
    char name[50], status[20], username[50];
    char buffer[300];

    FILE *file = fopen("members_profiles.txt", "r");
    if (!file) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    printf("\n--- Members List ---\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%*[^|]|%*[^|]|%[^|]|%s", name, status, username);
        printf("Name: %s | Status: %s | Username: %s\n", name, status, username);
    }

    fclose(file);
}

void reportActiveStaff() {
    char firstName[50], lastName[50], username[50], buffer[300];

    FILE *file = fopen("staff_profiles.txt", "r");
    if (!file) {
        printf("Error: Unable to open staff profiles file.\n");
        return;
    }

    printf("\n--- Active Staff Members ---\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%*s|%s", firstName, lastName, username);
        printf("Name: %s %s | Username: %s\n", firstName, lastName, username);
    }

    fclose(file);
}

void reportRemovedStaff() {
    char firstName[50], lastName[50], removalDate[20], buffer[300];

    FILE *file = fopen("removed_staff.txt", "r");
    if (!file) {
        printf("Error: Unable to open removed staff file.\n");
        return;
    }

    printf("\n--- Removed Staff Members ---\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", firstName, lastName, removalDate);
        printf("Name: %s %s | Removed On: %s\n", firstName, lastName, removalDate);
    }

    fclose(file);
}

void reportAllBooks() {
    char title[100], author[50], status[20], buffer[300];

    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("Error: Unable to open books file.\n");
        return;
    }

    printf("\n--- All Books ---\n");
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", title, author, status);
        printf("Title: %s | Author: %s | Status: %s\n", title, author, status);
    }

    fclose(file);
}

void reportAvailableBooks() {
    char title[100], author[50], status[20], buffer[300];

    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("Error: Unable to open books file.\n");
        return;
    }

    printf("\n--- Available Books for Loan ---\n");
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", title, author, status);
        if (strcmp(status, "Available") == 0) {
            printf("%d. Title: %s | Author: %s\n", ++count, title, author);
        }
    }

    if (count == 0) {
        printf("No books available for loan at the moment.\n");
    }

    fclose(file);
}

void reportLoanedBooks() {
    char title[100], author[50], status[20], buffer[300];

    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("Error: Unable to open books file.\n");
        return;
    }

    printf("\n--- Currently Loaned Books ---\n");
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", title, author, status);
        if (strcmp(status, "Loaned") == 0) {
            printf("%d. Title: %s | Author: %s\n", ++count, title, author);
        }
    }

    if (count == 0) {
        printf("No books are currently loaned.\n");
    }

    fclose(file);
}

#include <time.h> // For date comparisons

void reportOverdueBooks() {
    char title[100], borrower[50], dueDate[20], buffer[300];
    time_t now = time(NULL);
    struct tm *currentDatePtr = localtime(&now); // Get the current date
    struct tm currentDate = *currentDatePtr;     // Copy the date to a local struct

    FILE *file = fopen("loans.txt", "r");
    if (!file) {
        printf("Error: Unable to open loans file.\n");
        return;
    }

    printf("\n--- Overdue Books ---\n");
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%s", title, borrower, dueDate);

        // Parse due date
        struct tm due;
        if (!parseDate(dueDate, &due)) {
            printf("Error: Invalid date format for book '%s'. Skipping...\n", title);
            continue;
        }

        // Compare due date with current date
        if (difftime(now, mktime(&due)) > 0) {
            printf("%d. Title: %s | Borrower: %s | Due Date: %s\n", ++count, title, borrower, dueDate);
        }
    }

    if (count == 0) {
        printf("No overdue books at the moment.\n");
    }

    fclose(file);
}


void reportLoanHistory() {
    char title[100], borrower[50], loanDate[20], returnDate[20], buffer[300];
    char startDate[20], endDate[20];

    printf("Enter start date (YYYY-MM-DD): ");
    scanf("%s", startDate);
    printf("Enter end date (YYYY-MM-DD): ");
    scanf("%s", endDate);

    FILE *file = fopen("loans.txt", "r");
    if (!file) {
        printf("Error: Unable to open loans file.\n");
        return;
    }

    printf("\n--- Loan History from %s to %s ---\n", startDate, endDate);
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%[^|]|%s", title, borrower, loanDate, returnDate);

        if (strcmp(loanDate, startDate) >= 0 && strcmp(loanDate, endDate) <= 0) {
            printf("%d. Title: %s | Borrower: %s | Loan Date: %s | Return Date: %s\n",
                   ++count, title, borrower, loanDate, returnDate);
        }
    }

    if (count == 0) {
        printf("No loans found in the specified date range.\n");
    }

    fclose(file);
}

void reportBookRegistrationHistory() {
    char title[100], author[50], regDate[20], buffer[300];

    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("Error: Unable to open books file.\n");
        return;
    }

    printf("\n--- Book Registration History ---\n");
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%*[^|]|%s", title, author, regDate);
        printf("%d. Title: %s | Author: %s | Registration Date: %s\n", ++count, title, author, regDate);
    }

    if (count == 0) {
        printf("No books found in the registration history.\n");
    }

    fclose(file);
}

void reportBooksByStaff() {
    char title[100], author[50], regBy[50], buffer[300];
    char staffUsername[50];

    printf("Enter the staff username: ");
    scanf("%s", staffUsername);

    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("Error: Unable to open books file.\n");
        return;
    }

    printf("\n--- Books Registered by %s ---\n", staffUsername);
    int count = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        sscanf(buffer, "%[^|]|%[^|]|%*[^|]|%s", title, author, regBy);

        if (strcmp(regBy, staffUsername) == 0) {
            printf("%d. Title: %s | Author: %s\n", ++count, title, author);
        }
    }

    if (count == 0) {
        printf("No books found registered by '%s'.\n", staffUsername);
    }

    fclose(file);
}

int parseDate(const char *dateStr, struct tm *date) {
    int year, month, day;

    // Parse the date string
    if (sscanf(dateStr, "%d-%d-%d", &year, &month, &day) != 3) {
        return 0; // Return 0 if the format is incorrect
    }

    // Populate the tm struct
    date->tm_year = year - 1900; // tm_year is years since 1900
    date->tm_mon = month - 1;    // tm_mon is 0-based
    date->tm_mday = day;
    date->tm_hour = 0;           // Set default time
    date->tm_min = 0;
    date->tm_sec = 0;

    return 1; // Return 1 if successful
}

void staffMenu() {
    int choice;

    while (1) {
        printf("\n--- Staff Menu ---\n");
        printf("1. Register a New Member\n");
        printf("2. Renew Membership\n");
        printf("3. List All Members (Active, Former, and Inactive)\n");
        printf("4. Remove a Member\n");
        printf("5. Register a New Book\n");
        printf("6. View Reports\n");
        printf("7. Account Settings\n");
        printf("8. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerNewMember();
                break;
            case 2:
                renewMembership();
                break;
            case 3:
                listAllMembers();
                break;
            case 4:
                removeMember();
                break;
            case 5:
                registerNewBook();
                break;
            case 6:
                staffViewReports(); // Call the staff-specific reports function
                break;
            case 7:
                accountSettings();
                break;
            case 8:
                logout();
                return; // Exit staff menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}




void registerNewMember() {
    char firstName[50], lastName[50], email[50], phone[15], status[20] = "Active";
    char username[50], password[50];

    printf("\n--- Register a New Member ---\n");
    printf("Enter First Name: ");
    scanf("%s", firstName);
    printf("Enter Last Name: ");
    scanf("%s", lastName);
    printf("Enter Email: ");
    scanf("%s", email);
    printf("Enter Phone Number: ");
    scanf("%s", phone);
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    FILE *file = fopen("members_profiles.txt", "a");
    if (!file) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    fprintf(file, "%s|%s|%s|%s|%s|%s\n", firstName, lastName, email, phone, username, password);
    fclose(file);

    printf("New member '%s %s' has been successfully registered.\n", firstName, lastName);
}

void renewMembership() {
    char username[50], buffer[300], newStatus[20];
    int found = 0;

    printf("\n--- Renew Membership ---\n");
    printf("Enter the Username of the Member: ");
    scanf("%s", username);

    FILE *file = fopen("members_profiles.txt", "r");
    FILE *tempFile = fopen("temp_members.txt", "w");
    if (!file || !tempFile) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char fileUsername[50];
        sscanf(buffer, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%s", fileUsername);

        if (strcmp(username, fileUsername) == 0) {
            found = 1;
            printf("Enter New Status (Active/Inactive): ");
            scanf("%s", newStatus);

            char updatedMember[300];
            sprintf(updatedMember, "%.*s|%s\n", (int)(strchr(buffer, '|') - buffer), buffer, newStatus);

            fprintf(tempFile, "%s", updatedMember);
        } else {
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("members_profiles.txt");
        rename("temp_members.txt", "members_profiles.txt");
        printf("Membership for '%s' has been successfully renewed.\n", username);
    } else {
        remove("temp_members.txt");
        printf("Error: No member found with the username '%s'.\n", username);
    }
}

void listAllMembers() {
    char buffer[300];

    printf("\n--- List of All Members ---\n");

    FILE *file = fopen("members_profiles.txt", "r");
    if (!file) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char firstName[50], lastName[50], email[50], phone[15], username[50], status[20];
        sscanf(buffer, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s", firstName, lastName, email, phone, username, status);
        printf("Name: %s %s | Email: %s | Phone: %s | Username: %s | Status: %s\n", 
               firstName, lastName, email, phone, username, status);
    }

    fclose(file);
}

void removeMember() {
    char username[50], buffer[300];
    int found = 0;

    printf("\n--- Remove a Member ---\n");
    printf("Enter the Username of the Member to Remove: ");
    scanf("%s", username);

    FILE *file = fopen("members_profiles.txt", "r");
    FILE *tempFile = fopen("temp_members.txt", "w");
    if (!file || !tempFile) {
        printf("Error: Unable to open members profiles file.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char fileUsername[50];
        sscanf(buffer, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%s", fileUsername);

        if (strcmp(username, fileUsername) == 0) {
            found = 1;
            continue; // Skip this member
        }

        fprintf(tempFile, "%s", buffer);
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("members_profiles.txt");
        rename("temp_members.txt", "members_profiles.txt");
        printf("Member with username '%s' has been successfully removed.\n", username);
    } else {
        remove("temp_members.txt");
        printf("Error: No member found with the username '%s'.\n", username);
    }
}

void registerNewBook() {
    char title[100], author[50], status[20] = "Available";

    printf("\n--- Register a New Book ---\n");
    printf("Enter Book Title: ");
    scanf(" %[^\n]%*c", title); // Read title with spaces
    printf("Enter Author Name: ");
    scanf(" %[^\n]%*c", author);

    FILE *file = fopen("books.txt", "a");
    if (!file) {
        printf("Error: Unable to open books file.\n");
        return;
    }

    fprintf(file, "%s|%s|%s\n", title, author, status);
    fclose(file);

    printf("New book '%s' by '%s' has been successfully registered.\n", title, author);
}

void staffViewReports() {
    printf("\n--- Reports Menu (Staff) ---\n");
    printf("1. List of Active Members\n");
    printf("2. List of All Books\n");
    printf("3. List of Available Books\n");
    printf("4. List of Currently Loaned Books\n");
    printf("5. Return to Staff Menu\n");

    int choice;
    printf("Please select a report to view: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            listAllMembers(); // Reuse the listAllMembers() function
            break;
        case 2:
            reportAllBooks(); // Assume this function exists in your program
            break;
        case 3:
            reportAvailableBooks(); // Assume this function exists
            break;
        case 4:
            reportLoanedBooks(); // Assume this function exists
            break;
        case 5:
            printf("Returning to Staff Menu...\n");
            return; // Exit reports menu
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void accountSettings() {
    char currentUsername[50], buffer[300];
    char newUsername[50], newPassword[50];
    int found = 0;

    printf("\n--- Account Settings ---\n");
    printf("Enter your current username: ");
    scanf("%s", currentUsername);

    FILE *file = fopen("staff_profiles.txt", "r");
    FILE *tempFile = fopen("temp_staff.txt", "w");
    if (!file || !tempFile) {
        printf("Error: Unable to access staff profiles.\n");
        return;
    }

    while (fgets(buffer, sizeof(buffer), file)) {
        char fileUsername[50], filePassword[50];
        sscanf(buffer, "%[^|]|%s", fileUsername, filePassword);

        if (strcmp(fileUsername, currentUsername) == 0) {
            found = 1;
            printf("Enter new username: ");
            scanf("%s", newUsername);
            printf("Enter new password: ");
            scanf("%s", newPassword);

            fprintf(tempFile, "%s|%s\n", newUsername, newPassword);
        } else {
            fprintf(tempFile, "%s", buffer);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found) {
        remove("staff_profiles.txt");
        rename("temp_staff.txt", "staff_profiles.txt");
        printf("Account updated successfully!\n");
    } else {
        remove("temp_staff.txt");
        printf("Error: Username not found.\n");
    }
}

void logout() {
    printf("\nLogging out...\n");
    // Additional cleanup actions (if needed)
    return;
}




void adminMenu() {
    int choice;

    while (1) {
        // Display admin menu
        printf("\n--- Admin Menu ---\n");
        printf("1. Add New Staff\n");
        printf("2. View Staff List\n");
        printf("3. Delete Staff\n");
        printf("4. View Reports\n");
        printf("5. Logout\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);

        // Handle menu choices
        switch (choice) {
            case 1:
                addStaff();
                break;
            case 2:
                viewStaffList();
                break;
            case 3:
                deleteStaff();
                break;
            case 4:
                viewReports();
                break;
            case 5:
                printf("Logging out...\n");
                return; // Exit the admin menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}


int main() {
    int choice;

    while (1) {
       
        printf("\nWelcome to my Library Management Software...\n");
        printf("1. Admin Login\n");
        printf("2. Staff Login\n");
        printf("3. Member Login\n");
        printf("4. Exit\n");
        printf("Please Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                staffLogin();
                break;
            case 3:
                memberLogin();
                break;
            case 4:
                printf("\nExiting the program. Goodbye!\n");
                exit(0);
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }
    return 0;
}

