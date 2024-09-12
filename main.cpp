#include "mbed.h"
#include "arm_book_lib.h"

#define GAS_DETECTOR 3
#define OVERTEMP_DETECTOR 4

int main()
{
    //-----Código equivalente utilizando la clase PortIn-----//
    //DigitalIn gasDetector(D2);
    //DigitalIn overTempDetector(D3);

    //gasDetector.mode(PullDown);
    //overTempDetector.mode(PullDown);

    PortIn detectors(PortD,0x0C); //mask = 0b1100
    detectors.mode(PullDown);

    //-----Código equivalente utilizando la clase BusIn-----//
    //DigitalIn aButton(D4);
    //DigitalIn bButton(D5);
    //DigitalIn cButton(D6);
    //DigitalIn dButton(D7);

    //aButton.mode(PullDown);
    //bButton.mode(PullDown);
    //cButton.mode(PullDown);
    //dButton.mode(PullDown);

    BusIn buttons(D4,D5,D6,D7);

    buttons.mode(PullDown);


    DigitalOut alarmLed(LED1);

    alarmLed = OFF;

    bool alarmState = OFF;

    while (true) {

        if (detectors.read() != 0 ) {
            printf("El sensor de gas está %s\n", (detectors&0x04) ? "ACTIVADO" : "DESACTIVADO");
            printf("El sensor de temperatura está %s\n", (detectors&0x08) ? "ACTIVADO" : "DESACTIVADO");
            alarmState = ON;
            printf("%s\n","La alarma está activada");
        }

        alarmLed = alarmState;
        printf("aButton: %s\n",(buttons.mask()& 0b00010000) ? "ACTIVADO" : "DESACTIVADO");
        printf("bButton: %s\n",(buttons.mask()& 0b00100000) ? "ACTIVADO" : "DESACTIVADO");
        printf("cButton: %s\n",(buttons.mask()& 0b01000000) ? "ACTIVADO" : "DESACTIVADO");
        printf("dButton: %s\n",(buttons.mask()& 0b10000000) ? "ACTIVADO" : "DESACTIVADO");
        
        //aButton && bButton && !cButton && !dButton
        if (( buttons & buttons.mask() )== 0b00110000 ) {
            alarmState = OFF;
            
            printf("%s\n","La alarma está desactivada");
        }
    }
}

/*
11b- -Una ventaja puede ser si se requiere utilizar varios pines,y su configuración es la misma (entrada o salida)
se puede lograr mediante BusIn (BusOut) y PortIn (PortOut) el manejo
de una menor cantidad de variables en el código fuente.
Una desventaja sería cuando tengo que usar pines con modos distintos, ya que de esta forma debo utilizar todos con la 
misma configuración. Otra desventaja podría ser la manera de consultar si un pin está en alto o bajo, ya que se puede volver
poco práctico el armado de máscaras.
 */