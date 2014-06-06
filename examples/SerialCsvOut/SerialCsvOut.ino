/*
 SerialCsvOut.ino
 
 Sample code of JPEG Decoder for Arduino
 Public domain, Makoto Kurauchi <http://yushakobo.jp>
*/

#include <arduino.h>
//#include <rxduino.h>
//#include <RLduino78.h>
#include <SD.h>
#include "JPEGDecoder.h"


// CS Pin of SD card 
#define SDCS 10


void setup() {

    Serial.begin(115200);
    while(!Serial.available()); // Input wait
    Serial.read();

    Serial.print("Initializing SD card...");

    if (!SD.begin(SDCS)) { // Initialization of SD card
        Serial.println("initialization failed!");
        return;
    }
    Serial.println("initialization done.");
}

void loop() {
    char str[100];
    char filename[] = "test.jpg";
    uint8 *pImg;
    int x,y,bx,by;
    
    // Decoding start
    JpegDec.decode(filename,0);

    // Image Information
    Serial.print("Width     :");
    Serial.println(JpegDec.width);
    Serial.print("Height    :");
    Serial.println(JpegDec.height);
    Serial.print("Components:");
    Serial.println(JpegDec.comps);
    Serial.print("MCU / row :");
    Serial.println(JpegDec.MCUSPerRow);
    Serial.print("MCU / col :");
    Serial.println(JpegDec.MCUSPerCol);
    Serial.print("Scan type :");
    Serial.println(JpegDec.scanType);
    Serial.print("MCU width :");
    Serial.println(JpegDec.MCUWidth);
    Serial.print("MCU height:");
    Serial.println(JpegDec.MCUHeight);
    Serial.println("");
    
    // Output CSV
    sprintf(str,"#SIZE,%d,%d",JpegDec.width,JpegDec.height);
    Serial.println(str);

    while(JpegDec.read()){
        pImg = JpegDec.pImage ;

        for(by=0; by<JpegDec.MCUHeight; by++){
        
            for(bx=0; bx<JpegDec.MCUWidth; bx++){
            
                x = JpegDec.MCUx * JpegDec.MCUWidth + bx;
                y = JpegDec.MCUy * JpegDec.MCUHeight + by;
                
                if(x<JpegDec.width && y<JpegDec.height){

                    if(JpegDec.comps == 1){ // Grayscale
                    
                        sprintf(str,"#RGB,%d,%d,%u", x, y, pImg[0]);
                        Serial.println(str);

                    }else{ // RGB

                        sprintf(str,"#RGB,%d,%d,%u,%u,%u", x, y, pImg[0], pImg[1], pImg[2]);
                        Serial.println(str);
                    }
                }
                pImg += JpegDec.comps ;
            }
        }
    }
    
    for(;;);
}
