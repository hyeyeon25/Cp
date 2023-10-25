#include<stdio.h>
#include<stdlib.h>

typedef struct _NODE { // 연결 리스트의 노드 나타냄 
   int data;
   struct _NODE* next;
} NODE;

void print_list(NODE* head) // 실제 데이터 포함 x, 연결리스트의 시작 알림 
{
   NODE* p = head->next;
   while (p != NULL) {
      printf("%d", p->data);
      p = p->next;
   }
   printf("\n");
}
void insert_node(NODE* head, int data) // 연결리스트 끝에 새로운 노드 추가 
{
   NODE* p = head;
   while (p->next != NULL) {
      p = p->next;
   }
   NODE* new_node = (NODE*)malloc(sizeof(NODE));
   new_node->data = data;
   new_node->next = p->next;
   p->next = new_node;
}
void insert_node_first(NODE* head, int data) // 연결리스트의 첫 번째에 새로운 노드 추가 
{
   NODE* new_node = (NODE*)malloc(sizeof(NODE));
   new_node->data = data;
   new_node->next = head->next;
   head->next = new_node;
}
void delete_node_first(NODE* head)
{
   NODE*p = head->next->next;
   free(head->next);
   head->next = p;
}
void delete_node_last(NODE* head)
{
   NODE* p = head;
   NODE* p_prev = head;
   while (p->next != NULL) {
      p_prev = p;
      p = p -> next;
   }
   free(p);
   p_prev->next = NULL;
}
int main()
{
   NODE* head = (NODE*)malloc(sizeof(NODE));
   head->next = NULL;
   
   NODE* n1 = (NODE*)malloc(sizeof(NODE));
   n1->data = 1;
   n1->next = head->next;
   head->next = n1;

   NODE* n2 = (NODE*)malloc(sizeof(NODE));
   n2->data = 2;
   n2->next = n1->next;
   n1->next = n2;

   NODE* n3 = (NODE*)malloc(sizeof(NODE));
   n3->data = 3;
   n3->next = n2->next;
   n2->next = n3;

   print_list(head);

   insert_node(head, 4); // 리스트 끝에 추가 
   insert_node(head, 5);

   print_list(head);

   insert_node_first(head,6);
   print_list(head);

   delete_node_first(head); // 첫 번째 노드 제거 
   delete_node_first(head); // 두 번째 노드 제거 
   print_list(head);

   delete_node_last(head); // 마지막 노드 제거 
   print_list(head);
   return 0;
}
