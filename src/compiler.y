%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_MAX 100

typedef int *list_t, *list_it;

list_t new_list();
void del_list(list_t l);

void list_append(list_t *pl, int v);
// lhs += rhs, rhs is deleted automatically
void list_add(list_t *plhs, list_t rhs);
int list_size(list_t l);

list_it list_begin(list_t l);
list_it list_end(list_t l);
void list_it_next(list_it *it);

#define for_list(it, list) \
    for (list_it it = list_begin(list), _e = list_end(list); it != _e; list_it_next(&it))

#define amax 2048     /* 地址上界*/
#define cxmin 10      /* 代码起始地址 */
#define cxmax 200     /* 最多的虚拟机代码数 */
#define stacksize 500 /* 运行时数据栈元素最多为500个 */

/* 虚拟机代码指令 */
enum fct {
    nop, lit, opr, lod, sto, cal, ini, jmp, jpc,
};

/* 虚拟机代码结构 */
struct instruction {
    enum fct f; /* function code. 虚拟机代码
    指令 */
    int l;      /* level. 引用层与声明层的层次差 */
    int a;      /* displacement address. 根据f的不同而不同 */
};
struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */

#define txmax 100 /* 符号表容量 */
#define al 10     /* 标识符的最大长度 */
#define maxpl 10  /* max length of function parameter list */
#define levmax 3  /* 最大允许过程嵌套声明层数*/

enum entry_type {
    e_var,
};

enum sym_type {
    sym_unknown,
    sym_char,
    sym_int,
    sym_bool
};

struct var_entry {
    enum sym_type type;
    int addr;
    int is_arr;
    int arrlen;
};
/* 符号表结构 */
struct symbol_table {
    char name[al];        /* 名字 */
    enum entry_type kind; /* 类型：const，var或proc */
    int level;
    struct var_entry e_var;
} table[txmax]; /* 符号表 */

// symbol table entry
typedef struct symbol_table *STE;

int get_table_size();
STE insert_symbol(char *name, enum entry_type t);
STE find_symbol(char *name);
void fill_var_entry(STE ste, struct var_entry *ve);

int tx;           /* 符号表当前尾指针 */
int px;           /* 嵌套过程索引表proctable的指针 */
int lev;          /* 层次记录 */
int proctable[3]; /* 嵌套过程索引表，最多嵌套三层 */
char id[al];
int num;


typedef struct {
    list_t brklist;
    list_t contlist;
} a_statjump_t;

a_statjump_t A_STATJUMP_ZERO = {NULL, NULL};

typedef struct {
    list_t nextlist;
    int next_ip;
} a_purejump_t;

typedef struct {
    int level;
    int addr;
    int is_arr;
} a_lval_t;

int cx; /* 虚拟机代码指针, 取值范围[0, cxmax-1] */
void init_X0();
void gen(enum fct x, int y, int z);
void interpret();
int base(int l, int* s, int b);


int yylex(void);
int yywrap();
void yyerror(const char *s);

void cglit(int v) { gen(lit, 0, v); }
void cglod(int l, int a) { gen(lod, l, a); }
void cglodoff(int l) { gen(lod, l, 0); }
void cgsto(int l, int a) { gen(sto, l, a); }
void cgstooff(int l) { gen(sto, l, 0); }
void cgint(int n) { gen(ini, 0, n); }
void cgjmp(int a) { gen(jmp, 0, a); }
void cgjpc(int a) { gen(jpc, 0, a); }
void cgneg() { gen(opr, 0, 1); }
void cgadd() { gen(opr, 0, 2); }
void cgsub() { gen(opr, 0, 3); }
void cgmul() { gen(opr, 0, 4); }
void cgdiv() { gen(opr, 0, 5); }
void cgmod() { gen(opr, 0, 19); }
void cglnot() { gen(opr, 0, 7); }
void cgeq() { gen(opr, 0, 8); }
void cgne() { gen(opr, 0, 9); }
void cglt() { gen(opr, 0, 10); }
void cgge() { gen(opr, 0, 11); }
void cggt() { gen(opr, 0, 12); }
void cgle() { gen(opr, 0, 13); }
void cgland() { gen(opr, 0, 17); }
void cglor() { gen(opr, 0, 18); }
void cgwrite() { gen(opr, 0, 14); }
void cgread() { gen(opr, 0, 16); }

