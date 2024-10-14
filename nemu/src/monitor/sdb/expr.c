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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, NUM, COMPUTE

  /* TODO: Add more token types */

};


static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\n", TK_NOTYPE},    // spaces
  {"\\+", COMPUTE},         // plus
  {"==", TK_EQ},        // equal
  {"\\*", COMPUTE},         //乘
  {"-", COMPUTE},            //减
  {"/", COMPUTE},            //除
  {"\\(", COMPUTE},         //(
  {"\\)", COMPUTE},         //)
  {"[0-9]+", NUM},          //识别十进制整数
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};
typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[256] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static getOperatorPriority(char op){
  switch(op){
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 2;
    default:
     return 10;
  }
}

static bool check_parentheses(uint32_t p, uint32_t q){
  char chec[100] = {};
  int read = 0;
  uint32_t index = p;
  for(index = p; index <= q; index++){
    if(read < 0) break;
    if(tokens[index].str[0] == '(')
      {chec[read] = '(';read++;}
    else if(tokens[index].str[0] == ')')
      {read--;}
  }
    //printf("1\n");
  if(read != 0) 
    panic("() is nor matched\n");
  else{
    if(tokens[p].str[0] == '(')  return true;
    else return false;
  }
}
//没有换成数字
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
    return atoi(tokens[p].str);
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    char chec[32] = {};
    int flag[32] = {};
    int read = 0;
    uint32_t index = p;
    for(index = p; index <= q; index++){
      if(read < 0) panic("read is wrong\n");
      if(tokens[index].str[0] == '(')
        {chec[read] = '(';flag[read] = index;read++;}
      else if(tokens[index].str[0] == ')')
        {read -= 2;}
      else if(tokens[index].str[0] == '+' 
              || tokens[index].str[0] == '-' 
              || tokens[index].str[0] == '*' 
              || tokens[index].str[0] == '/')
        {chec[read] = tokens[index].str[0];flag[read] = index;read++;}
  }
  int tmp = 0;
  for(int i = 1; i <= read; i++){
    if(getOperatorPriority(chec[i]) <= getOperatorPriority(chec[tmp]))
      tmp = i;
  }
    /* We should do more things here. */
    op =  tokens[flag[tmp]].str[0];         //x           //the position of 主运算符 in the token expression;
    val1 = eval(p, tmp-1);
    val2 = eval(tmp+1, q);

    switch (op) {
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
          case(COMPUTE): 
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
  int i = 0;
  
  /*check whether expr is right*/
  //check_parentheses(0, nr_token);
  printf("expr:\n");
  printf("info:total:%d\n",nr_token-1);
  for(i = 1; i < nr_token; i++)
    printf("type:%d\tvalue:%s\n",tokens[i].type,tokens[i].str);
  printf("result:%d\n",eval(1,nr_token-1));
  //printf("result:%d\n",check_parentheses(1, nr_token));
  
  //printf("expr's value is %d",eval(0, nr_token));
  //while(tokens[i].str != NULL){
    //printf("str:%s\ttype:%d",tokens[i].str,tokens[i].type);
    //i++;
  //}
  /* TODO: Insert codes to evaluate the expression. */
  //TODO();

  return 0;
}
