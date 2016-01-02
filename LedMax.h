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
        static const int _displaysCols = 8; // número de displays o columnas por defecto
        static const byte _defaultIntensity = 0x05; // intensidad por defecto

        static const byte _asciiTable[];

        int _maxDisplay; // número de displays o columnas a manejar
        int _dinPin, _clkPin, _csPin; // pines de conexión
        byte _buffer[8];

        void setMax72xx(byte registro, byte valor);

        void drawByte(int x, int y, byte candidato, int color=1);

        enum comandosMax72xx {NOP, WRD0, WRD1, WRD2, WRD3, WRD4, WRD5, WRD6, WRD7, DECODEMODE, INTENSITY, SCANLIMIT, SHUTDOWN, NULL1, NULL2, TEST};

    public:
        LedMax(int dinPin, int clkPin, int csPin, int displays=_displaysCols); // por defecto en modo matriz 8x8

        void initMax72xx(byte intensidad=_defaultIntensity); // inicializa el integrado MAX72xx para que admita órdenes de pintado
        void clearDisplayCol(byte display); // borra la información mostrada, según el caso, en la columna de la matriz o el display indicado
        void clearAll(); // borra la información mostrada
        void push(); // desplaza a la izquierda todo
        
        // Métodos para displays LED
        void printAsciiChar(int display, char value, bool dp=false); // imprime caracter ASCII en display
        void printAsciiString(int initPos, int finalPos, String cadena); // imprime cadena ASCII en display

        // Métodos para matriz 8x8
        void plot(int x, int y, int color=1); // ilumina/borra punto en (x, y), por defecto a 1 (ilumina)
        void drawColumna(int x, int y, int lon, int color=1); // dibuja columna
        void invert(); // invierte matriz
};

#endif	//LedMax.h

