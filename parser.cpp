#include <stdexcept>
#include "parser.hpp"
const char* tokenToString(Token token) {
    switch (token) {
        case Token::Eof: return "Eof";
        case Token::CloseBracket: return "CloseBracket";
        case Token::Ident: return "Ident";
        case Token::OpenPar: return "OpenPar";
        case Token::IntConst: return "IntConst";
        case Token::OpSub: return "OpSub";
        case Token::SemiColon: return "SemiColon";
        case Token::OpenBracket: return "OpenBracket";
        case Token::Comma: return "Comma";
        case Token::OpMod: return "OpMod";
        case Token::ClosePar: return "ClosePar";
        case Token::OpDiv: return "OpDiv";
        case Token::OpMul: return "OpMul";
        case Token::OpAdd: return "OpAdd";
        case Token::GREATER: return "GREATER";
        case Token::GREATER_EQUAL: return "GREATER_EQUAL";
        case Token::LESSTHAN: return "LESSTHAN";
        case Token::NOT: return "NOT";
        case Token::OR: return "OR";
        case Token::LESS_EQUAL: return "LESS_EQUAL";
        case Token::NOTEQUAL: return "NOTEQUAL";
        case Token::EQUAL: return "EQUAL";
        case Token::AND: return "AND";
        case Token::OpenCurly: return "OpenCurly";
        case Token::CloseCurly: return "CloseCurly";
        case Token::KwPrint: return "KwPrint";
        case Token::KwRead: return "KwRead";
        case Token::KwIf: return "KwIf";
        case Token::KwReturn: return "KwReturn";
        case Token::KwRef: return "KwRef";
        case Token::KwInt: return "KwInt";
        case Token::KwVoid: return "KwVoid";
        case Token::KwClass: return "KwClass";
        case Token::KwWhile: return "KwWhile";
        case Token::KwElse: return "KwElse";
        case Token::Comillas: return "Comillas";
        case Token::doubleEqual: return "doubleEqual";
        case Token::Hexa: return "Hexa";
        case Token::Bin: return "Bin";
        
       


        default: return "Unknown Token";
    }
}

void Parser::parse()
{
    curr_tk = lex.getNextToken();
    while (curr_tk != Token::Eof) {
        //boolExpr();
      //  std::cout << "Token: " << tokenToString(curr_tk) << std::endl; 
     /*  if (curr_tk != Token::SemiColon) {
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected semicolon");
        }*/

        program();
        curr_tk = lex.getNextToken();
    }

}

void Parser::program()

{

    std::cout << "Program Start: " << tokenToString(curr_tk) << std::endl;
   if(curr_tk != Token::KwClass){
    
       throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected class keyword");
   }
       //std::cout << "Token: " << tokenToString(curr_tk) << std::endl;

    curr_tk = lex.getNextToken();
     std::cout << "Token: " << tokenToString(curr_tk) << std::endl;
    if(curr_tk != Token::Ident){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected identifier");
    }
    curr_tk = lex.getNextToken();

    if(curr_tk != Token::OpenCurly){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open curly");
    }
    curr_tk = lex.getNextToken();
    std::cout << "class Ident { : " << tokenToString(curr_tk) << std::endl;
    while (curr_tk != Token::Eof && curr_tk != Token::CloseCurly) {
        VarDcl();

        std::cout << "se termino de inicializar el metodo/variable en class" << tokenToString(curr_tk) << std::endl;
    }
    if(curr_tk == Token::Eof){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": ERROR SE LLEGO AL LINAL DEL ARCHIVO, se esperaba un curly");
    }
    if(curr_tk != Token::CloseCurly){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close curly");
    }

}
//-------------------------------------------------------------------------------------------------


void Parser:: read_stmt()
{
    if(curr_tk == Token::KwRead){
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::OpenPar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open parenthesis");
        }
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::Ident){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected identifier");
        }
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::ClosePar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
        }
        
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::SemiColon){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
        }

        curr_tk = lex.getNextToken();
        
    }

     

}
void Parser:: print_stmt()
{
    std::cout << "Se encontro un print stmt: " << tokenToString(curr_tk) << std::endl;
    if(curr_tk == Token::KwPrint){
        std::cout << "*********" << tokenToString(curr_tk) << std::endl;
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::OpenPar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open parenthesis");
        }
        curr_tk = lex.getNextToken();
        std::cout << "Despues de open par: " << tokenToString(curr_tk) << std::endl;
       if(curr_tk == Token::Comillas){
        std::cout << "Literal string en print " << tokenToString(curr_tk) << std::endl;
curr_tk = lex.getNextToken();
        while(curr_tk != Token::Comillas ){
            curr_tk = lex.getNextToken();
            std::cout << "adentr de comillas " << tokenToString(curr_tk) << std::endl;

             if(curr_tk == Token::Eof){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected Comillas ");
        }
        }
       curr_tk = lex.getNextToken();

       }else{

                std::cout << "boolexpr en print: " << tokenToString(curr_tk) << std::endl;

        boolExpr();   
    }
       

    }      std::cout << "Despues de close par: " << tokenToString(curr_tk) << std::endl;

    if(curr_tk != Token::ClosePar){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
    }
    curr_tk = lex.getNextToken();
    if(curr_tk != Token::SemiColon){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected semicolon");
    }
    curr_tk = lex.getNextToken();
}

