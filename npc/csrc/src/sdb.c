#include <common.h>
#include "reg.h"
#include "cpu_exec.h"
#include "difftest.h"
//#include"Vtop.h"  // 从top.v生成

// calculate the length of an array
#define ARRLEN(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define NR_CMD ARRLEN(cmd_table)
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};


int cmd_info(char *args){
    isa_regs_display();
    return 0;
}

static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(npc) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}


int cmd_si(char *args){
    cpu_exec(atoi(args));
    return 0;
}
static struct {
    const char *name;
    const char *description;
    int (*handler) (char *);
} cmd_table[] = {
    {"si", "continue the execution N instruction of the program", cmd_si},
    {"info", "print value of reg", cmd_info},
};

void sdb_mainloop(){
    for (char *str; ((str = rl_gets()) != NULL); ) {
        char *str_end = str + strlen(str);
        
        /* extract the first token as the command */
        char *cmd = strtok(str, " ");
        if (cmd == NULL) { continue; }

        /* treat the remaining string as the arguments,
        * which may need further parsing
        */
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL;
        }
        int i;
        for (i = 0; i < NR_CMD; i ++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) { return ; }
                    break;
            }
        }
        
        if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
        if(flag == 1) break;
    }
}