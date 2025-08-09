#include <stdio.h>
int
main(int argc, char *argv[]) {
    int cents_amount;
    printf("Enter amount in cents: ");
    scanf("%d", &cents_amount);
    if (cents_amount<0 || cents_amount>99) {
        printf("Invalid input.");
        return 1;
    }
    int cents_types[6] = {50, 20, 10, 5, 2, 1};
    int coin_num;
    int coin_remaining = cents_amount;
    int i = 0;
    while(coin_remaining > 0) {
        coin_num = coin_remaining/cents_types[i];
        coin_remaining = coin_remaining%cents_types[i];
        if (coin_num == 1) {
            printf("give a %dc coin\n", cents_types[i]);
        }
        else if (coin_num == 2) {
            printf("give two %dc coins\n", cents_types[i]);
        }
        i++;
    } 
    printf("amount remaining: 0c\n");
}
