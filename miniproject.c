#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

struct sub{
    char subt[100];//The name of the subtask
    struct sub* next;//The next pointer to the next subtask
};
struct node{
    struct tm date;//due date
    int priority;//Priority of task in 1-5
    char task[100];//The name of the task
    struct node *next;//The pointer to next most immediate task
    int status; // 1 if completed, 0 if not
    struct sub *s;//The pointer to list of subtasks assosciated with this array
    int cnt;//The no of subtasks
};

void display(struct node*, int);//function to display tasks
void insert(struct node**);//function to insert tasks


int main()
{
    system("clear");
    printf("-------------------------------------------------------------------");
    printf("\n\t\t\tTO-DO LIST ");
    printf("\n-------------------------------------------------------------------\n");

    struct node *first;
    int ch,x;
    first=NULL;//pointer to the first node of the list
    char c='n';
    while(1){
        if(c=='y')
            system("clear");
        printf("\n\n-------------------------------------------------------------------");
        printf("\n\t\t\t    MENU ");
        printf("\n-------------------------------------------------------------------\n");

        printf("\n1.Insert Task\n");
        printf("2.View Tasks\n");
        printf("3.View Highest Priority Tasks\n");
        printf("4.View Completed Tasks\n");
        printf("5.Mark Task as Completed\n");
        printf("6.Edit/Delete Task\n");
        printf("7.Save and Exit\n");
        scanf("%d",&ch);
        switch(ch){
        case 1:insert(&first);
                break;
        case 2:display(first,6);
                break;
        case 3: //TODO
                break;
        case 4: //TODO
                    break;
        case 5: //TODO
                break;
        case 6: //TODO
                break;
        case 7://TODO
                exit(0);
                break;
        default: printf("Enter a valid choice.\n");
                break;
        }
        printf("\nContinue?(y/n): ");
        scanf("\n");
        c=fgetc(stdin);
        if(c!='y')
            break;
    }
    exit(0);
}

//function to display tasks
void display(struct node* temp, int cutoff){
    if(temp==NULL){
        printf("NO WORK TO DO! GO HAVE SOME FUN!!\n");
        return;
    }
    
    int i=1;
    while (temp!=NULL) {
        if(temp->priority<cutoff){
            display_current(temp,i);
        }
        temp=temp->next;
        i+=1;
    }
}

//function to display required task
void display_current(struct node*current,int i){
    printf("\n%d. Task Name : %s",i,current->task);
    printf("Task Priority : %d\n",current->priority);
    printf("Due Date : %d/%d/%d\n",(current->date).tm_mday,(current->date).tm_mon,(current->date).tm_year);
    struct sub *subtask=current->s;

    if(subtask!=NULL)
        printf("Here are the subtasks for this task: ");
    int j=1;

    //loop traverses through subtasks
    while(subtask!=NULL){
        printf("\n\t%d. Subtask Name: %s",j,subtask->subt);
        j++;
        subtask=subtask->next;
    }
}

int no_of_tasks=0;
char  names[100][100];//stores list of task names used to ensure uniqueness

//function to insert tasks
void insert(struct node** list){
    no_of_tasks+=1;

    struct node *temp;
    temp=(struct node*)malloc(sizeof(struct node));
    temp->next=NULL;

    printf("Enter the following details! \n");

    char name[100],temp_sub;
    getuniquename:
    printf("Name of your task : ");
    scanf("\n");
    fgets(name,100,stdin);
    if(isunique(name)){
        strcpy(names[no_of_tasks],name);
    }
    else{
        printf("Enter unique name\n" );
        goto getuniquename;
    }

    printf("Priority in 1-5 : ");
    int priority;
    priority=getdig();
    while(validate_info(priority)==0){
        printf("INVALID PRIORITY\n");
        printf("Enter the priority again: ");
        priority=getdig();
     }

    printf("Due Date in DD MM YYYY format : ");
    struct tm date;
    getdat(&date.tm_mday,&date.tm_mon,&date.tm_year);

    // TODO: validate date format

    strcpy(temp->task,name);
    temp->priority=priority;
    temp->date=date;
    temp->status=0;
    temp->cnt=0;

    printf("Does this task have subtasks?(y/n)" );
    scanf("\n");
    scanf("%c",&temp_sub);
    if (temp_sub=='y'){
        temp->cnt+=1;
        struct sub *prev_s;
        prev_s=(struct sub*)malloc(sizeof(struct sub));
        printf("Enter subtask : ");
        scanf("\n");
        fgets(prev_s->subt,100,stdin);
        prev_s->next=NULL;
        temp->s=prev_s;
        printf("More subtasks?(y/n)\n");
        scanf("%c",&temp_sub);
        while(temp_sub=='y'){
            temp->cnt+=1;
            printf("Enter subtask : ");
            struct sub *curr_s;
            curr_s=(struct sub*)malloc(sizeof(struct sub));
            scanf("\n");
            fgets(curr_s->subt,100,stdin);
            curr_s->next=NULL;
            prev_s->next=curr_s;
            prev_s=prev_s->next;
            printf("More subtasks?(y/n)\n");
            scanf("%c",&temp_sub);
        }
    }
    if(*list==NULL){

        *list=temp;
        return;
    }
    struct node *curr;
    struct node *prev;
    prev=NULL;
    curr=*list;
    while((curr!=NULL)&&(compare_date(temp->date,curr->date))){
        prev=curr;
        curr=curr->next;
    }
    if(prev!=NULL && compare_date(temp->date,prev->date)==2){
        curr=prev;
        prev=NULL;

        while((curr!=NULL)&&(compare_date(temp->date,curr->date)==2&&(curr->priority<=temp->priority))){
            prev=curr;
            curr=curr->next;
        }
    }
    if(curr==NULL){
        prev->next=temp;
        return;
    }
    else{
        if(prev==NULL){
            temp->next=curr;
            *list=temp;
        }
        else{
            prev->next=temp;
            temp->next=curr;
        }
    }
}

//checks if entered name is unique
int isunique(char * name){
    int i;
    for (i=1;i<=no_of_tasks;i++){
        if(strcmp(names[i],name)==0)
            return 0;
    }
    return 1;
}

//gets character and converts it to be used as a number
int getdig(){
    char buf[5];
    int i;

    if (fgets(buf, sizeof(buf), stdin) != NULL){//checks if input was successful
        i = atoi(buf);
    }
    return(i);
}

//Validation of priority.
int validate_info(int p){
    if(!(p>=1 && p<=5))
        return 0;
    return 1;
}

//function to read date in required format
void getdat(int *date,int*month,int*year){
    char buf[12];

    if (fgets(buf, sizeof(buf), stdin) != NULL){//checks if input was successful
        *date = atoi(buf);
        *month=atoi(buf+2);
        *year=atoi(buf+5);
    }
}

//function to compare two dates
int compare_date(struct tm temp,struct tm curr){
    if((temp).tm_year>(curr).tm_year)
        return 1;
    else if((temp).tm_year<(curr).tm_year)
        return 0;
    if((temp).tm_mon>(curr).tm_mon)
        return 1;
    else if((temp).tm_mon<(curr).tm_mon)
        return 0;
    if((temp).tm_mday>(curr).tm_mday)
        return 1;
    else if((temp).tm_mday<(curr).tm_mday)
        return 0;
    return 2;
}
