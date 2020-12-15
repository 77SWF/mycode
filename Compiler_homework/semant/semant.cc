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

static Symbol return_type;
static bool in_loop=false;
static bool is_object_defined=true;

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
    print
    /*,
    if_stmt,
    while_stmt,
    for_stmt,
    break_stmt,
    continue_stmt,
    return_stmt,
    stmt_block
    */
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
            semant_error(curr_decl) << " Globle var " << variable_decl->getName() << " was previously defined.\n";
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

        //形参、最外有无返回语句
        call_decl->check(); //这里面objectEnv.enterscope()
        //语句块检查，语句块会嵌套，里面有返回类型检查
        StmtBlock func_stmtblock = call_decl->getBody();
        func_stmtblock->check(call_decl->getType());//传参：返回值
        objectEnv.exitscope();
    }
}

//函数声明：形参、声明的返回类型
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
        semant_error(curr_decl) << "Main function should have return type Void.\n";

    Variables formal_paras = main_decl->getVariables(); //形参链表
    if(formal_paras->len() != 0)
        semant_error(curr_decl) << "Function main can't have parameter.\n";
}

//变量声明的判定，在StmtBlock_class::check调用？可不用吧
void VariableDecl_class::check() {

}

//函数声明：形参/返回语句！！在check_calls调用，写好了
void CallDecl_class::check() {
    if(paras->len()>6)
    {
        semant_error(this) << "Function " << name << " has more than 6 parameters.\n";
        return;
    }    

    objectEnv.enterscope();
    //遍历这个函数声明的“x Float”形参
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

    //有无返回语句：getbody得到StmtBlock_class对象，getStmts得到语句链表Stmts
    //查最后一句的类型，要对每种___Stmt写返回类型的函数
    Stmts body_stmts = this->getBody()->getStmts();
    Stmt last_stmt = body_stmts->nth(body_stmts->len()-1);

    //无返回语句
    if(last_stmt->is_what_Stmt() != 4)
    {
        semant_error(this) << "Function " << this->getName() << " must have an overall return statement.\n";
        return;
    }
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
            semant_error(curr_vardecl) << "Var " << curr_vardecl->getName() << " cantnot be of type Void.\n";
        else if(objectEnv.probe(curr_vardecl->getName())) //用lookup会和形参冲突
            semant_error(curr_vardecl) << "var " << curr_vardecl->getName() << " was previously declared.\n";
        else
            objectEnv.addid(curr_vardecl->getName(),new Symbol(curr_vardecl->getType()));
    }

    //语句块后的语句链表，递归调用
    Stmts body_stmts = this->getStmts();
    
    for (int i = body_stmts->first(); body_stmts->more(i); i = body_stmts->next(i))
    {
        Stmt curr_stmt = body_stmts->nth(i);
        int type_id = curr_stmt->is_what_Stmt();
        
        if(type_id == 6 && !in_loop)
            semant_error(curr_stmt)<<"break must be used in a loop sentence.\n";
        else if(type_id == 5 && !in_loop)
            semant_error(curr_stmt)<< "continue must be used in a loop sentence.\n";
        else if(type_id == 4)
        {
            ReturnStmt_class* return_stmt = static_cast<ReturnStmt_class*>(curr_stmt);
            return_type = return_stmt->getValue()->checkType();

            //应该用不到，语法已规定
            if(return_type!=Int && return_type!=String && return_type!=Void && return_type!=Float && return_type!=Bool)
                semant_error(curr_stmt)<<"Return undefined type.\n";

            if(is_object_defined && !sameType(return_type,type))
                semant_error(curr_stmt)<<"Return "<<return_type<<",but need "<<type<<".\n";
        }
        else if(type_id == 2 || type_id == 3) 
        {
            in_loop = true;
            curr_stmt->check(type);
        }
        else if(type_id == 0||1)
            curr_stmt->check(type); //遇到StmtBlock会递归调用本函数
        else if(type_id == 7)
            curr_stmt->checkType();
    }
    objectEnv.exitscope();
}

void IfStmt_class::check(Symbol type) {
    if(condition->checkType() != Bool)
        semant_error(this)<< condition->checkType() << "condition of 'if' does not have type Bool.\n";
    
    thenexpr->check(type);
    elseexpr->check(type);

}

