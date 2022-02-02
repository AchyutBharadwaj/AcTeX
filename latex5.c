#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

typedef struct _FUNC {
  char *name;
  char *function;
  int position;
} FUNC;

typedef struct _POS {
  char *command;
  int start;
  int end;
  int line;
  int mark;
} POS;

typedef struct _MAX {
  int class;
  int def;
  int dir;
  int token;
  int pos;
  int tpos;
  int line;
  int exin;
  int ptr;
} MAX;

typedef struct _EXC {
  char *title;
  int start;
  int end;
  char yn;
} EXC;

FILE *readconfig(FUNC *class, FUNC *token, FUNC *def, FUNC *dir, FUNC *exin, FILE *fpn, MAX *max);
int include(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args);
int ehuduku(char *tbf, EXC *in, int max);
int exclude(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args);
int section(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args, FUNC *exin);
int dclass(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args, int i);
//int commands(char *command, FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args);
int filedet(char *file);
FILE *frein(char *tbr, FILE *fp, int till, char line);
void sprint(char *str, int time);
int huduku(char *tbf, FUNC *in, int max);
int scan(FILE *fpw, FILE *fpc, POS *pos, MAX *max, FUNC *class, FUNC *token, EXC *exc, char scex, FUNC *exin);
int ccompile(POS *pos, int count, FUNC *def, FILE *fpn, FUNC *token, MAX *max, FUNC *class, FUNC *dir, FUNC *exin);
int sfind(char *str, char *tbf);
int search(char *floc);
char *del(char *src, int n);
void compile(char *floc, int open, FUNC *def);
char *insert(char *str, char *ins);
void append(char *str, char *add, int start);
char *namedet(char *arg, FUNC *dir, MAX *max);
int tfind(int cloc, POS *pos, MAX *max);
int find(char *command, POS *pos, int count, char *prev, char *next, FUNC *token, MAX *max);
char *parse(char *instr);
char *parsecon(char *instr);
int replace(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, char *inc, MAX *max, int loc, char *prev, char *next, FUNC *token, EXC *exc);
int tempr(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, int lim, char call, MAX *max, FUNC *token, EXC *exc, char fn);
char **incommands(char *instr);

#define RED     "\x1b[1;3;31m"
#define GREEN   "\x1b[1;3;32m"
#define YELLOW  "\x1b[1;3;33m"
#define BLUE    "\x1b[1;3;34m"
#define MAGENTA "\x1b[1;3;35m"
#define CYAN    "\x1b[1;3;36m"
#define RESET   "\x1b[0m"

