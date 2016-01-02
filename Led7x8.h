/*
   Led7x8.h - Clase para control de 8 displays con 1 MAX72xx
   Created by Rafa Gomez, 2014
   Licensed under GPL v3
*/
 
#ifndef Led7x8_h
#define Led7x8_h
 
#include <Arduino.h>
#include <SPI.h>
 
class Led7x8 {
    private:
        static const int _displays7 = 8; // número de displays por defecto
        static const byte _defaultIntensity = 0x01; // intensidad por defecto
 
        static const byte _asciiTable[];
 
        int _maxDisplay;
        int _loadPin;
 
        void setMax72xx(uint8_t registro, uint8_t valor);
 
        enum comandosMax72xx {NOP, WRD0, WRD1, WRD2, WRD3, WRD4, WRD5, WRD6, WRD7, DECODEMODE, INTENSITY, SCANLIMIT, SHUTDOWN,, NU1, NU2, TEST};
 
    public:
        Led7x8(int loadPin, int displays=_displays7);
 
        void initMax72xx(int intensidad=_defaultIntensity);
        void clearDisplay();
        void shutdownMax72xx();
 
        void printAsciiChar(int display, char value, bool dp=false); // imprime caracter ASCII en display
        void printAsciiString(int initPos, int finalPos, String cadena); // imprime cadena ASCII en display
};
 
#endif  //Led7x8.h
