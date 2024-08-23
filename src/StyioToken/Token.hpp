#pragma once
#ifndef STYIO_TOKEN_H_
#define STYIO_TOKEN_H_

#include <string>
#include <unordered_map>
#include <vector>

enum class StyioDataTypeOption
{
  Undefined,
  Defined,

  Bool,  // Boolean
  Integer,
  Float,
  Decimal,

  Char,  // Character
  String,

  Tuple,
  List,

  Struct,

  Func,  // Function
};

struct StyioDataType
{
  StyioDataTypeOption option;
  std::string name;
  size_t num_of_bit = 0;

  bool isUndefined() {
    return option == StyioDataTypeOption::Undefined;
  }

  bool isInteger() {
    return option == StyioDataTypeOption::Integer;
  }

  bool isFloat() {
    return option == StyioDataTypeOption::Float;
  }

  bool equals(const StyioDataType other) const {
    return option == other.option and name == other.name;
  }
};

static std::unordered_map<std::string, StyioDataType> const DType_Table = {
  {"bool", StyioDataType{StyioDataTypeOption::Bool, "bool", 1}},

  {"int", StyioDataType{StyioDataTypeOption::Integer, "i32", 32}},
  {"long", StyioDataType{StyioDataTypeOption::Integer, "i64", 64}},

  {"i1", StyioDataType{StyioDataTypeOption::Integer, "i1", 1}},
  {"i8", StyioDataType{StyioDataTypeOption::Integer, "i8", 8}},
  {"i16", StyioDataType{StyioDataTypeOption::Integer, "i16", 16}},
  {"i32", StyioDataType{StyioDataTypeOption::Integer, "i32", 32}},
  {"i64", StyioDataType{StyioDataTypeOption::Integer, "i64", 64}},
  {"i128", StyioDataType{StyioDataTypeOption::Integer, "i128", 128}},

  {"float", StyioDataType{StyioDataTypeOption::Float, "f32", 32}},
  {"double", StyioDataType{StyioDataTypeOption::Float, "f64", 64}},

  {"f32", StyioDataType{StyioDataTypeOption::Float, "f64", 32}},
  {"f64", StyioDataType{StyioDataTypeOption::Float, "f64", 64}},

  {"char", StyioDataType{StyioDataTypeOption::Char, "char", 0}},

  {"string", StyioDataType{StyioDataTypeOption::String, "string", 0}},
  {"str", StyioDataType{StyioDataTypeOption::String, "string", 0}},
};

StyioDataType getMaxType(StyioDataType T1, StyioDataType T2);

std::string reprDataTypeOption(StyioDataTypeOption option);

enum class StyioOpType
{
  Undefined,            // Undefined
  End_Of_File,          // EOF
  Unary_Positive,       // + a
  Unary_Negative,       // - a
  Binary_Add,           // a + b
  Binary_Sub,           // a - b
  Binary_Mul,           // a * b
  Binary_Div,           // a / b
  Binary_Pow,           // a ** b
  Binary_Mod,           // a % b
  Greater_Than,         // a > b
  Less_Than,            // a < b
  Greater_Than_Equal,   // a >= b
  Less_Than_Equal,      // a <= b
  Self_Add_Assign,      // a += b
  Self_Sub_Assign,      // a -= b
  Self_Mul_Assign,      // a *= b
  Self_Div_Assign,      // a /= b
  Bitwise_NOT,          // ~ a
  Bitwise_AND,          // a & b
  Bitwise_OR,           // a | b
  Bitwise_XOR,          // a ^ b
  Bitwise_Left_Shift,   // shl(x, y)
  Bitwise_Right_Shift,  // shr(x, y)
  Logic_NOT,            // ! a
  Logic_AND,            // a && b
  Logic_XOR,            // a ⊕ b
  Logic_OR,             // a || b
  Equal,                // a == b
  Not_Equal,            // a != b
  If_Else_Flow,         // ?() => a : b

  Iterate,  // a >> b
  Extract,  // a << b
  Next,     // a => b

  Comment_SingleLine,  // // Like This
  Comment_MultiLine,   // /* Like This */
};