int main(int argc, char **argv) {
  FUNC *class;
  FUNC *def;
  FUNC *dir;
  FUNC *token;
  FUNC *exin;
  POS *pos;
  POS *tpos;
  MAX *max;
  EXC *exc;
  FILE *fpn;
  FILE *fpc;
  FILE *fpt;
  FILE *fpw;
  char *instr;
  char *inc;
  char *floc;
  char **args;
  int ret;
  int i = 0;
  int ptr;
  max = malloc(sizeof(MAX));
  max->class = 0;
  max->def = 0;
  max->dir = 0;
  max->token = 0;
  max->exin = 0;
  max->pos = 0;
  max->tpos = 0;
  max->line = 0;
  max->ptr = 0;
  pos = malloc(sizeof(POS)*100);
  tpos = malloc(sizeof(POS)*100);
  class = malloc(sizeof(FUNC)*100);
  token = malloc(sizeof(FUNC)*100);
  def = malloc(sizeof(FUNC)*100);
  dir = malloc(sizeof(FUNC)*100);
  exin = malloc(sizeof(FUNC)*100);
  instr = malloc(sizeof(char)*100);
  inc = malloc(sizeof(char)*100);
  floc = malloc(sizeof(char)*100);
  fpn = fopen("/home/achyut/latex/config/config.act", "r");
  fgets(instr, 100, fpn);
  inc = parsecon(instr);
  args = malloc(sizeof(char *)*100);
  while(i<100) {
    *(args+i) = malloc(sizeof(char)*20);
    i++;
  }
  i = 0;
  while(!feof(fpn)) {
    if(strcmp(inc, "class") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        class[max->class].name = malloc(sizeof(char)*1000);
        class[max->class].function = malloc(sizeof(char)*1000);
        strcpy(class[max->class].name, args[0]);
        strcpy(class[max->class].function, args[2]);
        max->class = max->class + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "token") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        token[max->token].name = malloc(sizeof(char)*1000);
        token[max->token].function = malloc(sizeof(char)*1000);
        strcpy(token[max->token].name, args[0]);
        if(strcmp(args[2], "") != 0) {
          strcpy(token[max->token].function, args[2]);
        }
        token[max->token].position = ftell(fpn) + 1;
        max->token = max->token + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "default") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        def[max->def].name = malloc(sizeof(char)*1000);
        def[max->def].function = malloc(sizeof(char)*1000);
        strcpy(def[max->def].name, args[0]);
        strcpy(def[max->def].function, args[2]);
        def[max->def].position = ftell(fpn) - strlen(args[2]) - 2;
        max->def = max->def + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "directory") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        dir[max->dir].name = malloc(sizeof(char)*1000);
        dir[max->dir].function = malloc(sizeof(char)*1000);
        strcpy(dir[max->dir].name, args[0]);
        strcpy(dir[max->dir].function, args[2]);
        max->dir = max->dir + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "exindef") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        exin[max->exin].name = malloc(sizeof(char)*1000);
        exin[max->exin].function = malloc(sizeof(char)*1000);
        strcpy(exin[max->exin].name, args[0]);
        strcpy(exin[max->exin].function, args[2]);
        max->exin = max->exin + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
    }
  }
  if(argc == 1) {
    printf(RED "ERROR! Missing file to be compiled." RESET "\n");
    return 0;
  }
  ret = filedet(argv[1]);
  if(ret == 0) {
    return 0;
  }
  exc = malloc(sizeof(EXC)*100);
  fpc = fopen(argv[1], "r");
  floc = namedet(argv[1], dir, max);
  fpw = fopen(floc, "w");
  ret = scan(fpw, fpc, pos, max, class, token, exc, 'n', exin);
  fseek(fpc, 0, SEEK_SET);
  max->pos = ret;
  ret = ccompile(pos, max->pos, def, fpn, token, max, class, dir, exin);
  if(ret == 1) {
    return 0;
  }
  ret = huduku(pos[0].command, class, max->class);
  max->class = ret;
  fpt = fopen(class[ret].function, "r");
  max->ptr = 0;
  ret = scan(fpw, fpt, tpos, max, class, token, exc, 'y', exin);
  ret = scan(fpw, fpc, pos, max, class, token, exc, 'y', exin);
  max->tpos = ret;
  fseek(fpt, 0, SEEK_SET);
  fseek(fpc, 0, SEEK_SET);
  max->ptr = 0;
  ret = tempr(fpt, fpw, fpc, pos, tpos, 1000000, 'r', max, token, exc, 't');
  fflush(fpw);
  compile(floc, 1, def);
  return 1;
}

char **incommands(char *instr) {
  int i;
  int j;
  int argc = 0;
  char x;
  char **args;
  i=0;
  argc = 0;
  args = malloc(sizeof(char *)*100);
  while(i<100) {
    *(args+i) = malloc(sizeof(char)*20);
    i++;
  }
  i = 0;
  while(instr[i] != '\n' && instr[i] != '\0' && instr[i] != '$') {
    j=0;
    while(instr[i] != ' ' && instr[i] != '\n' && instr[i] != '\0' && instr[i] != '=') {
      x = instr[i];
      *(*(args+argc)+j) = x;
      j++;
      i++;
    }
    while(instr[i] == ' ') {
      i++;
    }
    argc++;
    if(instr[i] == '=') {
      *(*(args+argc)) = instr[i];
      *(*(args+argc)+1) = '\0';
      if(instr[i+1] != ' ') {
        argc++;
        *(*(args+argc)+j) = '\0'; 
      }
      i++;
    }
  }
  return args;
}