void Parser::return_stmt()
{
    if(curr_tk == Token::KwReturn){
        curr_tk = lex.getNextToken();
            boolExpr();
    }
    if(curr_tk != Token::SemiColon){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected semicolon");
    }
    curr_tk = lex.getNextToken();
}

void Parser::call_stmt()
{
    
        if(curr_tk != Token::OpenPar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open parenthesis");
        }

        curr_tk = lex.getNextToken();
        boolExpr();
        while(curr_tk == Token::Comma){
            curr_tk = lex.getNextToken();
            boolExpr();
        }

        if(curr_tk != Token::ClosePar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
        }
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::SemiColon){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected semicolon");
        }
        curr_tk = lex.getNextToken();
    


}

void Parser:: while_stmt()
{

    std::cout << "Se encontro un cilco while: " << tokenToString(curr_tk) << std::endl;

    if(curr_tk == Token::KwWhile){
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::OpenPar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open parenthesis");
        }
        curr_tk = lex.getNextToken();
  
        boolExpr();
        std::cout << "Despues de la expresion booleana va un close" << tokenToString(curr_tk) << std::endl;
        if(curr_tk != Token::ClosePar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
        }
        curr_tk = lex.getNextToken();
        std::cout << "Vamos al bloque" << tokenToString(curr_tk) << std::endl;
        block();



    }
    
    std::cout << "Se termino el ciclo " << tokenToString(curr_tk) << std::endl;

}

void Parser:: block()
{
    // revisar el bloque 
    if(curr_tk != Token::OpenCurly){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open curly");
    }
        curr_tk = lex.getNextToken();
std::cout << "dentro del bloque " << tokenToString(curr_tk) << std::endl;
        while(curr_tk != Token::CloseCurly ){
                  std::cout << "ejecucion de while en el statement: " << tokenToString(curr_tk) << std::endl;
            stmt();
       std::cout << "statmente del bloque terminado " << tokenToString(curr_tk) << std::endl;
  if(curr_tk == Token::Eof){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close curly");
        }
        }
      
        
        if(curr_tk != Token::CloseCurly){
          throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close curly");
        }
        

            curr_tk = lex.getNextToken();
            std::cout << "Se termino el bloque " << tokenToString(curr_tk) << std::endl;
        
       
    
}

void Parser::if_stmt()
{
    if(curr_tk == Token::KwIf){
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::OpenPar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open parenthesis");
        }
        curr_tk = lex.getNextToken();
        boolExpr();
        if(curr_tk != Token::ClosePar){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
        }
        curr_tk = lex.getNextToken();
        block();
        if(curr_tk == Token::KwElse){
            std::cout << "Se encontro un else: " << tokenToString(curr_tk) << std::endl;
            curr_tk = lex.getNextToken();
            block();
        }
    }
}


void Parser:: assign_stmt()
{
 
        if(curr_tk == Token::OpenBracket){
        curr_tk = lex.getNextToken();
        std::cout << "Se asigna dentro de un bracekt: " << tokenToString(curr_tk) << std::endl;
        boolExpr();
        std::cout << "se examino la expresion  siguiente debe ser close bracket " << tokenToString(curr_tk) << std::endl;
        if(curr_tk != Token::CloseBracket){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected Closing bracket");
        }
        curr_tk = lex.getNextToken();
        }
       
        if(curr_tk != Token::EQUAL){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected equal/assign operator");
        }
                std::cout << "=" << tokenToString(curr_tk) << std::endl;

        curr_tk = lex.getNextToken();

         std::cout << " validar la otra parte de la asignacion Ident = bool" << tokenToString(curr_tk) << std::endl;

        boolExpr();
                 std::cout << "expression bool evaluada" << tokenToString(curr_tk) << std::endl;

        if(curr_tk != Token::SemiColon){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected semicolon");
        }
        curr_tk = lex.getNextToken();
 std::cout << "Se termino la asignacion dentro de el void/Int" << tokenToString(curr_tk) << std::endl;
}