void WhileStmt_class::check(Symbol type) {
    if(condition->checkType() != Bool)
        semant_error(this) << "condition of 'while' does not have type Bool.\n";

    body->check(type);
    in_loop = false;
}

void ForStmt_class::check(Symbol type) {
    if(!condition->is_empty_Expr() && condition->checkType() != Bool)
        semant_error(this) << "condition of 'if' isn't empty but does not have type Bool.\n";
    initexpr->checkType();
    loopact->checkType();
    body->check(type);
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
}

Symbol IfStmt_class::checkType(){
}

Symbol WhileStmt_class::checkType(){
}

Symbol ForStmt_class::checkType(){
}

Symbol ReturnStmt_class::checkType(){
}

Symbol ContinueStmt_class::checkType(){
}

Symbol BreakStmt_class::checkType(){
}

//函数调用：类里有实参列表了，每个function可以取到形参列表
Symbol Call_class::checkType(){
    bool is_error = false;
    Symbol returntype;

    if(name == print)
    {
        if(actuals->len()==0)
            semant_error(this)<<"printf() must has at last one parameter of type String.\n";
        else
        {
            int i = actuals->first();
            Actual first_actual = actuals->nth(i);
            if(first_actual->checkType() != String)
                semant_error(first_actual)<<"printf()'s first parameter must be of type String.\n";
            for (int i = actuals->first(); actuals->more(i); i = actuals->next(i))
                {
                    Actual cur_actual = actuals->nth(i);
                    cur_actual->checkType();
                }
        }
        type = Void;
        return type;
    }
    else if (funcdecl_Table.find(name) == funcdecl_Table.end())
    {
        semant_error(this)<< "Function " << name << " has not been defined.\n";
        return Void;
    }
    else
    {
        Decl curr_func = funcdecl_Table[name]; //不用find
        CallDecl_class* curr_funcdecl = static_cast<CallDecl_class*>(curr_func);
        returntype = curr_funcdecl->getType();
        Variables paras = curr_funcdecl->getVariables();

        int k1 = actuals->first();
        int k2 = paras->first();

        while(actuals->more(k1) && paras->more(k2))
        {
            
            Actual curr_actual = actuals->nth(k1);
            Variable curr_para = paras->nth(k2);

            //问题出现在curr_actual->checkType()！问题已解决
            //测试行：semant_error(this) << curr_para->getType() << ',' << curr_actual->checkType() <<'\n'
            //看if里！！！！写完，注释这个也没用了
            if(!sameType( curr_para->getType(),curr_actual->checkType()) )
            {
                //注释这行没用
                semant_error(this) << "function " << name << ", parameter " << curr_para->getName() << " should be " << curr_para->getType() << " ,but provided a " <<  curr_actual->checkType()  << ".\n";
                is_error = true;
            }
            k1 = actuals->next(k1);
            k2 = paras->next(k2);
            
            //more:n<链表里个数，返回1，否则0
            if(actuals->more(k1) xor paras->more(k2))
            {
                semant_error(this) << "Function " << name << " called with wrong number of arguments.\n";
                is_error = true;
            }
        
        }
        type = returntype;
        return type;
    }
    
}


Symbol Actual_class::checkType(){
    type = expr->checkType();
    return type;
}

//写了
Symbol Assign_class::checkType(){
    Symbol rtype = value->checkType();
    Symbol ltype = *objectEnv.lookup(lvalue);

    //先声明再使用
    if(objectEnv.lookup(lvalue) == 0)
    {
        semant_error(this) << "Assignment to undeclared variable " << lvalue << ".\n";
        type = rtype;
    }
    //若有声明，但左右类型不同
    else if(!sameType(rtype,ltype))
    {
        semant_error(this) << "Right value must have type " << ltype << " ,got " << rtype << ".\n";
        type = ltype;
    }
    else type = ltype;
    return type;
}

Symbol Add_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot add a " << type1 << " a " << type2 <<".\n";
    if(type1==Int && type2==Int)
        {setType(Int);
        return type;}
    else
        {setType(Float);
        return type;}
}