int tempr(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, int lim, char call, MAX *max, FUNC *token, EXC *exc, char fn) {
  char *instr;
  char *inc;
  int ret = 1;
  int endloc;
  int loc;
  int cloc;
  int tloc;
  int mline;
  char *prev;
  char *next;
  int x;
  mline = max->line;
  instr = malloc(sizeof(char)*1000);
  inc = malloc(sizeof(char)*1000);
  prev = malloc(sizeof(char)*100);
  next = malloc(sizeof(char)*100);
  fgets(instr, 1000, fpt);
  max->line = max->line + 1;
  while(!feof(fpt) && ret == 1 && ftell(fpt) <= lim) {
    inc = parse(instr);
    x = ftell(fpt);
    if(call == 'r') {
      if(x == exc[max->ptr].start && fn == 't') {
        if(exc[max->ptr].yn == 'y' ) {
          fseek(fpt, exc[max->ptr].end, SEEK_SET);
        }
        max->ptr = max->ptr + 1;
        fgets(instr, 1000, fpt);
        inc = parse(instr);
      }
      if(inc[0] != '#') {
        fprintf(fpw, "%s\n", inc);
        loc = ftell(fpw) - strlen(inc) - 1;
      }
      else if(inc[1] >= 48 && inc[1] <= 57) {
        fprintf(fpw, "%s\n", inc);
        loc = ftell(fpw) - strlen(inc) - 1;
      }
      else if(strcmp(inc, "#end") == 0) {
        fgets(instr, 1000, fpt);
        inc = parse(instr);
      }
      else {
        tloc = ftell(fpt);
        fgets(instr, 1000, fpt);
        inc = parse(instr);
        x = ftell(fpt);
        if(strcmp(inc, "#end") == 0) {
          fgets(instr, 1000, fpt);
          inc = parse(instr);
        }
        cloc = tfind(tloc, tpos, max);
        if(cloc < max->tpos) {
          if(cloc != 0) {
            strcpy(prev, tpos[cloc-1].command);
          }
          else {
            strcpy(prev, "NONE");
          }
          if(cloc+1 < max->tpos) {
            strcpy(next, tpos[cloc+1].command);
          }
          else {
            strcpy(next, "NONE");
          }
        }
        while(strncmp(inc, "#\\", 2) != 0) {
          fgets(instr, 1000, fpt);
          inc = parse(instr);
          x = ftell(fpt);
          //if(x == exc[max->ptr].start && fn == 't') {
          //  if(exc[max->ptr].yn == 'y') {
          //    fseek(fpt, exc[max->ptr].end, SEEK_SET);
          //    max->ptr = max->ptr + 1;
          //  }
          //  fgets(instr, 1000, fpt);
          //  inc = parse(instr);
          //}
          if(strcmp(inc, "#end") == 0) {
            fgets(instr, 1000, fpt);
            inc = parse(instr);
          }
        }
        ret = replace(fpt, fpw, fpc, pos, tpos, inc, max, loc, prev, next, token, exc);
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
          max->line = mline;
          return 0;
        }
      }
    }
    fgets(instr, 1000, fpt); 
    max->line = max->line + 1;
  }
  if(call == 's') {
    max->line = mline;
    return 0;
  }
  return ret;
}


int replace(FILE *fpt, FILE *fpw, FILE *fpc, POS *pos, POS *tpos, char *inc, MAX *max, int loc, char *prev, char *next, FUNC *token, EXC *exc) {
  int ccom;
  char *command;
  int i = 2;
  int ret;
  command = malloc(sizeof(char)*997);
  while(inc[i] != '\0') {
    command[i-2] = inc[i];
    i++;
  }
  ccom = find(command, pos, max->tpos, prev, next, token, max);
  if(ccom < max->pos) {
    fseek(fpc, pos[ccom].start, SEEK_SET);
    ret = tempr(fpc, fpw, fpc, pos, tpos, pos[ccom].end, 'r', max, token, exc, 'c');
  }
  else {
    fseek(fpw, loc, SEEK_SET);
    fprintf(fpw, "%%\n");
    ret = tempr(fpt, fpw, fpc, pos, tpos, 100000000, 'r', max, token, exc, 't');
  }
  return ret;
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
  j--;
  while(outstr[j] == ' ') {
    j--;
  }
  outstr[j+1] = '\0';
  return outstr;
}

