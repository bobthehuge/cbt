#ifndef AST_H
#define AST_H

#include "bth_lex.h"
typedef struct bth_lex_token Token;
#include "token.h"

typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_VOID,
    TYPE_POINTER,
    TYPE_ARRAY,
    TYPE_STRUCT,
    TYPE_UNION,
    TYPE_ENUM,
    TYPE_FUNCTION,
} TypeKind;

typedef struct
{
    TypeKind kind;
    char *space;
    char *name;
} Type;

typedef struct
{
} PPType;

typedef enum
{
    // Program structure
    NODE_PROGRAM,          // Root node containing all translation units
    NODE_TRANSLATION_UNIT, // Collection of declarations
    
    // Declarations
    NODE_FUNCTION_DECL,
    NODE_VAR_DECL,
    NODE_PARAM_DECL,
    NODE_TYPEDEF_DECL,
    NODE_STRUCT_DECL,
    NODE_UNION_DECL,
    NODE_ENUM_DECL,
    NODE_FIELD_DECL,       // Struct/union members
    
    // Type specifiers
    NODE_TYPE_SPECIFIER,
    NODE_STRUCT_SPECIFIER,
    NODE_UNION_SPECIFIER,
    NODE_ENUM_SPECIFIER,
    NODE_TYPE_QUALIFIER,   // const, volatile, etc.
    NODE_STORAGE_CLASS,   // extern, static, register, etc.
    
    // Statements
    NODE_COMPOUND_STMT,    // { ... }
    NODE_IF_STMT,
    NODE_SWITCH_STMT,
    NODE_WHILE_STMT,
    NODE_DO_WHILE_STMT,
    NODE_FOR_STMT,
    NODE_RETURN_STMT,
    NODE_BREAK_STMT,
    NODE_CONTINUE_STMT,
    NODE_GOTO_STMT,
    NODE_LABEL_STMT,
    NODE_CASE_STMT,
    NODE_DEFAULT_STMT,
    NODE_EXPR_STMT,        // Expression as statement (with ;)
    NODE_ASM_STMT,         // Inline assembly
    
    // Expressions
    NODE_EXPRESSION,
    NODE_BINARY_EXPR,
    NODE_UNARY_EXPR,
    NODE_CAST_EXPR,
    NODE_COND_EXPR,        // Ternary ?: operator
    NODE_ARRAY_SUBSCRIPT,
    NODE_FUNCTION_CALL,
    NODE_MEMBER_ACCESS,    // . operator
    NODE_PTR_MEMBER_ACCESS, // -> operator
    NODE_COMMA_EXPR,
    NODE_ASSIGN_EXPR,
    NODE_INIT_LIST,        // Initializer lists
    NODE_SIZEOF_EXPR,
    NODE_ALIGNOF_EXPR,
    NODE_OFFSETOF_EXPR,
    // NODE_GENERIC_SELECTION, // C11 _Generic
    
    // Primary expressions
    NODE_IDENTIFIER,
    NODE_CONSTANT,         // Numeric/character constants
    NODE_STRING_LITERAL,
    NODE_COMPOUND_LITERAL, // (type){init}
    
    // Preprocessor (if included in AST)
    NODE_PP_DIRECTIVE,
    NODE_PP_MACRO,
    NODE_PP_TOKEN_PASTE,
    NODE_PP_STRINGIZE,
    
    // Special cases
    NODE_EMPTY,           // Empty statement or declaration
    NODE_ERROR            // Placeholder for error recovery
} NodeType;

typedef enum {
    VALUE_INT,
    VALUE_UINT,
    VALUE_LONG,
    VALUE_ULONG,
    VALUE_LONGLONG,
    VALUE_ULONGLONG,
    VALUE_FLOAT,
    VALUE_DOUBLE,
    VALUE_LONGDOUBLE,
    VALUE_CHAR,
    VALUE_BOOL,
    VALUE_NULL,         // NULL pointer constant
    VALUE_ENUM,         // Enumeration constant
} ValueKind;

typedef struct {
    ValueKind kind;
    Type* type;         // Reference to full type information
    
    union {
        // Integer types
        struct {
            union {
                char c;
                unsigned char uc;
                short s;
                unsigned short us;
                int i;
                unsigned int u;
                long l;
                unsigned long ul;
                long long ll;
                unsigned long long ull;
            };
            int base;  // 8, 10, or 16 for display
            bool is_unsigned;
        } integer;
        
        // Floating-point types
        struct {
            union {
                float f;
                double d;
                long double ld;
            };
            bool hex_float; // C99 hexadecimal float format
        } floating;
        
        // Enumeration constants
        struct {
            char* name;
            int value;
        } enumerated;
        
        // Pointer (null only)
        struct {
            Type* pointed_type;
        } pointer;
    };
    
    // Source location for error reporting
    int line;
    int column;
} Value;