/* Token Precedence Map */
static std::unordered_map<StyioOpType, int> const TokenPrecedenceMap = {
  {StyioOpType::Unary_Positive, 999},  // + a
  {StyioOpType::Unary_Negative, 999},  // - a
  {StyioOpType::Bitwise_NOT, 999},     // ~ a
  {StyioOpType::Logic_NOT, 999},       // ! a

  {StyioOpType::Binary_Pow, 704},  // a ** b

  {StyioOpType::Binary_Mul, 703},  // a * b
  {StyioOpType::Binary_Div, 703},  // a / b
  {StyioOpType::Binary_Mod, 703},  // a % b

  {StyioOpType::Binary_Add, 702},  // a + b
  {StyioOpType::Binary_Sub, 702},  // a - b

  {StyioOpType::Bitwise_Left_Shift, 701},   // shl(x, y)
  {StyioOpType::Bitwise_Right_Shift, 701},  // shr(x, y)

  {StyioOpType::Greater_Than, 502},        // a > b
  {StyioOpType::Less_Than, 502},           // a < b
  {StyioOpType::Greater_Than_Equal, 502},  // a >= b
  {StyioOpType::Less_Than_Equal, 502},     // a <= b

  {StyioOpType::Equal, 501},      // a == b
  {StyioOpType::Not_Equal, 501},  // a != b

  {StyioOpType::Bitwise_AND, 303},  // a & b
  {StyioOpType::Bitwise_XOR, 302},  // a ^ b
  {StyioOpType::Bitwise_OR, 301},   // a | b

  {StyioOpType::Logic_AND, 203},  // a && b
  {StyioOpType::Logic_XOR, 202},  // a ⊕ b
  {StyioOpType::Logic_OR, 201},   // a || b

  {StyioOpType::If_Else_Flow, 101},  // ?() => a : b

  {StyioOpType::Self_Add_Assign, 1},  // a += b
  {StyioOpType::Self_Sub_Assign, 1},  // a -= b
  {StyioOpType::Self_Mul_Assign, 1},  // a *= b
  {StyioOpType::Self_Div_Assign, 1},  // a /= b

  {StyioOpType::Undefined, 0},    // Undefined
  {StyioOpType::End_Of_File, 0},  // Undefined
};

static std::unordered_map<StyioOpType, std::string> const TokenStrMap = {
  {StyioOpType::Undefined, "undefined"},  // undefined
  {StyioOpType::End_Of_File, "EOF"},      // EOF

  {StyioOpType::Binary_Pow, "**"},  // a ** b

  {StyioOpType::Binary_Mul, "*"},  // a * b
  {StyioOpType::Binary_Div, "/"},  // a / b
  {StyioOpType::Binary_Mod, "%"},  // a % b

  {StyioOpType::Binary_Add, "+"},  // a + b
  {StyioOpType::Binary_Sub, "-"},  // a - b

  {StyioOpType::Greater_Than, ">"},         // a > b
  {StyioOpType::Less_Than, "<"},            // a < b
  {StyioOpType::Greater_Than_Equal, ">="},  // a >= b
  {StyioOpType::Less_Than_Equal, "<="},     // a <= b

  {StyioOpType::Equal, "=="},      // a == b
  {StyioOpType::Not_Equal, "!="},  // a != b

  {StyioOpType::Bitwise_AND, "&"},  // a & b
  {StyioOpType::Bitwise_XOR, "^"},  // a ^ b
  {StyioOpType::Bitwise_OR, "|"},   // a | b

  {StyioOpType::Logic_AND, "&&"},  // a && b
  {StyioOpType::Logic_XOR, "⊕"},   // a ⊕ b
  {StyioOpType::Logic_OR, "||"},   // a || b

  {StyioOpType::Self_Add_Assign, "+="},  // a += b
  {StyioOpType::Self_Sub_Assign, "-="},  // a -= b
  {StyioOpType::Self_Mul_Assign, "*="},  // a *= b
  {StyioOpType::Self_Div_Assign, "/="},  // a /= b
};

