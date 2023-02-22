/* Demo project pingpongtoren + hoogtesensor 
 * 
 * 
 * pinout:  RC2 = receiver input
 *          RC7 = transmitter output
 *          RB6 = pulse lengte output
 *          RB4 = pwm output
 *          RC1 = motor output
 */

#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include "PI.h"
#include "UART.h"

/*
                         Main application
 */
void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // Initialiseer de hoogtemeting
    ADC_SelectChannel(Hoogtesensor);
    ADC_StartConversion();
    TMR2_Initialize();
    TMR2_StartTimer();
    int tel = 0;
    
    printf("Hello :) \r\n");

    while (1) {

        uartHandler();
        
        // PI moet op een vaste frequentie (elke 33ms) lopen voor de integrator
        if (TMR0_HasOverflowOccured()) {
            TMR0_Initialize();

            PI();
            
            tel++;
            
            if ( tel == 30 ) {
                ADC_SelectChannel(Potentiometer);
                ADC_StartConversion();
                PI_SetSetpoint(ADC_GetConversion(Potentiometer) >> 2);
                tel = 0;
            }
                      
            static uint8_t printCycle = 0; //door static toe te voegen wordt "printCycle" niet elke keer her geinitialiseerd maar behoudt het zijn vorige waarde
            if (printCycle++ > 30) {
                printLogs();
                printCycle = 0;
            }
            
            ADC_SelectChannel(Hoogtesensor);
            ADC_StartConversion();
        }
    }
}

/**
 End of File
 */