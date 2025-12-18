#ifndef smm_object_h
#define smm_object_h

#include "smm_common.h"

// 1. 노드(보드판 칸)의 유형 정의
typedef enum {
    NODE_LECTURE = 0,    // 강의
    NODE_RESTAURANT = 1, // 식당
    NODE_LAB = 2,        // 실험실
    NODE_HOME = 3,       // 집
    NODE_EXPERIMENT = 4, // 실험(이동)
    NODE_FOODCHANCE = 5, // 음식찬스
    NODE_FESTIVAL = 6    // 축제
} smmNode_e;

// 2. 성적 유형 정의
typedef enum {
    GRADE_AP, GRADE_A0, GRADE_AM,
    GRADE_BP, GRADE_B0, GRADE_BM,
    GRADE_CP, GRADE_C0, GRADE_CM
} smmGrade_e;

// 3. 기능을 수행할 함수 선언 (외부에서 쓸 수 있는 통로)
void* smmObj_genNode(char* name, int type, int credit, int energy);
void* smmObj_genFoodCard(char* name, int energy);
void* smmObj_genFestivalCard(char* message);

char* smmObj_getNodeName(void* obj);
int smmObj_getNodeType(void* obj);
int smmObj_getNodeCredit(void* obj);
int smmObj_getNodeEnergy(void* obj);
char* smmObj_getGradeName(smmGrade_e grade);

#endif /* smm_object_h */
