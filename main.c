
#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

// 파일 경로 정의
#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

// 플레이어 정보 구조체
typedef struct {
    char name[MAX_CHARNAME];
    int pos;            // 현재 위치
    int energy;         // 현재 에너지
    int credit;         // 획득 학점
    int is_experiment;  // 실험실에 갇혔는지 여부 (1:갇힘, 0:자유)
} player_t;

player_t players[MAX_PLAYER];
int player_nr = 0;

// 주사위 굴리기 함수
int rollDie() {
    return (rand() % MAX_DIE) + 1;
}

// 특정 유형의 노드 위치 찾기 (예: 실험실 위치 찾기)
int findNodeByType(int type) {
    int i;
    for (i = 0; i < player_nr; i++){
        void* node = smmdb_getData(LISTNO_NODE, i);
        if (smmObj_getNodeType(node) == type) return i;
    }
    return 0;
}

// 노드별 액션 처리
void actionNode(int p_idx) {
    player_t* p = &players[p_idx];
    void* node = smmdb_getData(LISTNO_NODE, p->pos);
    int type = smmObj_getNodeType(node);

    printf(" [%s] 칸에 도착했습니다!\n", smmObj_getNodeName(node));

    switch(type) {
        case NODE_LECTURE: // 0: 강의
            if (p->energy >= smmObj_getNodeEnergy(node)) {
                printf("  -> 수강하시겠습니까? (1:Yes, 0:No): ");
                int choice; scanf("%d", &choice);
                if (choice == 1) {
                    p->energy -= smmObj_getNodeEnergy(node);
                    p->credit += smmObj_getNodeCredit(node);
                    int gradeIdx = rand() % 9;
                    printf("  -> [성공] %d학점 획득! 성적: %s (남은 에너지: %d)\n", 
                            smmObj_getNodeCredit(node), smmObj_getGradeName(gradeIdx), p->energy);
                }
            } else {
                printf("  -> 에너지가 부족하여 수강할 수 없습니다.\n");
            }
            break;

        case NODE_RESTAURANT: // 1: 식당
            p->energy += smmObj_getNodeEnergy(node);
            printf("  -> 에너지를 %d 보충했습니다. (현재: %d)\n", smmObj_getNodeEnergy(node), p->energy);
            break;

        case NODE_EXPERIMENT: // 4: 실험 (실험실로 강제 이동)
            printf("  -> 실험실로 강제 소환됩니다!\n");
            p->pos = findNodeByType(NODE_LAB);
            p->is_experiment = 1;
            break;

        case NODE_FOODCHANCE: // 5: 음식 찬스
            {
                int cardIdx = rand() % smmdb_len(LISTNO_FOODCARD);
                void* card = smmdb_getData(LISTNO_FOODCARD, cardIdx);
                p->energy += smmObj_getNodeEnergy(card);
                printf("  -> [음식찬스] %s을(를) 먹고 에너지 %d 획득!\n", 
                        smmObj_getNodeName(card), smmObj_getNodeEnergy(card));
            }
            break;

        case NODE_FESTIVAL: // 6: 축제
            {
                int cardIdx = rand() % smmdb_len(LISTNO_FESTCARD);
                void* card = smmdb_getData(LISTNO_FESTCARD, cardIdx);
                printf("  -> [축제미션] %s\n", smmObj_getNodeName(card));
            }
            break;
    }
}

int main() {
    srand(time(NULL));
    FILE *fp;

    // 1. 보드 설정 파일 읽기
    fp = fopen(BOARDFILEPATH, "r");
    if (fp) {
        char name[MAX_CHARNAME]; int type, credit, energy;
        while (fscanf(fp, "%s %d %d %d", name, &type, &credit, &energy) != EOF) {
            smmdb_addTail(LISTNO_NODE, smmObj_genNode(name, type, credit, energy));
        }
        fclose(fp);
    }

    // 2. 음식 설정 파일 읽기
    fp = fopen(FOODFILEPATH, "r");
    if (fp) {
        char name[MAX_CHARNAME]; int energy;
        while (fscanf(fp, "%s %d", name, &energy) != EOF) {
            smmdb_addTail(LISTNO_FOODCARD, smmObj_genFoodCard(name, energy));
        }
        fclose(fp);
    }

    // 3. 축제 설정 파일 읽기
    fp = fopen(FESTFILEPATH, "r");
    if (fp) {
        char msg[MAX_CHARNAME];
        while (fgets(msg, sizeof(msg), fp)) {
            msg[strcspn(msg, "\n")] = 0; // 줄바꿈 제거
            smmdb_addTail(LISTNO_FESTCARD, smmObj_genFestivalCard(msg));
        }
        fclose(fp);
    }

    // 4. 플레이어 초기화
    printf("플레이어 인원수(2~10): ");
    scanf("%d", &player_nr);
    int i; 
    for (i = 0; i < player_nr; i++) {
        printf("%d번 플레이어 이름: ", i+1);
        scanf("%s", players[i].name);
        players[i].pos = 0;
        players[i].energy = 30; // 초기 에너지
        players[i].credit = 0;
        players[i].is_experiment = 0;
    }

    // 5. 게임 루프 시작
    int game_over = 0;
    while (!game_over) {
        int i; 
        for (i = 0; i < player_nr; i++) {
            printf("\n------------------------------------------\n");
            printf("[%s의 턴] 위치:%d 에너지:%d 학점:%d\n", players[i].name, players[i].pos, players[i].energy, players[i].credit);
            
            // 실험실 탈출 체크
            if (players[i].is_experiment) {
                int escape_roll = rollDie();
                printf("  실험실 탈출 시도... 주사위: %d ", escape_roll);
                if (escape_roll >= 4) { // 4 이상이면 탈출
                    printf("(탈출 성공!)\n");
                    players[i].is_experiment = 0;
                } else {
                    printf("(탈출 실패... 턴을 마칩니다.)\n");
                    continue;
                }
            }

            printf("  주사위를 던지려면 엔터를 누르세요...");
            getchar(); getchar(); // 입력 대기
            
            int die = rollDie();
            printf("  주사위 결과: %d\n", die);

            // 이동 처리
            int step; 
            for (step = 0; step < die; step++) {
                players[i].pos = (players[i].pos + 1) % smmdb_len(LISTNO_NODE);
                // 집(0번 위치)을 지나면 에너지 보충
                if (players[i].pos == 0) {
                    players[i].energy += 10;
                    printf("  -> [집]을 통과하며 에너지 10 보충!\n");
                }
            }

            actionNode(i);

            // 졸업 조건 확인
            if (players[i].credit >= GRADUATE_CREDIT && players[i].pos == 0) {
                printf("\n==========================================\n");
                printf("!!! 축하합니다! [%s] 플레이어가 졸업했습니다 !!!\n", players[i].name);
                printf("==========================================\n");
                game_over = 1;
                break;
            }
        }
    }

     system("PAUSE");	
     return 0;
}
