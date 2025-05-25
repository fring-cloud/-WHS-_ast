#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "json_c.c"


void analyze(json_value json, int flag, int collon, FILE* fp);
bool exists(json_value body, const char* element);
void changer(json_value json, FILE* fp);
void tap_line(int count, FILE* fp);
char* reading(const char* file_name);
void ext_changer(json_value json, FILE* fp);

void parse_funcdef(json_value json, int flag, int collon, FILE* fp);
void parse_decl(json_value json, int flag, int collon, FILE* fp);
void parse_compound(json_value json, int flag, int collon, FILE* fp);
void parse_binop(json_value json, int flag, int collon, FILE* fp);
void parse_id(json_value json, int flag, int collon, FILE* fp);
void parse_if(json_value json, int flag, int collon, FILE* fp);
void parse_const(json_value json, int flag, int collon, FILE* fp);
void parse_ident(json_value json, int flag, int collon, FILE* fp);
void parse_typedec(json_value json, int flag, int collon, FILE* fp);
void parse_fundec(json_value json, int flag, int collon, FILE* fp);
void parse_explist(json_value json, int flag, int collon, FILE* fp);
void parse_call(json_value json, int flag, int collon, FILE* fp);
void parse_ptr(json_value json, int flag, int collon, FILE* fp);
void parse_assign(json_value json, int flag, int collon, FILE* fp);
void parse_arr(json_value json, int flag, int collon, FILE* fp);
void parse_while(json_value json, int flag, int collon, FILE* fp);
void parse_return(json_value json, int flag, int collon, FILE* fp);
void parse_typename(json_value json, int flag, int collon, FILE* fp);
void parse_param(json_value json, int flag, int collon, FILE* fp);


int main(void){
    char file_name[100];
    char flag;
    char* str = reading(".\\ast.json");
    json_value json = json_create(str);
    FILE* fp;

    ext_changer(json, stdout);

    printf("\n\n\n===================================================================================\n\n");
    printf("\t1. save this souce code\n\t2. exit\n\n");
    printf("===================================================================================\n\n");
    flag = _getch();

    if(flag == '1'){
        system("cls");
        printf("[enter the file name.] >> ");
        scanf("%100s", file_name);
        fp = fopen(file_name, "a");
        if(fp == NULL){
            system("cls");
            printf("file open failed!!");
            return 0;
        }
        ext_changer(json, fp);
        printf("\n\n\t< File save Success >");
        fclose(fp);
        _getch();
        return 0;
    }else{
        system("cls");
        exit(1);
    }

    free(str);
}


void ext_changer(json_value json, FILE* fp){
    json_value ext = json_get(json, "ext");
    int ext_len = json_len(ext);


    for(int i = 0; i < ext_len; i++){
        json_value element = json_get(ext, i);
        changer(element, fp);
    }
}


int type_check (json_value target){
    char* node_type = json_get_string(target, "_nodetype");
  
    if(node_type == NULL){ return 0;}
    if(strcmp("FuncDef",node_type)==0){ return 1; }        // "FuncDef"
    if(strcmp("Compound",node_type)==0){ return 2; }       // "Compound"
    if(strcmp("If",node_type)==0){ return 3; }             // "If"
    if(strcmp("BinaryOp",node_type)==0){ return 4; }       // "BinaryOp"
    if(strcmp("ID",node_type)==0){ return 5; }             // "ID"
    if(strcmp("Constant",node_type)==0){ return 6; }       // "Constant"
    if(strcmp("IdentifierType",node_type)==0){ return 7; } // "IdentifierType"
    if(strcmp("TypeDecl",node_type)==0){ return 8; }       // "TypeDecl"
    if(strcmp("Decl",node_type)==0){ return 9; }           // "Decl"
    if(strcmp("FuncDecl",node_type)==0){ return 10; }       // "FuncDecl"
    if(strcmp("ExprList",node_type)==0){ return 11; }       // "ExprList"
    if(strcmp("FuncCall",node_type)==0){ return 12; }       // "FuncCall"
    if(strcmp("PtrDecl",node_type)==0){ return 13; }        // "PtrDecl"
    if(strcmp("Assignment",node_type)==0){ return 14; }     // "Assignment"
    if(strcmp("ArrayRef",node_type)==0){ return 15; }       // "ArrayRef"
    if(strcmp("While",node_type)==0){ return 16; }          // "While"
    if(strcmp("Return",node_type)==0){ return 17; }         // "Return"
    if(strcmp("Typename",node_type)==0){ return 18; }       // "Typename"
    if(strcmp("ParamList",node_type)==0){ return 19; }      // "ParamList"
}

