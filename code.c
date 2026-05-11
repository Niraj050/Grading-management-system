#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Student {
    char firstname[25];
    char lastname[25];
    char dob[12];
    char school[100];
    char regNumber[20];
    int numSubjects;
    char subjects[10][50];
    int theoryMarks[10];
    int practicalMarks[10];
    float gpa;
};

float calculateGPA(int marks) {
    if (marks >= 90) return 4.0;
    else if (marks >= 80) return 3.6;
    else if (marks >= 70) return 3.2;
    else if (marks >= 60) return 2.8;
    else if (marks >= 50) return 2.4;
    else if (marks >= 40) return 2.0;
    else if (marks >= 35) return 1.6;
    else return 0.0;
}

void linef() {
    printf("---------------------------------------------------------------\n");
}

int checkadmin() {
    char u[25], p[25];
    printf("\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    return (strcmp(u,"admin")==0 && strcmp(p,"adminpass")==0);
}

void inputstudent(struct Student *s) {
    printf("\nFirst Name: "); scanf("%s", s->firstname);
    printf("Last Name: "); scanf("%s", s->lastname);
    printf("DOB (yyyy-mm-dd): "); scanf("%s", s->dob);
    printf("School: "); scanf(" %[^\n]", s->school);
    printf("Reg No: "); scanf("%s", s->regNumber);

    printf("Number of Subjects: ");
    scanf("%d", &s->numSubjects);

    float total=0;
    for(int i=0;i<s->numSubjects;i++){
        printf("\nSubject %d: ", i+1);
        scanf("%s", s->subjects[i]);
        printf("Theory Marks: ");
        scanf("%d", &s->theoryMarks[i]);
        printf("Practical Marks: ");
        scanf("%d", &s->practicalMarks[i]);

        float g = (calculateGPA(s->theoryMarks[i]) +
                   calculateGPA(s->practicalMarks[i]))/2.0;
        total += g;
    }
    s->gpa = total/s->numSubjects;
}

void saveStudentData(struct Student *s){
    FILE *f = fopen("student_data.txt","a");
    fprintf(f,"RNO %s\n",s->regNumber);
    fprintf(f,"%s %s\n",s->firstname,s->lastname);
    fprintf(f,"%s\n",s->dob);
    fprintf(f,"%s\n",s->school);
    fprintf(f,"%d\n",s->numSubjects);
    for(int i=0;i<s->numSubjects;i++){
        fprintf(f,"%s %d %d\n",s->subjects[i],
                s->theoryMarks[i],s->practicalMarks[i]);
    }
    fprintf(f,"%.2f\n",s->gpa);
    fprintf(f,"END\n");
    fclose(f);
}

int searchstudentdata(char reg[], char dob[], struct Student *s){
    FILE *f = fopen("student_data.txt","r");
    if(!f) return 0;

    while(!feof(f)){
        fscanf(f,"RNO %s\n",s->regNumber);
        fscanf(f,"%s %s\n",s->firstname,s->lastname);
        fscanf(f,"%s\n",s->dob);
        fscanf(f," %[^\n]",s->school);
        fscanf(f,"%d\n",&s->numSubjects);

        for(int i=0;i<s->numSubjects;i++){
            fscanf(f,"%s %d %d\n",s->subjects[i],
                   &s->theoryMarks[i],&s->practicalMarks[i]);
        }
        fscanf(f,"%f\n",&s->gpa);
        char end[10]; fscanf(f,"%s\n",end);

        if(strcmp(s->regNumber,reg)==0 && strcmp(s->dob,dob)==0){
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void displaymarksheet(struct Student *s){
    system("cls");
    linef();
    printf("\tNEB Academic Transcript\n");
    linef();
    printf("Name: %s %s\n",s->firstname,s->lastname);
    printf("Reg No: %s\n",s->regNumber);
    printf("DOB: %s\n",s->dob);
    printf("School: %s\n",s->school);
    linef();

    float total=0;
    printf("Subject\tTH\tPR\tGPA\n");
    for(int i=0;i<s->numSubjects;i++){
        float g = (calculateGPA(s->theoryMarks[i]) +
                   calculateGPA(s->practicalMarks[i]))/2.0;
        total+=g;
        printf("%s\t%d\t%d\t%.2f\n",
               s->subjects[i],
               s->theoryMarks[i],
               s->practicalMarks[i],
               g);
    }
    linef();
    printf("Final GPA: %.2f\n", total/s->numSubjects);
    linef();
}

int main(){
    struct Student s;
    char ch;

    while(1){
        printf("\n1.Admin\n2.Student\n3.Exit\n");
        ch=getch();

        if(ch=='1'){
            if(checkadmin()){
                inputstudent(&s);
                saveStudentData(&s);
                printf("\nSaved Successfully!\n");
            }
        }
        else if(ch=='2'){
            char reg[20],dob[12];
            printf("\nReg No: "); scanf("%s",reg);
            printf("DOB: "); scanf("%s",dob);
            if(searchstudentdata(reg,dob,&s))
                displaymarksheet(&s);
            else
                printf("Student Not Found!\n");
        }
        else if(ch=='3') break;
    }
}