#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LEN 1024

typedef struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

TreeNode* createNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

typedef struct {
    TreeNode* data[MAX_LEN];
    int top;
} NodeStack;

void initNodeStack(NodeStack* s) { s->top = -1; }
int isNodeStackEmpty(NodeStack* s) { return s->top == -1; }
void pushNode(NodeStack* s, TreeNode* node) { s->data[++(s->top)] = node; }
TreeNode* popNode(NodeStack* s) { return isNodeStackEmpty(s) ? NULL : s->data[(s->top)--]; }
TreeNode* peekNode(NodeStack* s) { return isNodeStackEmpty(s) ? NULL : s->data[s->top]; }

typedef struct {
    TreeNode* data[MAX_LEN];
    int front;
    int rear;
} Queue;

void initQueue(Queue* q) { q->front = 0; q->rear = 0; }
int isQueueEmpty(Queue* q) { return q->front == q->rear; }
void enqueue(Queue* q, TreeNode* node) { q->data[q->rear++] = node; }
TreeNode* dequeue(Queue* q) { return isQueueEmpty(q) ? NULL : q->data[q->front++]; }

TreeNode* parseToLinkedTree(const char* str) {
    NodeStack stack;
    initNodeStack(&stack);

    TreeNode* root = NULL;
    TreeNode* last_created = NULL;
    int child_state = 0; 

    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        char ch = str[i];
        if (ch == ' ' || ch == '\t' || ch == '\n') continue;

        if (isupper(ch)) {
            TreeNode* newNode = createNode(ch);
            last_created = newNode;

            if (root == NULL) {
                root = newNode; 
            }
            else {
                TreeNode* parent = peekNode(&stack);
                if (parent != NULL) {
                    if (child_state == 1) {
                        parent->left = newNode;
                    }
                    else if (child_state == 2) {
                        parent->right = newNode;
                    }
                }
            }
        }
        else if (ch == '(') {
            if (last_created != NULL) {
                pushNode(&stack, last_created);
            }
            child_state = 1; 
        }
        else if (ch == ',') {
            child_state = 2;
        }
        else if (ch == ')') {
            popNode(&stack);
            child_state = 0;
        }
    }

    return root;
}

void printTreeLinked(TreeNode* root, int depth, int has_more_siblings[]) {
    if (root == NULL) return;

    if (depth == 0) {
        printf("%c\n", root->data);
    }
    else {
        for (int d = 1; d < depth; d++) {
            if (has_more_siblings[d]) printf("|   ");
            else printf("    ");
        }
        printf("+---%c\n", root->data);
    }

    int has_left = (root->left != NULL);
    int has_right = (root->right != NULL);

    if (has_left) {
        has_more_siblings[depth + 1] = has_right;
        printTreeLinked(root->left, depth + 1, has_more_siblings);
    }

    if (has_right) {
        has_more_siblings[depth + 1] = 0; 
        printTreeLinked(root->right, depth + 1, has_more_siblings);
    }
}

int countTotalNodes(TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + countTotalNodes(root->left) + countTotalNodes(root->right);
}

int countLeafNodes(TreeNode* root) {
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return countLeafNodes(root->left) + countLeafNodes(root->right);
}

int countNonLeafNodes(TreeNode* root) {
    if (root == NULL || (root->left == NULL && root->right == NULL)) return 0;
    return 1 + countNonLeafNodes(root->left) + countNonLeafNodes(root->right);
}

int getTreeHeight(TreeNode* root) {
    if (root == NULL) return 0;
    int left_h = getTreeHeight(root->left);
    int right_h = getTreeHeight(root->right);
    return 1 + (left_h > right_h ? left_h : right_h);
}

int getMaxDegree(TreeNode* root) {
    if (root == NULL) return 0;

    int current_degree = 0;
    if (root->left != NULL) current_degree++;
    if (root->right != NULL) current_degree++;

    int left_deg = getMaxDegree(root->left);
    int right_deg = getMaxDegree(root->right);

    int max_child_deg = (left_deg > right_deg) ? left_deg : right_deg;
    return (current_degree > max_child_deg) ? current_degree : max_child_deg;
}

void printTreeInfo(TreeNode* root) {
    printf("\n--- [2] 트리 정보 (연결 자료구조) ---\n");
    printf("1. 전체 노드의 수 : %d\n", countTotalNodes(root));
    printf("2. 단말 노드의 수 : %d\n", countLeafNodes(root));
    printf("3. 비단말 노드의 수: %d\n", countNonLeafNodes(root));
    printf("4. 트리의 높이     : %d\n", getTreeHeight(root));
    printf("5. 트리의 차수     : %d\n", getMaxDegree(root));
}

int isCompleteBinaryTree(TreeNode* root) {
    if (root == NULL) return 1;

    Queue q;
    initQueue(&q);
    enqueue(&q, root);

    int null_flag = 0; 

    while (!isQueueEmpty(&q)) {
        TreeNode* current = dequeue(&q);

        if (current == NULL) {
            null_flag = 1; 
        }
        else {
            if (null_flag) {
                return 0;
            }
            enqueue(&q, current->left);
            enqueue(&q, current->right);
        }
    }

    return 1;
}

int isFullBinaryTree(TreeNode* root) {
    int total_nodes = countTotalNodes(root);
    int height = getTreeHeight(root);
    int expected_nodes = (int)pow(2, height) - 1;

    return (total_nodes == expected_nodes);
}

int isSkewedBinaryTree(TreeNode* root) {
    int total_nodes = countTotalNodes(root);
    int height = getTreeHeight(root);

    return (total_nodes == height);
}

void checkTreeTypes(TreeNode* root) {
    printf("\n--- [3] 이진트리 형태 판별 ---\n");
    printf("1. 완전 이진트리 여부: %s\n", isCompleteBinaryTree(root) ? "예 (Yes)" : "아니오 (No)");
    printf("2. 포화 이진트리 여부: %s\n", isFullBinaryTree(root) ? "예 (Yes)" : "아니오 (No)");
    printf("3. 편향 이진트리 여부: %s\n", isSkewedBinaryTree(root) ? "예 (Yes)" : "아니오 (No)");
}

void freeTree(TreeNode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    char input[MAX_LEN];

    printf("이진트리의 괄호 표기법을 입력하세요 (예: A(B(D,E),C(,F))):\n> ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;

    TreeNode* root = parseToLinkedTree(input);

    if (root == NULL) {
        printf("[오류] 입력으로부터 트리를 생성하지 못했습니다.\n");
        return 1;
    }

    printf("\n--- [1] 계층적 이진트리 출력 ---\n");
    int has_more_siblings[MAX_LEN] = { 0 };
    printTreeLinked(root, 0, has_more_siblings);

    printTreeInfo(root);

    checkTreeTypes(root);

    freeTree(root);

    return 0;
}