static std::unordered_map<std::string, StyioOpType> const StrTokenMap = {
  {"", StyioOpType::Undefined},       // Undefined
  {"EOF", StyioOpType::End_Of_File},  // EOF

  {"**", StyioOpType::Binary_Pow},  // a ** b

  {"*", StyioOpType::Binary_Mul},  // a * b
  {"/", StyioOpType::Binary_Div},  // a / b
  {"%", StyioOpType::Binary_Mod},  // a % b

  {"+", StyioOpType::Binary_Add},  // a + b
  {"-", StyioOpType::Binary_Sub},  // a - b

  {">", StyioOpType::Greater_Than},         // a > b
  {"<", StyioOpType::Less_Than},            // a < b
  {">=", StyioOpType::Greater_Than_Equal},  // a >= b
  {"<=", StyioOpType::Less_Than_Equal},     // a <= b

  {"==", StyioOpType::Equal},      // a == b
  {"!=", StyioOpType::Not_Equal},  // a != b

  {"&", StyioOpType::Bitwise_AND},  // a & b
  {"^", StyioOpType::Bitwise_XOR},  // a ^ b
  {"|", StyioOpType::Bitwise_OR},   // a | b

  {"&&", StyioOpType::Logic_AND},  // a && b
  {"⊕", StyioOpType::Logic_XOR},   // a ⊕ b
  {"||", StyioOpType::Logic_OR},   // a || b

  {"+=", StyioOpType::Self_Add_Assign},  // a += b
  {"-=", StyioOpType::Self_Sub_Assign},  // a -= b
  {"*=", StyioOpType::Self_Mul_Assign},  // a *= b
  {"/=", StyioOpType::Self_Div_Assign},  // a /= b
};

enum class StyioContextType
{
};

enum class StyioPathType
{
  local_absolute_unix_like,
  local_absolute_windows,
  local_relevant_any,

  ipv4_addr,
  ipv6_addr,

  url_localhost,
  url_http,
  url_https,
  url_ftp,

  db_mysql,
  db_postgresql,
  db_mongo,

  remote_windows
};

enum class StyioASTType
{
  End,
  Pass,
  Break,
  Return,
  Comment,

  Naive,

  True,
  False,

  /* -----------------
   * None, Null, Empty
   */
  None,
  Empty,
  EmptyBlock,

  // -----------------

  /* -----------------
   * Basic Type
   */

  // Identifier: [a-zA-Z0-9_]
  Id,
  DType,
  TypedVar,
  OptArg,
  OptKwArg,
  Variable,
  Arg,

  Bool,
  // Integer (General)
  Integer,
  // Float (General)
  Float,
  // Character: '<Any Single Character>'
  Char,

  NumConvert,

  // Binary Operation
  BinOp,

  // -----------------

  /* -----------------
   * External Resource Identifier
   */

  // File Path
  LocalPath,
  RemotePath,
  WebUrl,
  DBUrl,

  // Package
  ExtPack,

  // -----------------

  /* -----------------
   * Collection
   */

  // ""
  String,
  // $""
  FmtStr,
  // [a0, a1, ..., an]
  List,
  Tuple,
  Set,
  // [start .. end]
  Range,

  // -----------------

  /* -----------------
   * Basic Operation
   */

  // Not
  Not,

  // Compare
  Compare,

  // Condition
  Condition,

  // Call
  Call,

  // Attribute
  Attribute,

  // Conditionals
  CondFlow_True,
  CondFlow_False,
  CondFlow_Both,

  // List Operation
  Access,           // [id]
  Access_By_Index,  // [index]
  Access_By_Name,   // ["name"]

  Get_Index_By_Value,          // [?= value]
  Get_Indices_By_Many_Values,  // [?^ values]

  Append_Value,          // [+: value]
  Insert_Item_By_Index,  // [+: index <- value]

  Remove_Last_Item,              // [-: ^-1]
  Remove_Item_By_Index,          // [-: index]
  Remove_Items_By_Many_Indices,  // [-: (i0, i1, ...)]
  Remove_Item_By_Value,          // [-: ?= value]
  Remove_Items_By_Many_Values,   // [-: ?^ (v0, v1, ...)]

  Get_Reversed,                  // [<]
  Get_Index_By_Item_From_Right,  // [[<] ?= value]
  // -----------------

