#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WORD_LENGTH 10

typedef struct Node{
    int data;
    char* word;
    struct Node *child;
    struct Node *sibling;
}NODE;

NODE* createNode(int data){
    NODE* newNode = (NODE*)malloc(sizeof(NODE));
    newNode->data = data;
    newNode->word = NULL;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

int readDictionary(char** words){
    FILE *fp;
    char word[MAX_WORD_LENGTH];
    int i = 0;
    fp = fopen("dictionary.txt", "r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    while(fscanf(fp, "%s", word) != EOF){
        words[i] = (char*)malloc(sizeof(char) * MAX_WORD_LENGTH);
        strcpy(words[i], word);
        i++;
    }
    fclose(fp);
    return i;
}

void printWords(char** words,int wordCounter){
    int i;
    for(i = 0; i < wordCounter; i++){
        printf("%d - %s\n", i+1, words[i]);
    }
    printf("\n");
}

char** setSymbols(){
    char** symbols;
    symbols = (char**)malloc(sizeof(char*) * 10);
    int i;
    for(i = 0; i < 5; i++){
        symbols[i] = (char*)malloc(sizeof(char) * 10);
    }
    symbols[0] = "#";
    symbols[1] = "çğüş";//This number is not supported
    symbols[2] = "abc";
    symbols[3] = "def";
    symbols[4] = "ghi";
    symbols[5] = "jkl";
    symbols[6] = "mno";
    symbols[7] = "pqrs";
    symbols[8] = "tuv";
    symbols[9] = "wxyz";
    return symbols;
}

void createTrie(NODE** root,char** words,int wordCounter, char** symbols){
    int i,j,k,isFound;

    for(i = 0; i < wordCounter; i++){
        NODE* current = *root;
        for(j = 0; j < strlen(words[i]); j++){
            isFound = 0;
            k = 0;
            while(!isFound && k < 10){
                if(strchr(symbols[k],words[i][j]) != NULL){
                    if(current->child == NULL){
                        printf("%d - ",k);
                        current->child = createNode(k);
                        current = current->child;
                    }else{
                        current = current->child;
                        while(current->data != k && current->sibling != NULL){
                            current = current->sibling;
                        }
                        if(current->data != k){
                            printf("%d - ",k);
                            current->sibling = createNode(k);
                            current = current->sibling;
                        }
                    }
                    isFound = 1;
                }
                k++;
            }
        }
        if(current->word != NULL){
            printf("%d - ",0);
            int flag = 0;
            NODE* tmp;
            while (!flag && current->child != NULL){
                tmp = current;
                current = current->child;
                while (current->data != 0 && current->sibling != NULL){
                    current = current->sibling;
                }
                if(current->data != 0){
                    current = tmp;
                    flag = 1;
                }
            }
            current->child = createNode(0);
            current->child->word = words[i];
        }else{
            current->word = words[i];
        }
        printf("(%s) - ",words[i]);
    }
}

void analyzeTheNumber(NODE* root,char* number){
    int i = 0,j,k;
    NODE* current = root;
    NODE* tmp;
    int flag = 0;

    while (!flag && i < strlen(number)){
        int digit = number[i] - '0';
        int isContainsZero = 0;
        current = current->child;
        tmp = current;
        while(current->data != digit && current->sibling != NULL){
            if(current->data == 0){
                isContainsZero = 1;
            }
            current = current->sibling;
        }
        if(current->data == 0){
            isContainsZero = 1;
        }
        if(current->data != digit && isContainsZero == 0){
            flag = 1;
        }else if(current->data != digit && isContainsZero == 1 ){
            current = tmp;
            while (current->data != 0){
                current = current->sibling;
            }
        }else if(current->data == digit){
            i++;
        }
    }

    if(!flag){
        if(current->word != NULL){
            printf("Found: %s\n",current->word);
            int flag2 = 0;
            while(!flag2 && current->child != NULL){
                current = current->child;
                while(current->data != 0 && current->sibling != NULL){
                    current = current->sibling;
                }
                if(current->data != 0){
                    flag2 = 1;
                }else{
                    printf("Found: %s\n",current->word);
                }
            }
        }else{
            printf("There is no word for this number in the dictionary.\n");
        }
    }else{
        printf("There is no word for this number in the dictionary.\n");
    }
}


int main(){

    char** words;
    int wordCounter = readDictionary(words);
    printWords(words,wordCounter);
    char** symbols = setSymbols();

    NODE* root = malloc(sizeof(NODE));

    createTrie(&root,words,wordCounter,symbols);

    int quit = 0;
    while(!quit){
        printf("\n\nEnter a number: ");
        char number[MAX_WORD_LENGTH];
        scanf("%s",number);

        analyzeTheNumber(root,number);

        printf("Do you want to quit? (1/0): ");
        scanf("%d",&quit);
    }

    return 0;
}
