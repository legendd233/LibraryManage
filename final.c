#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BOOKSHELF_SIZE 1000

typedef struct {
    int book_id;    //编号
    char title[100];    //书名
    char author[100];   //作者
    int year;           //年份
    char borrower[100]; //借阅者
} Book;

typedef struct {
    Book books[MAX_BOOKSHELF_SIZE];
    int num_books;
} Library;              //在馆书籍，顺序结构


typedef struct {
    Book books;
    struct Borrowed *next;
} Borrowed,*LinkList;              //外借表，链式储存结构

typedef struct {
    Book books[MAX_BOOKSHELF_SIZE];        //还书栈
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
        printf("栈已满，无法入栈。\n");
        return;
    }

    stack->top++;
    stack->books[stack->top] = book;
}

Book pop(Stack* stack) {
    Book book;

    if (is_stack_empty(stack)) {
        printf("栈为空，无法出栈。\n");
        strcpy(book.title, "");
        return book;
    }

    book = stack->books[stack->top];
    stack->top--;

    printf("书籍《%s》已出栈。\n", book.title);
    return book;
}






void init_library(Library* library) {   //初始化
    library->num_books = 0;
}

void InitBorrowed(LinkList *borrow){

*borrow=(LinkList)malloc(sizeof(Borrowed));
(*borrow)->next=NULL;


}


void add_book(Library* library, int book_id, const char* title, const char* author, int year) {
    if (library->num_books == MAX_BOOKSHELF_SIZE) {
        printf("图书馆已满，无法添加新书。\n");
        return;
    }

    Book* book = &library->books[library->num_books];
    book->book_id = book_id;
    strcpy(book->title, title);
    strcpy(book->author, author);
    book->year = year;
    book->borrower[0] = '\0';

    library->num_books++;
    printf("图书《%s》已添加到图书馆。\n", title);
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


                printf("图书《%s》已借出给%s。\n", library->books[mid].title, borrower);
                printf("****************************************\n");

                for(int i=mid;i<library->num_books-1;i++){

                    library->books[i]=library->books[i+1];

                }
                library->num_books--;


            } else {
                printf("图书《%s》已被借出。\n", library->books[mid].title);
            }
            return;
        }
        else if(book_id<library->books[mid].book_id) high=mid-1;
        else low=mid+1;

    }

    printf("找不到指定编号的图书。\n");
}

void delete_book(Library* library, int book_id) {  //清除
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

    printf("找不到指定编号的图书。\n");
}





void return_book(Library* library, Borrowed *borrow,Stack *stack,int book_id) {
    int i;

    Borrowed* prev = borrow;
    Borrowed* curr = borrow->next;

    while (curr != NULL) {
        if (curr->books.book_id == book_id) {
            push(stack, curr->books);
            printf("图书《%s》已归还。\n", curr->books.title);
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
                printf("图书《%s》已归还。\n", library->books[i].title);
            } else {
                */
                printf("该图书未被借出。\n");

            return;
        }

    }




    printf("找不到指定编号的图书。\n");
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
            printf("图书馆已满，无法添加图书。\n");
        }
    }


    // 清空还书栈
    init_stack(stack);

    // 冒泡排序，按照图书编号排序
    for (i = 0; i < library->num_books - 1; i++) {
        for (j = 0; j < library->num_books - i - 1; j++) {
            if (library->books[j].book_id > library->books[j + 1].book_id) {
                temp = library->books[j];
                library->books[j] = library->books[j + 1];
                library->books[j + 1] = temp;
            }
        }
    }

    printf("图书馆图书整理完成。\n");
}



void print_borrowed_books(LinkList borrowed) {
    if (borrowed->next == NULL) {
        printf("外借表为空。\n");
        return;
    }

    printf("外借表中的图书信息：\n");
    Borrowed* current = borrowed->next;
    while (current != NULL) {
        printf("借阅者：%s\n", current->books.borrower);
        current = current->next;
    }
}


void display_library_books(const Library* library) {
    if (library->num_books == 0) {
        printf("图书馆中没有图书。\n");
        return;
    }

    printf("图书馆中的图书信息：\n");
    for (int i = 0; i < library->num_books; i++) {
        const Book* book = &library->books[i];
        printf("----------------------------------------\n");
        printf("编号：%d\n", book->book_id);
        printf("书名：《%s》\n", book->title);
        printf("作者：%s\n", book->author);
        printf("年份：%d\n", book->year);

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
    int number=0;//书本序号
    char name[10];
    char author[10];



    printf("****************************************\n");
    printf("***********图书馆管理系统设计***********\n");
    printf("****************************************\n");

    // 初始化图书
    add_book(&library,1, "杀破狼", "Priest", 2015);
    add_book(&library,3, "将进酒", "唐酒卿", 2020);
    add_book(&library,6, "傲慢与偏见", "简·奥斯汀", 1813);
    add_book(&library,4, "穿堂惊掠琵琶声", "高台树色", 2021);
    add_book(&library,2, "活着", "余华", 1992);
    add_book(&library,5, "乌合之众", "古斯塔夫·勒庞", 1895);



do{
        // 显示馆藏信息
    display_library_books(&library);



    printf("****************************************\n");
    printf("1、借书 2、还书 3、旧书报废 4、新书入库 \n");
    printf("***** 5、清屏 6、整理图书馆 0、退出 ****\n");
    printf("****************************************\n");




    printf("请选择要执行的操作：");
    scanf("%d", &option);

    switch (option) {
        case 1:
            printf("请输入书本序号：");
            scanf("%d", &number);
            printf("请输入借阅者名字：");
            scanf("%s", &name);
            borrow_book(&library, number,name,borrowed);


            break;

        case 2:
            printf("请输入书本序号：");
            scanf("%d", &number);
            return_book(&library, borrowed,&stack,number);
            break;
        case 3:
            printf("请输入书本序号：");
            scanf("%d", &number);
            delete_book(&library,number);
            break;
        case 4:
            printf("请输入书本名称：");
            scanf("%s", &name);
            printf("请输入作者名称：");
            scanf("%s", &author);
            printf("请输入出版年份：");
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
                printf("无效的操作类型，请重新选择。\n");
                break;

    }



}while (option != 0);






/*




    // 借书示例
    borrow_book(&library, 1, "John",borrowed);
        // 图书整理示例
    organize_books(&library);

            // 显示馆藏信息
    display_library_books(&library);



    borrow_book(&library, 2, "Alice",borrowed);
    borrow_book(&library, 4, "Bob",borrowed);  // 尝试借阅不存在的图书



    print_borrowed_books(borrowed);

    // 还书示例
    return_book(&library, 1);
    return_book(&library, 3);  // 尝试归还不存在的图书

            // 显示馆藏信息
    display_library_books(&library);


    // 图书整理示例
    organize_books(&library);



*/

    return 0;
}
