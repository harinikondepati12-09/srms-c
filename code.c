#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> 

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

int loginSystem();
void getHiddenPassword(char *password);
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

struct Student{
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

//main function - display login only
int main(){
    if(loginSystem()){
        mainMenu(); 
    }else{
        printf("\nLogin failed. Exiting...\n");
    }
    return 0;
}

void getHiddenPassword(char *password){
    int i = 0;
    char ch;

    while(1){
        ch = getch(); // reads character without printing
        
        if(ch == 13) // ENTER key
            break;

        if(ch == 8 && i > 0){ // Backspace
            i--;
            printf("\b \b");
            continue;
        }

        password[i++] = ch;
        printf("*"); // print star instead of actual character
    }
    password[i] = '\0';
}

int loginSystem(){
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[20]; //copying credentails.txt

    printf("\n-Login Screen-\n");

    printf("Username:");
    scanf("%s", username);       
    printf("Password:");
    getHiddenPassword(password);

    //to open credential file 
    //refernce for comparing - username, password
    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if(!fp){
        printf("Error: credential.txt not found!\n");
        return 0;
    }
    while(fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3){
        if(strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0){ //password should match with credential password
            strcpy(currentRole, fileRole); //checking admin
            strcpy(currentUser, fileUser); //checking user
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void mainMenu(){
    if(strcmp(currentRole, "ADMIN") == 0){   //if current role is same as ADMIN display adminMenu
        adminMenu();
    }else if(strcmp(currentRole, "STAFF") == 0){   //display staffMenu
        staffMenu();
    }else{
        guestMenu();    //display Menu
    }
}

//admin-menu
void adminMenu(){
    int choice;
    do{
        printf("\n-Admin Menu-\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete student\n");
        printf("6.Logout\n");
        printf("Enter Choice:");
        scanf("%d", &choice);

        switch(choice){
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while(1);
}

//staff-menu
void staffMenu(){
    int choice;
    do{
        printf("\n-Staff Menu-\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student\n");
        printf("4.Logout\n");
        printf("Enter Choice:");
        scanf("%d", &choice);

        switch(choice){
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while(1);
}

//guest-menu
void guestMenu(){
    int choice;
    do{
        printf("\n-Guest Menu-\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3.Logout\n");
        printf("Enter Choice:");
        scanf("%d", &choice);

        switch(choice){
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: printf("Logging out...\n"); return;
            default: printf("Invalid choice!\n");
        }
    } while(1);
}

void addStudent(){
    FILE *fp = fopen(STUDENT_FILE, "a"); //append mode
    struct Student st;
    
    if(!fp){
        printf("Error opening file!\n");
        return;
    }
    printf("\nEnter Roll:");
    scanf("%d", &st.roll);
    printf("Enter Name:");
    scanf("%s", st.name);
    printf("Enter marks:");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student added successfully!\n");
}

void displayStudents(){
    FILE *fp = fopen(STUDENT_FILE, "r"); // read mode
    struct Student st;
    
    if(!fp){
        printf("No students record found!\n");
        return;
    }

    printf("\nRoll:\tName\tMarks\n");
    printf("---------------------------\n");

    while(fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3){
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
}

void searchStudent(){
    FILE *fp = fopen(STUDENT_FILE, "r"); // read mode
    struct Student st;
    int roll, found=0;

    if(!fp){
        printf("Error opening file!\n");
        return;
    }

    printf("Enter roll to search: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3){
        if(st.roll == roll){
            printf("Record found:\n");
            printf("Roll: %d\nName: %s\nMarks: %.2f\n", st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if(!found){
        printf("Record not found.\n");
    }

    fclose(fp);
}

void updateStudent(){
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if(!fp){
        printf("No student record found!\n");
        return;
    }

    printf("Enter roll to update: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3){
        if(st.roll == roll){
            found = 1;

            printf("Enter new name: ");
            scanf("%s", st.name);

            printf("Enter new marks: ");
            scanf("%f", &st.marks);

            fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
        } else {
            fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if(found)
        printf("Record updated successfully!\n");
    else
        printf("Record not found!\n");
}

void deleteStudent(){
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Student st;
    int roll, found = 0;

    if(!fp){
        printf("No student record found!\n");
        return;
    }

    printf("Enter roll to delete: ");
    scanf("%d", &roll);

    while(fscanf(fp, "%d %s %f", &st.roll, st.name, &st.marks) == 3){
        if(st.roll == roll){
            found = 1;
        } else {
            fprintf(temp, "%d %s %.2f\n", st.roll, st.name, st.marks);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if(found)
        printf("Record deleted successfully!\n");
    else
        printf("Record not found!\n");
}