char *parsecon(char *instr) {
  char *outstr;
  int i = 0;
  int j = 0;
  outstr = malloc(sizeof(char)*1000);
  while(instr[i] == ' ') {
    i++;
  }
  while(instr[i] != '\0' && instr[i] != '\n' && instr[i] != '#') {
    outstr[j] = instr[i];
    i++;
    j++;
  }
  j--;
  while(outstr[j] == ' ') {
    j--;
  }
  outstr[j+1] = '\0';
  return outstr;
}

int find(char *command, POS *pos, int count, char *prev, char *next, FUNC *token, MAX *max) {
  int i = 0;
  int ret;
  if(strcmp(pos[i].command, command) == 0 && strcmp(pos[i+1].command, next) == 0) {
    return i;
  }
  i++;
  ret = huduku(command, token, max->token);
  if(strcmp(token[ret].function, command) != 0) {
    while((strcmp(pos[i].command, command) != 0) && i < count || pos[i].mark == 1) {
      i++;
      if(pos[i].command == NULL) break;
    }
  }
  else {
    while((strcmp(pos[i].command, "general") != 0) && i < count || pos[i].mark == 1 || strcmp(pos[i-1].command, prev) != 0 || pos[i+1].command != NULL) {
      if(i >= count) {
        return i;
      }
      if(pos[i+1].command != NULL) {
        if(strcmp(pos[i+1].command, next) == 0 && (strcmp(pos[i-1].command, prev) == 0 || i == 1) && pos[i].mark == 0 && strcmp(pos[i].command, "general") == 0) {
          return i;
        }
      }
      i++;
      if(pos[i].command == NULL) break;
    }
  }
  if(i < count) {
    pos[i].mark = 1;
  }
  return i;
}

int tfind(int cloc, POS *pos, MAX *max) {
  int i = 0;
  while(pos[i].start != cloc && i < max->tpos) {
    i++;
  }
  return i;
}

