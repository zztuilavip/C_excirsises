#include <stdio.h>

int main(int argc, char *argv[]) {
    double inp, outp;
    int func = 0;

    // Prompt user until valid choice (1, 2, or 3)
    printf("Choose your conversion function:\n");
    printf("1. Temperature\n2. Length\n3. Mass\n");
    while (func < 1 || func > 3) {
        printf("Enter choice (1-3): ");
        if (scanf("%d", &func) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            func = 0; // reset
        }
    }

    // Units for input and output for each function
    char unit_in[3] = {'F', 'M', 'P'};  // Fahrenheit, Miles, Pounds
    char unit_out[3] = {'C', 'K', 'G'}; // Celsius, Kilometers, Grams

    // Conversion ratios and constants for: Temperature, Length, Mass
    // Temperature: (F - 32) * 5/9 = C -> ratio = 5/9 ~ 0.5556, constant = -32 (subtract)
    // Length: miles to kilometers (multiply by 1.609)
    // Mass: pounds to grams (multiply by 453.592)
    double ratio[3] = {0.5556, 1.609, 453.592};
    int constant[3] = {-32, 0, 0};

    char *arr[3] = {"temperature", "distance", "mass"};

    printf("Enter a quantity: ");
    scanf("%lf", &inp);

    // Calculate output
    outp = (inp + constant[func - 1]) * ratio[func - 1];

    printf("The %s %.2lf%c converts to %.2lf%c\n", arr[func - 1], inp, unit_in[func - 1], outp, unit_out[func - 1]);

    return 0;
}