Symbol Minus_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined )
        semant_error(this) << "Cannot minus a " << type1 << " a " << type2 <<".\n";
    if(type1==Int && type2==Int)
        {setType(Int);
        return type;}
    else
        {setType(Float);
        return type;}
}

Symbol Multi_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float))  && is_object_defined )
        semant_error(this) << "Cannot multiply a " << type1 << " a " << type2 <<".\n";
    if(type1==Int && type2==Int)
        {setType(Int);
        return type;}
    else
        {setType(Float);
        return type;}
}

Symbol Divide_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if(( (type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot divide a " << type1 << " a " << type2 <<".\n";
    if(type1==Int && type2==Int)
        {setType(Int);
        return type;}
    else
        {setType(Float);
        return type;}
}

Symbol Mod_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( (type1 != Int || type2 != Int) && is_object_defined )
        semant_error(this) << "Cannot mod a " << type1 << " a " << type2 <<".\n";
    setType(Int);
    return type;
}

Symbol Neg_class::checkType(){
    Symbol type1 = e1->checkType();
    if( (type1 != Int && type1 != Float) && is_object_defined)
        semant_error(this) << "a" <<  type1 << " doesn't have a negative " <<".\n";
    if(type1==Int)
        {setType(Int);
        return type;}
    else
        {setType(Float);
        return type;}
}

Symbol Lt_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot compare a " << type1 << " and a " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Le_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot compare a " << type1 << " and a " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Equ_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot compare a " << type1 << " and a " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Neq_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined )
        if(type1 != Bool || type2 != Bool)
            semant_error(this) << "Cannot compare a " << type1 << " and a " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Ge_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot compare a " << type1 << " and a " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Gt_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Int && type1 != Float) || (type2 != Int && type2 != Float)) && is_object_defined)
        semant_error(this) << "Cannot compare a " << type1 << " and a " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol And_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Bool) || (type2 != Bool)) && is_object_defined)
        semant_error(this) << "Cannot use && between " << type1 << " and " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Or_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( ((type1 != Bool) || (type2 != Bool)) && is_object_defined)
        semant_error(this) << "Cannot use || between " << type1 << " and " << type2 <<".\n";
    setType(Bool);
    return type;
}

Symbol Xor_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( (type1 != Bool) || (type2 != Bool))
        if( ((type1 != Int) || (type2 != Int)) && is_object_defined)
            semant_error(this) << "Cannot use ^ between " << type1 << " and " << type2 <<".\n";
        else 
            {setType(Int);
            return type;}
    else 
        {setType(Bool);
        return type;}
}

Symbol Not_class::checkType(){
    Symbol type1 = e1->checkType();
    if( type1 != Bool && is_object_defined )
        semant_error(this) << "Cannot use ! upon " << type1 <<".\n";
    setType(Bool);
    return type;
}

Symbol Bitand_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( (type1 != Int || type2 != Int) && is_object_defined )
        semant_error(this) << "Cannot use & between " << type1 << " and " << type2 <<".\n";
    setType(Int);
    return type;
}

Symbol Bitor_class::checkType(){
    Symbol type1 = e1->checkType();
    Symbol type2 = e2->checkType();
    if( (type1 != Int || type2 != Int) && is_object_defined )
        semant_error(this) << "Cannot use | between " << type1 << " and " << type2 <<".\n";
    setType(Int);
    return type;
}

Symbol Bitnot_class::checkType(){
    Symbol type1 = e1->checkType();
    if( type1 != Int && is_object_defined )
        semant_error(this) << "Cannot use unary op ~ upon " << type1 <<".\n";
    setType(Int);
    return type;
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
    if(objectEnv.lookup(var))
    {
        type = *objectEnv.lookup(var);
        is_object_defined=true;
    }    
    else 
    {    
        semant_error(this) << "object " << var << " has not been defined.\n";
        type = Void;
        is_object_defined=false;
    }
    return type;
}

Symbol No_expr_class::checkType(){
    setType(Void);
    return getType();
}

void Program_class::semant() {
    initialize_constants();
    install_calls(decls);
    check_main();
    install_globalVars(decls);
    check_calls(decls);
    
    if (semant_errors > 0) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}