  /* -----------------
   * Basic Util
   */

  // Get the Size / Length / .. of A Collection
  SizeOf,
  // -----------------

  /* -----------------
   * Variable Definition
   */

  // @
  Resources,
  // -----------------

  /* -----------------
   * Variable Assignment
   */

  // =
  MutBind,
  // :=
  FinalBind,
  // -----------------

  /* -----------------
   * Pipeline
   */

  Func,
  AnonyFunc,
  MatchCases,
  Struct,
  Eval,
  // -----------------

  /* -----------------
   * Control Flow: Loop
   */
  Infinite,
  // -----------------

  /* -----------------
   * Iterator
   */
  Loop,
  Iterator,
  // -----------------

  /* -----------------
   * Combination
   */
  IterWithMatch,
  // -----------------

  /* -----------------
   * Read
   */

  ReadFile,
  // -----------------

  /* -----------------
   * Write
   */

  Print,
  // -----------------

  /* -----------------
   * Layers
   */
  // (x, y, ...)
  VarTuple,
  // ?=
  CheckEq,
  // ?^
  CheckIsin,
  // ?()
  CheckCond,

  // Intermediate Connection Between Scopes
  Forward,
  If_Equal_To_Forward,
  If_Is_In_Forward,
  Cases_Forward,
  If_True_Forward,
  If_False_Forward,
  If_Both_Forward,

  Fill_Forward,
  Fill_If_Equal_To_Forward,
  Fill_If_Is_in_Forward,
  Fill_Cases_Forward,
  Fill_If_True_Forward,
  Fill_If_False_Forward,
  Fill_If_Both_Forward,
  // -----------------

  /* -----------------
   * Backward
   */

  Backward,

  // -----------------

  /* -----------------
   * Tuple Operations
   */

  TupleOperation,

  // -----------------

  /* -----------------
   * Chain of Data Processing
   */

  Chain_Of_Data_Processing,

  // -----------------

  /* -----------------
   * Code Block
   */

  MainBlock,
  Block,
  Cases,
  // -----------------

  Connection,
  FromTo
};

enum class InfiniteType
{
  Original,
  Incremental,
};

enum class IteratorType
{
  Original,
  WithLayer,
};

enum class LogicType
{
  RAW,
  NOT,
  AND,
  OR,
  XOR,
};

enum class CompType
{
  EQ,  // == Equal
  GT,  // >  Greater Than
  GE,  // >= Greater Than and Equal
  LT,  // <  Less Than
  LE,  // <= Less Than and Equal
  NE,  // != Not Equal
};

enum class IterOverWhat
{
  /*
   * Accept: 0 [No Variable]
   */
  InfLoop,  // [...]

  /*
   * Accept: 1 [Only One Variable]
   */
  List,   // [a0, a1, ..., an]
  Range,  // [a0...an]

  /*
   * Accept: 2 [Two Variables]
   */
  Dict,  // {k0: v0, k1: v1, kn: vn}

  /*
   * Accept: n [Any]
   */
  ListOfTuples,   // [(a0, b0, ...), (a1, b1, ...), ..., (an, bn, ...)]
  ListOfStructs,  // [s0, s1, ..., sn]
};

enum class NumPromoTy
{
  Bool_To_Int,
  Int_To_Float,
};

std::string
reprASTType(StyioASTType type, std::string extra = "");

std::string
reprToken(CompType token);

std::string
reprToken(StyioOpType token);

std::string
reprToken(LogicType token);

