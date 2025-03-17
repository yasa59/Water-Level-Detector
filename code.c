#include <xc.h>
#include <stdio.h>

// CONFIGURATION BITS (Use Internal 8MHz Oscillator)
#pragma config FOSC = INTRC_NOCLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000  // Define CPU Clock Frequency (8MHz)

// LCD Control Pins
#define RS RD0
#define EN RD1
#define D4 RD2
#define D5 RD3
#define D6 RD4
#define D7 RD5

// HC-SR04 Pins
#define TRIG RC0
#define ECHO RC1

// Buzzer Pin
#define BUZZER RB0

// Function to Send Command to LCD
void LCD_Command(unsigned char cmd) {
    RS = 0;  // Command mode
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1;
    __delay_ms(2);
    EN = 0;
    
    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// Function to Send Data to LCD
void LCD_Char(char data) {
    RS = 1;  // Data mode
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1;
    __delay_ms(2);
    EN = 0;
    
    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

// Function to Send a String to LCD
void LCD_String(const char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

// Function to Initialize LCD in 4-bit Mode
void LCD_Init() {
    TRISD = 0x00;  // Set PORTD as output
    __delay_ms(20);
    
    LCD_Command(0x02);  // 4-bit mode
    LCD_Command(0x28);  // 2 lines, 5x7 matrix
    LCD_Command(0x0C);  // Display ON, Cursor OFF
    LCD_Command(0x06);  // Auto Increment Cursor
    LCD_Command(0x01);  // Clear Display
    __delay_ms(2);
}

// Function to Initialize HC-SR04
void HC_SR04_Init() {
    TRISCbits.TRISC0 = 0;  // Set TRIG pin as output
    TRISCbits.TRISC1 = 1;  // Set ECHO pin as input
    TRIG = 0;  // Ensure TRIG starts LOW
}

// Function to Measure Distance Using HC-SR04
unsigned int Measure_Distance() {
    unsigned int pulse_time;
    unsigned int distance;

    // Ensure TRIG is LOW before triggering
    TRIG = 0;
    __delay_us(2);

    // Send Trigger pulse (10us HIGH)
    TRIG = 1;
    __delay_us(10);
    TRIG = 0;
    
    // Wait for Echo to go HIGH
    while (!ECHO);
    
    // Start Timer when Echo is HIGH
    TMR1H = 0;  // Clear Timer1 High byte
    TMR1L = 0;  // Clear Timer1 Low byte
    T1CON = 0x01;  // Enable Timer1, prescaler 1:1
    
    // Wait for Echo to go LOW
    while (ECHO);
    
    // Stop Timer when Echo goes LOW
    T1CON = 0x00;  // Disable Timer1
    
    // Calculate pulse time in microseconds
    pulse_time = (TMR1H << 8) | TMR1L;
    
    // Convert pulse time to distance in cm (Speed of sound: 343 m/s or 0.0343 cm/us)
    distance = (pulse_time * 0.0343) / 2;  // Divide by 2 for round-trip distance
    
    return distance;
}

// Main Program
void main() {
    unsigned int distance;
    OSCCON = 0b01110000;  // Set Internal Oscillator to 8MHz

    // Initialize Components
    LCD_Init();
    HC_SR04_Init();

    TRISBbits.TRISB0 = 0;  // Set BUZZER pin as output
    BUZZER = 0;  // Ensure buzzer starts OFF

    while (1) {
        distance = Measure_Distance();  // Measure distance
        LCD_Command(0x01);  // Clear Display
        LCD_Command(0x80);  // Move Cursor to First Line

        if (distance <= 15) {
            // Water level is HIGH
            LCD_String("Water Level HIGH");
            BUZZER = 1;  // Turn ON buzzer
        } 
        else if (distance <= 60) {
            // Water level is MID
            LCD_String("Water Level MID");
            BUZZER = 0;  // Turn OFF buzzer
        } 
        else if (distance >= 90) {
            // Water level is LOW
            LCD_String("Water Level LOW");
            BUZZER = 0;  // Turn OFF buzzer
        }

        __delay_ms(1000);  // Small delay before next reading
    }
}
