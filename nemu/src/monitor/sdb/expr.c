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

#include <isa.h>
#include <stdio.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, 
  LEFT, RIGHT,      //()
  NUM, HEX_NUM,      //
  DEREF, NEGATIVE,    //*-
  TK_TIMES, TK_DIVISION,TK_PLUS,TK_MINUS,     //+-*/
  TK_EQ, TK_NEQ, 
  TK_REG, TK_REG_NAME,               //reg
  /* TODO: Add more token types */
};


static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"[0]x[0-9]+", HEX_NUM},
  {"\\$[0]",TK_REG_NAME},
  {"(t|a|s)[0-9]+",TK_REG_NAME},
  {"(r|g|s|t)(a|p)",TK_REG_NAME},
  {" +", TK_NOTYPE},    // spaces
  {"\n", TK_NOTYPE},    // \n
  {"\\+", TK_PLUS},         // plus
  {"\\*", TK_TIMES},         //乘
  {"-", TK_MINUS},            //减
  {"/", TK_DIVISION},            //除
  {"\\(", LEFT},         //(
  {"\\)", RIGHT},         //)
  {"[0-9]+", NUM},          //识别十进制整数
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},
  //{"\\$", TK_REG},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};
typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[256] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;
//判断运算符优先级
static int getOperatorPriority(int op){
  switch(op){
    case DEREF:
    case NEGATIVE:
      return 2;
    case TK_PLUS:
    case TK_MINUS:
      return 3;
    case TK_TIMES:
    case TK_DIVISION:
      return 4;
    default:
     return 10;
  }
}
static int get_prime(int p, int q){
    char chec[32] = {};
    int flag[32] = {};
    int read = 0;
    uint32_t index = p;
    for(index = p; index <= q; index++){
      if(read < 0) panic("read is wrong\n");
      if(tokens[index].str[0] == '(')
        {chec[read] = '(';flag[read] = index;read++;}
      else if(tokens[index].str[0] == ')')
      {
        while(chec[read-1] != '('){
          read--;
          chec[read] = 0;
          flag[read] = 0;
        }
        read--;
        chec[read] = 0;
        flag[read] = 0;
        
      }
      else if(tokens[index].type == TK_MINUS 
              || tokens[index].type == TK_PLUS 
              || tokens[index].type == TK_TIMES 
              || tokens[index].type == TK_DIVISION)
        {chec[read] = tokens[index].str[0];flag[read] = index;read++;}
  }
  int tmp = 0;
  for(int i = 1; i <= read; i++){
    if(getOperatorPriority(tokens[flag[i]].type) <= getOperatorPriority(tokens[flag[tmp]].type))
      tmp = i;
  }
  return flag[tmp];
}
/*
*   func：判断表达式是否被一对匹配的括号包围着
*   描述：
*     先检查第一个和最后一个符号
*     然后再判断是否是被一对匹配的括号包围着
*/
static bool check_parentheses(uint32_t start, uint32_t end){
  
  int num_Lparentheses = 1;   //The amount of "(" by now
  if((strcmp(tokens[start].str, "(") != 0) || (strcmp(tokens[end].str, ")") != 0))
    return false;
   for(int i = start + 1; i <= end; i++)
  {
    if((strcmp(tokens[i].str, "(") == 0))   //encounter a "("
      num_Lparentheses++;
    else if((strcmp(tokens[i].str, ")") == 0)) //when encountering a ")", eliminate a "("
    {
      if(num_Lparentheses > 1)    //the first "(" is not eliminated yet
        num_Lparentheses--;
      else if((num_Lparentheses == 1) && (i == end))  //the first "(" and the last ")"
        return true;
      else
        return false;
    }
  }
  return false;
}

