/*
 * UFSC Campus Ararangua
 * Projeto Final: Sistema Aeronautico Antigelo
 * Autores: Danilo Silveira Ramos / Gabriel Raulino Dal Pont
 *
 */

#include <xc.h>          
#include <pic16f877a.h>  
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 4000000 //4Mhz

//**** configuration bits  **************************************************

#pragma config FOSC = XT       // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

//**** define pinos referentes a interface com LCD
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

//**** define pinos de I/O

#define AM PORTBbits.RB0
#define RES PORTBbits.RB1
#define COMP PORTBbits.RB2
#define BZ PORTBbits.RB3
#define CRITICO PORTBbits.RB4
#define ALERTA PORTBbits.RB5
#define NORMAL PORTBbits.RB6

//**** inclui a biblioteca do LCD escolhido
#include "lcd.h"

//**** define variaveis

float tempIntA, tempIntD, tempExtA, tempExtD;
float limite = 60; //60°C
float umidadeRelativa, pressao;
int flag;

void __interrupt() TrataInt(void) {

    if (INTF) {
        flag = 1;
        TMR1L++;
        if (TMR1L == 0) { // Verifica se houve overflow (8 bits para 16 bits)
            TMR1H++;
        }
        INTCONbits.INTF = 0; // Limpa a flag de interrupção
    }
    return;
}

//**** define funcoes

float calcularTemperatura(float adcValue) {
    float tensao = (adcValue / 1023.0f) * 5.0f;
    float temperatura = tensao * 100;

    return temperatura;
}

