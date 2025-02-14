//Wykorzystanie sygnału PWM do użycia diody RGB na dwóch pinach Timera1 i jednym pinie Timera2
//Zwiększanie "temperatury" za pomocą potencjometru i wyświetlenie jej na wyświetlaczu

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

#define RS PD0
#define E PD1

//......................................................... PWM .................................................................
void PWM_init()
{
  //Tryb pracy - Fast PWM 8bit
  TCCR1A |= (1<<WGM10);
  TCCR1B |= (1<<WGM12);
  TCCR2A |= (1<<WGM20);
  TCCR2A |= (1<<WGM21);

  //Compare output mode
  TCCR1A |= (1<<COM1A1);
  TCCR1A |= (1<<COM1B1);
  TCCR2A |= (1<<COM2B1);
 
  //Prescaler = 1
  TCCR1B |= (1<<CS10);
  TCCR2B |= (1<<CS20);
  
  //Częstotliwosc = 16000000/(1*(1+255)) = 62500Hz (62500 razy na sekunde zliczanie od 0 do 255)

  //Porównywacz
  OCR1A = 255;
  OCR1B = 255;
  OCR2B = 255;
}

void setColor(int r, int g, int b)
{
  r = 255-r;
  g = 255-g;
  b = 255-b;
  OCR1A = r;
  OCR1B = g;
  OCR2B = b;
}



//......................................................... ADC .................................................................
void ADC_init()
{
  //zasilanie
  ADMUX |= (1<<REFS0);
  ADMUX &= ~(1<<REFS1);

  //division factor
  ADCSRA |= (1<<ADPS0);
  ADCSRA |= (1<<ADPS1);
  ADCSRA |= (1<<ADPS2);

  //kanał A0
  ADMUX &= ~(1<<MUX0);
  ADMUX &= ~(1<<MUX1);
  ADMUX &= ~(1<<MUX2);
  ADMUX &= ~(1<<MUX3);
  
  //włączenie ADC
  ADCSRA |= (1<<ADEN);
}

int channelA0()
{
   ADCSRA |= (1<<ADSC);

  while(ADCSRA & (1<<ADSC))
  {}

  //zwracanie zmierzonego napięcia
  return ADC;
}


//......................................................... LCD .................................................................
void E_pulse()
{
  PORTD |= (1<<E);
  _delay_us(1);
  PORTD &= ~(1<<E);
  _delay_ms(2);
}