void Parser:: stmt()
{
    if(curr_tk == Token::KwRead){
        read_stmt();
    }else if(curr_tk == Token::KwPrint){
        print_stmt();
    }else if(curr_tk == Token::KwReturn){
        return_stmt();
    }else if(curr_tk == Token::KwWhile){
        while_stmt();
    }else if(curr_tk == Token::KwIf){
        if_stmt();
    }else if(curr_tk == Token::Ident){

    curr_tk = lex.getNextToken();
       std::cout << "STATMENT ES Ident: " << tokenToString(curr_tk) << std::endl;
    if(curr_tk == Token::OpenPar){
        std::cout << "Es un call stmt: " << tokenToString(curr_tk) << std::endl;
        call_stmt();
    }else if (curr_tk == Token::OpenBracket || curr_tk == Token::EQUAL){
        std::cout << "Es una asignacion ident = : " << tokenToString(curr_tk) << std::endl;
        assign_stmt();
    }
    }else{
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected statement");   
    }
}

void Parser::ParamDecl()
{
    std::cout << "Validar ParamDecl" << tokenToString(curr_tk) << std::endl;
    if(curr_tk == Token::KwRef){

        curr_tk = lex.getNextToken();
    }
    methodType();
    if(curr_tk != Token::Ident){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected identifier");
    }
    curr_tk = lex.getNextToken();
}

// void Parser::type()
// {
//     if(curr_tk != Token::KwInt){
//         throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected int keyword");
//     }
//     if(curr_tk== Token::OpenBracket){
//         curr_tk = lex.getNextToken();
//         if(curr_tk != Token::IntConst){
//             throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected int constant");
//         }
//         curr_tk = lex.getNextToken();
//         if(curr_tk != Token::CloseBracket){
//             throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close bracket");
//         }
        
//     }
//     curr_tk = lex.getNextToken();
// }

void Parser::paramList()
{
std::cout << "Validar lista de parametros" << tokenToString(curr_tk) << std::endl;
    ParamDecl();
    while(curr_tk == Token::Comma){
        curr_tk = lex.getNextToken();
        ParamDecl();
    }
}

void Parser::methodType()
{

std::cout << "Revisamos si es KwInt o Kw void: " << tokenToString(curr_tk) << std::endl;
    if(curr_tk == Token::KwInt){

           curr_tk = lex.getNextToken();
           std::cout << "Si tiene INT[const] : " << tokenToString(curr_tk) << std::endl;
           while(curr_tk == Token::OpenBracket){
                curr_tk = lex.getNextToken();
                        std::cout << "***Parser Method:open bracket " << tokenToString(curr_tk) << std::endl;

                if(curr_tk == Token::IntConst && curr_tk != Token::Hexa && curr_tk != Token::Bin){
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected int constant or hexa or bin");
                }

                if(curr_tk == Token::IntConst || curr_tk == Token::Hexa || curr_tk == Token::Bin){
               std::cout << "Se encontro un int const, hexa o bin: " << tokenToString(curr_tk) << std::endl;
                curr_tk = lex.getNextToken();
                }else{
                    throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected int constant");
                }

               

                if(curr_tk != Token::CloseBracket){
                     throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close bracket");
                }
                curr_tk = lex.getNextToken();
           }

    }
  else if (curr_tk == Token::KwVoid){
        std::cout << "Metodo de void: " << tokenToString(curr_tk) << std::endl;
          curr_tk = lex.getNextToken();
          std::cout << "despues de void va Ident: " << tokenToString(curr_tk) << std::endl;

  }else 
  throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected void or int keyword");
//Regresa a dcl();
}

void Parser::methodDecl(){
    methodType();
    if(curr_tk != Token::Ident){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected identifier");
    }
    curr_tk = lex.getNextToken();
    if(curr_tk != Token::OpenPar){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open parenthesis");
    }
    curr_tk = lex.getNextToken();
        paramList();
    if(curr_tk != Token::ClosePar){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
    }
    curr_tk = lex.getNextToken();
    if(curr_tk != Token::OpenCurly){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open curly");
    }
    curr_tk = lex.getNextToken();
   if(curr_tk == Token::KwInt){
       while(curr_tk == Token::KwInt){
           VarDcl();
       }
       
       if(curr_tk == Token::KwReturn){
        stmt();
        while(curr_tk == Token::KwReturn){
            stmt();

        }
    }
   }
   if(curr_tk != Token::CloseCurly){
       throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close curly");
   }
   curr_tk = lex.getNextToken();



}

