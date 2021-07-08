// #include <avr/io.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <util/delay.h>
// #include <avr/sfr_defs.h>
// #include <math.h>
// #include <avr/interrupt.h>
// #include <avr/eeprom.h>
// #include "HD44780.h"

// #ifndef _BV
// #define _BV(bit) (1 << (bit))
// #endif

// #ifndef inb
// #define inb(addr) (addr)
// #endif

// #ifndef outb
// #define outb(addr, data) addr = (data)
// #endif

// #ifndef sbi
// #define sbi(reg, bit) reg |= (_BV(bit))
// #endif

// #ifndef cbi
// #define cbi(reg, bit) reg &= ~(_BV(bit))
// #endif

// #ifndef tbi
// #define tbi(reg, bit) reg ^= (_BV(bit))
// #endif

// // MIN/MAX/ABS macros
// #define MIN(a, b) ((a < b) ? (a) : (b))
// #define MAX(a, b) ((a > b) ? (a) : (b))
// #define ABS(x) ((x > 0) ? (x) : (-x))

// char* choosen(char *str)
// {
// 	char *strcat(char *dest, const char *src);
// 	size_t len = strlen(str);
// 	char pause = ' ';
// 	for (int i = 0; i < 12 - len; i++)
// 	{
// 		strcat(str, pause);
// 	}
// 	char *tmp = "  	X";
// 	// strcat(str, tmp);
// 	return strcat(str, tmp);
// }

// void info()
// {
// 	char *hello = "Program PTM 2021";
// 	char *index = "252871 252839";
// 	LCD_Clear();
// 	LCD_GoTo(0, 0);
// 	LCD_WriteText(hello);
// 	LCD_GoTo(0, 1);
// 	LCD_WriteText(index);
// 	_delay_ms(4000);
// }

// void first_run_of_program(bool first_run)
// {
// 	while (first_run)
// 	{
// 		info();
// 		first_run = false;
// 	}
// }

// void menu()
// {
// 	LCD_Clear();
// 	char *liczby = "1. Liczby";
// 	char *stoper = "2. Stoper";
// 	char *zegar = "3. Zegar";
// 	char *miernik = "4. Miernik";
// 	char *buff_1;
// 	char *buff_2;

// 	LCD_GoTo(0, 0);
// 	buff_1 = choosen(liczby);
// 	LCD_WriteText(buff_1);
// 	LCD_GoTo(0, 1);
// 	buff_2 = stoper;
// 	LCD_WriteText(buff_2);

// }

// int main()
// {
// 	LCD_Initalize();
// 	LCD_Home();
// 	bool first_run = true;

// 	while (1)
// 	{
// 		first_run_of_program(first_run);
// 	}
// }


#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/iom32.h>

#include "HD44780.h"

//Dominika Arkabus  252839
//Piotr Kubon 		252871

#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

int n_buff=-99;
int number_1 = 0;
int number_2 = 0;
int operation = 0;
int ready = 0;

char buff[8];

int s=0;
int m=0;
int h=0;
char matrix_chars[] = {'7','8','9','-','4','5','6','+','1','2','3','C','*','0','#','='};

//funkcja do resetu zmiennych - wyzerowanie kalkulatora
void reset_cc(){
	ready=0;
	number_1=0;
	number_2=0;
	operation=0;
	n_buff = -99;
}
//funkcja zwraca zinkrementowany czas
void get_time(){
	s++;
	if(s>59)
	{
		m++;
		s = 0;
		if(m>59)
		{
			h++;
			m = 0;
			if(h>23){
				h=0;
			}
		}
	}
}
//funkcja do wy�wietlania czasu w formacie
void set_time(){
	if(h<10){
		sprintf(buff,"0%d:",h);
	}
	else{
		sprintf(buff,"%d:",h);
	}
	if(m<10){
		sprintf(&buff[3],"0%d:",m);
	}
	else{
		sprintf(&buff[3],"%d:",m);
	}
	if(s<10){
		sprintf(&buff[6],"0%d",s);
	}
	else{
		sprintf(&buff[6],"%d",s);
	}

}

//funkcja do realizacji zadania na 3
void zadanie_3(){
	while(1){

		get_time();
		set_time();

		LCD_Clear();
		LCD_GoTo(0, 0);
		LCD_WriteText(buff);
		LCD_GoTo(0, 1);
		LCD_WriteText("252871    252839");

		_delay_ms(1000);
	}
}

