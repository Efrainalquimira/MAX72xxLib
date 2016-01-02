/*
   LedMax.h - Clase para control de displays LED con 1 MAX72xx 
   Created by Rafa Gomez, 2015
   Licensed under GPL v3
   */


#ifndef LedMax_h
#define LedMax_h

#include <Arduino.h>

class LedMax {
    private:
        static const int _displays7 = 8; // número de displays por defecto
        static const int _display8x8 = -1; // condición de display matriz 8x8
        static const byte _defaultIntensity = 0x05; // intensidad por defecto

        static const byte _asciiTable[];

        int _maxDisplay;
        int _dinPin, _clkPin, _csPin;
        bool _displayMatrixMode;
        byte _buffer[8];

        void setMax72xx(byte registro, byte valor);

        void drawByte(int x, int y, byte candidato, int color=1);

        enum comandosMax72xx {NOP, WRD0, WRD1, WRD2, WRD3, WRD4, WRD5, WRD6, WRD7, DECODEMODE, INTENSITY, SCANLIMIT, SHUTDOWN, NU1, NU2, TEST};

    public:
        LedMax(int dinPin, int clkPin, int csPin, int displays=_display8x8); // por defecto en modo matriz 8x8

        void initMax72xx(byte intensidad=_defaultIntensity);
        void clearDisplay(byte display);
        void clearDisplays();
        
        void printAsciiChar(int display, char value, bool dp=false); // imprime caracter ASCII en display
        void printAsciiString(int initPos, int finalPos, String cadena); // imprime cadena ASCII en display

        void push(); // desplaza a la izquierda todo

        // Métodos para matriz 8x8
        void plot(int x, int y, int color=1); // ilumina/borra punto en (x, y), por defecto a 1 (ilumina)
        void drawColumna(int x, int y, int lon, int color=1);
};

#endif	//LedMax.h