/*
  To distinguish
    <= (less than or equal)
    from
    <= (left double arrow),
  construct a static map:
    {
      TOK_LE: (LANGLEBRAC, EQUAL),
      TOK_ARROW_DOUBLE_LEFT: (LANGLEBRAC, EQUAL)
    }
  use a function:
    check_pattern(StyioTokenType::TOK_LE)
*/
enum class StyioTokenType
{
  TOK_EOF = -1,        // EOF
  TOK_NULL = 0,        // ASCII 0 NUL
  TOK_LF = 10,         // ASCII 10 LF
  TOK_CR = 13,         // ASCII 13 CR
  TOK_SPACE = 32,      // ASCII 32 SPACE
  TOK_EXCLAM = 33,     // ASCII 33 !
  TOK_DQUOTE = 34,     // ASCII 34 "
  TOK_HASH = 35,       // ASCII 35 #
  TOK_DOLLAR = 36,     // ASCII 36 $
  TOK_PERCENT = 37,    // ASCII 37 %
  TOK_AMP = 38,        // ASCII 38 &
  TOK_SQUOTE = 39,     // ASCII 39 '
  TOK_LPAREN = 40,     // ASCII 40 (
  TOK_RPAREN = 41,     // ASCII 41 )
  TOK_STAR = 42,       // ASCII 42 *
  TOK_PLUS = 43,       // ASCII 43 +
  TOK_COMMA = 44,      // ASCII 44 ,
  TOK_MINUS = 45,      // ASCII 45 -
  TOK_DOT = 46,        // ASCII 46 .
  TOK_SLASH = 47,      // ASCII 47 / (slash)
  TOK_COLON = 58,      // ASCII 58 :
  TOK_SEMICOLON = 59,  // ASCII 59 ;
  TOK_LANGBRAC = 60,   // ASCII 60 <
  TOK_EQUAL = 61,      // ASCII 61 =
  TOK_RANGBRAC = 62,   // ASCII 62 >
  TOK_QUEST = 63,   // ASCII 63 ?
  TOK_AT = 64,         // ASCII 64 @
  TOK_LBOXBRAC = 91,   // [
  TOK_BACKSLASH = 92,  // ASCII 92 \ (backslash)
  TOK_RBOXBRAC = 93,   // ]
  TOK_HAT = 94,        // ASCII 94 ^
  TOK_UNDLINE = 95,    // ASCII 95 _
  TOK_BQUOTE = 96,     // ASCII 96 `
  TOK_LCURBRAC = 123,  // ASCII 123 {
  TOK_PIPE = 124,      // ASCII 124 |
  TOK_RCURBRAC = 125,  // ASCII 125 }
  TOK_TILDE = 126,     // ASCII 126 ~
  TOK_DEL = 127,       // ASCII 127 DEL

  TOK_NAME,
  TOK_INT,
  TOK_FLOAT,
  TOK_STRING,
  TOK_LINE_COMMENT,
  TOK_CLOSED_COMMENT,

  BINOP_BITAND,  // &
  BINOP_BITOR,   // |
  BINOP_BITXOR,  // ^

  TOK_BACKWARD,  // <<
  TOK_FORWARD,   // >>

  TOK_

  LOGIC_NOT,  // !
  LOGIC_AND,  // &&
  LOGIC_OR,   // ||
  LOGIC_XOR,  // ^

  UNARY_NEG,  // -

  BINOP_ADD,  // +
  BINOP_SUB,  // -
  BINOP_MUL,  // *
  BINOP_DIV,  // /
  BINOP_MOD,  // %
  BINOP_POW,  // **

  BINOP_GT,  // >
  BINOP_GE,  // >=
  BINOP_LT,  // <
  BINOP_LE,  // <=
  BINOP_EQ,  // ==
  BINOP_NE,  // !=

  PRINT,   // >_
  WALRUS,  // :=
  MATCH,   // ?=

  ARROW_DOUBLE_RIGHT,  // =>
  ARROW_DOUBLE_LEFT,   // <=
  ARROW_SINGLE_RIGHT,  // ->
  ARROW_SINGLE_LEFT,   // <-

  ELLIPSIS,       // ...
  INFINITE_LIST,  // [...]

  UNKNOWN,
};

class StyioToken
{
private:
  StyioToken(
    StyioTokenType token_type, 
    std::string token_literal
  ) :
      type(token_type), literal(token_literal) {
  }

public:
  StyioTokenType type;
  std::string literal;
  size_t length;

  static StyioToken* Create(StyioTokenType token_type) {
    return new StyioToken(token_type, "");
  }

  static StyioToken* Create(StyioTokenType token_type, std::string token_literal) {
    return new StyioToken(token_type, token_literal);
  }

