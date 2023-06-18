#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKSHELF_SIZE 1000

typedef struct {
    int book_id;    //���
    char title[100];    //����
    char author[100];   //����
    int year;           //���
    char borrower[100]; //������
} Book;

typedef struct {
    Book books[MAX_BOOKSHELF_SIZE];
    int num_books;
} Library;              //�ڹ��鼮��˳��ṹ


typedef struct {
    Book books;
    struct Borrowed *next;
} Borrowed,*LinkList;              //������ʽ����ṹ

typedef struct {
    Book books[MAX_BOOKSHELF_SIZE];        //����ջ
    int top;
} Stack;

void init_stack(Stack* stack) {
    stack->top = -1;
}

int is_stack_empty(const Stack* stack) {
    return stack->top == -1;
}

int is_stack_full(const Stack* stack) {
    return stack->top == MAX_BOOKSHELF_SIZE - 1;
}

void push(Stack* stack,  Book book) {
    if (is_stack_full(stack)) {
        printf("ջ�������޷���ջ��\n");
        return;
    }

    stack->top++;
    stack->books[stack->top] = book;
}

Book pop(Stack* stack) {
    Book book;

    if (is_stack_empty(stack)) {
        printf("ջΪ�գ��޷���ջ��\n");
        strcpy(book.title, "");
        return book;
    }

    book = stack->books[stack->top];
    stack->top--;

    printf("�鼮��%s���ѳ�ջ��\n", book.title);
    return book;
}






void init_library(Library* library) {   //��ʼ��
    library->num_books = 0;
}

void InitBorrowed(LinkList *borrow){

*borrow=(LinkList)malloc(sizeof(Borrowed));
(*borrow)->next=NULL;


}


void add_book(Library* library, int book_id, const char* title, const char* author, int year) {
    if (library->num_books == MAX_BOOKSHELF_SIZE) {
        printf("ͼ����������޷�������顣\n");
        return;
    }

    Book* book = &library->books[library->num_books];
    book->book_id = book_id;
    strcpy(book->title, title);
    strcpy(book->author, author);
    book->year = year;
    book->borrower[0] = '\0';

    library->num_books++;
    printf("ͼ�顶%s������ӵ�ͼ��ݡ�\n", title);
}

void borrow_book(Library* library, int book_id, const char* borrower,Borrowed* borrowed) {
    Borrowed *s;
    int low=0;
    int mid=0;
    int high=library->num_books;
    while(low<=high){

        mid=(low+high)/2;
        if(library->books[mid].book_id == book_id){

              if (library->books[mid].borrower[0] == '\0') {
                strcpy(library->books[mid].borrower, borrower);
                s=(LinkList)malloc(sizeof(Borrowed));
                s->books=library->books[mid];
                s->next=borrowed->next;
                borrowed->next=s;


                printf("ͼ�顶%s���ѽ����%s��\n", library->books[mid].title, borrower);
                printf("****************************************\n");

                for(int i=mid;i<library->num_books-1;i++){

                    library->books[i]=library->books[i+1];

                }
                library->num_books--;


            } else {
                printf("ͼ�顶%s���ѱ������\n", library->books[mid].title);
            }
            return;
        }
        else if(book_id<library->books[mid].book_id) high=mid-1;
        else low=mid+1;

    }

    printf("�Ҳ���ָ����ŵ�ͼ�顣\n");
}

void delete_book(Library* library, int book_id) {  //���
    Borrowed *s;
    int low=0;
    int mid=0;
    int high=library->num_books;
    while(low<=high){

        mid=(low+high)/2;
        if(library->books[mid].book_id == book_id){


                for(int i=mid;i<library->num_books-1;i++){

                    library->books[i]=library->books[i+1];

                }
                library->num_books--;


            return;
        }
        else if(book_id<library->books[mid].book_id) high=mid-1;
        else low=mid+1;

    }

    printf("�Ҳ���ָ����ŵ�ͼ�顣\n");
}





