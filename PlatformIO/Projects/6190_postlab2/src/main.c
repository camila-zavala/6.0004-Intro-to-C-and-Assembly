#include <stdio.h>
#include "6190.h"

void setup(){

    timerSetup();   // Configure timer
    setupDisplay(); // Configure display
    eraseBuffer();  // Erase random data from display
    drawBuffer();   // Display contents of buffer on LED array
}


int message_length(char* message){

    //////////////////////////////////////////
    // TO-DO: Complete message_length()     //
    //////////////////////////////////////////

    int length = 0; 
     int index = 0;
     while(1){
         if (*(message+index)== 0)
            return length;
         length += 1;
         index += 1;
         
     }

}


void fill_screen_buffer(char* message, int total_offset){

    //////////////////////////////////////////
    // TO-DO: Complete fill_screen_buffer() //
    //////////////////////////////////////////
    eraseBuffer();
    int first_index = total_offset/ 8;
    for (int j = 0 ; j < 5; j++){
        char letter = *(message + first_index + j);
        uint8_t* array = ascii[(int)letter];
        if (letter == 0)
            break;
        for (int i = 0; i<8; i++){
            if ((24 + (total_offset%8) - (j*8)) <= 0)
                screen_buffer[i] |= flip_8(array[i]) >> -(24 + (total_offset%8) - (j*8));
            if ((24 + (total_offset%8) - (j*8)) > 0)
                screen_buffer[i] |= flip_8(array[i]) << (24 + (total_offset%8) - (j*8));
            
       }
    }

}   


void app_main(){
    setup();

    char message[] = "     AUGUSTIN IS FUNNY     ";
    int len = message_length(message);  // TASK 1: CALCULATE MESSAGE LENGTH
    int offset = 0;
    while(1){
        if (offset == len * 8){  
            offset = 0; // Reset if we have reached the end of the message
        } else {
            eraseBuffer();  // Clear screen buffer
            fill_screen_buffer(message, offset); // TASK 2: FILL SCREEN BUFFER WITH CORRECT DATA
            drawBuffer();   // Transmit screen buffer data to screen
            offset++;        // Move forward
        }

        int start = millis();            // Get "start" time stamp
        while(millis() - start < 50);   // Wait until (current time stamp - "start") >= 100ms   
    }
}