char *namedet(char *arg, FUNC *dir, MAX *max) {
  char *name;
  int i;
  int j = 0;
  int ret;
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
  ret = huduku("source", dir, max->dir);
  name = insert(name, "/");
  name = insert(name, dir[ret].function);
  return name;
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
  out = malloc(sizeof(char)*1000);
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

void compile(char *floc, int open, FUNC *def) {
  char *command;
  char *new;
  char *mvto;
  char *newn;
  int i;
  int j = 0;
  int ret;
  int rett;
  rett = huduku("pdflatexout", def, 100);
  new = malloc(sizeof(char)*1000);
  newn = malloc(sizeof(char)*1000);
  mvto = malloc(sizeof(char)*1000);
  command = insert(floc, "-output-directory /home/achyut/latex/src/ ");
  if(strcmp(def[rett].function, "NULL") == 0) {
    append(command, " 1>/dev/null", strlen(command));
  }
  command = insert(command, "printf 'x' | pdflatex ");
  ret = system(command);
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
  out = malloc(sizeof(char)*1000);
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

int ccompile(POS *pos, int count, FUNC *def, FILE *fpn, FUNC *token, MAX *max, FUNC *class, FUNC *dir, FUNC *exin) {
  int i;
  int j;
  int error = 0;
  char con;
  char conn;
  int ret;
  int rett;
  int dummy;
  char *command;
  command = malloc(sizeof(char)*100);
  ret = huduku("compile", def, 100);
  sprint("Compiling AcTex..........................................................................\n", 10000);
  for(i=0; i<count; i++) {
    rett = huduku(pos[i].command, token, max->token);
    if(pos[i].start == 0) {
      printf("(Line no. %d)    " RED "ERROR! Illegal \\# token" RESET "\n", pos[i].line);
      error = 1;
    }
    if(pos[i].end == 0) {
      printf("(Line no. %d)    " RED "ERROR! Illegal /# token" RESET "\n", pos[i].line);
      error = 1;
    }
    if(rett >= max->token && i != 0) {
      printf("(Line no. %d)    " RED "ERROR! Illegal token <%s>" RESET "\n", pos[i].line, pos[i].command);
      error = 1;
      printf("Do you want to add token <%s> to the list of tokens (y or n)? ", pos[i].command);
      fflush(stdout);
      scanf("%c", &con);
      while(con == ' ') {
        scanf("%c", &con);
      }
      if(con == 'y') {
        j = 2;
        while(pos[i].command[j] != '\0') {
          command[j-2] = pos[i].command[j];
          j++;
        }
        fpn = frein(command, fpn, token[rett-1].position, 'n');
        //token[rett].name = malloc(sizeof(char)*100);
        //strcpy(token[rett].name, command);
        fpn = readconfig(class, token, def, dir, exin, fpn, max);
      }
    }
  }
  if(error == 0) {
    printf(GREEN "AcTeX compilation successful!" RESET "\nCompiling LaTeX...\n");
    return 0;
  }
  else if(strcmp(def[ret].function, "NONE") == 0) {
    scanf("%c", &con);
    printf(RED "AcTeX compilation unsuccessful. LaTeX compilation halted." RESET "\n");
    printf("Do you still want to continue ((y)es/(n)o/(N)ever/(Y)always)? ");
    fflush(stdout);
    scanf("%c", &con);
    while(con == ' ') {
      scanf("%c", &con);
    }
    if(con == 'y') {
      return 0;
    }
    else if(con == 'n') {
      return 1;
    }
    else if(con == 'Y') {
      fpn = frein("WARNING", fpn, def[ret].position, 's');
      return 0;
    }
    else if(con == 'N') {
      fpn = frein("ERROR", fpn, def[ret].position, 's');
      return 1;
    }
  }
  else if(strcmp(def[ret].function, "ERROR") == 0) {
    printf(RED "AcTeX compilation unsuccessful. LaTeX compilation halted." RESET "\n");
    return 1;
  }
  else if(strcmp(def[ret].function, "WARNING") == 0) {
    printf(MAGENTA "AcTeX compilation unsuccessful." RESET "\nCompiling LaTeX...\n");
    return 0;
  }
}

int scan(FILE *fpw, FILE *fpc, POS *pos, MAX *max, FUNC *class, FUNC *token, EXC *exc, char scex, FUNC *exin) {
  char *instr;
  char *inc;
  char *command;
  char **args;
  int i = 0;
  int ret;
  int j = 0;
  int flag = 1;
  args = malloc(sizeof(char *)*100);
  while(i<100) {
    *(args+i) = malloc(sizeof(char)*20);
    i++;
  }
  i = 0;
  instr = malloc(sizeof(char)*1000);
  inc = malloc(sizeof(char)*1000);
  command = malloc(sizeof(char)*998);
  fgets(instr, 1000, fpc);
  inc = parse(instr);
  max->line = max->line + 1;
  while(!feof(fpc)) {
    if(inc[0] != '#') {
      fgets(instr, 1000, fpc);
      inc = parse(instr);
      max->line = max->line + 1;
      flag = 0;
    }
    else if(inc[1] != '/' && inc[1] != '\\') {
      args = incommands(inc);
      if(strcmp(args[0], "#class") == 0) {
        ret = dclass(fpc, pos, max, class, exc, args, i);
        i++;
      }
      else if(strcmp(args[0], "#section") == 0 && scex == 'y') {
        ret = section(fpc, pos, max, class, exc, args, exin);
      }
      else if(strcmp(args[0], "#exclude") == 0 && scex == 'y') {
        ret = exclude(fpc, pos, max, class, exc, args);
      }
      else if(strcmp(args[0], "#include") == 0 && scex == 'y') {
        ret = include(fpc, pos, max, class, exc, args);
      }
      flag = 1;
    }
    else {
      j = 0;
      while(inc[j] != '\0') {
        command[j-2] = inc[j];
        j++;
      }
      command[j-2] = '\0';
      ret = huduku(command, token, max->token);
      pos[i].command = malloc(sizeof(char)*100);
      if(ret < max->token) {
        strcpy(pos[i].command, token[ret].name);
      }
      else {
        strcpy(pos[i].command, inc);
      }
      pos[i].start = ftell(fpc);
      ret = tempr(fpc, fpw, fpc, pos, pos, 100000, 's', max, token, exc, 'c');
      pos[i].end = ret;
      pos[i].line = max->line;
      pos[i].mark = 0;
      i++;
      flag = 1;
    }
    if(flag == 1) {
      fgets(instr, 1000, fpc);
      inc = parse(instr);
      max->line = max->line + 1;
    }
  }
  return i;
}

int huduku(char *tbf, FUNC *in, int max) {
  int i = 0;
  while(strcmp(in[i].name, tbf) != 0 && i < max) {
    if(strcmp(in[i].name, "general") == 0 && strcmp(in[i].function, tbf) == 0) {
      return i;
    }
    i++;
    if(in[i].name == NULL) break;
  }
  return i;
}

void sprint(char *str, int time) {
  int i = 0;
  while(i<strlen(str)) {
    printf("%c", str[i]);
    if(str[i-1] == '.' || str[i-1] == '!' || str[i-1] == '?') usleep(2*time);
    else if(str[i-1] == ',') usleep(1.5*time);
    else usleep(time);
    fflush(stdout);
    i++;
  }
  printf("\n");
}

FILE *frein(char *tbr, FILE *fp, int till, char line) {
  FILE *fpn;
  char x;
  char *maintain;
  int i = 0;
  maintain = malloc(sizeof(char)*100);
  fpn = fopen("/tmp/copy.act", "w");
  fseek(fp, 0, SEEK_SET);
  x = fgetc(fp);
  while(x != EOF && ftell(fp) != till) {
    fprintf(fpn, "%c", x);
    x = fgetc(fp);
  }
  if(line == 's') {
    fprintf(fpn, "%s", tbr);
    while(x != ' ' && x != '\n') {
      x = fgetc(fp);
    }
  }
  else if(line == 'n') {
    while(x != '\n') {
      maintain[i] = x;
      i++;
      x = fgetc(fp);
    }
    fseek(fp, till, SEEK_SET);
    fprintf(fpn, "  %s\n%s", tbr, maintain);
  }
  while(x != EOF) {
    fprintf(fpn, "%c", x);
    x = fgetc(fp);
  }
  fclose(fpn);
  fclose(fp);
  system("mv /tmp/copy.act /home/achyut/latex/config/config.act");
  fp = fopen("/home/achyut/latex/config/config.act", "r");
  return fp;
}

int filedet(char *file) {
  FILE *fp;
  fp = fopen(file, "r");
  if(fp == NULL) {
    printf(RED "ERROR! File <%s> does not exist." RESET "\n", file);
    return 0;
  }
  else {
    return 1;
  }
}

//int commands(char *command, FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args) {
//  int ret;
//  if(strcmp(command, "class") == 0) {
//    ret = dclass(fpc, pos, max, class, exc, args);
//    return ret;
//  }
//  else if(strcmp(command, "section") == 0) {
//    ret = exclude(fpc, pos, max, class, exc, args);
//    return ret;
//  }
//  else if(strcmp(command, "exclude") == 0) {
//    ret = exclude(fpc, pos, max, class, exc, args);
//    return ret;
//  }
//}

int dclass(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args, int i) {
  int ret;
  pos[i].command = malloc(sizeof(char)*100);
  strcpy(pos[i].command, args[2]);
  pos[i].start = ftell(fpc);
  pos[i].end = ftell(fpc);
  pos[i].line = max->line;
  ret = huduku(args[2], class, max->class);
  return ret;
}

int section(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args, FUNC *exin) {
  int ret;
  char *instr;
  char *inc;
  instr = malloc(sizeof(char)*1000);
  inc = malloc(sizeof(char)*1000);
  exc[max->ptr].title = malloc(sizeof(char)*100);
  strcpy(exc[max->ptr].title, args[1]);
  exc[max->ptr].start = ftell(fpc);
  fgets(instr, 1000, fpc);
  inc = parse(instr);
  while(strcmp(inc, "#end") != 0) {
    fgets(instr, 1000, fpc);
    inc = parse(instr);
  }
  exc[max->ptr].end = ftell(fpc);
  fseek(fpc, exc[max->ptr].start, SEEK_SET);
  ret = huduku(args[1], exin, max->exin);
  if(ret >= max->exin) {
    ret = huduku("other", exin, max->exin);
    if(strcmp(exin[ret].function, "in") == 0) {
      exc[max->ptr].yn = 'n';
    }
    else if(strcmp(exin[ret].function, "ex") == 0) {
      exc[max->ptr].yn = 'y';
    }
  }
  else {
    if(strcmp(exin[ret].function, "in") == 0) {
      exc[max->ptr].yn = 'n';
    }
    else if(strcmp(exin[ret].function, "ex") == 0) {
      exc[max->ptr].yn = 'y';
    }
  }
  max->ptr = max->ptr + 1;
  return 1;
}

int exclude(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args) {
  int ret;
  ret = ehuduku(args[1], exc, 1000);
  exc[ret].yn = 'y';
}

int ehuduku(char *tbf, EXC *in, int max) {
  int i = 0;
  while(strcmp(in[i].title, tbf) != 0 && i < max) {
    i++;
    if(in[i].title == NULL) break;
  }
  return i;
}

int include(FILE *fpc, POS *pos, MAX *max, FUNC *class, EXC *exc, char **args) {
  int ret;
  ret = ehuduku(args[1], exc, 1000);
  exc[ret].yn = 'n';
}

FILE *readconfig(FUNC *class, FUNC *token, FUNC *def, FUNC *dir, FUNC *exin, FILE *fpn, MAX *max) {
  char *instr;
  char *inc;
  char **args;
  int i = 0;
  max->class = 0;
  max->def = 0;
  max->dir = 0;
  max->token = 0;
  max->exin = 0;
  instr = malloc(sizeof(char)*100);
  inc = malloc(sizeof(char)*100);
  fpn = fopen("/home/achyut/latex/config/config.act", "r");
  fgets(instr, 100, fpn);
  inc = parsecon(instr);
  args = malloc(sizeof(char *)*100);
  while(i<100) {
    *(args+i) = malloc(sizeof(char)*20);
    i++;
  }
  i = 0;
  fseek(fpn, 0, SEEK_SET);
  while(!feof(fpn)) {
    if(strcmp(inc, "class") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        class[max->class].name = malloc(sizeof(char)*1000);
        class[max->class].function = malloc(sizeof(char)*1000);
        strcpy(class[max->class].name, args[0]);
        strcpy(class[max->class].function, args[2]);
        max->class = max->class + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "token") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        token[max->token].name = malloc(sizeof(char)*1000);
        token[max->token].function = malloc(sizeof(char)*1000);
        strcpy(token[max->token].name, args[0]);
        if(strcmp(args[2], "") != 0) {
          strcpy(token[max->token].function, args[2]);
        }
        token[max->token].position = ftell(fpn) + 1;
        max->token = max->token + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "default") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        def[max->def].name = malloc(sizeof(char)*1000);
        def[max->def].function = malloc(sizeof(char)*1000);
        strcpy(def[max->def].name, args[0]);
        strcpy(def[max->def].function, args[2]);
        def[max->def].position = ftell(fpn) - strlen(args[2]) - 2;
        max->def = max->def + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "directory") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        dir[max->dir].name = malloc(sizeof(char)*1000);
        dir[max->dir].function = malloc(sizeof(char)*1000);
        strcpy(dir[max->dir].name, args[0]);
        strcpy(dir[max->dir].function, args[2]);
        max->dir = max->dir + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else if(strcmp(inc, "exindef") == 0) {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
      while(strcmp(inc, "end") != 0) {
        args = incommands(inc);
        exin[max->exin].name = malloc(sizeof(char)*1000);
        exin[max->exin].function = malloc(sizeof(char)*1000);
        strcpy(exin[max->exin].name, args[0]);
        strcpy(exin[max->exin].function, args[2]);
        max->exin = max->exin + 1;
        fgets(instr, 100, fpn);
        inc = parsecon(instr);
      }
    }
    else {
      fgets(instr, 100, fpn);
      inc = parsecon(instr);
    }
  }
  return fpn;
}
