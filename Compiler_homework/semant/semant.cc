#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

static ostream& error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;

typedef std::map<Symbol, Decl> DeclTable;//key=词法提取的token，value=函数或变量声明
DeclTable funcdecl_Table; //表内value是父类Decl、不是子类CallDecl_class

///////////////////////////////////////////////
// helper func
///////////////////////////////////////////////


static ostream& semant_error() {
    semant_errors++;
    return error_stream;
}

static ostream& semant_error(tree_node *t) {
    error_stream << t->get_line_number() << ": ";
    return semant_error();
}

static ostream& internal_error(int lineno) {
    error_stream << "FATAL:" << lineno << ": ";
    return error_stream;
}

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////

static Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print,
    if_stmt,
    while_stmt,
    for_stmt,
    break_stmt,
    continue_stmt,
    return_stmt,
    stmt_block
    ;

bool isValidCallName(Symbol type) {
    return type != (Symbol)print;
}

bool isValidTypeName(Symbol type) {
    return type != Void;
}

//
// Initializing the predefined symbols.
//

static void initialize_constants(void) {
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // Main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}

/*
    TODO :
    you should fill the following function defines, so that semant() can realize a semantic 
    analysis in a recursive way. 
    Of course, you can add any other functions to help.
*/

static bool sameType(Symbol name1, Symbol name2) {
    return strcmp(name1->get_string(), name2->get_string()) == 0;
}

//预先配置所有函数声明
static void install_calls(Decls decls) {
    for (int i = decls->first(); decls->more(i); i = decls->next(i)) {
        curr_decl = decls->nth(i);
        //是函数声明还是变量声明
        if(curr_decl->isCallDecl())
        {
            if (curr_decl->getName() == print)
                semant_error(curr_decl) << "Redefinition of basic function printf.\n";
            if (funcdecl_Table.find(curr_decl->getName()) != funcdecl_Table.end())
                semant_error(curr_decl) << "Function" << curr_decl->getName() << "was previously defined.\n";
            else
                funcdecl_Table[curr_decl->getName()] = curr_decl;
        }
    }
}

//预先配置所有全局变量，到时候查用lookup才能查到
static void install_globalVars(Decls decls) {
    objectEnv.enterscope();

    for (int i = decls->first(); decls->more(i); i = decls->next(i)) {
        curr_decl = decls->nth(i);
        if(curr_decl->isCallDecl()) continue;
        VariableDecl_class* variable_decl = static_cast<VariableDecl_class*>(curr_decl);

        if(variable_decl->getType() == Void)
            semant_error(curr_decl) << "Variable" << curr_decl->getName() << "type cant't be Void.\n";
        else if(objectEnv.lookup(variable_decl->getName()))
            semant_error(curr_decl) << "Globle variable" << variable_decl->getName() << " is multiply defined.\n";
        else    
            objectEnv.addid(variable_decl->getName(),new Symbol(variable_decl->getType()));
    }
}

//函数和变量
static void check_calls(Decls decls) {
//要处理每个函数的：返回类型、有无return、形参、变量
    //遍历已声明的每个函数
    for (DeclTable::iterator it = funcdecl_Table.begin(); it != funcdecl_Table.end(); it++)
    {
        Symbol func_name = it->first;
        curr_decl = it->second;
        CallDecl_class* call_decl = static_cast<CallDecl_class*>(curr_decl);

        //形参、返回类型检查
        call_decl->check();
        //语句块检查，语句块会嵌套
        StmtBlock func_stmtblock = call_decl->getBody();
        func_stmtblock->check(Int);
    }
}

//函数声明：形参、返回值
static void check_main() {
    //main函数定义
    if(funcdecl_Table.find(Main) == funcdecl_Table.end())
    {
        semant_error() << "Function main is not defined.\n";
        return;
    }

    //main的返回/参数类型
    curr_decl = funcdecl_Table[Main];
    CallDecl_class* main_decl = static_cast<CallDecl_class*>(curr_decl);

    if(main_decl->getType() != Void) 
        semant_error(curr_decl) << isValidTypeName(main_decl->getType()) << "Function main need return Void.\n";

    Variables formal_paras = main_decl->getVariables(); //形参链表
    if(formal_paras->len() != 0)
        semant_error(curr_decl) << "Function main can't have parameter.\n";
}

//变量声明的判定，在StmtBlock_class::check调用？可不用吧
void VariableDecl_class::check() {

}

//函数声明：形参/返回语句！！在check_calls调用，写好了
void CallDecl_class::check() {
    objectEnv.enterscope();
    //遍历这个函数声明的每个“x Float”形参
    for (int i = paras->first(); paras->more(i); i = paras->next(i))
    {
        Variable curr_para = paras->nth(i);

        if(curr_para->getName() == Void)
            semant_error(curr_para) << "'Void' cannot be the type of a formal parameter.\n";
        else if(objectEnv.lookup(curr_para->getName()))
            semant_error(curr_para) << "Formal parameter " << curr_para->getName() << " is multiply defined.\n";
        else
            objectEnv.addid(curr_para->getName(),new Symbol(curr_para->getType()));
    }
    objectEnv.exitscope();

    //有无返回语句：getbody得到StmtBlock_class对象，getStmts得到语句链表Stmts
    //查最后一句的类型，要对每种___Stmt写返回类型的函数
    Stmts body_stmts = this->getBody()->getStmts();
    Stmt last_stmt = body_stmts->nth(body_stmts->len()-1);

    //无返回语句
    if(last_stmt->checkType() != return_stmt)
    {
        semant_error(this) << "Function " << this->getName() << " must have an overall return statement.\n";
        return;
    }

    //有返回语句
    //返回类型不在范围
    if(returnType!=Int && returnType!=Float && returnType!=String && returnType!=Bool && returnType!=Void)
    {
        semant_error(this) << "Function " << this->getName() << " return illegal type.\n";
        return;
    }
    //不匹配
    ReturnStmt_class* return_stmt = static_cast<ReturnStmt_class*>(last_stmt);
    Symbol returnexpr_type = return_stmt->getValue()->checkType();
    if(!sameType(returnexpr_type,returnType))
        semant_error(this) << "Inferred return type " << returnexpr_type << " of function " << name << " does not conform to declared return type " << returnType << ".\n";
}

//语句块检查
void StmtBlock_class::check(Symbol type) {
    //语句块前的局部变量
    VariableDecls_class* body_vardecls= this->getVariableDecls();
    objectEnv.enterscope();
    for (int i = body_vardecls->first(); body_vardecls->more(i); i = body_vardecls->next(i))
    {
        VariableDecl curr_vardecl = body_vardecls->nth(i);
        //声明的类型不是五种之一？没要求？
        if (curr_vardecl->getType() == Void)
            semant_error(curr_vardecl) << "Variable type cant't be Void.\n";
        else if(objectEnv.lookup(curr_vardecl->getName()))
            semant_error(curr_vardecl) << "Local variable " << curr_vardecl->getName() << " is multiply declared.\n";
        else
            objectEnv.addid(curr_vardecl->getName(),new Symbol(curr_vardecl->getType()));
    }
    objectEnv.exitscope();

    //语句块后的语句链表，递归调用
    Stmts body_stmts = this->getStmts();
    for (int i = body_stmts->first(); body_stmts->more(i); i = body_stmts->next(i))
    {
        Stmt curr_stmt = body_stmts->nth(i);
        Symbol type = curr_stmt->checkType();
        if(type == if_stmt||while_stmt||for_stmt||break_stmt||continue_stmt||return_stmt||stmt_block)
            curr_stmt->check(Int); //除了expr外的6种+遇到StmtBlock会递归调用本函数
    }
}

void IfStmt_class::check(Symbol type) {
    if(condition->checkType() != Bool)
        semant_error(this) << "condition of 'if' does not have type Bool.\n";
    
    thenexpr->check(Int);
    elseexpr->check(Int);
}

void WhileStmt_class::check(Symbol type) {
    if(condition->checkType() != Bool)
        semant_error(this) << "condition of 'while' does not have type Bool.\n";

    body->check(Int);
}

void ForStmt_class::check(Symbol type) {
    if(!condition->is_empty_Expr() && condition->checkType() != Bool)
        semant_error(this) << "condition of 'if' isn't empty but does not have type Bool.\n";
    initexpr->checkType();
    loopact->checkType();
}

void ReturnStmt_class::check(Symbol type) {
    value->checkType();
}

void ContinueStmt_class::check(Symbol type) {

}

void BreakStmt_class::check(Symbol type) {

}
//自己增的7个函数
Symbol StmtBlock_class::checkType(){
    type = stmt_block;
    return type;
}

Symbol IfStmt_class::checkType(){
    type = if_stmt;
    return type;
}

Symbol WhileStmt_class::checkType(){
    type = while_stmt;
    return type;
}

Symbol ForStmt_class::checkType(){
    type = for_stmt;
    return type;
}

Symbol ReturnStmt_class::checkType(){
    type = return_stmt;
    return type;
}

Symbol ContinueStmt_class::checkType(){
    type = continue_stmt;
    return type;
}

Symbol BreakStmt_class::checkType(){
    type = break_stmt;
    return type;
}

//函数调用：类里有实参列表了，每个function可以取到形参列表
Symbol Call_class::checkType(){

}

//实参：函数调用里可用吧？和形参要一起看把？那这里用来检查每个expr把
Symbol Actual_class::checkType(){

}

//写了
Symbol Assign_class::checkType(){
    Symbol rtype = value->checkType();

    //先声明再使用
    if(objectEnv.lookup(lvalue) == 0)
    {
        semant_error(this) << "Assignment to undeclared variable " << lvalue << ".\n";
        type = rtype;
        return type;
    }

    Symbol ltype = *objectEnv.lookup(lvalue);

    //赋值左右类型不同
    if(!sameType(rtype,ltype))
    {
        semant_error(this) << "Type " << rtype << " of assigned expression does not conform to declared type " << ltype << " of identifier " << lvalue << ".\n";
        type = ltype;
        return type;
    }
}

Symbol Add_class::checkType(){
 
}

Symbol Minus_class::checkType(){
 
}

Symbol Multi_class::checkType(){
 
}

Symbol Divide_class::checkType(){

}

Symbol Mod_class::checkType(){

}

Symbol Neg_class::checkType(){

}

Symbol Lt_class::checkType(){

}

Symbol Le_class::checkType(){

}

Symbol Equ_class::checkType(){

}

Symbol Neq_class::checkType(){

}

Symbol Ge_class::checkType(){

}

Symbol Gt_class::checkType(){

}

Symbol And_class::checkType(){

}

Symbol Or_class::checkType(){

}

Symbol Xor_class::checkType(){

}

Symbol Not_class::checkType(){

}

Symbol Bitand_class::checkType(){

}

Symbol Bitor_class::checkType(){

}

Symbol Bitnot_class::checkType(){

}

Symbol Const_int_class::checkType(){
    setType(Int);
    return type;
}

Symbol Const_string_class::checkType(){
    setType(String);
    return type;
}

Symbol Const_float_class::checkType(){
    setType(Float);
    return type;
}

Symbol Const_bool_class::checkType(){
    setType(Bool);
    return type;
}

Symbol Object_class::checkType(){

}

Symbol No_expr_class::checkType(){
    setType(Void);
    return getType();
}

void Program_class::semant() {
    initialize_constants();
    install_calls(decls);//写了
    check_main();//写了
    install_globalVars(decls);//写了
    check_calls(decls);
    
    if (semant_errors > 0) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}