void cgstack(int n) { gen(ini, 0, n); }

void fatal(char *s) {
    printf("Fatal error: %s\nAbort now!\n", s);
    exit(1);
}

void statjump_add(a_statjump_t *lhs, a_statjump_t rhs);
int getip();
void backpatch(list_t *pl, int ip);
void fill_symbol(list_t *pl, enum sym_type t);
int get_local_size();

%}

%union {
    int number;
    char *string;
    a_statjump_t statjump;
    a_purejump_t purejump;
    list_t symlist;
    enum sym_type type;
    a_lval_t lval;
}

%token T_IF T_ELSE
%token T_WHILE T_DO
%token T_BREAK T_CONTINUE
%token T_MAIN T_READ T_WRITE
%token T_TRUE T_FALSE
%token T_INT T_CHAR T_BOOL
%nonassoc P_IFX
%nonassoc T_ELSE

%right '='
%left T_OR
%left T_AND
%left T_EQ T_NE
%left '<' '>' T_LE T_GE
%left '+' '-'
%left '*' '/' '%'

%token <number> T_NUM
%token <string> T_IDENT

%type <number> h_get_loc
%type <purejump> h_jmp h_jmp_zero h_jmp_not_zero
%type <statjump> stat_list stat expr_stat sele_stat iter_stat comp_stat
%type <statjump> jump_stat read_stat write_stat
%type <type> decl_spec
%type <symlist> init_decl_list init_decl
%type <lval> expr

%%

program
    : T_MAIN '{' decl_list h_ini stat_list '}' {
        cgjmp(0);
    }
    ;

decl_list
    : decl_list decl_stat   {;}
    | decl_stat             {;}
    ;

decl_stat
    : decl_spec init_decl_list ';' {
        fill_symbol(&$2, $1);
    }
    ;

decl_spec
    : T_INT     { $$ = sym_int; }
    | T_CHAR    { $$ = sym_char; }
    | T_BOOL    { $$ = sym_bool; }
    ;

init_decl_list
    : init_decl { $$ = $1; }
    | init_decl_list ',' init_decl  {
        //   1        2      3
        $$ = $1;
        list_add(&$$, $3);
    }
    ;

init_decl
    : T_IDENT {
        STE ste = find_symbol($1);
        if (ste)
            fatal("reapeat definition");
        insert_symbol($1, e_var);
        free($1);

        $$ = NULL;
        list_append(&$$, get_table_size() - 1);
    }
    | T_IDENT '[' T_NUM ']' {
        STE ste = find_symbol($1);
        if (ste)
            fatal("reapeat definition");
        ste = insert_symbol($1, e_var);
        free($1);
        ste->e_var.is_arr = 1;
        ste->e_var.arrlen = $3;

        $$ = NULL;
        list_append(&$$, get_table_size() - 1);

    }

stat_list
    : stat_list stat {
        $$ = $1;
        statjump_add(&$$, $2);
    }
    | stat  { $$ = $1; }
    ;

stat
    : comp_stat
    | expr_stat
    | sele_stat
    | iter_stat
    | jump_stat
    | read_stat
    | write_stat
    ;

comp_stat
    : '{' '}'           { $$ = A_STATJUMP_ZERO; }
    | '{' stat_list '}' { $$ = $2; }
    ;

expr_stat
    : ';'       { $$ = A_STATJUMP_ZERO; }
    | expr ';'  { $$ = A_STATJUMP_ZERO; }
    ;

sele_stat
    : T_IF '(' expr ')' h_jmp_zero stat %prec P_IFX {
        int next_ip = getip();
        $$ = $6;

        backpatch(&$5.nextlist, next_ip);
    }
    | T_IF '(' expr ')' h_jmp_zero stat T_ELSE h_jmp stat {
        int next_ip = getip();
        $$ = $6;
        statjump_add(&$$, $9);

        backpatch(&$8.nextlist, next_ip);
        backpatch(&$5.nextlist, $8.next_ip);
    }
    ;

iter_stat
    : T_WHILE h_get_loc '(' expr ')' h_jmp_zero stat h_jmp {
        int next_ip = getip();
        $$ = A_STATJUMP_ZERO;

        backpatch(&$6.nextlist, next_ip);
        backpatch(&$8.nextlist, $2);
        backpatch(&$7.brklist, next_ip);
        backpatch(&$7.contlist, $2);
    }
    | T_DO h_get_loc stat T_WHILE h_get_loc '(' expr ')' h_jmp_not_zero ';' {
        int next_ip = getip();
        $$ = A_STATJUMP_ZERO;

        backpatch(&$9.nextlist, $2);
        backpatch(&$3.brklist, next_ip);
        backpatch(&$3.contlist, $5);
    }
    ;

