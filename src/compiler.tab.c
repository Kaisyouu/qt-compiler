/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "compiler.y"

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


#line 229 "compiler.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_COMPILER_TAB_H_INCLUDED
# define YY_YY_COMPILER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_IF = 258,
    T_ELSE = 259,
    T_WHILE = 260,
    T_DO = 261,
    T_BREAK = 262,
    T_CONTINUE = 263,
    T_MAIN = 264,
    T_READ = 265,
    T_WRITE = 266,
    T_TRUE = 267,
    T_FALSE = 268,
    T_INT = 269,
    T_CHAR = 270,
    T_BOOL = 271,
    P_IFX = 272,
    T_OR = 273,
    T_AND = 274,
    T_EQ = 275,
    T_NE = 276,
    T_LE = 277,
    T_GE = 278,
    T_NUM = 279,
    T_IDENT = 280
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 160 "compiler.y"

    int number;
    char *string;
    a_statjump_t statjump;
    a_purejump_t purejump;
    list_t symlist;
    enum sym_type type;
    a_lval_t lval;

#line 317 "compiler.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_COMPILER_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   350

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  58
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  117

#define YYUNDEFTOK  2
#define YYMAXUTOK   280


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    31,     2,     2,
      40,    41,    29,    27,    37,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    36,
      23,    18,    24,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    19,    20,    21,    22,    25,    26,    32,
      33
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   201,   201,   207,   208,   212,   218,   219,   220,   224,
     225,   233,   243,   258,   262,   266,   267,   268,   269,   270,
     271,   272,   276,   277,   281,   282,   286,   292,   303,   312,
     323,   327,   334,   341,   347,   348,   349,   350,   359,   370,
     371,   372,   373,   374,   375,   377,   378,   379,   380,   381,
     382,   384,   385,   386,   393,   397,   401,   411,   421
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_IF", "T_ELSE", "T_WHILE", "T_DO",
  "T_BREAK", "T_CONTINUE", "T_MAIN", "T_READ", "T_WRITE", "T_TRUE",
  "T_FALSE", "T_INT", "T_CHAR", "T_BOOL", "P_IFX", "'='", "T_OR", "T_AND",
  "T_EQ", "T_NE", "'<'", "'>'", "T_LE", "T_GE", "'+'", "'-'", "'*'", "'/'",
  "'%'", "T_NUM", "T_IDENT", "'{'", "'}'", "';'", "','", "'['", "']'",
  "'('", "')'", "$accept", "program", "decl_list", "decl_stat",
  "decl_spec", "init_decl_list", "init_decl", "stat_list", "stat",
  "comp_stat", "expr_stat", "sele_stat", "iter_stat", "jump_stat",
  "read_stat", "write_stat", "expr", "h_ini", "h_get_loc", "h_jmp",
  "h_jmp_zero", "h_jmp_not_zero", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,    61,   273,
     274,   275,   276,    60,    62,   277,   278,    43,    45,    42,
      47,    37,   279,   280,   123,   125,    59,    44,    91,    93,
      40,    41
};
# endif