  static std::string getTokName(StyioTokenType type) {
    switch (type) {
      case StyioTokenType::TOK_SPACE:
        return " ";

      case StyioTokenType::TOK_CR:
        return "<CR>";

      case StyioTokenType::TOK_LF:
        return "<LF>";

      case StyioTokenType::TOK_EOF:
        return "<EOF>";

      case StyioTokenType::TOK_NAME:
        return "<ID>";

      case StyioTokenType::TOK_INT:
        return "<INT>";

      case StyioTokenType::TOK_FLOAT:
        return "<FLOAT>";

      case StyioTokenType::TOK_STRING:
        return "<STRING>";

      case StyioTokenType::TOK_LINE_COMMENT:
        return "// COMMENT";

      case StyioTokenType::TOK_CLOSED_COMMENT:
        return "/* COMMENT */";

      case StyioTokenType::TOK_COMMA:
        return ",";

      case StyioTokenType::TOK_DOT:
        return ".";

      case StyioTokenType::TOK_COLON:
        return ":";

      case StyioTokenType::TOK_TILDE:
        return "~";

      case StyioTokenType::TOK_EXCLAM:
        return "!";

      case StyioTokenType::TOK_AT:
        return "@";

      case StyioTokenType::TOK_HASH:
        return "#";

      case StyioTokenType::TOK_DOLLAR:
        return "$";

      case StyioTokenType::TOK_PERCENT:
        return "%";

      case StyioTokenType::TOK_HAT:
        return "^";

      case StyioTokenType::TOK_QUEST:
        return "?";

      case StyioTokenType::TOK_SLASH:
        return "/";

      case StyioTokenType::TOK_BACKSLASH:
        return "\\";

      case StyioTokenType::TOK_PIPE:
        return "|";

      case StyioTokenType::ELLIPSIS:
        return "...";

      case StyioTokenType::TOK_SQUOTE:
        return "'";

      case StyioTokenType::TOK_DQUOTE:
        return "\"";

      case StyioTokenType::TOK_BQUOTE:
        return "`";

      case StyioTokenType::TOK_LPAREN:
        return "(";

      case StyioTokenType::TOK_RPAREN:
        return ")";

      case StyioTokenType::TOK_LBOXBRAC:
        return "[";

      case StyioTokenType::TOK_RBOXBRAC:
        return "]";

      case StyioTokenType::TOK_LCURBRAC:
        return "{";

      case StyioTokenType::TOK_RCURBRAC:
        return "}";

      case StyioTokenType::TOK_LANGBRAC:
        return "<";

      case StyioTokenType::TOK_RANGBRAC:
        return ">";

      case StyioTokenType::LOGIC_NOT:
        return "<NOT>";

      case StyioTokenType::LOGIC_AND:
        return "<AND>";

      case StyioTokenType::LOGIC_OR:
        return "<OR>";

      case StyioTokenType::LOGIC_XOR:
        return "<XOR>";

      case StyioTokenType::BINOP_BITAND:
        return "<BIT_AND>";

      case StyioTokenType::BINOP_BITOR:
        return "<BIT_OR>";

      case StyioTokenType::BINOP_BITXOR:
        return "<BIT_XOR>";

      case StyioTokenType::TOK_BACKWARD:
        return "<<";

      case StyioTokenType::TOK_FORWARD:
        return ">>";

      case StyioTokenType::PRINT:
        return ">_";

      case StyioTokenType::UNARY_NEG:
        return "<NEG>";

      case StyioTokenType::BINOP_ADD:
        return "<ADD>";

      case StyioTokenType::BINOP_SUB:
        return "<SUB>";

      case StyioTokenType::BINOP_MUL:
        return "<MUL>";

      case StyioTokenType::BINOP_DIV:
        return "<DIV>";

      case StyioTokenType::BINOP_MOD:
        return "<MOD>";

      case StyioTokenType::BINOP_POW:
        return "<POW>";

      case StyioTokenType::BINOP_GT:
        return "<GT>";

      case StyioTokenType::BINOP_GE:
        return "<GE>";

      case StyioTokenType::BINOP_LT:
        return "<LT>";

      case StyioTokenType::BINOP_LE:
        return "<LE>";

      case StyioTokenType::BINOP_EQ:
        return "<EQ>";

      case StyioTokenType::BINOP_NE:
        return "<NE>";

      case StyioTokenType::ARROW_SINGLE_RIGHT:
        return "->";

      case StyioTokenType::ARROW_SINGLE_LEFT:
        return "<-";

      case StyioTokenType::WALRUS:
        return ":=";

      case StyioTokenType::MATCH:
        return "?=";

      case StyioTokenType::INFINITE_LIST:
        return "[...]";

      default:
        return "<UNKNOWN>";
    }
  };