jump_stat
    : T_BREAK h_jmp ';' {
        $$.brklist = $2.nextlist;
        $$.contlist = NULL;
    }
    | T_CONTINUE h_jmp ';' {
        $$.brklist = NULL;
        $$.contlist = $2.nextlist;
    }
    ;

read_stat
    : T_READ T_IDENT ';' {
        $$ = A_STATJUMP_ZERO;
        free($2);
    }
    ;

write_stat
    : T_WRITE expr ';' {
        $$ = A_STATJUMP_ZERO;
        cgwrite();
    }

expr
    : T_NUM     { cglit($1); }
    | T_TRUE    { cglit(1); }
    | T_FALSE   { cglit(0); }
    | T_IDENT {
        STE ste = find_symbol($1);
        free($1);
        if (!ste) fatal("ident not found");
        $$.level = ste->level;
        $$.addr = ste->e_var.addr;
        $$.is_arr = 0;
        cglod($$.level, $$.addr);
    }
    | T_IDENT '[' expr ']' {
        STE ste = find_symbol($1);
        free($1);
        if (!ste) fatal("ident not found");
        $$.level = ste->level;
        $$.addr = ste->e_var.addr;
        $$.is_arr = 1;
        cglit($$.addr);
        cgadd();
        cglodoff($$.level);
    }
    | '(' expr ')'      { }
    | expr '*' expr     { cgmul(); }
    | expr '/' expr     { cgdiv(); }
    | expr '+' expr     { cgadd(); }
    | expr '-' expr     { cgsub(); }
    | expr '%' expr     { cgmod(); }

    | expr '<' expr     { cglt(); }
    | expr '>' expr     { cggt(); }
    | expr T_LE expr    { cgle(); }
    | expr T_GE expr    { cgge(); }
    | expr T_EQ expr    { cgeq(); }
    | expr T_NE expr    { cgne(); }

    | expr T_AND expr   { cgland(); }
    | expr T_OR expr    { cglor(); }
    | expr '=' expr     {
        if ($1.is_arr) cgstooff($1.level);
        else cgsto($1.level, $1.addr);
    }
    ;

h_ini
    : { cgstack(get_local_size()); }
    ;

h_get_loc
    : { $$ = getip(); }
    ;

h_jmp
    : {
        $$.nextlist = new_list();
        list_append(&$$.nextlist, getip());
        cgjmp(0);

        $$.next_ip = getip();
    }
    ;

h_jmp_zero
    : {
        $$.nextlist = new_list();
        list_append(&$$.nextlist, getip());
        cgjpc(0);

        $$.next_ip = getip();
    }
    ;

h_jmp_not_zero
    : {
        cglnot();
        $$.nextlist = new_list();
        list_append(&$$.nextlist, getip());
        cgjpc(0);

        $$.next_ip = getip();
    }

%%

void yyerror(const char *s) {
    printf("[error] %s\n", s);
}

FILE* fin;     /* 输入源文件 */
FILE* ftable;  /* 输出符号表 */
FILE* fcode;   /* 输出虚拟机代码 */
FILE* foutput; /* 输出出错示意（如有错） */
FILE* fresult; /* 输出执行结果 */

/* 初始化 */
void init_X0() {
    cx = cxmin;
    tx = 0;
    px = 0;
    lev = 0;
    proctable[0] = 0;
    num = 0;
}

/* 生成虚拟机代码 */
void gen(enum fct x, int y, int z) {
    if (cx >= cxmax) {
        fatal("Program is too long!"); /* 生成的虚拟机代码程序过长 */
    }
    if (z >= amax) {
        fatal("Displacement address is too big!"); /* 地址偏移越界 */
    }
    code[cx].f = x;
    code[cx].l = y;
    code[cx].a = z;
    cx++;
}

