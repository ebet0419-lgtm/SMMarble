#include "smm_common.h"
#include "smm_object.h"
#include <string.h>
#include <stdlib.h>

// 데이터 구조체 정의 (main.c 에서는 보이지 않음)
typedef struct {
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
} smmObj_t;

// 성적 이름을 문자열로 바꿔주기 위한 배열
static char* smmGradeName[] = {"A+", "A0", "A-", "B+", "B0", "B-", "C+", "C0", "C-"};

// 노드 객체 생성 함수
void* smmObj_genNode(char* name, int type, int credit, int energy) {
    smmObj_t* obj = (smmObj_t*)malloc(sizeof(smmObj_t));
    if (obj != NULL) {
        strcpy(obj->name, name);
        obj->type = type;
        obj->credit = credit;
        obj->energy = energy;
    }
    return (void*)obj;
}

// 음식 카드 생성 함수 (노드 구조체 재사용)
void* smmObj_genFoodCard(char* name, int energy) {
    return smmObj_genNode(name, NODE_FOODCHANCE, 0, energy);
}

// 축제 카드 생성 함수
void* smmObj_genFestivalCard(char* message) {
    return smmObj_genNode(message, NODE_FESTIVAL, 0, 0);
}

// 정보 추출 함수 (Getter)
char* smmObj_getNodeName(void* obj) { return ((smmObj_t*)obj)->name; }
int smmObj_getNodeType(void* obj) { return ((smmObj_t*)obj)->type; }
int smmObj_getNodeCredit(void* obj) { return ((smmObj_t*)obj)->credit; }
int smmObj_getNodeEnergy(void* obj) { return ((smmObj_t*)obj)->energy; }
char* smmObj_getGradeName(smmGrade_e grade) { return smmGradeName[grade]; }
