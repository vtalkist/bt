#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_INIT_SIZE 100
#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))

typedef struct info
{ 
    int code; 
    char* message; 
} INFO; 

typedef struct node 
{
    INFO data; 
    struct node* left; 
    struct node* right; 
} NODE;

typedef struct stack
{
    size_t size;
    size_t limit;
    NODE** data;
} Stack;

NODE* newNode(void);
int getData(INFO* element);
void addNode(NODE* element, NODE** root);
void printTree(NODE* root);
Stack* createStack();
void freeStack(Stack** s);
void push(Stack* s, NODE* item);
NODE* pop(Stack* s);
NODE* peek(Stack* s);
void iterPostorder(NODE* root);
NODE* getNodeByValue(NODE* root, int value);
int binarytreeCountRecursive(const NODE* root);
int getElementsWithOneChild(const NODE* root);
double getOneChildPercentsElement(const NODE* root);
void freeNodeMemory(NODE* node);
void freeTree(NODE* root);
char* gets(char* str);



NODE* newNode(void) 
{
    INFO buf; 
    NODE* newNode = malloc(sizeof(NODE)); 
    if (getData(&buf) == 0) 
        return NULL; 
    newNode->data.code = buf.code; 
    newNode->data.message = malloc(strlen(buf.message) + 1); 
    strcpy(newNode->data.message, buf.message); 
    newNode->left = NULL; 
    
    newNode->right = NULL; 
    return newNode; 
}

int getData(INFO* element) 
{
    char buf[150];
    printf("Введіть код повідомлення\n");
    scanf_s("%d", &element->code);
    printf("Введіть повідомлення\n");
    while (getchar() != '\n');
    if (*gets(buf) == '-')
        return 0;
    element->message = (char*)malloc(strlen(buf) + 1);
    strcpy(element->message, buf);
    return 1;
}

void addNode(NODE* element, NODE** root) 
{ 
    NODE* proot = *root;  
    if (proot == NULL) { 
        *root = element; 
        return; 
    }
    if (element->data.code < proot->data.code) 
        addNode(element, &proot->left); 
    else if (element->data.code > proot->data.code)
        addNode(element, &proot->right);  
}

void printTree(NODE* root) 
{ 
    if (root == NULL) return; 
    printTree(root->left); 
    printf("%d -> %s ", root->data.code, root->data.message);
    printTree(root->right); 
}

//****************************************************//
//                       STACK                        //
//****************************************************//

Stack* createStack()
{
    Stack* tmp = (Stack*)malloc(sizeof(Stack));
    tmp->limit = STACK_INIT_SIZE;
    tmp->size = 0;
    tmp->data = (NODE**)malloc(tmp->limit * sizeof(NODE*));
    return tmp;
}

void freeStack(Stack** s) 
{
    free((*s)->data);
    free(*s);
    *s = NULL;
}

void push(Stack* s, NODE* item) 
{
    if (s->size >= s->limit) {
        s->limit *= 2;
        s->data = (NODE**)realloc(s->data, s->limit * sizeof(NODE*));
    }
    s->data[s->size++] = item;
}

NODE* pop(Stack* s) 
{
    if (s->size == 0) {
        exit(7);
    }
    s->size--;
    return s->data[s->size];
}

NODE* peek(Stack* s) 
{
    return s->data[s->size - 1];
}

void iterPostorder(NODE* root) 
{ 
    Stack* ps = createStack();
    NODE* lnp = NULL;
    NODE* peekn = NULL;
    int count = 0;
    while (!ps->size == 0 || root != NULL) {
        if (root) {
            push(ps, root);
            root = root->left;
        }
        else {
            peekn = peek(ps);
            if (peekn->right && lnp != peekn->right) {
                root = peekn->right;
            }
            else {
                pop(ps);
                if ((peekn->left == NULL) ^ (peekn->right == NULL))
                {
                    printf("%d %s\n", peekn->data.code, peekn->data.message);
                    count++;
                }

                lnp = peekn;
            }
        }
    }
    if (count == 0)
        printf("Вузлів з одним дочірнім елементом не знайдено!!!\n");
    freeStack(&ps);
}
//****************************************************//
//                       STACK                        //
//****************************************************//

NODE* getNodeByValue(NODE* root, int value) 
{ 
    while (root) {
        if (CMP_GT(root->data.code, value)) {
            root = root->left;
            continue;
        }
        else if (CMP_LT(root->data.code, value)) {
            root = root->right;
            continue;
        }
        else {
            return root;
        }
    }
    return NULL;
}

int binarytreeCountRecursive(const NODE* root)
{
    int count = 1;
    if (root->left != NULL) {
        count += binarytreeCountRecursive(root->left);
    }
    if (root->right != NULL) {
        count += binarytreeCountRecursive(root->right);
    }
    return count;
}

int getElementsWithOneChild(const NODE* root)
{
    int kst = 0;
    if (!root) return 0;
    if ((root->left == NULL) ^ (root->right == NULL))
        kst = 1;
    return kst + getElementsWithOneChild(root->left) + getElementsWithOneChild(root->right);
}

double getOneChildPercentsElement(const NODE* root)
{
    int all_elem_count = binarytreeCountRecursive(root);
    int one_child_element_count = getElementsWithOneChild(root);
    if (all_elem_count == 0 || one_child_element_count == 0)
        return 0;
    return ((double)one_child_element_count / (double)all_elem_count * 100);
}

void freeNodeMemory(NODE* node)
{
    free(node->data.message);
    free(node);
}

void freeTree(NODE* root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    freeNodeMemory(root);
}

int main()
{
    system("chcp 1251");
    NODE* root = NULL;
    NODE* node;
    int check_node_code_value = 0;
    printf("Щоб зупинити ввід, введіть в поле коду повідомлення знак -\n");
    while ((node = newNode()) != NULL) 
        addNode(node, &root);
    printf("===================================================\nВведіть код, який потрібно знайти: ");
    scanf_s("%d", &check_node_code_value);
    if (getNodeByValue(root, check_node_code_value) != NULL)
        printf("Повідомлення із заданим кодом знайдено! %d %s\n", getNodeByValue(root, 3)->data.code, getNodeByValue(root, 3)->data.message);
    else
        printf("Повідомлення із заданим кодом не знайдено!\n");
    printf("Відсоток вузлів з одним дочірнім становить: %lf\n", getOneChildPercentsElement(root));
    printf("Вузли з одним дочірнім та їх значення:\n");
    iterPostorder(root);
    freeTree(root);
    system("pause");
    return 0;
}