/* 解释程序 */
void interpret() {
    int p = cxmin;        /* 指令指针 */
    int b = 1;            /* 指令基址 */
    int t = 0;            /* 栈顶指针 */
    struct instruction i; /* 存放当前指令 */
    int s[stacksize];     /* 栈 */

    printf("Start pl0\n");
    //fprintf(fresult, "Start pl0\n");
    s[0] = 0; /* s[0]不用 */
    s[1] = 0; /* 主程序的三个联系单元均置为0 */
    s[2] = 0;
    s[3] = 0;
    do {
        i = code[p]; /* 读当前指令 */
        p = p + 1;
        switch (i.f) {
            case nop:
                fatal("exec nop");
            case lit: /* 将常量a的值取到栈顶 */
                t = t + 1;
                s[t] = i.a;
                break;
            case opr: /* 数学、逻辑运算 */
                switch (i.a) {
                    case 0: /* 函数调用结束后返回 */
                        t = b - 1;
                        p = s[t + 3];
                        b = s[t + 2];
                        break;
                    case 1: /* 栈顶元素取反 */
                        s[t] = -s[t];
                        break;
                    case 2: /* 次栈顶项加上栈顶项，退两个栈元素，相加值进栈 */
                        t = t - 1;
                        s[t] = s[t] + s[t + 1];
                        break;
                    case 3: /* 次栈顶项减去栈顶项 */
                        t = t - 1;
                        s[t] = s[t] - s[t + 1];
                        break;
                    case 4: /* 次栈顶项乘以栈顶项 */
                        t = t - 1;
                        s[t] = s[t] * s[t + 1];
                        break;
                    case 5: /* 次栈顶项除以栈顶项 */
                        t = t - 1;
                        s[t] = s[t] / s[t + 1];
                        break;
                    case 6: /* 栈顶元素的奇偶判断 */
                        s[t] = s[t] % 2;
                        break;
                    case 7: /* 栈顶元素逻辑取反 */
                        s[t] = !s[t];
                        break;
                    case 8: /* 次栈顶项与栈顶项是否相等 */
                        t = t - 1;
                        s[t] = (s[t] == s[t + 1]);
                        break;
                    case 9: /* 次栈顶项与栈顶项是否不等 */
                        t = t - 1;
                        s[t] = (s[t] != s[t + 1]);
                        break;
                    case 10: /* 次栈顶项是否小于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] < s[t + 1]);
                        break;
                    case 11: /* 次栈顶项是否大于等于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] >= s[t + 1]);
                        break;
                    case 12: /* 次栈顶项是否大于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] > s[t + 1]);
                        break;
                    case 13: /* 次栈顶项是否小于等于栈顶项 */
                        t = t - 1;
                        s[t] = (s[t] <= s[t + 1]);
                        break;
                    case 14: /* 栈顶值输出 */
                        printf("%d\n", s[t]);
                        fprintf(fresult, "%d\n", s[t]);
                        t = t - 1;
                        break;
                    case 15: /* 输出换行符 */
                        printf("\n");
                        fprintf(fresult, "\n");
                        break;
                    case 16: /* 读入一个输入置于栈顶 */
                        t = t + 1;
                        printf("?");
                        fprintf(fresult, "?");
                        scanf("%d", &(s[t]));
                        fprintf(fresult, "%d", s[t]);
                        break;
                    case 17: /* AND */
                        t = t - 1;
                        s[t] = s[t] && s[t + 1];
                        break;
                    case 18: /* OR */
                        t = t - 1;
                        s[t] = s[t] || s[t + 1];
                        break;
                    case 19: /* MOD */
                        t = t - 1;
                        s[t] = s[t] % s[t + 1];
                        break;
                    default:
                        fatal("unrecognized opr");
                }
                break;
            case lod:
                t = t + 1;
                switch (i.a) {
                    case 0:
                        s[t] = s[base(i.l, s, b) + s[t - 1]];
                        break;
                    default: /* 取相对当前过程的数据基地址为a的内存的值到栈顶 */
                        s[t] = s[base(i.l, s, b) + i.a];
                        break;
                }
                break;
            case sto: /* 栈顶的值存到相对当前过程的数据基地址为a的内存 */
                switch (i.a) {
                    case 0:
                        if (t <= 3) fatal("stack out of bound");
                        s[base(i.l, s, b) + s[t - 2]] = s[t];
                        t = t - 3;
                        break;
                    default:
                        s[base(i.l, s, b) + i.a] = s[t];
                        t = t - 1;
                        break;
                }
                break;
            case cal:                       /* 调用子过程 */
                s[t + 1] = base(i.l, s, b); /* 将父过程基地址入栈，即建立静态链 */
                s[t + 2] = b;               /* 将本过程基地址入栈，即建立动态链 */
                s[t + 3] = p;               /* 将当前指令指针入栈，即保存返回地址 */
                b = t + 1;                  /* 改变基地址指针值为新过程的基地址 */
                p = i.a;                    /* 跳转 */
                break;
            case ini: /* 在数据栈中为被调用的过程开辟a个单元的数据区 */
                t = t + i.a;
                break;
            case jmp: /* 直接跳转 */
                p = i.a;
                break;
            case jpc: /* 条件跳转 */
                if (s[t] == 0)
                    p = i.a;
                t = t - 1;
                break;
        }
    } while (p != 0);
    printf("End pl0\n");
    /* fprintf(fresult, "\n\n"); */
}

