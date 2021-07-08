#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/iom32.h>

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif
#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif


/* ----- Przyciski -----
Do przyciskow potrzebne jest zainicjowanie portow
    cbi(DDRC, PC0);
	cbi(DDRC, PC1);
	sbi(PORTC, PC0);
	sbi(PORTC, PC1);
bit_is_clear(PINC, PC0) - sprawdzenie czy jest wcisniete
bit_is_set(PINC, PC0) - sprawdzenie czy jest wycisniete
*/


/* ----- Wyswietlacz LCD -----
Do wyswietlacza potrzebne jest:
    LCD_Initalize(); - zainicjowanie wyswietlacza
	LCD_Home();
	LCD_Clear(); - czysci wyswietlacz
	LCD_GoTo(0, 0); - idzie na okreslone miejsce na wyswietlaczu
    sprintf(buff_2,"on"); - '   buff_2 = "on"  '
    LCD_WriteText(buff_2); - wypisuje na wyswietlaczu
*/


/* ----- Wyswietlacz 7-segmentowy -----
Do wyswietlacza potrzebne jest:
void TimerInit() - // Inicjalizacja Timer1 do wywolywania przerwania z czestotliwoscia 2Hz
{
	sbi(TCCR1B,WGM12);
    sbi(TCCR1B,CS12);
    OCR1A = 15625;	//2Hz
    sbi(TIMSK,OCIE1A);
}

void seg7Init() - // Inicjalizacja segmentu
{
	DDRC = 0xff;
	PORTC = 0xff;
}

void seg7ShowCyfra(uint8_t cyfraDoWyswietlenia) - wyswietla podana cyfre
{
	PORTC = cyfra[cyfraDoWyswietlenia];
}

void seg7null()- // wygaszenie wyswietlacza
{
	PORTC = null[0];
}
ISR(TIMER1_COMPA_vect) - przerwania, nie wiem co robią ale są ultra potrzebne, 7seg działa na przerywaniach

Do wyswietlacza potrzebne jest:
    TimerInit(); - inicjacja 
	seg7Init(); - inicjacja
    seg7ShowCyfra(2); - wyswietla 2
    seg7null(); - wylacza wyswietlacz
*/


/* ----- Przetwornik AC-DC -----
Do przetwornika potrzebne jest:
void ADC_init()
{
	sbi(ADMUX, REFS0); // konfiguracja napiecia referencyjnego na AVCC
	cbi(ADMUX, REFS1); // konfiguracja podzielnika czestotliwosci dla ukladu przetwornika. Mniejsza od 100KHz

	//	Czestotliwosc sygnalu taktujacego 62,5 kHz
	sbi(ADCSRA, ADPS0); //Tymi bitami definiujemy pozadana relacje 
	sbi(ADCSRA, ADPS1); //	 miedzy czestotliwoscia zegara sytemowego XTAL,
	sbi(ADCSRA, ADPS2); //	 a czestotliwoscia przebiegu taktujacego przetwornik.

	sbi(ADCSRA, ADEN); //uruchomienie ukladu przetwornika

	//	konfiguracja/wybor kanalu/pinu na ktorym bedzie dokonywany pomiar
	cbi(ADMUX, MUX0); //Bity wyboru wejscia analogowego.
	cbi(ADMUX, MUX1); //   Wejscie wybrane kombinacja tych bitow
	cbi(ADMUX, MUX2); //   jest dolaczone do przetwornika
	cbi(ADMUX, MUX3);
}

int uint16_tADC_10bit()
{
	sbi(ADCSRA, ADSC); //uruchomienie pojedynczego pomiaru - ustawienie bitu
	while (bit_is_set(ADCSRA, ADSC)) //oczekiwanie na zakończenie pomiaru – oczekiwanie na wyzerowanie bitu
	{
	}
	return ADC; // wynik pomiaru
}

int ADC_10bit()
{
	int A_D = uint16_tADC_10bit(); //odczytana wartosc z rejestru pomiarowego przetwornika A/D
	return A_D; //zwraca wynik pomiaru z przetwornika
}

int ADC_measure(){
	//wzor na na przeliczenie wartosci zwracanej [V]: (ADC_10bit()*ref_val)/1024
	int max_val = 1022;//powinno byc 1024 ale lepiej dziala tak
	int ref_val = 5;	//napiecie referencyjne
	int ret = (int)(((double)ADC_10bit()*ref_val)*100/max_val);

	return ret; //zwraca wyliczona wartosc
}
Do przetwornika potrzebne jest:
    ADC_init(); - inicjacja
    val = ADC_measure(); - mierzenie wartosci z przetwornika
    
*/
