#include "smm_common.h"
#include "smm_database.h"
#include <string.h>

#define LIST_END            -1
#define MAX_LIST            LISTNO_OFFSET_GRADE + MAX_PLAYER

// 연결 리스트를 위한 노드 정의
typedef struct node {
    int index;
    void* obj;
    struct node* next;
    struct node* prev;
} node_t;

static node_t* list_database[MAX_LIST];
static int list_cnt[MAX_LIST];

// 데이터 추가 함수
int smmdb_addTail(int list_nr, void* obj) {
    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    newNode->obj = obj;
    newNode->next = NULL;
    
    if (list_database[list_nr] == NULL) {
        newNode->index = 0;
        newNode->prev = NULL;
        list_database[list_nr] = newNode;
    } else {
        node_t* temp = list_database[list_nr];
        while (temp->next != NULL) temp = temp->next;
        newNode->index = temp->index + 1;
        newNode->prev = temp;
        temp->next = newNode;
    }
    list_cnt[list_nr]++;
    return newNode->index;
}

// 데이터 개수 반환
int smmdb_len(int list_nr) {
    return list_cnt[list_nr];
}

// 데이터 가져오기
void* smmdb_getData(int list_nr, int index) {
    node_t* temp = list_database[list_nr];
    while (temp != NULL) {
        if (temp->index == index) return temp->obj;
        temp = temp->next;
    }
    return NULL;
}

// 데이터 삭제
int smmdb_deleteData(int list_nr, int index) {
    // 기본 구현 생략 
    return 0;
}