void Parser::VarDcl(){
// Esto lo tengo maloo
 //methodType();
 //method decl()

    decl();
 std::cout << "verificamos si es ; o si Viene parenthesis  " << tokenToString(curr_tk) << std::endl;
if(curr_tk == Token::OpenPar){
           
//revisar parametros 
    curr_tk = lex.getNextToken();
std::cout << "veomos si tiene parametros  " << tokenToString(curr_tk) << std::endl;
    if(curr_tk != Token::ClosePar){
        std::cout << "si tiene params: " << tokenToString(curr_tk) << std::endl;
        paramList();
    }
//curr_tk = lex.getNextToken();
std::cout << "final de declaracion de parametros   " << tokenToString(curr_tk) << std::endl;
    if(curr_tk != Token::ClosePar){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected close parenthesis");
    }

    curr_tk = lex.getNextToken();
            std::cout << "Abrimos el curly, next: " << tokenToString(curr_tk) << std::endl;

    if(curr_tk != Token::OpenCurly){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open curly");
    }

     std::cout << "var o stmt? " << tokenToString(curr_tk) << std::endl;
while(curr_tk != Token::CloseCurly){

    if(curr_tk == Token::Eof){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected open curly");
    }
    std::cout << "dentro de un metodo { " << tokenToString(curr_tk) << std::endl;
    
    curr_tk = lex.getNextToken();
     std::cout << "sera vardel? o stmt " << tokenToString(curr_tk) << std::endl;

    if(curr_tk == Token::KwInt){
        while(curr_tk == Token::KwInt){

            std::cout << "se declara una int var   " << tokenToString(curr_tk) << std::endl;
            VarDcl();
        }

    }
    if(curr_tk == Token::KwIf || curr_tk == Token::KwWhile || curr_tk == Token::KwReturn || curr_tk == Token::KwPrint || curr_tk == Token::KwRead || curr_tk == Token::Ident){
        while(curr_tk == Token::KwIf || curr_tk == Token::KwWhile || curr_tk == Token::KwReturn || curr_tk == Token::KwPrint || curr_tk == Token::KwRead || curr_tk == Token::Ident){
           std::cout << "se encontro un statement dentro de un curly  " << tokenToString(curr_tk) << std::endl;
            stmt();
        }

    }
}
    
}else{

   
    while(curr_tk == Token::Comma){
         std::cout << "hay multiples variables " << tokenToString(curr_tk) << std::endl;
        curr_tk = lex.getNextToken();
        if(curr_tk != Token::Ident){
            throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected identifier");
        }
        curr_tk = lex.getNextToken();
    }
 std::cout << "terminamos con esta inicializacion " << tokenToString(curr_tk) << std::endl;
    if(curr_tk != Token::SemiColon){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected semicolon");
    }

  
}
curr_tk = lex.getNextToken();
}
void Parser::decl(){
      

    methodType();
           
std::cout << "Despues de saber si es Int, Int[]* o void: " << tokenToString(curr_tk) << std::endl;
    if(curr_tk != Token::Ident){
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected identifier");
    }
    curr_tk = lex.getNextToken();
  std::cout << "Se verifico que sea ident" << tokenToString(curr_tk) << std::endl;
}

