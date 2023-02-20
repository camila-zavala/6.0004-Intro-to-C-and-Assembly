#include <stdio.h>
#include <stdint.h>
#include "6190.h"

// Constants
#define SCREEN_ROWS 8
#define SCREEN_COLS 32
enum snake_direction { up, down, left, right };

// Setup for random value generator
#define RANDOM_VALUE_ADDR 0x600260B0 
#define RTC_CNTL_CLK_CONF_REG 0x60008070


// Snake struct definition
struct Snake {
    uint8_t segments[SCREEN_COLS*SCREEN_ROWS]; // can get as large as entire screen in theory
    uint8_t direction;
    uint8_t length; 
};


void setupRandom(){
    int *rtc_cntl_clk_conf_rg = (int*) RTC_CNTL_CLK_CONF_REG;
    *rtc_cntl_clk_conf_rg |= (1 << 10);
}


void setup(){
    timerSetup();
    setupRandom();
    setupDisplay();
    eraseBuffer();
    int snake_buttons[5] = {BTNF, BTNU, BTND, BTNR, BTNL};
    for (int i = 0; i < 5; i++){
        pinSetup(snake_buttons[i], GPIO_INPUT);
    }
}


uint8_t get_x(uint8_t location){
    /* Function that returns the x coordinate from an 8-bit value containing both x and y coordinates.
        The x coordinate is encoded in the upper (most significant) 5 bits of the input.
    Arguments: 
        location: an unsigned 8-bit value representing a location on an 8x32 gameboard.
    Returns: 
        uint8_t representing the x-coordinate (0-31) encoded in the upper 5 bits of the argument.
    */

    //////////////////////////////
    // TO-DO: Implement get_x() // 
    //////////////////////////////

    uint8_t x = ((location & 0b11111000)>> 3);
    return x;
}


uint8_t get_y(uint8_t location){
    /* Function that returns the y coordinate from an 8-bit value containing both x and y coordinates.
    Arguments: 
        location: an unsigned 8-bit value representing a location on an 8x32 gameboard.
    Returns: 
        uint8_t representing the y-coordinate (0-7) encoded in the lower 3 bits of the argument.
    */

    //////////////////////////////
    // TO-DO: Implement get_y() // 
    //////////////////////////////

    uint8_t y = location & 0b00000111;
    return y; 

}


void set_x(uint8_t *location, uint8_t new_x){
    /* Function that updates the upper 5 bits of the value stored at location to be equal to new_x.
    Arguments: 
        uint8_t *location: a pointer to an unsigned 8-bit value representing a location on an 8x32 gameboard.
        uint8_t new_x: an unsigned integer representing the value (0-31) that we'd like to set for the x location.
    */

    //////////////////////////////
    // TO-DO: Implement set_x() // 
    //////////////////////////////

    new_x = new_x%32; 
    *location &= 0b00000111; 
    *location |= new_x << 3; 
}


void set_y(uint8_t *location, uint8_t new_y){
    /* Function that updates the lower 3 bits of the value stored at location to be equal to new_y.
    Arguments: 
        uint8_t *location: a pointer to an unsigned 8-bit value representing a location on an 8x32 gameboard.
        uint8_t new_y an unsigned integer representing the value (0-7) that we'd like to set for the y location.
    */

    //////////////////////////////
    // TO-DO: Implement set_y() // 
    //////////////////////////////
    
    new_y = new_y%8; 
    *location &= 0b11111000; 
    *location |= new_y;
}


void setPixel(uint8_t location, uint8_t val){
    /* Function that sets the value of an indivdual pixel (LED) on the display. 	
    Arguments: 	
        location: The location of the LED to be turned on/off on the LED array	
        val: The binary (0 or 1) value indicating if the LED at the location
             should be off (0) or on (1).	
    */	
   
    /////////////////////////////////
    // TO-DO: Implement setPixel() // 
    /////////////////////////////////

    uint8_t x = get_x(location);
    uint8_t y = get_y(location);
    
    if (val == (uint8_t) 1)
        screen_buffer[y] |= val << x;
    else
        screen_buffer[y] &= ~(1 << x);
    
}


