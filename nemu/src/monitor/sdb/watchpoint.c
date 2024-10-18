/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char str[1024];
  uint32_t value;
  uint32_t old_value;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

/*没有释放head、free*/
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
	wp_pool[i].value = NULL;
	wp_pool[i].old_value = 0;
  }

  head = (WP *)malloc(sizeof(WP));
  free_ = (WP *)malloc(sizeof(WP));
  free_->next = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
/*	返回监视点池最后;一个节点作为空闲的监视点结构
*	同时插入到head中
*/
WP* new_up(char *args){
	if(free_ == NULL) panic("watchpoint is full\n");
	WP *ptr = free_;
	WP *pre = free_;			//指向tmp的上一个节点
	while(ptr->next != NULL){
		pre =ptr;
		ptr = ptr->next;
	}
	pre->next = NULL;
	WP *ptr_h = head;
	while(ptr_h->next != NULL){
		ptr_h = ptr_h->next;
	}
	ptr_h->next = ptr;
	strcpy(ptr->str, args);
	return ptr;
}

/*找到节点并删除节点，然后插入free_的尾部*/
/*假设一直会找的wp，以后再处理找不到的情况*/
void free_wp(char *args){
	int num = atoi(args);
	int count = 0;
	WP *tmp = head->next;
	WP *del = head;
	while(tmp != NULL){
		count++;
		if(count == num){
			break;
		}
		del = tmp;
		tmp = tmp->next;
	}
	if(count == num){
		//delete
		del->next = tmp->next;
		//add free_
		WP *last = free_->next;
		while(last->next != NULL){
			last = last->next;
		}
		last->next = tmp;
		tmp->next = NULL;
	}
}

bool watchpoint_check(){
	bool a = true;
	bool *success = &a;
	int flag = 0;
	uint32_t value_1 = 0;
	WP *tmp = head->next;
	while(tmp != NULL){
		value_1 = expr(tmp->str, success);
		if(tmp->value != value_1)
		{
			flag = 1;
			tmp->old_value = tmp->value;
			tmp->value = value_1;
			printf("watchpoint :%s\n",tmp->str);
			printf("old value:%d\tnew value:%d\n",tmp->old_value,tmp->value);
		}
		tmp = tmp->next;
	}
	if(flag == 1) return true;
	else return false;
}

void watchpoint_print(){
	WP *tmp = head->next;
	int i = 1;
	printf("NUM\texpr\t\n");
	while(tmp != NULL){
		printf("%d\t%s\n",i,tmp->str);
		i++;
		tmp = tmp->next;
	}
}