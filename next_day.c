#include <stdio.h>
int is_leap_year(int year) {
	return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ;
}

int date_in_month(int month, int year) { 
	int length_of_month;
if (month == 2) {
   if (is_leap_year(year)) {
	 length_of_month = 29;
   }
   else {
	length_of_month = 28;
   }
}
 else if (month == 4 || month == 6 ||
           month == 9 || month == 11) {
    length_of_month = 30;
} 
else if (month == 1 || month == 3 ||
			month == 5 || month == 7 || month ==8 ||
			month==10 || month ==12){
    length_of_month = 31;
}
else {length_of_month = -1;
	}
 return length_of_month;
}
void next_day(int *date, int *month, int *year) {
	int day_in_month = date_in_month(*month, *year);
	if (day_in_month == -1){
		printf("Invalid input!");
		return;
	}
	if (*date < day_in_month){
		(*date) ++;
	}
	else {
		*date = 1;
		if (*month < 12) {
			(*month) ++;
		}
		else {
			(*month) = 1;
			(*year) ++;
		}
	}
}
int
main(int argc, char *argv[]) {
	int date, month, year;
	printf("Enter a date in dd/mm/yyyy format: ");
	scanf("%d/%d/%d", &date, &month, &year);
	int length_of_month = date_in_month(month, year);
	if ((date<1)||(date>length_of_month)||(length_of_month==-1)) {
		printf("Invalid input.");
		return 1;
	}
	next_day(&date, &month, &year);
	printf("The date tomorrow is: %02d/%02d/%04d", date, month, year);
	return 0;
}