/* 通过过程基址求上l层过程的基址 */
int base(int l, int* s, int b) {
    int b1;
    b1 = b;
    while (l > 0) {
        b1 = s[b1];
        l--;
    }
    return b1;
}

list_t new_list() {
    list_t l = malloc(LIST_MAX + 1);
    if (!l) {
        perror("malloc");
        fatal("malloc fail");
    }
    l[0] = 0;
    return l;
}

void del_list(list_t l) {
    free(l);
}

void list_append(list_t *pl, int v) {
    if (!*pl) *pl = new_list();
    list_t l = *pl;
    if (l[0] >= LIST_MAX)
        fatal("list full");
    l[++l[0]] = v;
}

void list_add(list_t *plhs, list_t rhs) {
    if (!*plhs) {
        *plhs = rhs;
        return;
    }
    if (!rhs) return;
    list_t lhs = *plhs;
    if (lhs[0] + rhs[0] >= LIST_MAX)
        fatal("list full");
    for (int i = 1; i <= rhs[0]; ++i)
        lhs[++lhs[0]] = rhs[i];
    del_list(rhs);
}

int list_size(list_t l) {
    return l[0];
}

list_it list_begin(list_t l) {
    return l + 1;
}

list_it list_end(list_t l) {
    return l + l[0] + 1;
}

void list_it_next(list_it *it) {
    ++*it;
}

void init_var_entry(struct var_entry *ve) {
    memset(ve, 0, sizeof(struct var_entry));
}
int get_table_size() {
    return tx;
}
STE insert_symbol(char *name, enum entry_type t) {
    if (tx >= txmax)
        fatal("symbol table overflow");
    strcpy(table[tx].name, name);
    table[tx].kind = t;
    table[tx].level = lev;
    switch (t) {
        case e_var:
            init_var_entry(&table[tx].e_var);
            break;
        default:
            fatal("entry type");
    }
    return &table[tx++];
}

void fill_var_entry(STE ste, struct var_entry *ve) {
    if (ste < table || ste >= table + txmax)
        fatal("fill entry out of bound");
    ste->e_var = *ve;
}
STE find_symbol(char *name) {
    STE rst = NULL;
    for (int i = 0; i < tx; ++i)
        if (!strcmp(name, table[i].name))
            if (rst == NULL || rst->level < table[i].level)
                rst = table + i;
    return rst;
}

int addr = 3;

int getip() {
    return cx;
}

void statjump_add(a_statjump_t *lhs, a_statjump_t rhs) {
    list_add(&lhs->brklist, rhs.brklist);
    list_add(&lhs->contlist, rhs.contlist);
}

void backpatch(list_t *pl, int ip) {
    if (!*pl) {
        printf("Warnning: backpatch null");
        return;
    }
    for_list(it, *pl) {
        if (code[*it].f != jmp && code[*it].f != jpc)
            fatal("backpatch non jump instruction\n");
        code[*it].a = ip;
    }
    del_list(*pl);
    *pl = NULL;
}

void fill_symbol(list_t *pl, enum sym_type t) {
    if (!*pl) {
        printf("Warnning: backpatch null");
        return;
    }
    for_list(it, *pl) {
        if (table[*it].kind != e_var)
            fatal("backpatch non var symbol\n");
        table[*it].e_var.type = t;
        if (table[*it].e_var.is_arr) {
            table[*it].e_var.addr = addr;
            addr += table[*it].e_var.arrlen;
        } else 
            table[*it].e_var.addr = addr++;
    }
    del_list(*pl);
    *pl = NULL;
}

