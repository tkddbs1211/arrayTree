#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_NODES 1024

char tree_array[MAX_NODES];
int max_index = 0; 

void initTreeArray() {
    for (int i = 0; i < MAX_NODES; i++) {
        tree_array[i] = '\0';
    }
    max_index = 0;
}

typedef struct {
    int data[MAX_NODES];
    int top;
} IntStack;

void initStack(IntStack* s) { s->top = -1; }
int isEmpty(IntStack* s) { return s->top == -1; }
void push(IntStack* s, int val) { s->data[++(s->top)] = val; }
int pop(IntStack* s) { return isEmpty(s) ? -1 : s->data[(s->top)--]; }
int peek(IntStack* s) { return isEmpty(s) ? -1 : s->data[s->top]; }

int parseToTreeArray(const char* str) {
    initTreeArray();
    IntStack parent_stack;
    initStack(&parent_stack);

    int len = strlen(str);
    int curr_parent = -1;
    int child_state = 0; 

    for (int i = 0; i < len; i++) {
        char ch = str[i];

        if (ch == ' ' || ch == '\t' || ch == '\n') continue;

        if (isupper(ch)) {
            int target_idx = 1;

            if (curr_parent != -1) {
                if (child_state == 1) {
                    target_idx = 2 * curr_parent; 
                }
                else if (child_state == 2) {
                    target_idx = 2 * curr_parent + 1; 
                }
            }

            if (target_idx >= MAX_NODES) {
                printf("[오류] 트리의 높이가 너무 높아 배열 범위를 초과했습니다.\n");
                return 0;
            }

            tree_array[target_idx] = ch;
            if (target_idx > max_index) max_index = target_idx;
        }
        else if (ch == '(') {

            int parent_idx = -1;

            if (child_state == 0) parent_idx = 1;
            else if (child_state == 1) parent_idx = 2 * curr_parent;
            else if (child_state == 2) parent_idx = 2 * curr_parent + 1;

            push(&parent_stack, parent_idx);
            curr_parent = parent_idx;
            child_state = 1; 
        }
        else if (ch == ',') {
            child_state = 2; 
        }
        else if (ch == ')') {
            pop(&parent_stack);
            if (!isEmpty(&parent_stack)) {
                curr_parent = peek(&parent_stack);
            }
            else {
                curr_parent = -1;
            }
            child_state = 0;
        }
    }

    return (max_index > 0);
}

void printTreeArray(int idx, int depth, int has_more_siblings[]) {
    if (idx >= MAX_NODES || tree_array[idx] == '\0') return;

    if (depth == 0) {
        printf("%c\n", tree_array[idx]);
    }
    else {
        for (int d = 1; d < depth; d++) {
            if (has_more_siblings[d]) printf("|   ");
            else printf("    ");
        }
        printf("+---%c\n", tree_array[idx]);
    }

    int left_idx = 2 * idx;
    int right_idx = 2 * idx + 1;

    int has_left = (left_idx < MAX_NODES && tree_array[left_idx] != '\0');
    int has_right = (right_idx < MAX_NODES && tree_array[right_idx] != '\0');

    if (has_left) {
        has_more_siblings[depth + 1] = has_right; 
        printTreeArray(left_idx, depth + 1, has_more_siblings);
    }

    if (has_right) {
        has_more_siblings[depth + 1] = 0; 
        printTreeArray(right_idx, depth + 1, has_more_siblings);
    }
}

void printTreeInfo() {
    int total_nodes = 0;
    int leaf_nodes = 0;
    int non_leaf_nodes = 0;
    int max_depth = 0;
    int max_degree = 0;

    for (int i = 1; i <= max_index; i++) {
        if (tree_array[i] != '\0') {
            total_nodes++;

            int left_idx = 2 * i;
            int right_idx = 2 * i + 1;

            int left_exists = (left_idx < MAX_NODES && tree_array[left_idx] != '\0');
            int right_exists = (right_idx < MAX_NODES && tree_array[right_idx] != '\0');

            int degree = left_exists + right_exists;
            if (degree > max_degree) max_degree = degree;

            if (degree == 0) {
                leaf_nodes++;
            }
            else {
                non_leaf_nodes++;
            }

            int current_depth = (int)(log2(i)) + 1;
            if (current_depth > max_depth) {
                max_depth = current_depth;
            }
        }
    }

    printf("\n--- [2] 트리 정보 ---\n");
    printf("1. 전체 노드의 수 : %d\n", total_nodes);
    printf("2. 단말 노드의 수 : %d\n", leaf_nodes);
    printf("3. 비단말 노드의 수: %d\n", non_leaf_nodes);
    printf("4. 트리의 높이     : %d\n", max_depth);
    printf("5. 트리의 차수     : %d\n", max_degree);
}

void checkTreeTypes() {
    int total_nodes = 0;
    int max_depth = 0;

    for (int i = 1; i <= max_index; i++) {
        if (tree_array[i] != '\0') {
            total_nodes++;
            int current_depth = (int)(log2(i)) + 1;
            if (current_depth > max_depth) max_depth = current_depth;
        }
    }

    int is_complete = 1;
    for (int i = 1; i <= total_nodes; i++) {
        if (tree_array[i] == '\0') {
            is_complete = 0;
            break;
        }
    }

    int is_full = 0;
    int expected_full_nodes = (int)pow(2, max_depth) - 1;
    if (total_nodes == expected_full_nodes) {
        is_full = 1;
    }

    int is_skewed = (total_nodes == max_depth);

    printf("\n--- [3] 이진트리 형태 판별 ---\n");
    printf("1. 완전 이진트리 여부: %s\n", is_complete ? "예 (Yes)" : "아니오 (No)");
    printf("2. 포화 이진트리 여부: %s\n", is_full ? "예 (Yes)" : "아니오 (No)");
    printf("3. 편향 이진트리 여부: %s\n", is_skewed ? "예 (Yes)" : "아니오 (No)");
}

int main() {
    char input[1024];

    printf("이진트리의 괄호 표기법을 입력하세요 (예: A(B(D,E),C(,F))):\n> ");
    if (fgets(input, sizeof(input), stdin) == NULL) return 1;

    if (!parseToTreeArray(input)) {
        printf("[오류] 트리 변환에 실패했습니다.\n");
        return 1;
    }

    printf("\n--- [1] 계층적 이진트리 출력 ---\n");
    int has_more_siblings[MAX_NODES] = { 0 };
    printTreeArray(1, 0, has_more_siblings);

    printTreeInfo();

    checkTreeTypes();

    return 0;
}