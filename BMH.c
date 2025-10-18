#include <stdio.h>
#include <stdlib.h>
#define MAX_CHAR 256

void print_int_array(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf(" %3d", A[i]);
    }
    printf("\n");
}

int shift_array(char T[], char P[], int shift_vector[], int len_t, int len_p){
    for(int v = 0; v < MAX_CHAR; v++){
        shift_vector[v] = len_p;
    }
    for(int i = 0; i < len_p - 1; i++){
        shift_vector[P[i]] = len_p - i - 1;
    }
    print_int_array(shift_vector, len_t);
    return 0;
}

int bmh_search(char T[], char P[], int len_t, int len_p){
    int L[MAX_CHAR];
    shift_array(T, P, L, 20, 7);
    int s = 0, i = len_p - 1;
    while(s <= len_t - len_p){
        if(T[s + i] != P[i]){
            s  += L[T[s + len_p - 1]];
            i = len_p - 1;
        }
        else if(i == 0){
            return s;
        }
        else{
            i--;
        }
    }
    return -1;
}

int main(int argc, char *argv[]){
    char pattern[7] = "#sells#";
    char text[20] = "she#sells#sea#shells";
    int x = bmh_search(text, pattern, 20, 7);
    printf("%d", x);
}