int get_local_size() {
    return addr;
}

char* fname;

int listswitch = 1;  /* 显示虚拟机代码与否 */
int tableswitch = 1; /* 显示符号表与否 */

int err = 0;

#define sappendf(buf, ...) sprintf(buf + strlen(buf), __VA_ARGS__)

/* 输出所有目标代码  */
void listall() {
    int i;
    char name[][5] = {
        {"nop"},
        {"lit"},
        {"opr"},
        {"lod"},
        {"sto"},
        {"cal"},
        {"int"},
        {"jmp"},
        {"jpc"},
    };
    char comment[][5] = {
        {"ret"},   // 0
        {"neg"},   // 1
        {"+"},     // 2
        {"-"},     // 3
        {"*"},     // 4
        {"/"},     // 5
        {"odd"},   // 6
        {"!"},     // 7
        {"=="},    // 8
        {"!="},    // 9
        {"<"},     // 10
        {">="},    // 11
        {">"},     // 12
        {"<="},    // 13
        {"writ"},  // 14
        {"\\n"},   // 15
        {"read"},  // 16
        {"&&"},    // 17
        {"||"},    // 18
    };
    if (listswitch) {
        char buf[100];
        puts("===code section===");
        for (i = cxmin; i < cx; i++) {
            buf[0] = '\0';
            sappendf(buf, "%d %s %d %d", i, name[code[i].f], code[i].l, code[i].a);
            if (code[i].f == opr)
                sappendf(buf, "  # %s\n", comment[code[i].a]);
            else
                sappendf(buf, "\n");
            printf("%s", buf);
            fprintf(fcode, "%s", buf);
        }
        puts("===code end===");
    }
}

/* 输出符号表 */
void displaytable() {
    char sym_type_name[][5] = {
        {"?"},
        {"char"},
        {"int"},
        {"bool"}
    };
    if (tableswitch) { /* 输出符号表 */
        char buf[500];
        puts("===symbol table===");
        for (int i = 0; i < get_table_size(); i++) {
            switch (table[i].kind) {
                case e_var:
                    buf[0] = '\0';
                    sappendf(buf, "%s %d", table[i].name, table[i].level);
                    sappendf(buf, " %s", sym_type_name[table[i].e_var.type]);
                    sappendf(buf, " %d", table[i].e_var.addr);
                    sappendf(buf, " %d", table[i].e_var.is_arr);
                    if (table[i].e_var.is_arr) 
                        sappendf(buf, " %d", table[i].e_var.arrlen);
                    else
                        sappendf(buf, " null");
                    sappendf(buf, "\n");
                    printf("%s", buf);
                    fprintf(ftable, "%s", buf);
                    break;
            }
        }
        puts("===symbol table end===");
    }
}

void redirectInput(FILE* file);

int main(int argc, char* argv[]) {
    fname = argv[1];

    if ((fin = fopen(fname, "r")) == NULL) {
        perror("fopen");
        fatal("Can't open the input file");
    }
    if ((foutput = fopen("foutput.txt", "w")) == NULL) {
        perror("fopen");
        fatal("Can't open foutput.txt");
    }
    if ((ftable = fopen("ftable.txt", "w")) == NULL) {
        perror("fopen");
        fatal("Can't open ftable.txt");
    }
    /* if ((fresult = fopen("fresult.txt", "w")) == NULL) {
        perror("fopen");
        fatal("Can't open fresult.txt");
    } */

    redirectInput(fin);
    init_X0();
    yyparse();
    if (err == 0) {
        printf("\n===Parsing success!===\n");
        fprintf(foutput, "\n===Parsing success!===\n");
        if ((fcode = fopen("fcode.txt", "w")) == NULL) {
            perror("fopen");
            fatal("Can't open fcode.txt");
        }

        if ((fresult = fopen("fresult.txt", "w")) == NULL) {
            perror("fopen");
            fatal("Can't open fresult.txt");
        }

        listall(); /* 输出所有代码 */
        fclose(fcode);

        displaytable();
        fclose(ftable);

        interpret(); /* 调用解释执行程序 */
        fclose(fresult);
    } else {
        printf("%d errors in PL/0 program\n", err);
        fprintf(foutput, "%d errors in PL/0 program\n", err);
    }

    fclose(foutput);
    fclose(fin);
    return 0;
}