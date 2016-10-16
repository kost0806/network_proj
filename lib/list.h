typedef struct NODE {
	struct NODE *prev;
	struct NODE *next;
	void *value;
	int value_size;
} NODE;

typedef struct LIST {
	NODE *head;
	NODE *tail;
	int size;
} LIST;

/**
 * 리스트 생성
 * @return LIST* : list 구조체 주소
 */
LIST* list_create();

/**
 * 리스트 초기화
 * @param LIST *list : 초기화할 list 주소
 */
void list_init(LIST *list);

/**
 * 리스트 node 추가
 * @param LIST *list : 대상 리스트
 * @param void *value : node 안에 들어갈 값. 스택 변수 사용 금지.
 * @param int value_size : value의 크기
 * @return int : 삽입된 노드의 인덱스. 에러시 -1. 스택 변수 사용 금지
 */
int list_insert_by_value(LIST *list, void *value, int value_size);

/**
 * 리스트 node 추가
 * @param LIST *list : 대상 리스트
 * @param NODE *node : 추가할 node
 * @return int : 삽입된 노드의 인덱스. 에러시 -1. 스택 변수 사용 금지
 */
int list_insert(LIST *list, NODE *node);

/**
 * 리스트 node 삭제
 * @param LIST *list : 대상 리스트
 * @param NODE *node : 삭제할 node. NULL일때 index로 삭제
 * @param int index : 삭제할 node의 인덱스. node가 NULL일경우 반영
 * @return int : 삭제된 node의 인덱스. 에러시 -1
 */
int list_remove(LIST *list, NODE *node, int index);

/**
 * 리스트 node 하나를 가져온다.
 * @param LIST *list : 대상 리스트
 * @param int index : 가져올 node의 인덱스
 * @return NODE* : 가져올 node의 주소. 에러시 NULL	
 */
NODE* list_get(LIST *list, int index);

/**
 * 원하는 값을 가진 node의 인덱스를 가져온다.
 * @param LIST *list : 대상 리스트
 * @param void *value : 찾길 원하는 값
 * @param int size : value의 크기
 * @return int : node의 인덱스
 */
int list_search(LIST *list, void *value, int size);

/**
 * 새로운 node를 생성
 * @return NODE* : node의 주소
 */
NODE* node_create();

/**
 * node의 value에 heap 공간을 할당해준다.
 * @param NODE *node : 대상 노드
 * @param int value_size : 할당받을 크기
 * @return void* : 할당된 공간의 주소
 */
void* node_create_value_space(NODE *node, int value_size);