#define COLLON_ON 1
#define COLLON_OFF 0

#define FUNC_DEF 1
#define COMPOUND 2
#define IF 3
#define BINOP 4
#define ID 5
#define CONSTANT 6
#define IDENT 7
#define TYPE_DECL 8
#define DECL 9
#define FUNC_DECL 10
#define EXP_LIST 11
#define FUNC_CALL 12
#define PTR 13
#define ASSIGN 14
#define ARRAY 15
#define WHILE 16
#define RETURN 17
#define TYPE_NAME 18
#define PARAM 19
#define SIGN 20

int global_args = 1;

bool exists(json_value body, const char* element){
    json_value tar = json_get(body, element);
    if(tar.value == NULL){
      return false;
    }
      return true;
}


void changer(json_value json, FILE* fp){
    if(type_check(json) == DECL){
        analyze(json, 0, COLLON_ON, fp);
        fprintf(fp, "\n");
    }else{
        analyze(json, 0, COLLON_ON, fp);
        fprintf(fp, "\n\n");
    }
    global_args = 0;
}

void analyze(json_value json, int flag, int collon, FILE* fp){
    switch(type_check(json)){
        case FUNC_DEF://-----------------------"FuncDef"
            parse_funcdef(json, flag, collon, fp);
            break;
        case DECL://---------------------------"Decl"
            parse_decl(json, flag, collon, fp);
            break;
        case COMPOUND://-----------------------"Compound"
            parse_compound(json, flag, collon, fp);
            break;
        case IF://-----------------------------"If"
            parse_if(json, flag, collon, fp);
            break;
        case BINOP://--------------------------"BinaryOp"
            parse_binop(json, flag, collon, fp);
            break;
        case ID://-----------------------------"ID"
            parse_id(json, flag, collon, fp);
            break;
        case CONSTANT://-----------------------"Constant"
            parse_const(json, flag, collon, fp);
            break;
        case IDENT://--------------------------"IdentifierType"
            parse_ident(json, flag, collon, fp);
            break;
        case TYPE_DECL://----------------------"TypeDecl"
            parse_typedec(json, flag, collon, fp);
            break;
        case FUNC_DECL://----------------------"FuncDecl"
            parse_fundec(json, flag, collon, fp);
            break;
        case EXP_LIST://-----------------------"ExprList"
            parse_explist(json, flag, collon, fp);
            break;
        case FUNC_CALL://----------------------"FuncCall"
            parse_call(json, flag, collon, fp);
            break;
        case PTR://----------------------------"PtrDecl"
            parse_ptr(json, flag, collon, fp);
            break;
        case ASSIGN://-------------------------"Assignment"
            parse_assign(json, flag, collon, fp);
            break;
        case ARRAY://--------------------------"ArrayRef"
            parse_arr(json, flag, collon, fp);
            break;
        case WHILE://--------------------------"While"
            parse_while(json, flag, collon, fp);
            break;
        case RETURN://-------------------------"Return"
            parse_return(json, flag, collon, fp);
            break;
        case TYPE_NAME://----------------------"Typename"
            parse_typename(json, flag, collon, fp);
            break;
        case PARAM://--------------------------"ParamList"
            parse_param(json, flag, collon, fp);
            break;
    }
}


void parse_funcdef(json_value json, int flag, int collon, FILE* fp){
    analyze(json_get(json, "decl"), 0, COLLON_OFF, fp);
    analyze(json_get(json, "body"), 0, collon, fp);
}

void parse_decl(json_value json, int flag, int collon, FILE* fp){
    if(exists(json, "init")){ // 변수 초기화 
        analyze(json_get(json, "type"), 0, COLLON_OFF, fp);
        fprintf(fp, " = "); 
        analyze(json_get(json, "init"), 0, SIGN, fp);
    }else{
        if(type_check(json_get(json, "type")) == FUNC_DECL){
            if(collon == COLLON_OFF){
                analyze(json_get(json, "type"), 0, COLLON_OFF, fp);      // 함수 선언
            }else{
                analyze(json_get(json, "type"), 0, collon, fp); // 함수 정의
            }    
        }
        else{ // 변수 선언
            analyze(json_get(json, "type"), 0, collon, fp);
        }
    }
}

