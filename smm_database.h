#ifndef smm_database_h
#define smm_database_h

// 리스트 번호 정의
#define LISTNO_NODE             0
#define LISTNO_FOODCARD         1
#define LISTNO_FESTCARD         2
#define LISTNO_OFFSET_GRADE     3

// 제공된 함수 선언
int smmdb_addTail(int list_nr, void* obj);          // 데이터를 맨 뒤에 추가
int smmdb_deleteData(int list_nr, int index);       // 데이터 삭제
int smmdb_len(int list_nr);                         // 저장된 데이터 개수 확인
void* smmdb_getData(int list_nr, int index);        // 특정 번호의 데이터 가져오기

#endif /* smm_database_h */
