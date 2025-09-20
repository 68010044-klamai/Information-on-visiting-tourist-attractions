#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "visitors.csv"

typedef struct {
    char visitorName[100];
    char destination[100];
    char visitDate[20];     // YYYY-MM-DD
    int numberOfPeople;
} VisitRecord;

// ------------------ ฟังก์ชันแสดงเมนู ------------------
void displayMenu() {
    printf("\n=== Visitor Management System ===\n");
    printf("1. Read all records from CSV\n");
    printf("2. Add new record\n");
    printf("3. Search record\n");
    printf("4. Update record\n");
    printf("5. Delete record\n");
    printf("0. Exit\n");
    printf("Choose: ");
}

// ------------------ อ่านข้อมูลจาก CSV ------------------
void readCSV(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line[256];
    printf("\n--- Visitor Records ---\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);
}

// ------------------ เพิ่มข้อมูลใหม่ ------------------
void addRecord(const char *filename) {
    FILE *fp = fopen(filename, "a"); // append mode
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    VisitRecord v;
    printf("Enter Visitor Name: ");
    scanf(" %[^\n]", v.visitorName);
    printf("Enter Destination: ");
    scanf(" %[^\n]", v.destination);
    printf("Enter Visit Date (YYYY-MM-DD): ");
    scanf("%s", v.visitDate);
    printf("Enter Number of People: ");
    scanf("%d", &v.numberOfPeople);

    fprintf(fp, "%s,%s,%s,%d\n", v.visitorName, v.destination, v.visitDate, v.numberOfPeople);
    fclose(fp);
    printf("✅ Record added successfully.\n");
}

// ------------------ ค้นหาข้อมูล ------------------
void searchRecord(const char *filename, const char *keyword) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    char line[256];
    int found = 0;

    printf("\n--- Search Results for '%s' ---\n", keyword);
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, keyword)) { // ถ้าเจอ keyword ในบรรทัด
            printf("%s", line);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching records found.\n");
    }

    fclose(fp);
}

// ------------------ อัปเดตข้อมูล ------------------
void updateRecord(const char *filename, const char *name) {
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!fp || !temp) {
        printf("File error.\n");
        return;
    }

    char line[256];
    int updated = 0;

    while (fgets(line, sizeof(line), fp)) {
        VisitRecord v;
        sscanf(line, "%99[^,],%99[^,],%19[^,],%d",
               v.visitorName, v.destination, v.visitDate, &v.numberOfPeople);

        if (strcmp(v.visitorName, name) == 0) {
            printf("Updating record for %s\n", v.visitorName);
            printf("Enter new Destination: ");
            scanf(" %[^\n]", v.destination);
            printf("Enter new Visit Date (YYYY-MM-DD): ");
            scanf("%s", v.visitDate);
            printf("Enter new Number of People: ");
            scanf("%d", &v.numberOfPeople);

            fprintf(temp, "%s,%s,%s,%d\n", v.visitorName, v.destination, v.visitDate, v.numberOfPeople);
            updated = 1;
        } else {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(filename);
    rename("temp.csv", filename);

    if (updated) {
        printf("✅ Record updated successfully.\n");
    } else {
        printf("No record found for %s\n", name);
    }
}

// ------------------ ลบข้อมูล ------------------
void deleteRecord(const char *filename, const char *name) {
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.csv", "w");

    if (!fp || !temp) {
        printf("File error.\n");
        return;
    }

    char line[256];
    int deleted = 0;

    while (fgets(line, sizeof(line), fp)) {
        VisitRecord v;
        sscanf(line, "%99[^,],%99[^,],%19[^,],%d",
               v.visitorName, v.destination, v.visitDate, &v.numberOfPeople);

        if (strcmp(v.visitorName, name) == 0) {
            printf("Deleting record for %s\n", v.visitorName);
            deleted = 1;
            continue; // ไม่เขียนบรรทัดนี้ลง temp.csv
        } else {
            fputs(line, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(filename);
    rename("temp.csv", filename);

    if (deleted) {
        printf("✅ Record deleted successfully.\n");
    } else {
        printf("No record found for %s\n", name);
    }
}

// ------------------ main ------------------
int main() {
    int choice;
    char keyword[100];
    char name[100];

    do {
        displayMenu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                readCSV(FILENAME);
                break;
            case 2:
                addRecord(FILENAME);
                break;
            case 3:
                printf("Enter keyword (Name or Destination): ");
                scanf(" %[^\n]", keyword);
                searchRecord(FILENAME, keyword);
                break;
            case 4:
                printf("Enter visitor name to update: ");
                scanf(" %[^\n]", name);
                updateRecord(FILENAME, name);
                break;
            case 5:
                printf("Enter visitor name to delete: ");
                scanf(" %[^\n]", name);
                deleteRecord(FILENAME, name);
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while(choice != 0);

    return 0;
}