void LCD_command(unsigned char com)
{
  PORTD &= ~(1<<RS);
  //przesłanie starszych bitów (bez pomijania innych ustawionych na tym porcie)
  PORTD = ((com & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  //przesłanie młodszych bitów (bez pomijania innych ustawionych na tym porcie), jako starszych, po wcześniejszym ich przesunięciu
  PORTD = (((com<<4) & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  //_delay_ms(2);
}

void LCD_sign(unsigned char sign)
{
  PORTD |= (1<<RS);
  //przesłanie starszych bitów (bez pomijania innych ustawionych na tym porcie)
  PORTD = ((sign & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  //przesłanie młodszych bitów (bez pomijania innych ustawionych na tym porcie), jako starszych, po wcześniejszym ich przesunięciu
  PORTD = (((sign<<4) & 0b11110000) | (PORTD & 0b00001111));
  E_pulse();
  _delay_us(2);
}

void LCD_init()
{
  //ustawienie jako wyjście pinów D7,6,5,4,1,0 (danych równoległych oraz RS i E)
  DDRD = 0b11111111;

  _delay_ms(20);
 
  //Display/cursor home - Ustawia adres DDRAM 0 w liczniku adresów. Przywraca również przesunięcie wyświetlacza do pierwotnej pozycji. Zawartość pamięci DDRAM pozostaje niezmieniona
  LCD_command(0b00000010);
  _delay_us(2000);
 
  //Function set - przesył, ilosc linijek, czcionka
  LCD_command(0b00101000);
  _delay_us(50);

  //Display ON/OFF - - Włącza/wyłącza cały wyświetlacz, włącza/wyłącza kursor i znak pozycji kursora.
  LCD_command(0b00001100);
  _delay_us(50);

  //Entry mode set - Ustawia kierunek ruchu kursora i określa przesunięcie wyświetlania. Operacje te są wykonywane podczas zapisu i odczytu danych.
  LCD_command(0b00000110);
  _delay_us(50);

  //Display cursor shift - Przesuwa kursor i przesuwa ekran bez zmiany zawartości pamięci DDRAM.
  LCD_command(0b00010100);
  _delay_us(50);

  //Display clear - czyści wyświetlacz
  LCD_command(0b00000001);
}

void LCD_clear()
{
   LCD_command(0b00000001);
}

void LCD_set_cursor(unsigned char row, unsigned char col)
{
    unsigned char pos;
    
    if (row == 0) {
        pos = 0x80 + col; // Pierwsza linia
    } else if (row == 1) {
        pos = 0xC0 + col; // Druga linia
    } else {
        return; // Nieprawidłowy wiersz
    }
    
    LCD_command(pos);
}

void LCD_print(const char *array)
{
  for(int i=0; i<strlen(array); i++)
  {
      LCD_sign(array[i]); 
  }
}



//......................................................... MAIN ................................................................
int main()
{
  DDRB |= (1<<PB1);
  DDRB |= (1<<PB2);
  DDRD |= (1<<PD3);

  PWM_init();
  ADC_init();
  LCD_init();

  int resultA0 = 0;
  float voltageA0 = 0.0;
  int temp = 0;
  char T[20];
  char T_LCD[20];
  char V[20];
  char V_LCD[20];
  
  while(1)
  {
    resultA0 = channelA0();
    voltageA0 = resultA0 * 5.0/1023.0;
    temp = voltageA0 * 22;

    dtostrf(temp, 3, 0, T);
    sprintf(T_LCD, "%s C", T);

     dtostrf(voltageA0, 5, 2, V);
     sprintf(V_LCD, "%s V", V);
     
     LCD_set_cursor(0, 0);
     LCD_print(T_LCD);
     LCD_set_cursor(1, 0);
     LCD_print(V_LCD);
     
     
      
    //-> zielony
    if((temp > 1)&&(temp <= 50))
    {
      //krok = 255/50 = 5.1
      int g = temp*5.1;
      setColor(0,g,0);
    }
    
    //zielony -> niebieski
    else if((temp > 50)&&(temp <= 70))
    {
      //krok = 255/20 = 12.75
      int t = temp-50;
      int g = 255-t*12.75;
      int b = t*12.57;
      setColor(0,g,b);
    }
    
    //niebieski -> fiolet
    else if((temp > 70)&&(temp <= 80))
    {
      //krok = 155/10 = 15.5
      int t = temp-70;
      int b = 255-t*5.5;
      int r = t*10.5;
      setColor(r,0,b);
    }
    
    //fiolet -> żółty
    else if((temp > 80)&&(temp <= 90))
    {
      //krok = 155/10 = 15.5
      int t = temp-80;
      int r = 255-t*15.5;
      int b = 255-t*15.5;
      int g = t*15.5;
      setColor(r,g,b);
    }
    
    //żółty -> czerwony
    else if(temp > 90)
    { 
      //krok = 155/10 = 15.5
      int t = temp -90;
      if((255-t*33)>0)
      {
        int r = t*28.3;
        int b = 255-t*33;
        int g = 255-t*33;
        setColor(r,g,b);
      }
      else
      {
        int r = t*28.3;
        int b = 0;
        int g = 0;
        setColor(r,g,b);
      }
     
    }
    else
    {
      setColor(0,0,0);
    }


    //_delay_ms(500);
    // LCD_clear();
  }

  return 0;
}
