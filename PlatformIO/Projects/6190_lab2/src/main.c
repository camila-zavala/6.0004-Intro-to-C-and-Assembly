#include <stdio.h>
#include "6190.h"


int switch_pins[8] = {SW0, SW1, SW2, SW3, SW4, SW5, SW6, SW7}; 


void setup(){

    timerSetup();   // Configure timer
    setupDisplay(); // Configure display
    eraseBuffer();  // Erase random data from display
    drawBuffer();   // Display contents of buffer on LED array

    ////////////////////////////////////////
    // TO-DO: Complete setup()            //
    // Configure SW0-Sw6 and BTNF as input//
    ////////////////////////////////////////
    for (int i = 0; i< 8; i++ ){
        pinSetup(*(switch_pins+ i) , GPIO_INPUT);

    }

}


char switch_to_binary(){
    /*
    Return: 
        uint8_t: 8-bit value. Bits 0-6 correspond to switch inputs, bit 7 is 0.
    */

    ////////////////////////////////////////
    // TO-DO: Complete switch_to_binary() //
    ////////////////////////////////////////


    uint8_t input = 00000000;
    for (int i = 0; i < 7; i+=1) {
        input |= pinRead(*(switch_pins+i)) << i; 

    }
    return input;

}


void draw_ascii_string(char* string_to_draw){   
    /* Function that draws a string on the LED array using the mapping defined in ascii.h 
    by filling in a buffer.
    
    Argument: 
        char* string_to_draw: String to draw on the LED array.
    */

    /////////////////////////////////////////
    // TO-DO: Complete draw_ascii_string() // 
    /////////////////////////////////////////
    eraseBuffer();
    for (int j = 0 ; j<4; j++){
        if (*(string_to_draw + j)== 0)
            break;
        for (int i = 0; i<8; i++){
            uint8_t* array = ascii[(int)(*(string_to_draw +j))];
            screen_buffer[i] |= flip_8(array[i]) << (24 - 8*j);
        }
    }
    drawBuffer(); 
}



void app_main(){
    setup();

    char ascii_input = 0;
    int previous = 0;
    int button = pinRead(BTNF);
    char array[100]; // {'0', '\0', 0, 0, 0, ..}
    int index = 0;
    while(1){
        ascii_input = switch_to_binary();
        button = pinRead(BTNF);
        if (button == 0 && previous == 1){
            array[index] = ascii_input;
            index += 1;
            if (ascii_input == '\0'){
                draw_ascii_string(array);
                // printf("ASCII string: %s\n", array);
                index = 0;
                //char array[100] = "0";   // printing this value using %x formatter gives a hexadecimal value... 
            }
            
        }
        previous = button;
        // Code to slow down loop so that "bouncing" associated with a single button press doesn't 
        // look like more than one button press. Write all of your code in above this point in the loop.
        int start = millis();           // Get "start" time stamp
        while(millis() - start < 50);   // Wait until current time stamp - "start" is greater than 50ms
    }
}