static uint32_t eval(uint32_t p, uint32_t q){
  char op;
  uint32_t val1, val2;
   if (p > q) {
    /* Bad expression */
    panic("bad expr\n");
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if(tokens[p].type == HEX_NUM){
      return strtol(tokens[p].str, NULL, 16);
    }
    else if (tokens[p].type == NUM)
    {
      return atoi(tokens[p].str);
    }
    else if(tokens[p].type == TK_REG_NAME){
      bool *success = false;
      uint32_t num;
      num = isa_reg_str2val(tokens[q].str, success);
      if(!success) panic("reg\n");
      else return num; 
    }
  }
  else if(p == (q-1)){
    if(tokens[p].type == TK_REG){
      bool *success;
      uint32_t num;
      num = isa_reg_str2val(tokens[q].str, success);
      if(!(*success)) panic("reg\n");
      else return num; 
    }
    else if(tokens[p].type == NEGATIVE){
      return -atoi(tokens[q].str);
    }
    else if (tokens[p].type == DEREF)
    {
      if(tokens[p+1].type == HEX_NUM){
      uint32_t *ptr = (uint32_t *)strtol(tokens[p+1].str, NULL, 16);
      uint32_t now = paddr_read(ptr, 4);
      return now;
      }
      else if (tokens[p].type == NUM){
        uint32_t *ptr = (uint32_t *)atoi(tokens[p+1].str);
        uint32_t now = *ptr;
        return now;
      }
    }
    
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    /* We should do more things here. */
    op =  get_prime(p,q);         //x           //the position of 主运算符 in the token expression;
    val1 = eval(p, op-1);
    val2 = eval(op+1, q);

    switch (tokens[op].str[0]) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
			default: assert(0);
    }
  }
}

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}



static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;
  int pos=0;

  nr_token = 0;
  for(int j = 0; j < 256; j++)
    memset(tokens[j].str,'\0' , 32);

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
           i, rules[i].regex, position, substr_len, substr_len, substr_start);
        
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        
        switch (rules[i].token_type) {
          case(TK_NOTYPE): break;
          case(TK_DIVISION): 
          case(TK_TIMES): 
          case(TK_PLUS): 
          case(TK_MINUS): 
          case(DEREF): 
          case(NEGATIVE):
          case(LEFT): 
          case(RIGHT): 
          case(HEX_NUM): 
          case(TK_REG): 
          case(TK_REG_NAME):
          case(TK_EQ): 
          case(TK_NEQ): 
            tokens[pos].type = rules[i].token_type;
            memcpy(tokens[pos].str, substr_start, substr_len);
            nr_token++;
            pos++;
            //tokens[pos].str[substr_len] = '\0';
            //printf("index:%d\tsubstr%s\ttok%s\n",pos,substr_start,tokens[pos].str);
            break;
          case(NUM):  
            tokens[pos].type = rules[i].token_type;
            memcpy(tokens[pos].str, substr_start, substr_len);
            nr_token++;
            pos++;
            //tokens[pos].str[substr_len] = '\0';
            //printf("index:%d\tsubstr%s\ttok%s\n",pos,substr_start,tokens[pos].str);
            break;
          default: printf("WRONG TYPE%s\n",substr_start);
        }
        break;
      }
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
	/*指针解引用的识别*/ 
	int i = 0;
  for(i = 0; i < nr_token; i++){
	if(tokens[i].str[0] == '*' && (i == 0 || tokens[i - 1].type != NUM)){
		tokens[i].type = DEREF;
  } 
  }
  for(i = 0; i < nr_token; i++){
	if(tokens[i].str[0] == '-' && (i == 0 || tokens[i - 1].type == NUM)){
		tokens[i].type = NEGATIVE;
  } 
  }


  /*check whether expr is right*/
  //check_parentheses(0, nr_token);
  // if(atoi(tokens[0].str) != eval(1,nr_token-1)){
  // printf("expr:\n");
  // printf("info:total:%d\n",nr_token-1);
  // for(i = 1; i < nr_token; i++)
  //   printf("%s",tokens[i].str);
  // printf("\n");
  // printf("result:%s\tresult:%u\n",tokens[0].str,eval(1,nr_token-1));
  // }
  //printf("result:%d\n",check_parentheses(1, nr_token));
  
  //printf("expr's value is %d",eval(0, nr_token));
  //while(tokens[i].str != NULL){
    //printf("str:%s\ttype:%d",tokens[i].str,tokens[i].type);
    //i++;
  //}
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();

  return eval(0,nr_token-1);
}
