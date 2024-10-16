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

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
	wp_pool[i].value = 0;
	wp_pool[i].old_value = 0;
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
/*	返回监视点池最后一个节点作为空闲的监视点结构
*	同时插入到head中
*/
WP* new_up(char *args){
	if(free_ == NULL) panic("watchpoint is full\n");
	WP *tmp = free_;
	WP *sec_tmp = free_;			//指向tmp的上一个节点
	while(tmp != NULL){
		sec_tmp = tmp;
		tmp = tmp->next;
	}
	sec_tmp->next = NULL;
	WP *join = head;
	while(join != NULL){
		join = join->next;
	}
	join = sec_tmp;
	strcpy(sec_tmp->str, args);
	return sec_tmp;
}

/*找到节点并删除节点，然后插入free_的尾部*/
void free_wp(WP *wp){
	WP *tmp = head;
	WP *del = head;
	while(tmp->next != NULL){
		del = tmp;
		tmp = tmp->next;
		if(tmp == wp){
			break;
		}
	}
	//delete
	del->next = tmp->next;
	//add free_
	WP *last = free_;
	while(last->next != NULL){
		last = last->next;
	}
	last->next = tmp;
	tmp->next = NULL;
}

bool watchpoint_check(){
	bool a = true;
	bool *success = &a;
	int flag = 0;
	uint32_t value_1 = 0;
	WP *tmp = head;
	while(tmp != NULL){
		tmp = tmp->next;
		value_1 = expr(tmp->str, success);
		if(tmp->value == value_1)
		{
			flag = 1;
			tmp->old_value = tmp->value;
			tmp->value = value_1;
			printf("watchpoint :%s\n",tmp->str);
			printf("old value:%d\tnew value:%d\n",tmp->old_value,tmp->value);
		}
	}
	if(flag == 1) return true;
	else return false;
}

void watchpoint_print(){
	WP *tmp = head;
	int i = 1;
	printf("NUM\texpr\t\n");
	while(tmp != NULL){
		tmp = tmp->next;
		printf("%d\t%s\n",i,tmp->str);
		i++;
	}
}