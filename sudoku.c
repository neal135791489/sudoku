#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct record record;
typedef struct recordStack recordStack;
struct record{
    int step;
    int value;
};
struct recordStack{
    record **stack;
    int top;
};

void get_two_dimension(int** data, char *filename){   
    FILE* stream = fopen(filename, "r");
    char line[1024];
    int i = 0;
    while (fgets(line, 1024, stream)){//逐行讀取
        int j = 0;
        char *tok = strtok(line, ","); 
        for (;tok && *tok; j++, tok = strtok(NULL, ",\n")){
            data[i][j] = atoi(tok);//轉換成浮點數 
        }//字符串拆分操作 
        i++;
    }   
    fclose(stream);
}   
int get_row(char *filename){//return number of row
    char line[1024];
    int i = 0;
    FILE *stream = fopen(filename, "r");
    while(fgets(line, 1024, stream)){
        i++;
    }
    fclose(stream);
    return i;
}
int get_col(char* filename){
    char line[1024];
    int i = 0;
    FILE *stream = fopen(filename, "r");
    fgets(line, 1024, stream);
    char* token = strtok(line, ",");
    while(token){
        token = strtok(NULL, ",");
        i++;
    }
    fclose(stream);
    return i;
}
void print_table(int **table, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            printf("%d, ", table[i][j]);
        }
        printf("\n");
    }
}
int get_block_num(int row, int col){
    //012
    //345
    //678
    int r = row / 3;
    int c = col / 3;
    return r * 3 + c;
}
void check_able_num(int** table, int row, int col, int **rowCheck, int **colCheck, int **blockCheck){
    int blockNum;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(table[i][j] != 0){
                rowCheck[i][table[i][j] - 1] = 1;
                colCheck[j][table[i][j] - 1] = 1;
                blockNum = get_block_num(i, j);
                blockCheck[blockNum][table[i][j] - 1] = 1;
            }
        }
    }
}
int goto_next(int** table, int step, int **rowCheck, int **colCheck, int **blockCheck, recordStack *rStack){
    int x = step / 9, y = step % 9, exit;
    if(step == 81){ 
        printf("done!");
        return 1;
    }
    if(table[x][y] == 0){
        int chenged = 0, value = 0;
        int blockNum = get_block_num(x, y);
        for(int i = 0; i < 9; i++){
            if(rowCheck[x][i] == 0 && colCheck[y][i] == 0 && blockCheck[blockNum][i] == 0){
                value = i;
                table[x][y] = i + 1;
                rowCheck[x][value] = 1;
                colCheck[y][value] = 1;
                blockCheck[blockNum][value] = 1;
                chenged = 1;
                record* rd = (record *)malloc(sizeof(record));
                rd->step = step;
                rd->value = i;
                printf("step: %d, value: %d\n", step, i);
                rStack->stack[++rStack->top] = rd;
                printf("go to next step\n");
                exit = goto_next(table, step+1, rowCheck, colCheck, blockCheck, rStack);
                if(exit == 1) 
                    return 1;
                rd = rStack->stack[rStack->top--];
                int x = rd->step / 9, y = rd->step % 9, blockNum = get_block_num(x, y), value = rd->value;
                table[x][y] = 0;
                rowCheck[x][value] = 0;
                colCheck[y][value] = 0;
                blockCheck[blockNum][value] = 0;
                printf("go back step : %d, value: %d\n ", step, value);
                printf("top:%d\n", rStack->top);
            }
        }
        printf("no way\n");
        return 0;
    }
    else{
        printf("the step is not 0\n");
        exit = goto_next(table, step+1, rowCheck, colCheck, blockCheck, rStack);
        if(exit) 
            return 1;
    }
}
int main(){
    char filename[] = "sample2.csv";
    int col = 0, row = 0;
    row = get_row(filename);
    col = get_col(filename);
    int **table = (int **)malloc(row * sizeof(int *));
    for(int i = 0; i < row; i++)
        table[i] = (int *)malloc(col * sizeof(int));
    get_two_dimension(table, filename);
    print_table(table, row, col); 
    int step = 0;
    int **rowCheck = (int **)malloc(row * sizeof(int *));
    int **colCheck = (int **)malloc(row * sizeof(int *));
    int **blockCheck = (int **)malloc(row * sizeof(int *));
    recordStack* rStack = (recordStack *)malloc(sizeof(recordStack));
    rStack->stack = (record **)malloc(100 * sizeof(record *));
    rStack->top = -1;
    for(int i = 0; i < row; i++){
        rowCheck[i] = (int *)malloc(col * sizeof(int));
        colCheck[i] = (int *)malloc(col * sizeof(int));
        blockCheck[i] = (int *)malloc(col * sizeof(int));
        for(int j = 0; j < col; j++){
            rowCheck[i][j] = 0;
            colCheck[i][j] = 0;
            blockCheck[i][j] = 0;
        }
    }
    check_able_num(table, row, col, rowCheck, colCheck, blockCheck);
    goto_next(table, step, rowCheck, colCheck, blockCheck, rStack);
    printf("\n");
    print_table(table, row, col);
}