/*
   Led7x8 - Clase para control de 8 displays con 1 MAX72xx
   Created by Rafa Gomez, 2014
   Licensed under GPL v3
   */
 
#include "Led7x8.h"
 
const byte Led7x8::_asciiTable[128] = { // tabla de caracteres ASCII (sólo los "pintables" en 7 segmentos)
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
 
Led7x8::Led7x8(int loadPin, int displays)
{
    _loadPin = loadPin;
    _maxDisplay = (displays <= _displays7) ? displays : _displays7;
}
 
void Led7x8::initMax72xx(int intensidad)
{
    // Pin para LOAD como salida
    pinMode(_loadPin, OUTPUT);
 
    // Configura SPI a MSB primero
    SPI.setBitOrder(MSBFIRST);
    // Start SPI
    SPI.begin();
 
    // Habilita modo no-decode
    setMax72xx(DECODEMODE, 0x00); // 0x00 para modo no-decode
    // Configura intensidad
    setMax72xx(INTENSITY, intensidad);
    // Scan N dígitos
    setMax72xx(SCANLIMIT, (byte)(_maxDisplay - 1));
    // Enciende LEDs
    setMax72xx(SHUTDOWN, 0x01);
    setMax72xx(TEST, 0x01);
    setMax72xx(TEST, 0x00);
 
    // Limpia LEDs
    clearDisplay();
}
 
void Led7x8::setMax72xx(uint8_t orden, uint8_t valor) {
 
    // Set LOAD/CS a LOW
    digitalWrite(_loadPin, LOW);
    // Envía orden
    SPI.transfer(orden);
    // Envía el valor
    SPI.transfer(valor);
    // Carga dato
    digitalWrite(_loadPin, HIGH);
}
 
void Led7x8::printAsciiChar(int display, char value, bool dp)
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
        setMax72xx(display, ascii);
    }
}
 
void Led7x8::clearDisplay()
{
    for (uint8_t i = 1; i <= _maxDisplay; i++)
    {
        setMax72xx(i, 0x00);
    }
}
 
void Led7x8::printAsciiString(int initPos, int finalPos, String cadena)
{
    int tam = cadena.length();
    bool dp = false;
    int ultimo = (tam <= finalPos-initPos) ? tam : finalPos-initPos;
    int offset = 0;
 
    for(int i=0; i <= ultimo; i++)
    {
        dp = false;
        if( (i<ultimo) && (cadena.charAt(i+1) == '.') ) dp = true;
        if(cadena.charAt(i) != '.') { printAsciiChar(initPos+i-offset, cadena.charAt(i), dp); } else { offset++; ultimo++;}
    }
}
 
void Led7x8::shutdownMax72xx()
{
    setMax72xx(SHUTDOWN, 0x00);
}
