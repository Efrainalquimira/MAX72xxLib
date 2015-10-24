#include "Led7x8.h"
 
Led7x8 lc(10);
 
void setup()
{
    lc.initMax72xx();
}
 
void loop()
{
    String hola = "Hola Arduino";
    for(int i=0; i<hola.length(); i++) {
        lc.printAsciiString(0, 7, hola.substring(i));
        delay(200);
    }
}
