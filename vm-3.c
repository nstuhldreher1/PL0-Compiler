/* Nicholas Stuhldreher and Josh Gumprecht
* Homework 1
* 5/31/22
* Systems Software
*/
#include <stdio.h>
#include "compiler.h"

#define MAX_PAS_LENGTH 500

int base(int L);

typedef struct IR{
    int op;
    int L;
    int M;
}Instruction;

int bp;
int sp;
int pas[MAX_PAS_LENGTH];


void virtual_machine(instruction* code){
    //Initiate variables program counter, halt flag, and IR.
    int PC = 0;
    Instruction IR;
    int halt = 1;
    


    //File input
    int numInts = 0;
    int c;
    int i = 0;

    while(code[i].op != -1) {
        pas[numInts] = code[i].op;
        pas[numInts+1] = code[i].l;
        pas[numInts+2] = code[i].m;
        i++;
        numInts += 3;
    }

    //Initialize sp, bp, bp initial, and number of lexicographical levels, and indexes where bars go.
    sp = numInts - 1;
    bp = sp + 1;
    int bpInitial = bp;
    int numActivationRecords = 0;
    int barIndexes[MAX_PAS_LENGTH];

    //for barLevels index
    int j = 0;

    printf("                 PC  BP  SP     stack\n");
    printf("Initial values:  %d  %d  %d\n", PC, bp, sp);    
    
    
    while(halt == 1){
        //Fetch instruction
        IR.op = pas[PC];
        IR.L = pas[PC + 1];
        IR.M = pas[PC+ 2];
        
        PC += 3;

        //Execute instruction
        switch(IR.op){
            //LIT
            case 1:
                sp = sp + 1;
                pas[sp] = IR.M;
                
                //print values for table
                printf("LIT  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;

            //OPR
            case 2:
                //Operation Number
                switch(IR.M){

                    //RTN
                    case 0:
                        sp = bp - 1;
                        bp = pas[sp + 2];
                        PC = pas[sp + 3];
                        numActivationRecords--;
                        
                        //print
                        printf("RTN  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;
                        break;
                    //NEG
                    case 1:
                        pas[sp] = -1 * pas[sp];

                        printf("NEG  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;
                        break;
                        
                    //ADD
                    case 2:
                        sp = sp - 1;
                        pas[sp] = pas[sp] + pas[sp + 1];

                        printf("ADD  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;
                        break;
                    
                    //SUB
                    case 3:
                        sp = sp - 1;
                        pas[sp] = pas[sp] - pas[sp + 1];
                        
                        printf("SUB  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;
                        break;
                    //MUL 
                    case 4:
                        sp = sp - 1;
                        pas[sp] = pas[sp] * pas[sp + 1];
                        
                        printf("MUL  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //DIV
                    case 5:
                        sp = sp - 1;
                        pas[sp] = pas[sp] / pas[sp + 1];

                        printf("DIV  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //MOD
                    case 6:
                        sp = sp - 1;
                        pas[sp] = pas[sp] % pas[sp + 1];
                        
                        printf("MOD  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //EQL
                    case 7:
                        sp = sp - 1;
                        if(pas[sp] == pas[sp + 1]){
                            pas[sp] = 1;
                        }
                        else{
                            pas[sp] = 0;
                        }
                        printf("EQL  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //NEQ
                    case 8:
                        sp = sp - 1;
                        if(pas[sp] != pas[sp + 1]){
                            pas[sp] = 1;
                        }
                        else{
                            pas[sp] = 0;
                        }

                        printf("NEQ  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //LSS
                    case 9:
                        sp = sp - 1;
                        if(pas[sp] < pas[sp + 1]) {
                            pas[sp] = 1;
                        }
                        else{
                            pas[sp] = 0;
                        }

                        printf("LSS  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;   
                    //LEQ
                    case 10:
                        sp = sp - 1;
                        if(pas[sp] <= pas[sp + 1]){
                            pas[sp] = 1;
                        }
                        else{
                            pas[sp] = 0;
                        }
                        
                        printf("LEQ  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //GTR
                    case 11:
                        sp = sp - 1;
                        if(pas[sp] > pas[sp + 1]){
                            pas[sp] = 1;
                        }
                        else{
                            pas[sp] = 0;
                        }

                        printf("GTR  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                    //GEQ
                    case 12:
                        sp = sp - 1;
                        if(pas[sp] >= pas[sp + 1]) {
                            pas[sp] = 1;
                        }
                        else{
                            pas[sp] = 0;
                        }
                        
                        printf("GEQ  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                        for(int i = bpInitial; i <= sp; i++){
                            if(i == barIndexes[j] && barIndexes[j] != 0){
                                printf("  |");
                                j++;
                            }
                            printf("  %d", pas[i]);
                        }
                        printf("\n");
                        j = 0;

                        break;
                }

                break;
            
            //LOD
            case 3:
                sp = sp + 1;
                pas[sp] = pas[base(IR.L) + IR.M];

                //print values for table
                printf("LOD  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;
                
                break;

            //STO
            case 4:
                pas[base(IR.L) + IR.M] = pas[sp];
                sp = sp - 1;

                //print values for table
                printf("STO  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;
            
            //CAL
            case 5:
                
                barIndexes[numActivationRecords] = sp + 1;

                pas[sp + 1] = base(IR.L); //static link
                pas[sp + 2] = bp;   //dynamic link
                pas[sp + 3] = PC;   //return adress
                bp = sp + 1;
                PC = IR.M;

                numActivationRecords++;

                //print values for table
                printf("CAL  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;

            //INC
            case 6:
                //new activation record has been created so must store index of bar
                sp = sp + IR.M;
                
                //print values for table
                printf("INC  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;
            
            //JMP
            case 7:
                PC = IR.M;

                //print values for table
                printf("JMP  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;

            //JPC
            case 8:
                if(pas[sp] == 0){
                    PC = IR.M;
                }

                sp = sp - 1;

                //print values for table
                printf("JPC  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;

            //SYS
            case 9:
               
                switch(IR.M){
                    //Print 
                    case 1:
                        printf("Output result is: %d\n", pas[sp]);
                        sp = sp - 1;
                        break;
                    
                    //Scan
                    case 2:
                        sp = sp + 1;
                        printf("Please Enter an Integer: ");
                        scanf("%d", &pas[sp]);
                        break;
                    
                    //Halt
                    case 3:
                        halt = 0;
                        break;

                }

                //print values for table
                printf("SYS  %d  %d      %d  %d  %d   ", IR.L, IR.M, PC, bp, sp); 
                for(int i = bpInitial; i <= sp; i++){
                    if(i == barIndexes[j] && barIndexes[j] != 0){
                        printf("  |");
                        j++;
                    }
                    printf("  %d", pas[i]);
                }
                printf("\n");
                j = 0;

                break;

        }

 
    }

}


int base(int L)
{
    int arb = bp; // arb = activation record base
    while ( L > 0)     //find base L levels down
    {
        arb = pas[arb];
        L--;
    }
    return arb;
}
