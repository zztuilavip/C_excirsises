#include <stdlib.h>
#include <stdio.h>

void print_int_array(int A[], int n);
int prefix_array(int F[], char P[], int len_p);

int prefix_array(int failure_array[], char P[], int len_p){
    int s = 2;
    int c = 0;
    failure_array[0] = -1;
    failure_array[1] = 0;
    while(s < len_p){
        if(P[c] == P[s - 1]){
            c++;
            failure_array[s] = c;
            s++;
        }
        else if(c > 0){
            c = failure_array[c];
        }
        else{
            failure_array[s] = 0;
            s++;
        }
    }
    return 0;
}

void print_int_array(int A[], int n) {
    for (int i = 0; i < n; i++) {
        printf(" %3d", A[i]);
    }
}

int kmp_search(char T[], char P[], int len_t, int len_p){
    int F[len_p];
    prefix_array(F, P, len_p);
    int s = 0, i = 0;
    while (s < len_t - len_p){
        if(T[s + i] == P[i]){
            i++;
            if(i == len_p){
                return s;
            }
        }
        else{
            s = s + i - F[i];
            i = F[i] > 0 ? F[i] : 0;
        }
    }
    return -1;
}

int main(int argc, char *argv[]){
    char pattern[7] = "#sells#";
    char text[20] = "she#sells#sea#shells";
    int s = kmp_search(text, pattern, 20, 7);
    printf("%d", s);
}
