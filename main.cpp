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

    PortIn detectors(PortD,0x0C); //mask = 0b1100, se activan los pines 2 y 3 del puerto D
    detectors.mode(PullDown); //Se configuran los puertos con una resistencia de PullDown

    //-----Código equivalente utilizando la clase BusIn-----//
    //DigitalIn aButton(D4);
    //DigitalIn bButton(D5);
    //DigitalIn cButton(D6);
    //DigitalIn dButton(D7);

    //aButton.mode(PullDown);
    //bButton.mode(PullDown);
    //cButton.mode(PullDown);
    //dButton.mode(PullDown);

    //Con el uso de BusIn se configuran varios pines como DigitalIn para leerlos a la vez con una máscara
    BusIn buttons(D4,D5,D6,D7); //Se configuran D4,D5,D6 y D7 como DigitalIn con el objeto BusIn

    buttons.mode(PullDown); //Resistencia de pull-down para las entradas configuradas.


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
        //Las máscaras utilizadas se aplican sabiendo que BusIn(a,b,c,d,e,f,g,h) se leen en el byte como hgfedcba
        printf("aButton: %s\n",(buttons.mask()& 0b0001) ? "ACTIVADO" : "DESACTIVADO");
        printf("bButton: %s\n",(buttons.mask()& 0b0010) ? "ACTIVADO" : "DESACTIVADO");
        printf("cButton: %s\n",(buttons.mask()& 0b0100) ? "ACTIVADO" : "DESACTIVADO");
        printf("dButton: %s\n",(buttons.mask()& 0b1000) ? "ACTIVADO" : "DESACTIVADO");
        
        //aButton && bButton && !cButton && !dButton
        if (( buttons & buttons.mask() )== 0b0011 ) {
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