void drawBoard(struct Snake *snake, uint8_t food){
    /* Function to render the snake and food on the board on each given time-step.
    Arguments:
        struct Snake *snake: pointer to snake struct
        uint8_t food: location of food to render
    */

    //////////////////////////////////
    // TO-DO: Implement drawBoard() // 
    //////////////////////////////////

    eraseBuffer();
    for (int i = 0; i< snake->length; i++){
        setPixel(snake->segments[i], (uint8_t) 1);
    }
    setPixel(food, (uint8_t) 1);
    drawBuffer();

}


void updateSnake(struct Snake *snake){
    /* Function to update the snake on each time-step based on its direction.
    Arguments: 
        struct Snake *snake: pointer to snake struct
    */

    ////////////////////////////////////
    // TO-DO: Implement updateSnake() // 
    ////////////////////////////////////

    
    for (int i = snake->length; i >0; i--){
        snake->segments[i] = snake->segments[i-1];
    }
    if (snake->direction == right)
        set_x(&(snake->segments[0]), ((get_x(snake->segments[0]))- (uint8_t)1));
    if (snake->direction == left)
        set_x(&(snake->segments[0]), ((get_x(snake->segments[0]))+ (uint8_t)1));
    if (snake->direction == up)
        set_y(&(snake->segments[0]), ((get_y(snake->segments[0]))- (uint8_t)1));
    if (snake->direction == down)
        set_y(&(snake->segments[0]), ((get_y(snake->segments[0]))+ (uint8_t)1));
}


int generateFood(struct Snake *snake, uint8_t *food){
    /* Function to randomly generate food for the snake.
    Arguments: 
        struct Snake *snake: pointer to snake struct
        uint8_t *food: pointer to variable holding the food's location
    Returns:
        int: 0 if food does not conflict with the snake, 1 if it does.
    */

    int *random_value_ptr = (int*) RANDOM_VALUE_ADDR; // read this address to obtain a random integer value

    ////////////////////////////////////
    // TO-DO: Complete generateFood() // 
    ////////////////////////////////////
    int random = *random_value_ptr;
    int counter = 0;
    int anti_counter = 0;

    for (int i = 0; i < snake->length; i++){
        if ((uint8_t)random != snake->segments[i]){
            counter += 1;
        }
        else{
            anti_counter += 1;
        }
    }
    if (counter == snake->length){
        *food = random;
        return 0;
    }
    return 1;
}


uint8_t snakeAteFood(uint8_t *snake_segments, uint8_t food){
    /* Function to determine if the snake collided with (and ate) the food.
    Arguments: 
        uint8_t *snake_segments: pointer to the snake segments array
        uint8_t food: variable holding the food's location
    Returns:
        int: 0 if the snake did not eat the food, 1 if it did.
    */

    /////////////////////////////////////
    // TO-DO: Implement snakeAteFood() // 
    /////////////////////////////////////

    if (snake_segments[0] == food)
        return 1;
    else
        return 0;

}


uint8_t snakeCollisionCheck(struct Snake *snake){
    /* Function to determine if the snake collided with itself.
    Arguments: 
        struct Snake *snake: pointer to snake struct
    Returns:
        int: 0 if the snake did not collide with itself, 1 if it did.
    */

    ////////////////////////////////////////////
    // TO-DO: Implement snakeCollisionCheck() // 
    ////////////////////////////////////////////

    for (int i = 1; i < snake->length; i++){
        if(snake->segments[0] == snake->segments[i])
            return 1;
    } 
    return 0;
}

