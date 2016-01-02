/*
   LedMax - Clase para control de displays LED con 1 MAX72xx
   Created by Rafa Gomez, 2015
   Licensed under GPL v3
   */

#include "LedMax.h"


const byte LedMax::_asciiTable[128] = { ///< tabla de caracteres ASCII (sólo los "pintables" en 7 segmentos)
    0,0,0,0,0,0,0,0, // 0-7
    0,0,0,0,0,0,0,0, // 8-15
    0,0,0,0,0,0,0,0, // 16-23
    0,0,0,0,0,0,0,0, // 24-31
    0,0,B00100010,0,0,0,0,B00000010, // 32-39
    B01001110,B01111000,0,0,B00010000,B00000001,B10000000,B00011000, // 40-47
    B01111110,B00110000,B01101101,B01111001,B00110011,B01011011,B01011111,B01110000, // 48-55 
    B01111111,B01111011,0,0,0,B00001001,0,B01100101, // 56-63
    B01111101,B01110111,B00011111,B01001110,B00111101,B01001111,B01000111,B01011110, // 64-71
    B00110111,B00110000,B00111100,B00110111,B00001110,B01110110,B01110110,B01111110, // 72-79
    B01100111,B01110011,B00000101,B01011011,B00001111,B00111110,B00011100,B00011100, // 80-87
    B00110111,B00111011,B01101101,B01001110,B00010001,B01111000,0,B00001000, // 88-95
    B00000010,B01110111,B00011111,B00001101,B00111101,B01001111,B01000111,B01111011, // 96-103
    B00010111,B00010000,B00111100,B00110111,B00001110,B00010101,B00010101,B00011101, // 104-111 
    B01100111,B01110011,B00000101,B00011001,B00001111,B00011100,B00011100,B00011100, // 112-119
    B00110111,B00111011,B01101101,B01001110,B00000110,B01111000,B01000000,0 // 120-127
};


///
/// Constructor clase LedMax
///
LedMax::LedMax(int dinPin, int clkPin, int csPin, int displays)
{
    _dinPin = dinPin;
    _clkPin = clkPin;
    _csPin = csPin;

    // Set control pins to output
    pinMode(_dinPin, OUTPUT);
    pinMode(_clkPin, OUTPUT);
    pinMode(_csPin, OUTPUT);

    _displayMatrixMode = (displays == _display8x8) ? true : false;
    _maxDisplay = ( (displays <= _displays7) && (displays != _display8x8) ) ? displays : _displays7;

    memset(_buffer, 0, sizeof(_buffer));
}


///
/// Inicializa el MAX72xx
///
void LedMax::initMax72xx(byte intensidad)
{
    // Activa CS
    digitalWrite(_csPin, HIGH);
    
    // Enciende LEDs
    setMax72xx(TEST, 0x00);
    //// Scan N dígitos
    setMax72xx(SCANLIMIT, 0x07);
    // Enable modo no-decode
    setMax72xx(DECODEMODE, 0x00); // 0x00 para modo no-decode

    // Apaga/enciende LEDs
    setMax72xx(SHUTDOWN, 0x00);
    setMax72xx(SHUTDOWN, 0x01);

    // Set intensidad
    setMax72xx(INTENSITY, intensidad);

    // Limpia LEDs
    clearDisplays();
}


///
/// Envía orden a MAX72xx
///
void LedMax::setMax72xx(volatile byte orden, volatile byte valor) {

    // Set LOAD/CS a LOW
    digitalWrite(_csPin, LOW);
    // Envía orden
    shiftOut(_dinPin, _clkPin, MSBFIRST, orden);
    // Envía el valor
    shiftOut(_dinPin, _clkPin, MSBFIRST, valor);
    // Carga dato
    digitalWrite(_csPin, HIGH);
}


///
/// Limpia todos los displays (o las columnas)
///
void LedMax::clearDisplays()
{
    for (byte i = 1; i <= _maxDisplay; i++)
    {
        _buffer[i-1] = 0x00;
        setMax72xx(i, 0x00);
    }
}


///
/// Limpia un display o columna
///
void LedMax::clearDisplay(byte display)
{
    _buffer[display-1] = 0x00;
    setMax72xx(display, 0x00);
}


///
/// Imprime carácter ASCII
///
void LedMax::printAsciiChar(int display, char value, bool dp) 
{
    display = _maxDisplay - display; // orden de izq. a der.
    if(display < 1 || display > _maxDisplay)
    {
        // Posición de display inválida
    }
    else
    {
        byte ascii = _asciiTable[value];
        if(dp) ascii|=B10000000;
        _buffer[display-1] = ascii;
        setMax72xx(display, ascii);
    }
}


///
/// Imprime cadena
///
void LedMax::printAsciiString(int initPos, int finalPos, String cadena) 
{
    int tam = cadena.length();
    bool dp = false;
    int ultimo = (tam <= finalPos-initPos) ? tam : finalPos-initPos;
    int offset = 0;

    for(int i=0; i <= ultimo; i++)
    {
        dp = false;
        if( (i<ultimo) && (cadena.charAt(i+1) == '.') ) dp = true;
        if(cadena.charAt(i) != '.') 
        {
            printAsciiChar(initPos+i-offset, cadena.charAt(i), dp);
        } 
        else 
        { 
            offset++; 
            ultimo++;
        }
    }
}


///
/// Desplaza todo a la izquierda
///
void LedMax::push()
{
    for(int i = 1; i <= _maxDisplay; i++)
    {
        _buffer[i-1] = _buffer[i];
        setMax72xx(i, _buffer[i-1]);
    }
    clearDisplay(_maxDisplay);
}


///
/// Pinta píxel
///
void LedMax::plot(int x, int y, int color) // x e y comienzan por 0
{
    byte candidato = 0x01;
    drawByte(x, y, candidato, color);
}


///
/// Pinta una columna
///
void LedMax::drawColumna(int x, int y, int lon, int color)
{
    if(lon < 0)
    {
        y = y + lon + 1;
        lon = lon * -1;
    }
    byte candidato = 0xFF >> (8 - lon); // inicializamos la columna completa a "1"
    drawByte(x, y, candidato, color);
}


///
/// Pinta un byte (columna/dígito)
///
void LedMax::drawByte(int x, int y, byte candidato, int color)
{
    if( x > 7 || x < 0 || y > 7 || y < 0 )
        return;

    switch(color)
    {
        case 0: // Apaga
            candidato = ( ~(candidato << y) ) & _buffer[x];
            break;
        case 1: // Enciende
            candidato = ( (candidato << y) ) | _buffer[x];
            break;
        case 2: // Invierte
            candidato = ( (candidato << y) ) ^ _buffer[x];
            break;
        default:
            break;
    }
    if (_buffer[x] != candidato) {
        _buffer[x] = candidato;
        setMax72xx(x + 1, _buffer[x]);
    }
    else
    {
        // do nothing
    }
}