void return_book(Library* library, Borrowed *borrow,Stack *stack,int book_id) {
    int i;

    Borrowed* prev = borrow;
    Borrowed* curr = borrow->next;

    while (curr != NULL) {
        if (curr->books.book_id == book_id) {
            push(stack, curr->books);
            printf("ͼ�顶%s���ѹ黹��\n", curr->books.title);
            prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    for (i = 0; i < library->num_books; i++) {
        if (library->books[i].book_id == book_id) {
           /* if (library->books[i].borrower[0] != '\0') {
                library->books[i].borrower[0] = '\0';
                printf("ͼ�顶%s���ѹ黹��\n", library->books[i].title);
            } else {
                */
                printf("��ͼ��δ�������\n");

            return;
        }

    }




    printf("�Ҳ���ָ����ŵ�ͼ�顣\n");
}

void organize_books(Library* library,Stack* stack) {
    int i, j;
    Book temp;

    while (!is_stack_empty(stack)) {
        Book book = pop(stack);

        if (library->num_books < MAX_BOOKSHELF_SIZE) {
            library->books[library->num_books] = book;
            library->num_books++;
        } else {
            printf("ͼ����������޷����ͼ�顣\n");
        }
    }


    // ��ջ���ջ
    init_stack(stack);

    // ð�����򣬰���ͼ��������
    for (i = 0; i < library->num_books - 1; i++) {
        for (j = 0; j < library->num_books - i - 1; j++) {
            if (library->books[j].book_id > library->books[j + 1].book_id) {
                temp = library->books[j];
                library->books[j] = library->books[j + 1];
                library->books[j + 1] = temp;
            }
        }
    }

    printf("ͼ���ͼ��������ɡ�\n");
}



void print_borrowed_books(LinkList borrowed) {
    if (borrowed->next == NULL) {
        printf("����Ϊ�ա�\n");
        return;
    }

    printf("�����е�ͼ����Ϣ��\n");
    Borrowed* current = borrowed->next;
    while (current != NULL) {
        printf("�����ߣ�%s\n", current->books.borrower);
        current = current->next;
    }
}


void display_library_books(const Library* library) {
    if (library->num_books == 0) {
        printf("ͼ�����û��ͼ�顣\n");
        return;
    }

    printf("ͼ����е�ͼ����Ϣ��\n");
    for (int i = 0; i < library->num_books; i++) {
        const Book* book = &library->books[i];
        printf("----------------------------------------\n");
        printf("��ţ�%d\n", book->book_id);
        printf("��������%s��\n", book->title);
        printf("���ߣ�%s\n", book->author);
        printf("��ݣ�%d\n", book->year);

    }
}


int main() {
    Library library;
    init_library(&library);
    Borrowed *borrowed;
    InitBorrowed(&borrowed);
    Stack stack;
    init_stack(&stack);
    int option;
    int number=0;//�鱾���
    char name[10];
    char author[10];



    printf("****************************************\n");
    printf("***********ͼ��ݹ���ϵͳ���***********\n");
    printf("****************************************\n");

    // ��ʼ��ͼ��
    add_book(&library,1, "ɱ����", "Priest", 2015);
    add_book(&library,3, "������", "�ƾ���", 2020);
    add_book(&library,6, "������ƫ��", "�򡤰�˹͡", 1813);
    add_book(&library,4, "���þ���������", "��̨��ɫ", 2021);
    add_book(&library,2, "����", "�໪", 1992);
    add_book(&library,5, "�ں�֮��", "��˹��������", 1895);



do{
        // ��ʾ�ݲ���Ϣ
    display_library_books(&library);



    printf("****************************************\n");
    printf("1������ 2������ 3�����鱨�� 4��������� \n");
    printf("***** 5������ 6������ͼ��� 0���˳� ****\n");
    printf("****************************************\n");




    printf("��ѡ��Ҫִ�еĲ�����");
    scanf("%d", &option);

    switch (option) {
        case 1:
            printf("�������鱾��ţ�");
            scanf("%d", &number);
            printf("��������������֣�");
            scanf("%s", &name);
            borrow_book(&library, number,name,borrowed);


            break;

        case 2:
            printf("�������鱾��ţ�");
            scanf("%d", &number);
            return_book(&library, borrowed,&stack,number);
            break;
        case 3:
            printf("�������鱾��ţ�");
            scanf("%d", &number);
            delete_book(&library,number);
            break;
        case 4:
            printf("�������鱾���ƣ�");
            scanf("%s", &name);
            printf("�������������ƣ�");
            scanf("%s", &author);
            printf("�����������ݣ�");
            scanf("%d", &number);
            add_book(&library,library.num_books+1, name, author, number);
            break;
        case 5:
                system("clear || cls");
                break;
        case 6:
                organize_books(&library,&stack);
                break;

        case 0:
                break;

        default:
                printf("��Ч�Ĳ������ͣ�������ѡ��\n");
                break;

    }



}while (option != 0);






/*




    // ����ʾ��
    borrow_book(&library, 1, "John",borrowed);
        // ͼ������ʾ��
    organize_books(&library);

            // ��ʾ�ݲ���Ϣ
    display_library_books(&library);



    borrow_book(&library, 2, "Alice",borrowed);
    borrow_book(&library, 4, "Bob",borrowed);  // ���Խ��Ĳ����ڵ�ͼ��



    print_borrowed_books(borrowed);

    // ����ʾ��
    return_book(&library, 1);
    return_book(&library, 3);  // ���Թ黹�����ڵ�ͼ��

            // ��ʾ�ݲ���Ϣ
    display_library_books(&library);


    // ͼ������ʾ��
    organize_books(&library);



*/

    return 0;
}