  size_t length() {
    switch (type) {
      case StyioTokenType::TOK_SPACE:
        return 1;

      case StyioTokenType::TOK_CR:
        return 1;

      case StyioTokenType::TOK_LF:
        return 1;

      case StyioTokenType::TOK_EOF:
        return 1;

      case StyioTokenType::TOK_NAME:
        return literal.length();

      case StyioTokenType::TOK_INT:
        return literal.length();

      case StyioTokenType::TOK_FLOAT:
        return literal.length();

      case StyioTokenType::TOK_STRING:
        return literal.length() + 2;

      case StyioTokenType::TOK_LINE_COMMENT:
        return literal.length() + 2;

      case StyioTokenType::TOK_CLOSED_COMMENT:
        return literal.length() + 4;

      case StyioTokenType::TOK_COMMA:
        return 1;

      case StyioTokenType::TOK_DOT:
        return 1;

      case StyioTokenType::TOK_COLON:
        return 1;

      case StyioTokenType::TOK_TILDE:
        return 1;

      case StyioTokenType::TOK_EXCLAM:
        return 1;

      case StyioTokenType::TOK_AT:
        return 1;

      case StyioTokenType::TOK_HASH:
        return 1;

      case StyioTokenType::TOK_DOLLAR:
        return 1;

      case StyioTokenType::TOK_PERCENT:
        return 1;

      case StyioTokenType::TOK_HAT:
        return 1;

      case StyioTokenType::TOK_QUEST:
        return 1;

      case StyioTokenType::TOK_SLASH:
        return 1;

      case StyioTokenType::TOK_BACKSLASH:
        return 1;

      case StyioTokenType::TOK_PIPE:
        return 1;

      case StyioTokenType::ELLIPSIS:
        return 3;

      case StyioTokenType::TOK_SQUOTE:
        return 1;

      case StyioTokenType::TOK_DQUOTE:
        return 1;

      case StyioTokenType::TOK_BQUOTE:
        return 1;

      case StyioTokenType::TOK_LPAREN:
        return 1;

      case StyioTokenType::TOK_RPAREN:
        return 1;

      case StyioTokenType::TOK_LBOXBRAC:
        return 1;

      case StyioTokenType::TOK_RBOXBRAC:
        return 1;

      case StyioTokenType::TOK_LCURBRAC:
        return 1;

      case StyioTokenType::TOK_RCURBRAC:
        return 1;

      case StyioTokenType::TOK_LANGBRAC:
        return 1;

      case StyioTokenType::TOK_RANGBRAC:
        return 1;

      case StyioTokenType::LOGIC_NOT:
        return 1;

      case StyioTokenType::LOGIC_AND:
        return 2;

      case StyioTokenType::LOGIC_OR:
        return 2;

      case StyioTokenType::LOGIC_XOR:
        return 1;

      case StyioTokenType::BINOP_BITAND:
        return 1;

      case StyioTokenType::BINOP_BITOR:
        return 1;

      case StyioTokenType::BINOP_BITXOR:
        return 1;

      case StyioTokenType::TOK_BACKWARD:
        return 2;

      case StyioTokenType::TOK_FORWARD:
        return 2;

      case StyioTokenType::PRINT:
        return 2;

      case StyioTokenType::UNARY_NEG:
        return 1;

      case StyioTokenType::BINOP_ADD:
        return 1;

      case StyioTokenType::BINOP_SUB:
        return 1;

      case StyioTokenType::BINOP_MUL:
        return 1;

      case StyioTokenType::BINOP_DIV:
        return 1;

      case StyioTokenType::BINOP_MOD:
        return 1;

      case StyioTokenType::BINOP_POW:
        return 2;

      case StyioTokenType::BINOP_GT:
        return 1;

      case StyioTokenType::BINOP_GE:
        return 2;

      case StyioTokenType::BINOP_LT:
        return 1;

      case StyioTokenType::BINOP_LE:
        return 2;

      case StyioTokenType::BINOP_EQ:
        return 2;

      case StyioTokenType::BINOP_NE:
        return 2;

      case StyioTokenType::ARROW_SINGLE_RIGHT:
        return 2;

      case StyioTokenType::ARROW_SINGLE_LEFT:
        return 2;

      case StyioTokenType::WALRUS:
        return 2;

      case StyioTokenType::MATCH:
        return 2;

      case StyioTokenType::INFINITE_LIST:
        return 5;

      default:
        return 0;
    }
  }