typedef struct Symbol {
    char* name;
    Type* type;
    int scope_level;
    // Other properties like storage class, etc.
} Symbol;

typedef struct SymbolTable {
    Symbol** symbols;
    int count;
    int capacity;
    struct SymbolTable* parent; // For nested scopes
} SymbolTable;

struct ASTNode;

typedef struct {
    ASTNode* left;
    TokenKind op;
    ASTNode* right;
} BinaryExprNode;

typedef struct {
    TokenKind op;
    ASTNode* operand;
} UnaryExprNode;

typedef struct {
    Type* target_type;
    ASTNode* expression;
} CastExprNode;

typedef struct {
    ASTNode* condition;
    ASTNode* then_expr;
    ASTNode* else_expr;
} CondExprNode;

typedef struct {
    ASTNode* array;
    ASTNode* index;
} ArraySubscriptNode;

typedef struct {
    ASTNode* function;
    ASTNode** args;
    int arg_count;
} FunctionCallNode;

typedef struct {
    ASTNode* structure;
    char* member;
} MemberAccessNode;

typedef struct {
    ASTNode* pointer;
    char* member;
} PtrMemberAccessNode;

typedef struct {
    ASTNode** expressions;
    int expr_count;
} CommaExprNode;

typedef struct {
    ASTNode* lhs;
    TokenKind op;          // =, +=, -=, etc.
    ASTNode* rhs;
} AssignExprNode;

typedef struct {
    char* name;
    Symbol* symbol;        // Resolved during semantic analysis
} IdentifierNode;

typedef struct {
    Value value;
    Type* type;
} ConstantNode;

typedef struct {
    char* value;
    Type* type;
} StringLiteralNode;

typedef struct {
    Type* type;
    ASTNode* initializer;
} CompoundLiteralNode;

typedef struct {
    ASTNode** initializers;
    int init_count;
} InitListNode;

typedef struct {
    ASTNode* expression;   // Either type or expr
} SizeofExprNode;

typedef struct {
    Type* type;
} AlignofExprNode;

typedef struct {
    Type* type;
    char* member;
} OffsetofExprNode;

typedef struct {
    ASTNode* controlling;
    ASTNode** associations; // Array of type-value pairs
    int assoc_count;
} GenericSelectionNode;

typedef struct {
    ASTNode** declarations;  // Array of top-level declarations
    int decl_count;
} ProgramNode;

typedef struct {
    ASTNode** declarations;  // Array of declarations in this TU
    int decl_count;
    char* filename;
} TranslationUnitNode;

typedef struct {
    char* name;
    Type* type;
    ASTNode* init_value;     // Optional initializer
    TokenKind storage;
} VarDeclNode;

typedef struct {
    char* name;
    Type* return_type;
    ASTNode** parameters;    // Array of ParamDeclNodes
    int param_count;
    ASTNode* body;           // CompoundStmtNode
    TokenKind storage;
} FunctionDeclNode;

typedef struct {
    char* name;
    Type* type;
} ParamDeclNode;

typedef struct {
    char* name;
    Type* type;
} TypedefDeclNode;

typedef struct {
    char* tag;               // Struct name (optional)
    ASTNode** fields;        // Array of FieldDeclNodes
    int field_count;
} StructDeclNode;

typedef struct {
    char* tag;               // Union name (optional)
    ASTNode** fields;        // Array of FieldDeclNodes
    int field_count;
} UnionDeclNode;

typedef struct {
    char* tag;               // Enum name (optional)
    ASTNode** enumerators;   // Array of EnumConstantNodes
    int enum_count;
} EnumDeclNode;

typedef struct {
    char* name;
    Type* type;
    ASTNode* bit_width;      // Optional bit-field width
} FieldDeclNode;

typedef struct {
    Type* type;
} TypeSpecifierNode;

typedef struct {
    char* tag;              // Struct name
    ASTNode* definition;    // Optional StructDeclNode
} StructSpecifierNode;

typedef struct {
    char* tag;              // Union name
    ASTNode* definition;    // Optional UnionDeclNode
} UnionSpecifierNode;

typedef struct {
    char* tag;              // Enum name
    ASTNode* definition;   // Optional EnumDeclNode
} EnumSpecifierNode;

typedef struct {
    TokenKind qual;     // CONST, VOLATILE, etc.
} TypeQualifierNode;

typedef struct {
    TokenKind storage;   // EXTERN, STATIC, etc.
} StorageClassNode;

typedef struct {
    ASTNode** items;        // Array of statements/declarations
    int item_count;
    SymbolTable* scope;
} CompoundStmtNode;