void app_main() {
    setup(); // System setup
    eraseBuffer();
    int ongoing = 0;

    // Snake initialization
    struct Snake snake;
    // COMMENT OUT FOR PART 10
    // snake.segments[0] = 0;
    // snake.direction = left;
    // snake.length = 1;

    // UNCOMMENT FOR PART 10
    for (int i=0; i <3; i++){
        set_x(&snake.segments[i], 5);
        set_y(&snake.segments[i], 3-i);
    }
    snake.length = 3;
    snake.direction = left;

    uint8_t food = 0; // food in top right corner of the game board

    while(generateFood(&snake, &food) != 0); // Generate food initially
    drawBoard(&snake, food);


    int left_button = pinRead(BTNL);
    int right_button = pinRead(BTNR);
    int up_button = pinRead(BTNU);
    int down_button = pinRead(BTND);
    int decay_variable = 0; 
    int quitting = 0; 
    int previous_button = pinRead(BTNF);
    
    uint8_t previous[] = {left_button, right_button, up_button, down_button};

    while(1){        
        
        //////////////////////////////////////////////
        // INSERT BUTTON PRESS DETECTION LOGIC HERE //
        //////////////////////////////////////////////
        left_button = pinRead(BTNL); 
        right_button = pinRead(BTNR);
        up_button = pinRead(BTNU);
        down_button = pinRead(BTND);
        quitting = pinRead(BTNF); 
        decay_variable = snake.length; 

        if ((ongoing == 1) && (decay_variable >0)){
            snake.length--;
            drawBoard(&snake, food);
            int beginning = millis();
            while(millis() - beginning < 200); 
            continue; 
        }
        
        if(ongoing == 0){
            if (left_button == 0 && previous[0] ==1){
                if (snake.direction != right){
                    snake.direction = left;
                }
            }
            previous[0] = left_button;
            if (right_button == 0 && previous[1] ==1){
                if (snake.direction != left){
                    snake.direction = right;
                } 
            }
            previous[1] = right_button; 
            if (up_button == 0 && previous[2] ==1){
                if (snake.direction != down)
                    snake.direction = up;
            }
            previous[2] = up_button;
            if (down_button == 0 && previous[3] ==1){
                if (snake.direction!= up)
                    snake.direction = down;
            }
            previous[3] = down_button;
            previous_button = quitting; 

            updateSnake(&snake);
        }
        if (snakeCollisionCheck(&snake) == 1){
            // Game ends
            ongoing = 1; 
            continue; 
        }
        if (pinRead(BTNF) == 0){
                for (int i=0; i <3; i++){
                    set_x(&snake.segments[i], 5);
                    set_y(&snake.segments[i], 3-i);
                }
                snake.length = 3;
                snake.direction = left;
                ongoing = 0;
                drawBoard(&snake, food);
                while (generateFood(&snake, &food) != 0);
        }
        if ((quitting == 0) && (previous_button ==1)){
                for (int i=0; i <3; i++){
                    set_x(&snake.segments[i], 5);
                    set_y(&snake.segments[i], 3-i);
                }
                snake.length = 3;
                snake.direction = left;
                ongoing = 0;
                drawBoard(&snake, food);
                while (generateFood(&snake, &food) != 0);
        }
        if (snakeAteFood(snake.segments, food) == 1){
                //////////////////////////////
                // TO-DO: "Grow" snake      // 
                //////////////////////////////
                snake.length += 1;
                uint8_t last_segment = snake.segments[snake.length -1];
                uint8_t second_to_last = snake.segments[snake.length -2]; 
                uint8_t last_x = get_x(last_segment);
                uint8_t last_y = get_y(last_segment);
                uint8_t second_to_last_x = get_x(second_to_last);
                uint8_t second_to_last_y = get_y(second_to_last);
                uint8_t new_x = last_x + (last_x - second_to_last_x); 
                uint8_t new_y = last_y + (last_y - second_to_last_y); 
                set_x(&(snake.segments[snake.length]), new_x);
                set_y(&(snake.segments[snake.length]), new_y);

                // Generate food
                while (generateFood(&snake, &food) != 0); // generate food
        }
        drawBoard(&snake, food);

        // Code to limit frequency of loop so that gameplay is possible.
        // Put all game code above this point.
        int start = millis();
        while (millis() - start < 200); // One loop iteration every ~200ms
    }
}