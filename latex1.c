#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum _CMD {NONE, PAPER, REPORT, TPAGE, ABSTRACT, INTRO, DATA, BODY, CONCLUSION, REFERENCES, REPLACE} CMD;

typedef struct _POS {
  CMD command;
  int start;
  int end;
  int line;
  int mark;
} POS;

int ccompile(POS *pos, int count);
int tfind(int cloc, POS *pos, int max);
int sfind(char *str, char *tbf);
int search(char *floc);
char *del(char *src, int n);
void compile(char *floc, int open);
char *insert(char *str, char *ins);
void append(char *str, char *add, int start);
CMD strtoenum(char *str);
char *namedet(char *arg);
int find(char *command, POS *pos, int count, CMD prev, CMD next);
char *parse(char *instr);
int scan(FILE *fpw, FILE *fpc, POS *pos);
int replace(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, char *inc, int count, int max, int loc, CMD prev, CMD next);
int tempr(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, int lim, char call, int count, int max, int *line);

#define RED     "\x1b[1;3;31m"
#define GREEN   "\x1b[1;3;32m"
#define YELLOW  "\x1b[1;3;33m"
#define BLUE    "\x1b[1;3;34m"
#define MAGENTA "\x1b[1;3;35m"
#define CYAN    "\x1b[1;3;36m"
#define RESET   "\x1b[0m"

int main(int argc, char **argv) {
  FILE *fpc;
  FILE *fpt;
  FILE *fpw;
  POS *pos;
  POS *tpos;
  char *floc;
  int ret = 1;
  int lim = 10000000;
  char *fname;
  char call = 'r';
  int open;
  int i;
  int j;
  int *line;
  line = malloc(sizeof(int));
  *line = 0;
  if(argc == 1) {
    printf(RED "ERROR! Missing file to be compiled." RESET "\n");
    return 0;
  }
  pos = malloc(sizeof(POS)*100);
  tpos = malloc(sizeof(POS)*100);
  fname = malloc(sizeof(char)*100);
  fpc = fopen(argv[1], "r");
  floc = namedet(argv[1]);
  fpw = fopen(floc, "w");
  i = scan(fpw, fpc, pos);
  ret = ccompile(pos, i);
  if(ret == 1) {
    return 0;
  }
  if(pos[0].command == PAPER) {
    strcpy(fname, "/home/achyut/latex/template2.tex");
  }
  else if(pos[0].command == REPORT) {
    strcpy(fname, "/home/achyut/latex/template.tex");
  }
  fpt = fopen(fname, "r");
  j = scan(fpw, fpt, tpos);
  fseek(fpt, 0, SEEK_SET);
  ret = tempr(fpt, fpw, fpc, pos, tpos, lim, call, i, j, line);
  fflush(fpw);
  compile(floc, 1);
  return ret;
}

int tempr(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, int lim, char call, int count, int max, int *line) {
  char *instr;
  char *inc;
  int ret = 1;
  int endloc;
  int loc;
  int cloc;
  int tloc;
  int mline;
  mline = *line;
  CMD prev;
  CMD next;
  instr = malloc(sizeof(char)*1000);
  inc = malloc(sizeof(char)*1000);
  fgets(instr, 1000, fpt);
  *line = *line + 1;
  while(!feof(fpt) && ret == 1 && ftell(fpt) <= lim) {
    inc = parse(instr);
    if(call == 'r') {
      if(inc[0] != '#') {
        fprintf(fpw, "%s\n", inc);
        loc = ftell(fpw) - strlen(inc) - 1;
      }
      else if(inc[1] >= 48 && instr[1] <= 57) {
        fprintf(fpw, "%s\n", inc);
        loc = ftell(fpw) - strlen(inc) - 1;
      }
      else {
        tloc = ftell(fpt);
        fgets(instr, 1000, fpt);
        inc = parse(instr);
        cloc = tfind(tloc, tpos, max);
        if(cloc < max) {
          if(tpos+cloc-1 != NULL) {
            prev = tpos[cloc-1].command;
          }
          else {
            prev = NONE;
          }
          if(cloc+1 <= max) {
            next = tpos[cloc+1].command;
          }
          else {
            next = NONE;
          }
        }
        while(strncmp(inc, "#\\", 2) != 0) {
          fgets(instr, 1000, fpt);
          inc = parse(instr);
        }
        ret = replace(fpt, fpw, fpc, pos, tpos, inc, count, max, loc, prev, next);
      }
    }
    else if(call == 's') {
      if(inc[0] == '#') {
        endloc = ftell(fpt) - 1;
        if(inc[1] == '\\') {
          return endloc;
        }
        else {
          fseek(fpt, endloc - strlen(instr), SEEK_SET);
          *line = mline;
          return 0;
        }
      }
    }
    fgets(instr, 1000, fpt); 
    *line = *line + 1;
  }
  if(call == 's') {
    *line = mline;
    return 0;
  }
  return ret;
}