//funkcja do odczytywania ktory przycisk wcisnieto -> 0-15 wierszami
int get_matrix_button_number(){
	int n = -1;
	for(int i=0;i<4;i++){
		PORTD = (0x01<<i);				//kolejno daje 1 na d0, d1, d2, d3
		_delay_ms(10);
		if (bit_is_set(PIND, PD4)) {
			n=0 + (4*i);
		}
		if (bit_is_set(PIND, PD5)) {
			n=1 + (4*i);
		}
		if (bit_is_set(PIND, PD6)) {
			n=2 + (4*i);
		}
		if (bit_is_set(PIND, PD7)) {
			n=3 + (4*i);
		}
	}
	return n;
}
//funkcja do konwersji nm. wcisnietego przycisku na symbol przycisku
void convert(int button_number){
	sprintf(buff,"-1");
	if(button_number>=0){
		sprintf(buff,"Wcisnieto: %c ",matrix_chars[button_number]);
	}
}

//funkcja do realizacji zadania 4
void zadanie_4(){
	PORTD = 0x00;

	while(1){
		int n = get_matrix_button_number();
		if(n!=n_buff){
			convert(n);

			LCD_Clear();
			LCD_GoTo(0, 1);
			LCD_WriteText(buff);

			_delay_ms(10);
			n_buff = n;
		}
	}
}

//funkcja do powiekszania liczb
void add_to_number(int nn){
	if(operation == 0){
		if(number_1 < 1000){
	       	number_1 = (number_1*10) + nn;
		}
    }
    else{
    	if(number_2 < 1000){
    		number_2 = (number_2*10) + nn;
    	}
    }
}
//funkcja do realizacji przeznaczenia po wcisnieciu przycisku
void set_sth(int button_number){
	if(button_number>=0){
		switch( button_number )
		{
			case 0:
				add_to_number(7);
				break;
			case 1:
				add_to_number(8);
				break;
			case 2:
				add_to_number(9);
				break;
			case 4:
				add_to_number(4);
				break;
			case 5:
				add_to_number(5);
				break;
			case 6:
				add_to_number(6);
				break;
			case 8:
				add_to_number(1);
				break;
			case 9:
				add_to_number(2);
				break;
			case 10:
				add_to_number(3);
				break;
			case 13:
				add_to_number(0);
				break;

			case 3:									//	-
				operation = 1;
				ready=0;
				break;
			case 7:									//	+
				operation = 2;
				ready=0;
				break;
			case 12:								//	*
				operation = 3;
				ready=0;
				break;
			case 11:								//	C
				reset_cc();
				break;
			case 15:								//	=
				ready++;
				break;
		}
	}
}
//funkcja zwracajaca symbol operacji
char get_operation_symbol(){
	switch( operation )
	{
		case 0:
			return ' ';
		case 1:
			return '-';
		case 2:
			return '+';
		case 3:
			return '*';
	}
	return '?';
}
//funkcja do wyswietlania info z kalkulatora
void plot(){
	if(n_buff >= 0){						// to znaczy - nacisjelismy cokolwiek
		if(number_2 == 0){
			sprintf(buff,"%d%c",number_1,get_operation_symbol());
		}
		else{
			sprintf(buff,"%d%c%d",number_1,get_operation_symbol(),number_2);
		}
	}
	else{
		sprintf(buff," ");
	}
}
//funkcja do wykonywania operacji +-*C
void make_operation(){
	switch( ready )
	{
		case 1:
			if(operation == 1){
				number_1=number_1-number_2;
			}
			if(operation == 2){
				number_1=number_1+number_2;
			}
			if(operation == 3 && number_1*number_2 >= 0){
				number_1=number_1*number_2;
			}
			number_2=0;
			operation=0;
			break;
		case 2:
			reset_cc();
			break;
	}
}

//funkcja do realizacji zadania na 5
void zadanie_5(){
	PORTD = 0x00;

	while(1){
		int n = get_matrix_button_number();
		if(n!=n_buff){
			set_sth(n);
			make_operation();
			plot();

			LCD_Clear();
			LCD_GoTo(0, 1);
			LCD_WriteText(buff);

			_delay_ms(10);
			n_buff = n;
		}

	}
}



// glowna funkcja programu
int main() {
	LCD_Initalize();
	LCD_Home();

	//zadanie_3();
	//zadanie_4();
	zadanie_5();

}