#define YYPACT_NINF (-32)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       9,   -31,    20,    -9,   -32,   -32,   -32,   -32,    -9,   -32,
     -12,   -32,   141,   -11,    -4,   -32,    12,   -32,   -32,   -32,
     -32,    -5,    18,   -32,   -32,   -32,    11,    58,   -32,    18,
      94,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   -32,   258,
       2,   -32,   -12,    18,    14,   141,    19,    21,    23,   277,
      18,   -32,   105,   137,   -32,   -32,    18,    18,    18,    18,
      18,    18,    18,    18,    18,    18,    18,    18,    18,    18,
     -32,    17,   -32,   164,    18,    55,   -32,   -32,   -32,   -32,
     236,   -32,   -32,   296,   308,   319,   -14,   -14,    92,    92,
      92,    92,    -6,    -6,   -32,   -32,   -32,   -32,   -32,   188,
     -32,   -32,   141,   -32,    22,    63,   141,    18,   -32,   -32,
     212,   141,   -32,   -32,   -32,    36,   -32
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     6,     7,     8,    54,     4,
       0,     3,     0,    11,     0,     9,     0,    55,    55,    56,
      56,     0,     0,    35,    36,    34,    37,     0,    24,     0,
       0,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     5,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,     0,     2,    13,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,    10,     0,     0,     0,    30,    31,    32,    33,
       0,    23,    39,    53,    52,    51,    49,    50,    45,    46,
      47,    48,    42,    43,    40,    41,    44,    12,    57,     0,
      55,    38,     0,    57,     0,    26,     0,     0,    56,    56,
       0,     0,    28,    58,    27,     0,    29
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -32,   -32,   -32,    65,   -32,   -32,    32,    48,   -26,   -32,
     -32,   -32,   -32,   -32,   -32,   -32,   -21,   -32,   -16,   -20,
     -25,   -32
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     8,     9,    10,    14,    15,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    12,    44,    46,
     102,   115
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      47,    49,    45,     3,    55,     5,     6,     7,    53,    61,
      62,    63,    64,    65,    66,    67,    68,    69,     1,    75,
       4,    13,    73,    67,    68,    69,    55,    40,    48,    80,
      23,    24,    41,    42,    71,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    50,
      25,    26,    43,    99,    74,    76,    97,    77,    29,    78,
     100,    16,   107,    17,    18,    19,    20,   108,    21,    22,
      23,    24,   116,    11,    72,    52,   105,     0,   106,     0,
     109,     0,     0,     0,   104,   114,   110,     0,   111,   112,
      25,    26,    27,    51,    28,     0,     0,    16,    29,    17,
      18,    19,    20,     0,    21,    22,    23,    24,    16,     0,
      17,    18,    19,    20,     0,    21,    22,    23,    24,    65,
      66,    67,    68,    69,     0,     0,    25,    26,    27,    54,
      28,     0,     0,     0,    29,     0,     0,    25,    26,    27,
      81,    28,     0,     0,    16,    29,    17,    18,    19,    20,
       0,    21,    22,    23,    24,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,     0,
       0,     0,     0,    25,    26,    27,     0,    28,    82,     0,
       0,    29,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    98,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   103,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   113,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,     0,     0,
       0,     0,     0,     0,     0,   101,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
       0,     0,     0,     0,    70,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,     0,
       0,     0,     0,    79,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69
};