  std::string as_str() {
    if (type == StyioTokenType::TOK_LF) {
      return std::string(" ║ ") + getTokName(this->type) + "\n";
    }
    else if (type == StyioTokenType::TOK_NAME) {
      return std::string(" ║ ") + this->literal;
    }
    else if (type == StyioTokenType::TOK_NAME
             || type == StyioTokenType::TOK_INT
             || type == StyioTokenType::TOK_FLOAT) {
      return std::string(" ║ ") + getTokName(this->type) + ": " + this->literal;
    }
    else if (type == StyioTokenType::TOK_STRING) {
      return std::string(" ║ ") + "\"" + this->literal + "\"";
    }
    else {
      return std::string(" ║ ") + getTokName(this->type);
    }
  }
};

static std::unordered_map<StyioTokenType, std::vector<StyioTokenType> > const
  StyioTokenMap = {
    // =>
    {StyioTokenType::ARROW_DOUBLE_RIGHT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_EQUAL,
       StyioTokenType::TOK_RANGBRAC
     }
    },
    // <=
    {StyioTokenType::ARROW_DOUBLE_LEFT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_LANGBRAC,
       StyioTokenType::TOK_EQUAL
     }
    },
    // ->
    {StyioTokenType::ARROW_SINGLE_LEFT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_MINUS,
       StyioTokenType::TOK_RANGBRAC
     }
    },
    // <-
    {StyioTokenType::ARROW_SINGLE_LEFT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_LANGBRAC,
       StyioTokenType::TOK_MINUS
     }
    },
    /*
      Binary Operations
    */
    // ==
    {StyioTokenType::BINOP_EQ,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_EQUAL,
       StyioTokenType::TOK_EQUAL
     }
    },
    // !=
    {StyioTokenType::BINOP_NE,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_EXCLAM,
       StyioTokenType::TOK_EQUAL
     }
    },
    // >=
    {StyioTokenType::BINOP_GE,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_RANGBRAC,
       StyioTokenType::TOK_EQUAL
     }
    },
    // >
    {StyioTokenType::BINOP_GT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_RANGBRAC
     }
    },
    // <=
    {StyioTokenType::BINOP_LE,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_LANGBRAC,
       StyioTokenType::TOK_EQUAL
     }
    },
    // <
    {StyioTokenType::BINOP_LT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_LANGBRAC
     }
    },

    /*
      Special
    */
    // >_
    {StyioTokenType::PRINT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_RANGBRAC,
       StyioTokenType::TOK_UNDLINE
     }
    },
    // :=
    {StyioTokenType::WALRUS,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_COLON,
       StyioTokenType::TOK_EQUAL
     }
    },
    // ?=
    {StyioTokenType::MATCH,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_QUEST,
       StyioTokenType::TOK_EQUAL
     }
    },

    /*
      LOGIC
    */
   // ! (Alternative)
    {StyioTokenType::LOGIC_NOT,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_EXCLAM
     }
    },
    // &&
    {StyioTokenType::LOGIC_AND,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_AMP,
       StyioTokenType::TOK_AMP
     }
    },
    // ||
    {StyioTokenType::LOGIC_OR,
     std::vector<StyioTokenType>{
       StyioTokenType::TOK_PIPE,
       StyioTokenType::TOK_AMP
     }
    },
};

#endif