void parse_compound(json_value json, int flag, int collon, FILE* fp){ 
    if(exists(json, "block_items")){
        json_value block_arr = json_get(json, "block_items");
        fprintf(fp, "{\n");
        int block_len = json_len(block_arr);

        for(int i = 0; i < block_len; i++){
            json_value element = json_get(block_arr, i);
            fprintf(fp, "\t");
            tap_line(flag, fp);
            analyze(element, flag, collon, fp);
            fprintf(fp, "\n");
        }
        tap_line(flag, fp);
        fprintf(fp, "}");

    }else{
        fprintf(fp, "{ }");
    }
}


void parse_if(json_value json, int flag, int collon, FILE* fp){
    fprintf(fp, "if( ");
    if(exists(json, "cond")){analyze(json_get(json, "cond"),0, COLLON_OFF, fp); fprintf(fp, ")");}
    if(exists(json, "iftrue")){
        analyze(json_get(json, "iftrue"), flag + 1, collon, fp);
    }
    if(exists(json, "iffalse")){
        fprintf(fp, "else"); 
        if(type_check((json_get(json, "iffalse"))) == IF){
            fprintf(fp, " ");
            analyze(json_get(json, "iffalse"), flag + 1, collon, fp);
        }else{
            fprintf(fp, "{\n");
            tap_line(flag+1, fp);
            analyze(json_get(json, "iffalse"), flag + 1, collon, fp);
            fprintf(fp, "\n");
            tap_line(flag, fp);
            fprintf(fp, "}");
        }
    }
}

void parse_binop(json_value json, int flag, int collon, FILE* fp){
    analyze(json_get(json, "left"), 0, COLLON_OFF, fp);
    fprintf(fp, " %s ", json_get_string(json, "op"));
    if(exists(json, "right")){
        int check = 0;
        if(type_check(json_get(json, "right")) == CONSTANT ){ check = 1;};
        analyze(json_get(json, "right"), 0, COLLON_OFF, fp);
    }
    if(collon){fprintf(fp, ";");}
}

void parse_id(json_value json, int flag, int collon, FILE* fp){
    fprintf(fp, "%s", json_get_string(json, "name"));
    if(collon == SIGN){fprintf(fp, ";");}
}

void parse_const(json_value json, int flag, int collon, FILE* fp){
    char* type = json_get_string(json, "type");

    if(strcmp("int", type)==0){fprintf(fp, "%s", json_get_string(json, "value"));}
    if(strcmp("char", type)==0){fprintf(fp, "%s",json_get_string(json, "value"));}
    if(strcmp("string", type)==0){fprintf(fp, "%s",json_get_string(json, "value"));}
    if(collon){fprintf(fp, ";");}
}

void parse_ident(json_value json, int flag, int collon, FILE* fp){
    json_value j_type = json_get(json, "names");
    json_value type_arr = json_get(j_type, 0);
    char* type = json_get_string(type_arr);
    if(flag == 100){
        fprintf(fp, "%s* ", type);
    }else{
        fprintf(fp, "%s ", type);
    }

}

void parse_typedec(json_value json, int flag, int collon, FILE* fp){
    if(exists(json, "type")){

        if(flag == 100){
            analyze(json_get(json, "type"),100, collon, fp); 
        }else{
            analyze(json_get(json, "type"),0, collon, fp);
        }  
        if(!exists(json, "declname")){
            json_value v = json_get(json, "type");
            v = json_get(v, "names");
            v = json_get(v, 0);
            char* ch = json_get_string(v);

            if(strcmp("void", ch) != 0){
                fprintf(fp, "argv%d", global_args);
                global_args +=1;
            }
        }else{
            fprintf(fp, "%s", json_get_string(json, "declname"));
        }
    }
    if(collon == COLLON_ON){fprintf(fp, ";");}
}