typedef struct {
    ASTNode* condition;
    ASTNode* then_branch;
    ASTNode* else_branch;   // Optional
} IfStmtNode;

typedef struct {
    ASTNode* condition;
    ASTNode* body;
} WhileStmtNode;

typedef struct {
    ASTNode* condition;
    ASTNode* body;
} DoWhileStmtNode;

typedef struct {
    ASTNode* target_loop;    // Reference to the loop/switch being broken
    Token token;             // Original token for error reporting
} BreakStmtNode;

typedef struct {
    ASTNode* target_loop;    // Reference to the loop/switch being broken
    Token token;             // Original token for error reporting
} ContinueStmtNode;

typedef struct {
    ASTNode* condition;      // The switch condition expression
    ASTNode* body;          // The compound statement containing cases
    SymbolTable* scope;     // Scope for the switch body
    bool has_default;       // Whether a default case exists
} SwitchStmtNode;

typedef struct {
    ASTNode* init;          // Optional
    ASTNode* condition;     // Optional
    ASTNode* update;        // Optional
    ASTNode* body;
} ForStmtNode;

typedef struct {
    ASTNode* expression;    // Optional
} ReturnStmtNode;

typedef struct {
    char* label;
} GotoStmtNode;

typedef struct {
    char* label;
    ASTNode* statement;
} LabelStmtNode;

typedef struct {
    ASTNode* expression;
    ASTNode* statement;
} CaseStmtNode;

typedef struct {
    ASTNode* statement;
} DefaultStmtNode;

typedef struct {
    ASTNode* expression;
} ExprStmtNode;

typedef struct {
    char* assembly;
    ASTNode* outputs;       // Optional
    ASTNode* inputs;        // Optional
    ASTNode* clobbers;      // Optional
} AsmStmtNode;

typedef struct {
    PPType type;
    char* directive;
    ASTNode* content;
} PPDirectiveNode;

typedef struct {
    char* name;
    ASTNode** params;
    int param_count;
    ASTNode* replacement;
} PPMacroNode;

typedef struct {
    // No additional fields needed
} EmptyNode;

typedef struct {
    char* message;
    Token* token;
} ErrorNode;

typedef struct ASTNode {
    NodeType type;
    Type* expr_type;      // Type of the expression (for type checking)
    int first_line;
    int first_column;
    int last_line;
    int last_column;
    
    union {
        /* Program Structure */
        ProgramNode program;
        TranslationUnitNode translation_unit;
        
        /* Declarations */
        VarDeclNode var_decl;
        FunctionDeclNode func_decl;
        ParamDeclNode param_decl;
        TypedefDeclNode typedef_decl;
        StructDeclNode struct_decl;
        UnionDeclNode union_decl;
        EnumDeclNode enum_decl;
        FieldDeclNode field_decl;
        
        /* Type Specifiers */
        TypeSpecifierNode type_spec;
        StructSpecifierNode struct_spec;
        UnionSpecifierNode union_spec;
        EnumSpecifierNode enum_spec;
        TypeQualifierNode type_qual;
        StorageClassNode storage_class;
        
        /* Statements */
        CompoundStmtNode compound_stmt;
        IfStmtNode if_stmt;
        WhileStmtNode while_stmt;
        DoWhileStmtNode do_while_stmt;
        ForStmtNode for_stmt;
        ReturnStmtNode return_stmt;
        BreakStmtNode break_stmt;
        ContinueStmtNode continue_stmt;
        GotoStmtNode goto_stmt;
        LabelStmtNode label_stmt;
        CaseStmtNode case_stmt;
        DefaultStmtNode default_stmt;
        ExprStmtNode expr_stmt;
        AsmStmtNode asm_stmt;
        SwitchStmtNode switch_stmt;
        
        /* Expressions */
        BinaryExprNode binary_expr;
        UnaryExprNode unary_expr;
        CastExprNode cast_expr;
        CondExprNode cond_expr;
        ArraySubscriptNode array_subscript;
        FunctionCallNode func_call;
        MemberAccessNode member_access;
        PtrMemberAccessNode ptr_member_access;
        CommaExprNode comma_expr;
        AssignExprNode assign_expr;
        InitListNode init_list;
        SizeofExprNode sizeof_expr;
        AlignofExprNode alignof_expr;
        OffsetofExprNode offsetof_expr;
        GenericSelectionNode generic_sel;
        
        /* Primary Expressions */
        IdentifierNode identifier;
        ConstantNode constant;
        StringLiteralNode string_literal;
        CompoundLiteralNode compound_literal;
        
        /* Preprocessor (Optional) */
        PPDirectiveNode pp_directive;
        PPMacroNode pp_macro;
        
        /* Special Cases */
        EmptyNode empty;
        ErrorNode error;
    } u;
} ASTNode;

#endif