int replace(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, char *inc, int count, int max, int loc, CMD prev, CMD next) {
  int ccom;
  char *command;
  int i = 2;
  int ret;
  char bs = 8;
  int *line;
  line = malloc(sizeof(int));
  command = malloc(sizeof(char)*997);
  while(inc[i] != '\0') {
    command[i-2] = inc[i];
    i++;
  }
  ccom = find(command, pos, count, prev, next);
  if(ccom < count) {
    fseek(fpc, pos[ccom].start, SEEK_SET);
    ret = tempr(fpc, fpw, fpc, pos, tpos, pos[ccom].end, 'r', count, max, line);
  }
  else {
    fseek(fpw, loc, SEEK_SET);
    fprintf(fpw, "%%\n");
    ret = tempr(fpt, fpw, fpc, pos, tpos, 100000000, 'r', count, max, line);
  }
  return ret;
}

int scan(FILE *fpw, FILE *fpc, POS *pos) {
  char *instr;
  char *inc;
  int i = 0;
  int ret;
  int *count;
  count = malloc(sizeof(int));
  *count = 0;
  instr = malloc(sizeof(char)*1000);
  inc = malloc(sizeof(char)*1000);
  fgets(instr, 1000, fpc);
  inc = parse(instr);
  *count = *count + 1;
  while(!feof(fpc)) {
    if(inc[0] != '#') {
      fgets(instr, 1000, fpc);
      inc = parse(instr);
    }
    else if(inc[1] != '/' && inc[1] != '\\') {
      if(strcmp(inc, "#paper") == 0) {
        pos[i].command = PAPER;
        pos[i].start = ftell(fpc);
        pos[i].end = ftell(fpc);
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#report") == 0) {
        pos[i].command = REPORT;
        pos[i].start = ftell(fpc);
        pos[i].end = ftell(fpc);
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      fgets(instr, 1000, fpc);
      inc = parse(instr);
    }
    else {
      if(strcmp(inc, "#/tpage") == 0) {
        pos[i].command = TPAGE;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/abstract") == 0) {
        pos[i].command = ABSTRACT;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/intro") == 0) {
        pos[i].command = INTRO;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/data") == 0) {
        pos[i].command = DATA;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/body") == 0) {
        pos[i].command = BODY;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/conclusion") == 0) {
        pos[i].command = CONCLUSION;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/references") == 0) {
        pos[i].command = REFERENCES;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else if(strcmp(inc, "#/replace") == 0) {
        pos[i].command = REPLACE;
        pos[i].start = ftell(fpc);
        ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', i, i, count);
        pos[i].end = ret;
        pos[i].line = *count;
        pos[i].mark = 0;
        i++;
      }
      else {
        pos[i].command = NONE;
        pos[i].start = 0;
        pos[i].end = 0;
        pos[i].line = *count;
        pos[i].mark = 1;
        i++;
      }
      fgets(instr, 1000, fpc);
      inc = parse(instr);
    }
    *count = *count + 1;
  }
  return i;
}

char *parse(char *instr) {
  char *outstr;
  int i = 0;
  int j = 0;
  outstr = malloc(sizeof(char)*1000);
  while(instr[i] == ' ') {
    i++;
  }
  if(instr[i] == '%' && instr[i+1] == '#') {
    i++;
  }
  while(instr[i] != '\0' && instr[i] != '\n') {
    outstr[j] = instr[i];
    i++;
    j++;
  }
  outstr[j] = '\0';
  return outstr;
}

int find(char *command, POS *pos, int count, CMD prev, CMD next) {
  int i = 0;
  if(pos[i].command == strtoenum(command) && pos[i+1].command == next) {
    return i;
  }
  i++;
  if(strtoenum(command) != REPLACE) {
    while((pos[i].command != strtoenum(command)) && i < count || pos[i].mark == 1) {
      i++;
    }
  }
  else {
    while((pos[i].command != strtoenum(command)) && i < count || pos[i].mark == 1 || pos[i-1].command != prev || pos[i+1].command != next) {
      if(pos[i].command == strtoenum(command) && (pos[i-1].command == PAPER || pos[i-1].command == REPORT)) {
        return i;
      }
      if(i >= count) {
        return i;
      }
      i++;
    }
  }
  if(i < count) {
    pos[i].mark = 1;
  }
  return i;
}

int tfind(int cloc, POS *pos, int max) {
  int i = 0;
  while(pos[i].start != cloc && i < max) {
    i++;
  }
  return i;
}

char *namedet(char *arg) {
  char *name;
  int i;
  int j = 0;
  i = strlen(arg) - 1;
  name = malloc(sizeof(char)*100);
  while(arg[i] != '/') {
    j++;
    i--;
  }
  j--;
  i = strlen(arg) - 1;
  while(arg[i] != '/') {
    name[j] = arg[i];
    j--;
    i--;
  }
  name = insert(name, "/home/achyut/latex/src/");
  return name;
}

CMD strtoenum(char *str) {
  if(strcmp(str, "paper") == 0) return PAPER;
  else if(strcmp(str, "report") == 0) return REPORT;
  else if(strcmp(str, "tpage") == 0) return TPAGE;
  else if(strcmp(str, "abstract") == 0) return ABSTRACT;
  else if(strcmp(str, "intro") == 0) return INTRO;
  else if(strcmp(str, "data") == 0) return DATA;
  else if(strcmp(str, "body") == 0) return BODY;
  else if(strcmp(str, "conclusion") == 0) return CONCLUSION;
  else if(strcmp(str, "references") == 0) return REFERENCES;
  else if(strcmp(str, "replace") == 0) return REPLACE;
}

void append(char *str, char *add, int start) {
  int i = 0;
  while(add[i] != '\0' && add[i] != '\n') {
    str[start+i] = add[i];
    i++;
  }
  str[start+i] = '\0';
}

char *insert(char *str, char *ins) {
  int i = 0;
  int j = 0;
  char *out;
  out = malloc(sizeof(char)*100);
  while(ins[j] != '\0') {
    out[j] = ins[j];
    j++;
  }
  while(str[i] != '\0') {
    out[j] = str[i];
    i++;
    j++;
  }
  out[j] = '\0';
  return out;
}

void compile(char *floc, int open) {
  char *command;
  char *new;
  char *mvto;
  char *newn;
  int i;
  int j = 0;
  int ret;
  new = malloc(sizeof(char)*100);
  newn = malloc(sizeof(char)*100);
  mvto = malloc(sizeof(char)*100);
  command = insert(floc, "-output-directory /home/achyut/latex/src/ ");
  command = insert(command, "printf 'x' | pdflatex ");
  ret = system(command);
  //printf(RED "RETURN VALUE: %d" RESET "\n", ret);
  if(ret == 0) {
    system(command);
    printf(GREEN "LaTeX compilation successful!" RESET "\n");
    new = del(floc, 4);
    append(new, "pdf", strlen(new));
    command = insert(new, "mv ");
    strcpy(mvto, " /home/achyut/latex/rel/");
    i = strlen(new) - 1;
    while(new[i] != '/') {
      j++;
      i--;
    }
    j--;
    i = strlen(new) - 1;
    while(new[i] != '/') {
      newn[j] = new[i];
      j--;
      i--;
    }
    append(mvto, newn, strlen(mvto));
    append(command, mvto, strlen(command));
    system(command);
    command = insert(mvto, "xdg-open");
    open = search(mvto);
    //printf(RED "RETURN VALUE %d" RESET "\n", open);
    if(open == 0) {
      system(command);
    }
  }
  else {
    printf(RED "LaTeX compilation error! Please check your LaTeX code." RESET "\n");
  }
}

char *del(char *src, int n) {
  int len;
  int i = 0;
  char *out;
  out = malloc(sizeof(char)*100);
  len = strlen(src) - n;
  while(i <= len) {
    out[i] = src[i];
    i++;
  }
  out[i] = '\0';
  return out;
}

int search(char *floc) {
  FILE *fp;
  char *command;
  int fcount = 1;
  char *dummy;
  command = malloc(sizeof(char)*100);
  dummy = malloc(sizeof(char)*200);
  command = insert(floc, "ps -aef | grep '");
  append(command, "'", strlen(command));
  fp = popen(command, "r");
  fgets(dummy, 200, fp);
  while(!feof(fp)) {
    if(sfind(dummy, "grep") == 0) {
      fcount++;
    }
    fgets(dummy, 200, fp);
  }
  if(fcount >= 2) {
    return 1;
  }
  else {
    return 0;
  }
}

int sfind(char *str, char *tbf) {
  int i = 0;
  int len;
  len = strlen(tbf);
  for(i=0; i<(strlen(str) - len); i++) {
    if(strncmp(str+i, tbf, len) == 0) {
      return 1;
    }
  }
  return 0;
}

int ccompile(POS *pos, int count) {
  int i;
  int error = 0;
  char con;
  for(i=0; i<count; i++) {
    if(pos[i].start == 0) {
      printf("(Line no. %d)  " RED "ERROR! Illegal \\# token" RESET "\n", pos[i].line);
      error = 1;
    }
    else if(pos[i].end == 0) {
      printf("(Line no. %d)  " RED "ERROR! Illegal /# token" RESET "\n", pos[i].line);
      error = 1;
    }
  }
  if(error == 0) {
    printf(GREEN "AcTeX compilation successful!" RESET "\nCompiling LaTeX...\n");
    return 0;
  }
  else {
    printf(RED "AcTeX compilation unsuccessful. LaTeX compilation halted." RESET "\n");
    printf("Do you still want to continue (y/n)? (There is a potential risk of LaTeX giving an error).");
    scanf("%c", &con);
    while(con == ' ') {
      scanf("%c", &con);
    }
    if(con == 'n') {
      return 1;
    }
    else if(con == 'y') {
      return 0;
    }
  }
}
