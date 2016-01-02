#include "LedMax.h"

LedMax lc(12,11,10);

int color = 1;
int col = 8;

void setup() 
{
    lc.initMax72xx();

    for(col=0; col<8; col++)
    {
        plot(col, col);
    }
}


void loop()
{
    delay(500);
    lc.push();
    plot(7, col++); 
}


void plot(int i, int j)
{
    if( (j % 4) == 0 )
    {
        lc.drawColumna(i, j % 4 + 2, 4);
    }
    else
    {
        lc.plot(i, j % 4 + 2);
    }
}