int main() {
    TRISA = 0b11111111; //configura PORTA como entrada.
    TRISB = 0b00000001; //configura PORTB como entrada/saida.
    TRISD = 0b00000000; //configura PORTD como saida.
    OPTION_REGbits.nRBPU = 0; //ativa resistores de pull-up
    char buffer[30];

    T1CONbits.TMR1ON = 1; // 1 = Habilita o Timer1;
    T1CONbits.TMR1CS = 1; // 1 = Clock externo no pino RC0/T1CKI;
    T1CONbits.T1CKPS1 = 0; // 00 = Prescaler 1:1
    T1CONbits.T1CKPS0 = 0;
    TRISCbits.TRISC0 = 1; // Configura RC0/T1CKI como entrada
    TMR1H = 0; // Inicializa o registrador alto do Timer1 com 0
    TMR1L = 0; // Inicializa o registrador baixo do Timer1 com 0

    INTCON = 0b10010000; //ativa os bits GIE e INTE, ou seja, a interrupcao externa
    OPTION_REGbits.INTEDG = 1; // Interrupção na borda de subida (RB0)

    ADCON1bits.PCFG0 = 0; //configura as entradas analogicas
    ADCON1bits.PCFG1 = 0; //configura as entradas analogicas
    ADCON1bits.PCFG2 = 0; //configura as entradas analogicas
    ADCON1bits.PCFG3 = 0; //configura as entradas analogicas
    
    //inicializar variaveis
    RES = 0;
    COMP = 0;
    BZ = 0;
    CRITICO = 0;
    ALERTA = 0;
    NORMAL = 0;
    Lcd_Init();
    Lcd_Clear(); // Limpa o display

    //inicializa registradores do AD
    ADRESL = 0x00; //inicializar valor analogico com 0
    ADRESH = 0x00;
    ADCON0bits.ADON = 1; //Liga AD 

    OPTION_REGbits.PSA = 1; // utiliza pre-scaler timer0 para WDT
    OPTION_REGbits.PS0 = 1; // PS0-PS2: 111 pre-scaler = 1:128
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;

    CLRWDT(); //reseta a contagem do WDT para nao resetar

    while (1) {
        CLRWDT(); //reseta a contagem do WDT
        Lcd_Clear();

        if (flag == 1) {
            COMP = 1;
            BZ = 1;
            for (int i = 0; i < 5000; i++) {
                __delay_ms(1);
                CLRWDT(); // Reseta o Watchdog Timer a cada iteração
            }
            COMP = 0;
            BZ = 0;
        }

        flag = 0;

        ADCON1bits.ADFM = 1;
        ADCON0bits.CHS = 0b000;
        __delay_us(20); //tempo de conversao
        ADCON0bits.GO = 1; //converte
        while (ADCON0bits.GO); //garante conversao
        tempIntA = (ADRESH << 8) + ADRESL; // passa valores convertido do reg para a variAvel
        tempIntD = calcularTemperatura(tempIntA);

        if (tempIntD < limite) {
            RES = 1;
        }
        if (tempIntD > limite) {
            RES = 0;
        }

        ADCON1bits.ADFM = 1;
        ADCON0bits.CHS = 0b001;
        __delay_us(20); //tempo de conversao
        ADCON0bits.GO = 1; //converte
        while (ADCON0bits.GO); //garante conversao
        tempExtA = (ADRESH << 8) + ADRESL; // passa valores convertido do reg para a variAvel
        tempExtD = calcularTemperatura(tempExtA);

        ADCON1bits.ADFM = 0;
        ADCON0bits.CHS0 = 0; //configura canal 0 como entrada anal?gica
        ADCON0bits.CHS1 = 1; //configura canal 0 como entrada anal?gica
        ADCON0bits.CHS2 = 0; //configura canal 0 como entrada anal?gica
        __delay_us(20); //tempo de conversao
        ADCON0bits.GO = 1; //converte
        while (ADCON0bits.GO); //garante conversa
        int umidadeADC = ADRESH;

        if (umidadeADC == 0) {
            umidadeRelativa = 0;
        }
        if (umidadeADC == 25) {
            umidadeRelativa = 10;
        }
        if (umidadeADC == 51) {
            umidadeRelativa = 20;
        }
        if (umidadeADC == 76) {
            umidadeRelativa = 30;
        }
        if (umidadeADC == 102) {
            umidadeRelativa = 40;
        }
        if (umidadeADC == 127) {
            umidadeRelativa = 50;
        }
        if (umidadeADC == 153) {
            umidadeRelativa = 60;
        }
        if (umidadeADC == 179) {
            umidadeRelativa = 70;
        }
        if (umidadeADC == 204) {
            umidadeRelativa = 80;
        }
        if (umidadeADC == 230) {
            umidadeRelativa = 90;
        }
        if (umidadeADC == 255) {
            umidadeRelativa = 100;
        }

        ADCON1bits.ADFM = 0;
        ADCON0bits.CHS0 = 1; //configura canal 0 como entrada anal?gica
        ADCON0bits.CHS1 = 1; //configura canal 0 como entrada anal?gica
        ADCON0bits.CHS2 = 0; //configura canal 0 como entrada anal?gica
        __delay_us(20); //tempo para garantir selecao de canal
        ADCON0bits.GO = 1; //converte
        while (ADCON0bits.GO); //garante conversao
        int pressaoADC = ADRESH; // Lê o valor bruto do ADC

        if (pressaoADC == 0) {
            pressao = 15;
        }
        if (pressaoADC == 25) {
            pressao = 25;
        }
        if (pressaoADC == 51) {
            pressao = 35;
        }
        if (pressaoADC == 76) {
            pressao = 45;
        }
        if (pressaoADC == 102) {
            pressao = 55;
        }
        if (pressaoADC == 127) {
            pressao = 65;
        }
        if (pressaoADC == 153) {
            pressao = 75;
        }
        if (pressaoADC == 179) {
            pressao = 85;
        }
        if (pressaoADC == 204) {
            pressao = 95;
        }
        if (pressaoADC == 230) {
            pressao = 105;
        }
        if (pressaoADC == 255) {
            pressao = 115;
        }

        float pressaohPa = pressao * 10;

        if (tempExtD <= 0) {
            if (pressaohPa <= 800 && umidadeRelativa >= 80 && tempIntD > 59) {
                CRITICO = 1;
                ALERTA = 0;
                NORMAL = 0;
                flag = 1; // para ativar o compressor
                TMR1L++;
                if (TMR1L == 0) { // Verifica se houve overflow (8 bits para 16 bits)
                    TMR1H++;
                }
            }
            if (pressaohPa > 800 && umidadeRelativa >= 80) {
                ALERTA = 1;
                CRITICO = 0;
                NORMAL = 0;
                COMP = 0;
            }
            if (pressaohPa <= 800 && umidadeRelativa < 80) {
                ALERTA = 1;
                CRITICO = 0;
                NORMAL = 0;
                COMP = 0;
            }
            if (pressaohPa > 800 && umidadeRelativa < 80) {
                ALERTA = 0;
                CRITICO = 0;
                NORMAL = 1;
                COMP = 0;
            }
        } else {
            ALERTA = 0;
            CRITICO = 0;
            NORMAL = 1;
            COMP = 0;
        }
        Lcd_Clear(); // Limpa o display
        Lcd_Set_Cursor(1, 1); // Posiciona o cursor na linha 1, coluna 1

        // Exibe a temperatura interna (INT);
        if (tempIntD < 0) {
            int modulo = -(int) tempIntD;
            sprintf(buffer, "I-%dC", modulo); // Formata a string com a temperatura interna
            Lcd_Write_String(buffer); // Escreve a string no LCD
        }
        sprintf(buffer, "I %dC", (int) tempIntD); // Formata a string com a temperatura interna
        Lcd_Write_String(buffer); // Escreve a string no LCD

        Lcd_Set_Cursor(1, 10); // Posiciona o cursor na linha 1, coluna 9

        // Exibe a temperatura externa (EXT)
        sprintf(buffer, "O %dC", (int) tempExtD); // Formata a string com a temperatura externa
        Lcd_Write_String(buffer); // Escreve a string no LCD

        Lcd_Set_Cursor(2, 1); // Posiciona o cursor na linha 2, coluna 1

        // Exibe a pressão (kPa)
        sprintf(buffer, "%d kPa", (int) pressao); // Formata a string com a pressão
        Lcd_Write_String(buffer); // Escreve a string no LCD

        Lcd_Set_Cursor(2, 9); // Posiciona o cursor na linha 2, coluna 9

        // Exibe a umidade relativa (RH)
        sprintf(buffer, "RH %d%%", (int) umidadeRelativa); // Formata a string com a umidade relativa
        Lcd_Write_String(buffer); // Escreve a string no LCD

        __delay_ms(1000); // Atraso de 1 segundo para atualização do display

    }
}