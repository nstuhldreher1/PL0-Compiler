/*
Nicholas Stuhldreher and Josh Gumprecht
Homework 2
6/25/22
Systems Software
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "compiler.h"

#define TOKEN_SIZE 500


lexeme identifyWord(char * buffer);
lexeme identifyNum(char*  buffer);
void identifySymbol(char* buffer);
void printTokens();
void analyzeSymbols(char* buffer);
char getChar(char * input);
void ungetChar();


lexeme *  list;
int i;
int indexInput = 0;
int errorflag = 0;
//number -> digit(digit)*
//identifier -> letter(letter|digit)*
//int 5ab = 5
//var x := 5 int x = 5

lexeme  * lexical_analyzer(char *input){
    
    char c;
    
    int j;
    int ASCII;
    i = 0;
    char buffer[50];

    list = malloc(sizeof(lexeme) * TOKEN_SIZE);

    
    //Loops until EOF
    while((c = getChar(input)) != '\0'){
        j = 1;

        memset(buffer, 0, sizeof(buffer));
        ASCII = c; 
       if(isspace(ASCII) > 0 || iscntrl(ASCII) > 0){
            continue;
        }
        
        
        //reset buffer
        
        //If its a letter
        if(isalpha(c) != 0){

            buffer[0] = c;

            //Create buffer of string
            while((isdigit(c) > 0) || (isalpha(c) > 0)){
                c = getChar(input); 
                if(isalpha(c) == 0 && isdigit(c) == 0){
                    ungetChar();
                    break;
                }
                buffer[j] = c;
                j++;
                
                //printf("%c ", c);
            }

           // printf("%s\n", buffer);

            //create token
           list[i] = identifyWord(buffer);


            if(ispunct(list[i - 1].name[0]) == 1 || list[i - 1].value){

            }

        }
        
        
        //If its a digit
        else if(isdigit(c) != 0){

            buffer[0] = c;

            //Create buffer of number
            while(isdigit(c) > 0){
                c = getChar(input); 
                if(isdigit(c) == 0){
                    ungetChar();
                    break;
                }
                buffer[j] = c;
                j++;
            }
            

            //create token
            list[i] = identifyNum(buffer);

            //Check for invalid identifier
            c = getChar(input);
            if(isalpha(c) > 0){
                        list[i].error_type = 1;
                        
            }
            ungetChar();

        }
        

        //If its a symbol
        else{
            
            buffer[0] = c;


            //Create buffer of symbol
            while(isdigit(c) == 0 && isalpha(c)==0 && isspace(c) == 0){
                c = getChar(input); 

                if(ispunct(c) == 0){
                    ungetChar();
                    break;
                }
                buffer[j] = c;

                j++;
                
            }
            if(isalpha(c) > 0){
                        list[i].error_type = 1;
            }
           identifySymbol(buffer);


            //If pointer hits a comment
            if(list[i].error_type == 6){
                char current;

                //Loop through text file until end comment is found. If not found neverending comment
                while((current = getChar(input)) != '\0'){
                    
                    if(current == '*'){
                        current = getChar(input);
                        if(current == '/'){
                            break;
                        }
                    }
                }
                //Check for never ending comment
                if(current == '\0'){
                        list[i].error_type = 5;
                        ungetChar();
                }
            }

        }

            i++;
        
        
    }
    
    printTokens(list, i);
    //if error retrun null
    if(errorflag == 1){
        list = NULL;
        return list;
    }else{
        return list;
    }
    
}


//Generate a token for a word
 lexeme identifyWord(char* buffer){
    lexeme answer;
    int i = 0;



    if(strlen(buffer) > 11){
        answer.error_type = 3;
    }
    else{
        if(strcmp("const", buffer) == 0){
            
            strcpy(answer.name, "const");
            answer.type = constsym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("var", buffer) == 0){

            strcpy(answer.name, "var");
            answer.type = varsym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("procedure", buffer) == 0){
            
            strcpy(answer.name, "procedure");
            answer.type = procsym;
            answer.error_type = 0;
            return answer;
            
        }
        else if(strcmp("call", buffer) == 0){

            strcpy(answer.name, "call");
            answer.type = callsym;
            answer.error_type = 0;
            return answer;
            
        }
        else if(strcmp("begin", buffer) == 0){
            
            strcpy(answer.name, "begin");
            answer.type = beginsym;
            answer.error_type = 0;
            return answer;
            
        }
        else if(strcmp("end", buffer) == 0){
            
            strcpy(answer.name, "end");
            answer.type = endsym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("if", buffer) == 0){
            
            strcpy(answer.name, "if");
            answer.type = ifsym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("else", buffer) == 0){
            
            strcpy(answer.name, "else");
            answer.type = elsesym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("while", buffer) == 0){
            
            strcpy(answer.name, "while");
            answer.type = whilesym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("do", buffer) == 0){
            
            strcpy(answer.name, "do");
            answer.type = dosym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("read", buffer) == 0){
            
            strcpy(answer.name, "read");
            answer.type = readsym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("write", buffer) == 0){
            
            strcpy(answer.name, "write");
            answer.type = writesym;
            answer.error_type = 0;
            return answer;

        }
        else if(strcmp("then", buffer) == 0){
            
            strcpy(answer.name, "then");
            answer.type = thensym;
            answer.error_type = 0;
            return answer;

        }
        else{

            
            strcpy(answer.name, buffer);
            answer.type = identsym;
            answer.error_type = 0;
            return answer;

        }

    }
    return answer;
    
}

//Generate Number token
lexeme identifyNum(char * buffer) {
    lexeme token;
    int i;

    if(strlen(buffer) > 5) {
        token.error_type = 2;
    }
    else {
        token.error_type = 0;
        int j = 0;
        token.value = atoi(buffer);
        token.type = numbersym;
    }
    return token;
}

//If it is symbol generate token
void identifySymbol(char* buffer){
    int n = strlen(buffer);
for(int j = 0; j < n; j++){
    
    

        //if more then one symbol
        if(buffer[j] == '!'){
            if(j != j-2 && buffer[j+1] == '='){

                strcpy(list[i].name, "!=");
                list[i].type = neqsym;
                list[i].error_type = 0;
                j++;

            }
            else{
                list[i].error_type = 4;
            }
        }
        else if(buffer[j] == ':'){
            if(j != j-2 && buffer[j+1] == '='){

                strcpy(list[i].name, ":=");
                list[i].type = becomessym;
                list[i].error_type = 0;
                j++;

            }
            else{
                list[i].error_type = 4;
            }
        }
        else if(buffer[j] == '>'){
            if(j != j-2 && buffer[j+1] == '=') {
                strcpy(list[i].name, ">=");
                list[i].type = geqsym;
                list[i].error_type = 0;
                j++;
            }
            else{
                strcpy(list[i].name, ">");
                list[i].type = gtrsym;
                list[i].error_type = 0;
            }
        }
        else if(buffer[j] == '<'){
            if(j != j-2 && buffer[j+1] == '=' ) {
                strcpy(list[i].name, "<=");
                list[i].type = leqsym;
                list[i].error_type = 0;
                j++;
            }
            else{
                strcpy(list[i].name, "<");
                list[i].type = lessym;
                list[i].error_type = 0;
            }
        }
        else if(buffer[j] == '/'){
            if(j != j-2 && buffer[j+1] == '*') {
                list[i].error_type = 6;
                return;
            }
            else if(j != j-2 && buffer[j+1] == '=' ) {
                strcpy(list[i].name, "/=");
                list[i].type = divassignsym;
                list[i].error_type = 0;
                j++;
            }
            else {
            strcpy(list[i].name, "/");
            list[i].type = slashsym;
            list[i].error_type = 0;
            }
        }
        
        
         else if(buffer[j] == '+'){

            if(j != j-2 && buffer[j+1] == '=' ) {
                strcpy(list[i].name, "+=");
                list[i].type = addassignsym;
                list[i].error_type = 0;
                j++;
            }
            else{
                strcpy(list[i].name, "+");
                list[i].type = plussym;
                list[i].error_type = 0;
            }

        }
        else if(buffer[j] == '-'){
            
            if(j != j-2 && buffer[j+1] == '=' ) {
                strcpy(list[i].name, "-=");
                list[i].type = subassignsym;
                list[i].error_type = 0;
                j++;
            }
            else{
                strcpy(list[i].name, "-");
                list[i].type = minussym;
                list[i].error_type = 0;
            }
        }
        else if(buffer[j] == '*'){

            if(j != j-2 && buffer[j+1] == '=' ) {
                strcpy(list[i].name, "*=");
                list[i].type = timesassignsym;
                list[i].error_type = 0;
                j++;
            }
            else{
                strcpy(list[i].name, "*");
                list[i].type = multsym;
                list[i].error_type = 0;
            }
            
        }
        else if(buffer[j] == '%'){

            if(j != j-2 && buffer[j+1] == '=' ) {
                strcpy(list[i].name, "%=");
                list[i].type = modassignsym;
                list[i].error_type = 0;
                j++;
            }
            else{
                strcpy(list[i].name, "%");
                list[i].type = modsym;
                list[i].error_type = 0;
            }
            
        }

        //One symbol
        else if(buffer[j] == '('){

            strcpy(list[i].name, "(");
            list[i].type = lparentsym;
            list[i].error_type = 0;
            
        }
        else if(buffer[j] == ')'){

            strcpy(list[i].name, ")");
            list[i].type = rparentsym;
            list[i].error_type = 0;
            
        }
        else if(buffer[j] == ','){
            
            strcpy(list[i].name, ",");
            list[i].type = commasym;
            list[i].error_type = 0;

        }
        else if(buffer[j] == ';'){
            strcpy(list[i].name, ";");
            list[i].type = semicolonsym;
            list[i].error_type = 0;
        }
        else if(buffer[j] == '='){
            strcpy(list[i].name, "=");
            list[i].type = eqlsym;
            list[i].error_type = 0;
        }
        else if(buffer[j] == '.'){
            strcpy(list[i].name, ".");
            list[i].type = periodsym;
            list[i].error_type = 0;
        }
        else if (buffer[j] == '>'){
            strcpy(list[i].name, ">");
            list[i].type = gtrsym;
            list[i].error_type = 0;
        }
        else if(buffer[j] == '<'){
            strcpy(list[i].name, "<");
            list[i].type = lessym;
            list[i].error_type = 0;
        }
        else{
            list[i].error_type = 4;
        }

        i++;
    }

    i-= 1;
    
}

void printTokens(){
    printf("Lexeme Table:\n");
    printf("lexeme           token type\n");

    for(int j =0; j < i; j++){
        if(list[j].error_type > 0){
            if(list[j].error_type == 1){
                printf("Lexical Analyzer Error: Invalid Identifier\n");
            }
            else if(list[j].error_type == 2){
                printf("Lexical Analyzer Error: Number Length\n");
            }
            else if(list[j].error_type == 3){
                printf("Lexical Analyzer Error: Identifier Length\n");
            }
            else if(list[j].error_type == 4){
                printf("Lexical Analyzer Error: Invalid Symbol\n");
            }
            else if(list[j].error_type == 5){
                printf("Lexical Analyzer Error: Never Ending Comment\n");
            }
        }

        else if(list[j].type == numbersym){
            printf("%11d %11d\n", list[j].value, list[j].type );
        }
        else{
            printf("%11s %11d\n", list[j].name, list[j].type);
        }
    }

    printf("Token List:\n");
    for(int j = 0; j < i; j++){
        if(list[j].error_type > 0){
            errorflag = 1;
            continue;
        }
        if(list[j].type == numbersym){
            printf("%d %d ", list[j].type, list[j].value);
        }
        else if(list[j].type == identsym){
            printf("%d %s ", list[j].type, list[j].name);
        }
        else{
            printf("%d ", list[j].type);
        }
    }
    printf("\n");
}

char getChar(char * input){
    char c;
    c = input[indexInput];
    indexInput++;
    return c;
}

void ungetChar(){
    indexInput--;
}