//-------------------------------------------------------------------------------------------------
void Parser::boolExpr()
    {
      boolTerm();
      while(curr_tk == Token::OR) {
          curr_tk = lex.getNextToken();
          boolTerm();

      }
    }
    void Parser::boolTerm()
    {
        boolFactor();
        while(curr_tk == Token::AND) {
            curr_tk = lex.getNextToken();
            boolFactor();
        }
  
    }
    void Parser::boolFactor()
    {
        std::cout << "Not or Relation " << tokenToString(curr_tk) << std::endl;

        if (curr_tk == Token::NOT) {
            std::cout << "Token:Not " << tokenToString(curr_tk) << std::endl;
           
            curr_tk = lex.getNextToken();
          
            boolFactor();

        }else
        {

            
            relationExpr();


        }
       
    }
    void Parser::relationExpr()
    {
     // std::cout << "Token:relation:  " << tokenToString(curr_tk) << std::endl;
     
 arithExpr();


 while (curr_tk == Token::Ident||curr_tk == Token::doubleEqual || curr_tk == Token::NOTEQUAL || curr_tk == Token::LESSTHAN || curr_tk == Token::LESS_EQUAL || curr_tk == Token::GREATER || curr_tk == Token::GREATER_EQUAL) {
      
       std::cout << "Token:relation:  " << tokenToString(curr_tk) << std::endl;
     
       if(curr_tk == Token::doubleEqual){
            curr_tk = lex.getNextToken();
            arithExpr();
        }else if(curr_tk == Token::NOTEQUAL){
          
            curr_tk= lex.getNextToken();
            arithExpr();
        }else if(curr_tk == Token::LESSTHAN){
            curr_tk= lex.getNextToken();
            arithExpr();
        }else if(curr_tk == Token::LESS_EQUAL){
            curr_tk= lex.getNextToken();
            arithExpr();
        }else if(curr_tk == Token::GREATER){
         //   std::cout << "Token:Greater " << tokenToString(curr_tk) << std::endl;
            curr_tk= lex.getNextToken();
    //       std::cout << "Token:Greater " << tokenToString(curr_tk) << std::endl;

            arithExpr();
        }else if(curr_tk == Token::GREATER_EQUAL){
            curr_tk= lex.getNextToken();
            arithExpr();
        }
        else{
            
           arithExpr();

            //3==3 hasta el primer 3  llega y luego intenta devorverlo como final
            //por eso mas arriba habia puesto un curr_tk = lex.getNextToken(); para que no se quede en el mismo token
            //pero cunado es con not no se puede hacer eso porque entonce consume un token que no 

            
        }
 }
 std::cout << "OUT OF THE RELATION  " << tokenToString(curr_tk) << std::endl;
    
    }

//-------------------------------------------------------------------------------------------------
void Parser::arithExpr()
{
   //  std::cout << "Token: artith: " << tokenToString(curr_tk) << std::endl;
    term();

   
    while (curr_tk == Token::OpAdd || curr_tk == Token::OpSub) {
        curr_tk = lex.getNextToken();
        term();
    }
}

void Parser::term()
{
    factor();
    while (curr_tk == Token::OpMul || curr_tk == Token::OpDiv || curr_tk == Token::OpMod) {
        curr_tk = lex.getNextToken();
        factor();
    }
}

void Parser::factor()
{
    if (curr_tk == Token::OpAdd || curr_tk == Token::OpSub) {
        curr_tk = lex.getNextToken();
    }
      
    primary();
     std::cout << "Factor**: " << tokenToString(curr_tk) << std::endl; 
}

void Parser::primary()
{
    
    if (curr_tk == Token::IntConst || curr_tk == Token::Bin || curr_tk == Token::Hexa) {
               std::cout << "Se encontro un int const, hexa o bin: " << tokenToString(curr_tk) << std::endl;

        curr_tk = lex.getNextToken();
    } else if (curr_tk == Token::Ident) {
        curr_tk = lex.getNextToken();
        if (curr_tk == Token::OpenBracket) {
            curr_tk = lex.getNextToken();
                   std::cout << "primary - necesita ser inst cosnt " << tokenToString(curr_tk) << std::endl;
                    boolExpr();
            // if (curr_tk != Token::IntConst) {
            //     throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Expected int constant");
            // }
            // curr_tk = lex.getNextToken();

            if (curr_tk != Token::CloseBracket) {
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Missing close bracket");
            }
            curr_tk = lex.getNextToken();
        }else if(curr_tk == Token::OpenPar){
            curr_tk = lex.getNextToken();
           // arithExpr();
           boolExpr();
            while(curr_tk == Token::Comma){
                curr_tk = lex.getNextToken();
               // arithExpr();
                 boolExpr();
            }
            if (curr_tk != Token::ClosePar) {
                // std::cout << "*Token: Inside " << tokenToString(curr_tk) << std::endl; 
                throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Missing close parenthesis");

            }
            curr_tk = lex.getNextToken();
        }
    } else if (curr_tk == Token::OpenPar) {
       curr_tk = lex.getNextToken();
         //std::cout << "*Token: " << tokenToString(curr_tk) << std::endl; 
       //arithExpr();
         boolExpr();
       if (curr_tk != Token::ClosePar) {
         std::cout << "*Token: Outside  " << tokenToString(curr_tk) << std::endl; 
          throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) + ": Missing close parenthesis");
       }

       curr_tk = lex.getNextToken();
    } else {
        throw std::runtime_error("Line " + std::to_string(lex.getLineNo()) +
                                ": Expected int constant, identifier or open parenthesis, but found '"
                                + lex.getText() + "'");
    }
    
}
//-------------------------------------------------------------------------------------------------
    