# c-atmega328p-pwm-rgb-temperature

**Project Description - English**

This project demonstrates temperature simulation using a potentiometer, visualizing it on an RGB LED via PWM, and displaying the values on an LCD screen. The potentiometer simulates temperature changes, and the RGB LED dynamically changes color based on the value.

Features:

- Simulated temperature measurement via potentiometer (ADC)

- RGB LED control via PWM, changing color based on potentiometer value

- Displaying "temperature" and voltage on an LCD screen

- Using ATmega328P microcontroller in pure C

Hardware Requirements:

- ATmega328P microcontroller (e.g., Arduino Nano or standalone AVR)

- RGB LED connected to PB1, PB2, and PD3

- Potentiometer connected to A0 (simulating temperature changes)

- HD44780-compatible LCD display

- Connecting wires

- 5V power supply

Usage Instructions:

- Compile the code in an AVR C-compatible environment (e.g., Atmel Studio, PlatformIO, AVR-GCC).

- Connect the potentiometer to A0 and the RGB LED to PB1, PB2, and PD3.

- Connect the LCD screen according to the schematic (4-bit mode).

- Upload the program to the microcontroller using an ISP programmer.

- Turn the potentiometer – its value will be converted into simulated temperature, which:

- Changes the RGB LED color (e.g., from green, through blue, purple, to red).

- Displays on the LCD screen along with the corresponding voltage.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

**Opis projektu - Polski**

Ten projekt demonstruje symulację temperatury przy użyciu potencjometru, wizualizację jej na diodzie RGB za pomocą PWM oraz wyświetlanie wartości na ekranie LCD. Potencjometr symuluje zmianę temperatury, a kolor diody RGB zmienia się dynamicznie w zależności od jej wartości.

Funkcjonalność:

- Symulacja temperatury poprzez odczyt napięcia z potencjometru (ADC)

- Sterowanie diodą RGB poprzez PWM, zmieniając kolor w zależności od wartości potencjometru

- Wyświetlanie "temperatury" oraz napięcia na ekranie LCD

- Obsługa mikrokontrolera ATmega328P w czystym C

Wymagania sprzętowe:

- Mikrokontroler ATmega328P (np. Arduino Nano lub standalone AVR)

- Dioda RGB podłączona do PB1, PB2 i PD3

- Potencjometr podłączony do A0 (symulacja temperatury)

- Wyświetlacz LCD kompatybilny z HD44780

- Przewody połączeniowe

- Zasilanie 5V

Instrukcja użytkowania:

- Skompiluj kod w środowisku obsługującym AVR C (np. Atmel Studio, PlatformIO, AVR-GCC).

- Podłącz potencjometr do A0 i diodę RGB do PB1, PB2 i PD3.

- Podłącz wyświetlacz LCD zgodnie ze schematem (tryb 4-bitowy).

- Wgraj program do mikrokontrolera za pomocą programatora ISP.

- Obracaj potencjometr – jego wartość zostanie przekształcona na symulowaną temperaturę, która:

- Zmieni kolor diody RGB (np. od zielonego, przez niebieski, fioletowy, aż do czerwonego).

- Zostanie wyświetlona na ekranie LCD razem z odpowiadającym jej napięciem.

