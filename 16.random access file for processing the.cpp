#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "employees.dat"

struct Employee {
    int id;
    char name[50];
    float salary;
};

void addEmployee() {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    struct Employee emp;
    printf("Enter Employee ID: ");
    scanf("%d", &emp.id);
    printf("Enter Name: ");
    scanf("%s", emp.name);
    printf("Enter Salary: ");
    scanf("%f", &emp.salary);

    fwrite(&emp, sizeof(struct Employee), 1, fp);
    fclose(fp);
    printf("Employee record added successfully.\n");
}

void displayEmployees() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    struct Employee emp;
    printf("\nEmployee Records:\n");
    while (fread(&emp, sizeof(struct Employee), 1, fp)) {
        printf("ID: %d, Name: %s, Salary: %.2f\n", emp.id, emp.name, emp.salary);
    }
    fclose(fp);
}

void searchEmployee() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int searchID;
    printf("Enter Employee ID to search: ");
    scanf("%d", &searchID);

    struct Employee emp;
    while (fread(&emp, sizeof(struct Employee), 1, fp)) {
        if (emp.id == searchID) {
            printf("Employee Found: ID: %d, Name: %s, Salary: %.2f\n", emp.id, emp.name, emp.salary);
            fclose(fp);
            return;
        }
    }
    printf("Employee with ID %d not found.\n", searchID);
    fclose(fp);
}

void modifyEmployee() {
    FILE *fp = fopen(FILE_NAME, "rb+");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int modifyID;
    printf("Enter Employee ID to modify: ");
    scanf("%d", &modifyID);

    struct Employee emp;
    while (fread(&emp, sizeof(struct Employee), 1, fp)) {
        if (emp.id == modifyID) {
            printf("Enter New Name: ");
            scanf("%s", emp.name);
            printf("Enter New Salary: ");
            scanf("%f", &emp.salary);

            fseek(fp, -sizeof(struct Employee), SEEK_CUR);
            fwrite(&emp, sizeof(struct Employee), 1, fp);
            fclose(fp);
            printf("Employee record updated successfully.\n");
            return;
        }
    }
    printf("Employee with ID %d not found.\n", modifyID);
    fclose(fp);
}

void deleteEmployee() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) {
        printf("Error opening temporary file.\n");
        fclose(fp);
        return;
    }

    int deleteID;
    printf("Enter Employee ID to delete: ");
    scanf("%d", &deleteID);

    struct Employee emp;
    int found = 0;
    while (fread(&emp, sizeof(struct Employee), 1, fp)) {
        if (emp.id == deleteID) {
            found = 1;
        } else {
            fwrite(&emp, sizeof(struct Employee), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Employee record deleted successfully.\n");
    else
        printf("Employee with ID %d not found.\n", deleteID);
}

int main() {
    int choice;
    while (1) {
        printf("\nEmployee Management System:\n");
        printf("1. Add Employee\n2. Display Employees\n3. Search Employee\n4. Modify Employee\n5. Delete Employee\n6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addEmployee(); break;
            case 2: displayEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: modifyEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: return 0;
            default: printf("Invalid choice. Try again.\n");
        }
    }
}