static const yytype_int8 yycheck[] =
{
      20,    22,    18,    34,    30,    14,    15,    16,    29,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     9,    45,
       0,    33,    43,    29,    30,    31,    52,    38,    33,    50,
      12,    13,    36,    37,    32,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    38,
      32,    33,    40,    74,    40,    36,    39,    36,    40,    36,
       5,     3,    40,     5,     6,     7,     8,     4,    10,    11,
      12,    13,    36,     8,    42,    27,   102,    -1,   103,    -1,
     106,    -1,    -1,    -1,   100,   111,   107,    -1,   108,   109,
      32,    33,    34,    35,    36,    -1,    -1,     3,    40,     5,
       6,     7,     8,    -1,    10,    11,    12,    13,     3,    -1,
       5,     6,     7,     8,    -1,    10,    11,    12,    13,    27,
      28,    29,    30,    31,    -1,    -1,    32,    33,    34,    35,
      36,    -1,    -1,    -1,    40,    -1,    -1,    32,    33,    34,
      35,    36,    -1,    -1,     3,    40,     5,     6,     7,     8,
      -1,    10,    11,    12,    13,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    32,    33,    34,    -1,    36,    41,    -1,
      -1,    40,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    41,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    41,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    -1,    36,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    36,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    43,    34,     0,    14,    15,    16,    44,    45,
      46,    45,    59,    33,    47,    48,     3,     5,     6,     7,
       8,    10,    11,    12,    13,    32,    33,    34,    36,    40,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      38,    36,    37,    40,    60,    60,    61,    61,    33,    58,
      38,    35,    49,    58,    35,    50,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      36,    32,    48,    58,    40,    50,    36,    36,    36,    36,
      58,    35,    41,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    39,    41,    58,
       5,    39,    62,    41,    60,    50,    62,    40,     4,    50,
      58,    61,    61,    41,    50,    63,    36
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    42,    43,    44,    44,    45,    46,    46,    46,    47,
      47,    48,    48,    49,    49,    50,    50,    50,    50,    50,
      50,    50,    51,    51,    52,    52,    53,    53,    54,    54,
      55,    55,    56,    57,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    59,    60,    61,    62,    63
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     6,     2,     1,     3,     1,     1,     1,     1,
       3,     1,     4,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     1,     2,     6,     9,     8,    10,
       3,     3,     3,     3,     1,     1,     1,     1,     4,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     0,     0,     0,     0,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 201 "compiler.y"
                                               {
        cgjmp(0);
    }
#line 1618 "compiler.tab.c"
    break;

  case 3:
#line 207 "compiler.y"
                            {;}
#line 1624 "compiler.tab.c"
    break;

  case 4:
#line 208 "compiler.y"
                            {;}
#line 1630 "compiler.tab.c"
    break;

  case 5:
#line 212 "compiler.y"
                                   {
        fill_symbol(&(yyvsp[-1].symlist), (yyvsp[-2].type));
    }
#line 1638 "compiler.tab.c"
    break;

  case 6:
#line 218 "compiler.y"
                { (yyval.type) = sym_int; }
#line 1644 "compiler.tab.c"
    break;

  case 7:
#line 219 "compiler.y"
                { (yyval.type) = sym_char; }
#line 1650 "compiler.tab.c"
    break;

  case 8:
#line 220 "compiler.y"
                { (yyval.type) = sym_bool; }
#line 1656 "compiler.tab.c"
    break;

  case 9:
#line 224 "compiler.y"
                { (yyval.symlist) = (yyvsp[0].symlist); }
#line 1662 "compiler.tab.c"
    break;

  case 10:
#line 225 "compiler.y"
                                    {
        //   1        2      3
        (yyval.symlist) = (yyvsp[-2].symlist);
        list_add(&(yyval.symlist), (yyvsp[0].symlist));
    }
#line 1672 "compiler.tab.c"
    break;

  case 11:
#line 233 "compiler.y"
              {
        STE ste = find_symbol((yyvsp[0].string));
        if (ste)
            fatal("reapeat definition");
        insert_symbol((yyvsp[0].string), e_var);
        free((yyvsp[0].string));

        (yyval.symlist) = NULL;
        list_append(&(yyval.symlist), get_table_size() - 1);
    }
#line 1687 "compiler.tab.c"
    break;

  case 12:
#line 243 "compiler.y"
                            {
        STE ste = find_symbol((yyvsp[-3].string));
        if (ste)
            fatal("reapeat definition");
        ste = insert_symbol((yyvsp[-3].string), e_var);
        free((yyvsp[-3].string));
        ste->e_var.is_arr = 1;
        ste->e_var.arrlen = (yyvsp[-1].number);

        (yyval.symlist) = NULL;
        list_append(&(yyval.symlist), get_table_size() - 1);

    }
#line 1705 "compiler.tab.c"
    break;

  case 13:
#line 258 "compiler.y"
                     {
        (yyval.statjump) = (yyvsp[-1].statjump);
        statjump_add(&(yyval.statjump), (yyvsp[0].statjump));
    }
#line 1714 "compiler.tab.c"
    break;

  case 14:
#line 262 "compiler.y"
            { (yyval.statjump) = (yyvsp[0].statjump); }
#line 1720 "compiler.tab.c"
    break;

  case 22:
#line 276 "compiler.y"
                        { (yyval.statjump) = A_STATJUMP_ZERO; }
#line 1726 "compiler.tab.c"
    break;

  case 23:
#line 277 "compiler.y"
                        { (yyval.statjump) = (yyvsp[-1].statjump); }
#line 1732 "compiler.tab.c"
    break;

  case 24:
#line 281 "compiler.y"
                { (yyval.statjump) = A_STATJUMP_ZERO; }
#line 1738 "compiler.tab.c"
    break;

  case 25:
#line 282 "compiler.y"
                { (yyval.statjump) = A_STATJUMP_ZERO; }
#line 1744 "compiler.tab.c"
    break;

  case 26:
#line 286 "compiler.y"
                                                    {
        int next_ip = getip();
        (yyval.statjump) = (yyvsp[0].statjump);

        backpatch(&(yyvsp[-1].purejump).nextlist, next_ip);
    }
#line 1755 "compiler.tab.c"
    break;

  case 27:
#line 292 "compiler.y"
                                                          {
        int next_ip = getip();
        (yyval.statjump) = (yyvsp[-3].statjump);
        statjump_add(&(yyval.statjump), (yyvsp[0].statjump));

        backpatch(&(yyvsp[-1].purejump).nextlist, next_ip);
        backpatch(&(yyvsp[-4].purejump).nextlist, (yyvsp[-1].purejump).next_ip);
    }
#line 1768 "compiler.tab.c"
    break;

  case 28:
#line 303 "compiler.y"
                                                           {
        int next_ip = getip();
        (yyval.statjump) = A_STATJUMP_ZERO;

        backpatch(&(yyvsp[-2].purejump).nextlist, next_ip);
        backpatch(&(yyvsp[0].purejump).nextlist, (yyvsp[-6].number));
        backpatch(&(yyvsp[-1].statjump).brklist, next_ip);
        backpatch(&(yyvsp[-1].statjump).contlist, (yyvsp[-6].number));
    }
#line 1782 "compiler.tab.c"
    break;

  case 29:
#line 312 "compiler.y"
                                                                            {
        int next_ip = getip();
        (yyval.statjump) = A_STATJUMP_ZERO;

        backpatch(&(yyvsp[-1].purejump).nextlist, (yyvsp[-8].number));
        backpatch(&(yyvsp[-7].statjump).brklist, next_ip);
        backpatch(&(yyvsp[-7].statjump).contlist, (yyvsp[-5].number));
    }
#line 1795 "compiler.tab.c"
    break;

  case 30:
#line 323 "compiler.y"
                        {
        (yyval.statjump).brklist = (yyvsp[-1].purejump).nextlist;
        (yyval.statjump).contlist = NULL;
    }
#line 1804 "compiler.tab.c"
    break;

  case 31:
#line 327 "compiler.y"
                           {
        (yyval.statjump).brklist = NULL;
        (yyval.statjump).contlist = (yyvsp[-1].purejump).nextlist;
    }
#line 1813 "compiler.tab.c"
    break;

  case 32:
#line 334 "compiler.y"
                         {
        (yyval.statjump) = A_STATJUMP_ZERO;
        free((yyvsp[-1].string));
    }
#line 1822 "compiler.tab.c"
    break;

  case 33:
#line 341 "compiler.y"
                       {
        (yyval.statjump) = A_STATJUMP_ZERO;
        cgwrite();
    }
#line 1831 "compiler.tab.c"
    break;

  case 34:
#line 347 "compiler.y"
                { cglit((yyvsp[0].number)); }
#line 1837 "compiler.tab.c"
    break;

  case 35:
#line 348 "compiler.y"
                { cglit(1); }
#line 1843 "compiler.tab.c"
    break;

  case 36:
#line 349 "compiler.y"
                { cglit(0); }
#line 1849 "compiler.tab.c"
    break;

  case 37:
#line 350 "compiler.y"
              {
        STE ste = find_symbol((yyvsp[0].string));
        free((yyvsp[0].string));
        if (!ste) fatal("ident not found");
        (yyval.lval).level = ste->level;
        (yyval.lval).addr = ste->e_var.addr;
        (yyval.lval).is_arr = 0;
        cglod((yyval.lval).level, (yyval.lval).addr);
    }
#line 1863 "compiler.tab.c"
    break;

  case 38:
#line 359 "compiler.y"
                           {
        STE ste = find_symbol((yyvsp[-3].string));
        free((yyvsp[-3].string));
        if (!ste) fatal("ident not found");
        (yyval.lval).level = ste->level;
        (yyval.lval).addr = ste->e_var.addr;
        (yyval.lval).is_arr = 1;
        cglit((yyval.lval).addr);
        cgadd();
        cglodoff((yyval.lval).level);
    }
#line 1879 "compiler.tab.c"
    break;

  case 39:
#line 370 "compiler.y"
                        { }
#line 1885 "compiler.tab.c"
    break;

  case 40:
#line 371 "compiler.y"
                        { cgmul(); }
#line 1891 "compiler.tab.c"
    break;

  case 41:
#line 372 "compiler.y"
                        { cgdiv(); }
#line 1897 "compiler.tab.c"
    break;

  case 42:
#line 373 "compiler.y"
                        { cgadd(); }
#line 1903 "compiler.tab.c"
    break;

  case 43:
#line 374 "compiler.y"
                        { cgsub(); }
#line 1909 "compiler.tab.c"
    break;

  case 44:
#line 375 "compiler.y"
                        { cgmod(); }
#line 1915 "compiler.tab.c"
    break;

  case 45:
#line 377 "compiler.y"
                        { cglt(); }
#line 1921 "compiler.tab.c"
    break;

  case 46:
#line 378 "compiler.y"
                        { cggt(); }
#line 1927 "compiler.tab.c"
    break;

  case 47:
#line 379 "compiler.y"
                        { cgle(); }
#line 1933 "compiler.tab.c"
    break;

  case 48:
#line 380 "compiler.y"
                        { cgge(); }
#line 1939 "compiler.tab.c"
    break;

  case 49:
#line 381 "compiler.y"
                        { cgeq(); }
#line 1945 "compiler.tab.c"
    break;

  case 50:
#line 382 "compiler.y"
                        { cgne(); }
#line 1951 "compiler.tab.c"
    break;

  case 51:
#line 384 "compiler.y"
                        { cgland(); }
#line 1957 "compiler.tab.c"
    break;

  case 52:
#line 385 "compiler.y"
                        { cglor(); }
#line 1963 "compiler.tab.c"
    break;

  case 53:
#line 386 "compiler.y"
                        {
        if ((yyvsp[-2].lval).is_arr) cgstooff((yyvsp[-2].lval).level);
        else cgsto((yyvsp[-2].lval).level, (yyvsp[-2].lval).addr);
    }
#line 1972 "compiler.tab.c"
    break;

  case 54:
#line 393 "compiler.y"
      { cgstack(get_local_size()); }
#line 1978 "compiler.tab.c"
    break;

  case 55:
#line 397 "compiler.y"
      { (yyval.number) = getip(); }
#line 1984 "compiler.tab.c"
    break;

  case 56:
#line 401 "compiler.y"
      {
        (yyval.purejump).nextlist = new_list();
        list_append(&(yyval.purejump).nextlist, getip());
        cgjmp(0);

        (yyval.purejump).next_ip = getip();
    }
#line 1996 "compiler.tab.c"
    break;

  case 57:
#line 411 "compiler.y"
      {
        (yyval.purejump).nextlist = new_list();
        list_append(&(yyval.purejump).nextlist, getip());
        cgjpc(0);

        (yyval.purejump).next_ip = getip();
    }
#line 2008 "compiler.tab.c"
    break;

  case 58:
#line 421 "compiler.y"
      {
        cglnot();
        (yyval.purejump).nextlist = new_list();
        list_append(&(yyval.purejump).nextlist, getip());
        cgjpc(0);

        (yyval.purejump).next_ip = getip();
    }
#line 2021 "compiler.tab.c"
    break;


#line 2025 "compiler.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 430 "compiler.y"


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