void parse_fundec(json_value json, int flag, int collon, FILE* fp){
    analyze(json_get(json, "type"), 0,  COLLON_OFF, fp);
    fprintf(fp, "( ");
    if(exists(json, "args")){analyze(json_get(json, "args"), 0, COLLON_OFF, fp);}
    fprintf(fp, " )");
    if(collon == COLLON_ON){fprintf(fp, ";");}
}


void parse_explist(json_value json, int flag, int collon, FILE* fp){
    if(exists(json, "exprs")){
        json_value expr_arr = json_get(json, "exprs");
        int exp_len = json_len(expr_arr);
        for(int i = 0; i < exp_len; i++){
            if(exp_len >= 1 && i > 0){fprintf(fp, " ,");}
            analyze(json_get(expr_arr, i), 0, COLLON_OFF, fp);
        }
    }
}


void parse_call(json_value json, int flag, int collon, FILE* fp){
    if(exists(json, "name")){analyze(json_get(json, "name"),0, COLLON_OFF, fp);}
    fprintf(fp, "( ");
    if(exists(json, "args")){analyze(json_get(json, "args"),0, collon, fp);}
    fprintf(fp, " )");
    if(collon != COLLON_OFF){fprintf(fp, ";");}
    // func() + 1;  func2(func1()); 
}

void parse_ptr(json_value json, int flag, int collon, FILE* fp){
    if(exists(json, "type")){analyze(json_get(json, "type"), 100, collon, fp);}
}

void parse_assign(json_value json, int flag, int collon, FILE* fp){
    analyze(json_get(json, "lvalue"), 0, COLLON_OFF, fp);
    fprintf(fp, " %s ", json_get_string(json, "op"));

    if(type_check(json_get(json, "rvalue")) == BINOP){
        analyze(json_get(json, "rvalue"), 0, COLLON_OFF, fp);
    }else{
        analyze(json_get(json, "rvalue"), 0, COLLON_OFF, fp);
    }
    // if(collon && type_check(json_get(json, "rvalue")) != FUNC_CALL){printf(";");}
    if(collon != COLLON_OFF){fprintf(fp, ";");}
}

void parse_arr(json_value json, int flag, int collon, FILE* fp){
    analyze(json_get(json,"name"),0, collon, fp);
    fprintf(fp, "[");
    if(exists(json, "subscript")){analyze(json_get(json, "subscript"), 0, COLLON_OFF, fp);}
    fprintf(fp, "]");
}

void parse_while(json_value json, int flag, int collon, FILE* fp){
    fprintf(fp, "while( ");
    analyze(json_get(json, "cond"), 0, COLLON_OFF, fp);
    fprintf(fp, " )");
    analyze(json_get(json, "stmt"), flag+1, collon, fp);
}

void parse_return(json_value json, int flag, int collon, FILE* fp){
    int check = 0;
    if(type_check(json_get(json, "expr")) == CONSTANT){ check = 1;}
    fprintf(fp, "return ");
    analyze(json_get(json, "expr"), check, SIGN, fp);
}

void parse_typename(json_value json, int flag, int collon, FILE* fp){
    analyze(json_get(json, "type"), 0, TYPE_NAME, fp);
}

void parse_param(json_value json, int flag, int collon, FILE* fp){
    json_value param_list = json_get(json, "params");
    int param_len = json_len(param_list);
    for(int i = 0; i < param_len; i++){
        analyze(json_get(param_list, i), 0, COLLON_OFF, fp);
        if(i < param_len -1){
            fprintf(fp, ", ");
        }
    }
}


void tap_line(int count, FILE* fp){
    for(int i = 0; i < count; i++){
        fprintf(fp, "\t");
    }
}


char* reading(const char* file_name)
{
  // file 크기 알아내기 
  FILE* fd = fopen(file_name, "r");
  if(fd == NULL) { 
    printf("File read fail."); 
    exit(-1); 
  }
  char num_test[100];
  int file_size = 0; 
  while(fgets(num_test, 100, fd) != NULL){
    int line = strlen(num_test);
    file_size += line;
  }
  fclose(fd);

  // file 내용 복사
  FILE* fp =fopen(file_name, "r");
  if(fp == NULL){
    printf("File2 read fail."); 
    exit(-1);
  }
  char* string = (char *)malloc(file_size);
  memset(string, '\0', file_size);
  while(fgets(num_test, 100, fp)){
    strcat(string, num_test);
  }
  fclose(fp);
  return string;
}

