// Raajitha Rajkumar
// rrajk001@ucr.edu
// 862015848
// CS122A Fall 2020 
// Project 3

// GOOGLE DOC
// https://docs.google.com/document/d/10akuYpiCIjdnMMDVdfzZes-XEXlNZsZgoolliM0lSVw/edit?usp=sharing
//
// YOUTUBE VIDEO
// hhttps://www.youtube.com/watch?v=vEBBIri-gTs

/* Welcome to the ultimate ship navigation software. This program allows you to captain your very own ship in the waters 
   and change directions. The mapping is depicted in the output box below RIMS and the controlled output is on port B and D. 
   You can change the direction of your ship, and the speed. You also have to get past obstacles in order to prevent your 
   ship from wrecking.*/
   
/* A0 - turns on ship engine
   A1 - moves ship right
   A2 - moves ship left
   A6 - speeds up propellers
   A7 - slows down propellers
   B0 - ship is on
   B1 - ship is moving right
   B2 - ship is moving left
   B3 - warning is being handled
   B4 - direction is north
   B5 - direction is east
   B6 - direction is south
   B7 - direction is west
   D - propeller meter 
*/

#include "RIMS.h"

// Timer functions
volatile unsigned char TimerFlag = 0;

void TimerISR() {
   TimerFlag = 1;
}

// period functions
int period = 2800;
int warningTimer = 10;

// initialize navigation variables for ship, start at north
int isNorth = 1; 
int isEast = 0; 
int isSouth = 0;
int isWest = 0;
int signalDirection = 0;

// initialize engine variables
int engineOn = 0; // tells whether engine is on
int propellorSpeed = 100; // propellor speed

// struct for ship path diagram
typedef struct path path;

struct path { 
    char *shipPath;
    struct path* left;
    struct path* right;
    struct path* switchDirection;
};

// this function makes the entire ship path
struct path* construct_path(){
    // assigning ship path nodes
    struct path* straight = 0;
    struct path* first_left = 0;
    struct path* second_left = 0;
    struct path* third_left = 0;
    struct path* first_leftr = 0;
    struct path* second_leftr = 0;
    struct path* third_leftr = 0;
    struct path* first_right = 0;
    struct path* second_right = 0;
    struct path* third_right = 0;
    struct path* first_rightl = 0;
    struct path* second_rightl = 0;
    struct path* third_rightl = 0;
    struct path* switch_left = 0;
    struct path* switch_right = 0;
    
    // allocating ship path node memory
    straight = (struct path*)malloc(sizeof(path));
    first_left = (struct path*)malloc(sizeof(path));
    second_left = (struct path*)malloc(sizeof(path));
    third_left = (struct path*)malloc(sizeof(path));
    first_right = (struct path*)malloc(sizeof(path));
    second_right = (struct path*)malloc(sizeof(path));
    third_right = (struct path*)malloc(sizeof(path));
    first_leftr = (struct path*)malloc(sizeof(path));
    second_leftr = (struct path*)malloc(sizeof(path));
    third_leftr = (struct path*)malloc(sizeof(path));
    first_rightl = (struct path*)malloc(sizeof(path));
    second_rightl = (struct path*)malloc(sizeof(path));
    third_rightl = (struct path*)malloc(sizeof(path));
    switch_left = (struct path*)malloc(sizeof(path));
    switch_right = (struct path*)malloc(sizeof(path));
    
    // assigning path mark ticks
    straight->shipPath = "                              '\r\n";
    first_left->shipPath = "                             '\r\n                            '\r\n                           '\r\n";
    second_left->shipPath = "                          '\r\n                         '\r\n                        '\r\n";
    third_left->shipPath = "                       '\r\n                      '\r\n                    '\r\n";
    first_right->shipPath = "                               '\r\n                                '\r\n                                 '\r\n";
    second_right->shipPath = "                                  '\r\n                                   '\r\n                                    '\r\n";
    third_right->shipPath = "                                     '\r\n                                      '\r\n                                       '\r\n";
    switch_left->shipPath = "                   '\r\n                 '\r\n             '\r\n'   '   '    \r\n";
    switch_right->shipPath = "                                        '\r\n                                          '\r\n                                               '\r\n                                                    '    '    '    '    '    '    ' \r\n";
    first_leftr->shipPath = "                            '\r\n                             '\r\n                              '\r\n";
    second_leftr->shipPath = "                         '\r\n                          '\r\n                           '\r\n";
    third_leftr->shipPath = "                         '\r\n                         '\r\n                         '\r\n";
    first_rightl->shipPath = "                                '\r\n                               '\r\n                              '\r\n";
    second_rightl->shipPath = "                                   '\r\n                                  '\r\n                                 '\r\n";
    third_rightl->shipPath = "                                      '\r\n                                      '\r\n                                   '\r\n";
    
    // mapping ship path nodes together 
    straight->left = first_left;
    straight->right = first_right;
    first_left->left = second_left;
    first_left->right = first_leftr;
    first_leftr->right = first_right;
    first_leftr->left = first_left;
    first_right->left = first_rightl;
    first_right->right = second_right;
    first_rightl->left = first_left;
    first_rightl->right = first_right;
    second_left->left = third_left;
    second_left->right = second_leftr;
    second_leftr->right = first_leftr;
    second_leftr->left = second_left;
    second_right->right = third_right; 
    second_right->left = second_rightl;
    second_rightl->left = first_rightl;
    second_rightl->right = second_right;
    third_left->switchDirection = switch_left;
    third_right->switchDirection = switch_right;
    third_left->right = third_leftr;
    third_leftr->right = second_left;
    third_leftr->left = third_left;
    third_right->left = third_rightl;
    third_rightl->left = second_rightl;
    third_rightl->right= third_right;
    switch_left->right = third_leftr;
    switch_left->left = straight;
    switch_right->right = straight;
    switch_right->left = third_rightl;
    
    return straight;
}

// prints north compass
void print_north(){
   printf("\n\n       GENERATING NAUTICAL MAP\n\n");
   printf("                      SOUTH\n");
   printf("                        ^\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("  EAST<---------------->WEST\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        V\n");
   printf("                       NORTH\n\n\n");
   return;
}

// prints east compass
void print_east(){
   printf("\n\n       GENERATING NAUTICAL MAP\n\n");
   printf("                      WEST\n");
   printf("                        ^\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("  SOUTH<---------------->NORTH\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        V\n");
   printf("                       EAST\n\n\n");
   return;
}

// prints south compass
void print_south(){
   printf("\n\n       GENERATING NAUTICAL MAP\n\n");
   printf("                      NORTH\n");
   printf("                        ^\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("  WEST<---------------->EAST\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        V\n");
   printf("                      SOUTH\n\n\n");
   return;
}

// prints west compass
void print_west(){

 
   printf("\n\n       GENERATING NAUTICAL MAP\n\n");
   printf("                      EAST\n");
   printf("                        ^\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("  NORTH<---------------->SOUTH\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        |\n");
   printf("                        V\n");
   printf("                       WEST\n\n\n");
   return;
}


// this function checks if the direction has shifted
int checkNavigationShift(){
    int changed = 0;
    if(signalDirection){
        changed = 1;
    }
    return changed;
}

// this function outputs the propellor meter on D
// the speed of the ship increases as the meter rises to D0
void outputPropellors(){
    if(propellorSpeed <= 50){
        D0 = 0;
        D1 = 0;
        D2 = 0;
        D3 = 0;
        D4 = 0;
        D5 = 1;
        D6 = 1;
    }else if(propellorSpeed <= 100){
        D0 = 0;
        D1 = 0;
        D2 = 0;
        D3 = 0;
        D4 = 1;
        D5 = 1;
        D6 = 1;
    }else if(propellorSpeed <= 125){
        D0 = 0;
        D1 = 0;
        D2 = 0;
        D3 = 1;
        D4 = 1;
        D5 = 1;
        D6 = 1;
    }else if(propellorSpeed <= 150){
        D0 = 0;
        D1 = 0;
        D2 = 1;
        D3 = 1;
        D4 = 1;
        D5 = 1;
        D6 = 1;
    }else if(propellorSpeed <= 175){
        D0 = 0;
        D1 = 1;
        D2 = 1;
        D3 = 1;
        D4 = 1;
        D5 = 1;
        D6 = 1;
    }else if(propellorSpeed <= 200){
        D0 = 0;
        D1 = 1;
        D2 = 1;
        D3 = 1;
        D4 = 1;
        D5 = 1;
        D6 = 1;
    }
}

// This is a synchronous state machine that controls the propellors
// of the ship. The faster the propellors, the faster the ship  
// goes. This will be controlled by the extra ports.
enum propellor_control{propellorsOff, propellorsOn, speedUp, speedDown} propellor;

int acceleration = 1; /// for propellor acceleration

void propellorsControl(){
    switch(propellor){
        case propellorsOff:
            if(A0){
                propellor = propellorsOn; // turn on
            }else if(!A0){
                propellor = propellorsOff; // turn off
            }
            break;
        case propellorsOn:
            if(!A0){
                propellor = propellorsOff; // turn off
            }else if(A0 && A6){
                propellor = speedUp;
            }else if(A0 && A7){
                propellor = speedDown;
            }else if((A6 && A7) || (!A6 && !A7)){
                propellor = propellorsOn;
            }
            break;
        case speedUp:
            if(!A0){
                propellor = propellorsOff;
            }else if(A0 && A6){
                acceleration = acceleration + 0.01;
                propellor = speedUp;
            }else if(A0 && A7){
                acceleration = acceleration + 0.01;
                propellor = speedDown;
            }else if((A6 && A7) || (!A6 && !A7) || (propellorSpeed>= 200)){
                propellor = propellorsOn;
            }
            break;
        case speedDown:
            if(!A0){
                propellor = propellorsOff;
            }else if(A0 && A6){
                acceleration = acceleration + 0.01;
                propellor = speedUp;
            }else if(A0 && A7){
                acceleration = acceleration + 0.01;
                propellor = speedDown;
            }else if((A6 && A7) || (!A6 && !A7) || (propellorSpeed<= 50)){
                propellor = propellorsOn;
            }
            break;
    }
    switch(propellor){
        case propellorsOff:
            D0 = 0;
            D1 = 0;
            D2 = 0;
            D3 = 0;
            D4 = 0;
            D5 = 0;
            D6 = 0;
            break;
        case propellorsOn:
            outputPropellors();
            break;
        case speedUp:
            if(propellorSpeed <= 200){
               propellorSpeed = propellorSpeed + acceleration*15;
               outputPropellors();
               period = period - 300;
            }
            break;
        case speedDown:
            if(propellorSpeed >= 0){
               propellorSpeed = propellorSpeed - acceleration*15;
               outputPropellors();
               period = period + 300;
               break;
            }
    }
}

// This is a synchronous state machine that is constantly running 
// and outputting the ships mapping trail. The trail will go in
// one direction and when it maps to a new one it will reload the 
// page and reprint a new compass.
enum boat_navigation{engineOff, straight, shipright, shipleft, switchdirection, waiting} navigation; 

struct path* trail = 0; // mapping variable for the whole ship path

void boatNavigation(){
    switch(navigation){
        case engineOff:
            if(A0){
                trail = construct_path(); // construct ship path
                engineOn = 1; // initialize engine 
                print_north();
                navigation = straight; // set navigation 
            }else if(!A0){
                navigation = engineOff;
            }
            break;
        case straight:
            if(A0){ // if engine is one
                signalDirection = 0;
                if(A1){
                    navigation = shipright; // go right
                }else if(A2){
                    navigation = shipleft; // go left
                }else{
                    navigation = straight; // go straight
                }
            }
            break; 
        case shipright:
            if(trail->switchDirection){
                navigation = switchdirection;
            }else if(A0 && A1){
               navigation = shipright; // go right
            }else if(A0 && A2){
               navigation = shipleft;
            }else{
               navigation = waiting; 
            }
            break;
        case shipleft:
            if(trail->switchDirection){
                navigation = switchdirection;
            }else if(A0 && A2){
               navigation = shipleft;
            }else if(A0 && A1){
               navigation = shipright;
            }else{
               navigation = waiting; 
            }
            break;
        case switchdirection:
            if(!A0){
                navigation = engineOff;
            }
            signalDirection = 1;
            trail = trail->switchDirection;
            printf("%s", trail->shipPath);
            if(isNorth && B1 == 1){
                trail = trail->left;
                isNorth = 0;
                isEast = 1;
                print_east();
                navigation = straight;
            }else if(isNorth && B2 == 1){
                trail = trail->right;
                isNorth = 0;
                isWest = 1;
                print_west();
                navigation = straight;
            }else if(isEast && B2 == 1){
                trail = trail->right;
                isEast = 0;
                isNorth = 1;
                print_north();
                navigation = straight;
            }else if(isEast && B1 == 1){
                trail = trail->left;
                isEast = 0;
                isSouth = 1;
                print_south();
                navigation = straight;
            }else if(isSouth && B2 == 1){
                trail = trail->right;
                isSouth = 0;
                isEast = 1;
                print_east();
                navigation = straight;
            }else if(isSouth && B1 == 1){
                trail = trail->left;
                isSouth = 0;
                isWest = 1;
                print_west();
                navigation = straight;
            }else if(isWest && B2 == 1){
                trail = trail->right;
                isWest = 0;
                isSouth = 1;
                print_south();
                navigation = straight;
            }else if(isWest && B1 == 1){
                trail = trail->left;
                isWest = 0;
                isNorth = 1;
                print_north();
                navigation = straight;
            }
            break;
        case waiting:
            if(A0 && A1){
                navigation = shipright;
            }else if(A0 && A2){
                navigation = shipleft;
            }else if(!A0){
                navigation = engineOff;
            }else if(trail->switchDirection){
                navigation = switchdirection;
            }else{
                navigation = waiting;
            }
            break;
    }
    switch(navigation){
        case engineOff:
            B = 0;
            if(!engineOn){
               printf("\n----PRESS A0 TO START SHIP ENGINE----\n");
            }else if(A0){
               B0 = 1;
            }
            break;
        case straight:
            if(A0){  
               B0 = 1;
               B1 = 0;
               B2 = 0;
               printf("%s", trail->shipPath);
            }  
            break;
        case shipleft:
            if(A0 && A2){
               B0 = 1;
               B1 = 1;
               B2 = 0;
               trail = trail->left;
               printf("%s", trail->shipPath);   
            }           
            break;
        case shipright:
            if(A0 && A1){
               B0 = 1;
               B1 = 0;
               B2 = 1;
               trail = trail->right;
               printf("%s", trail->shipPath);
                
            }
            break;
        case switchdirection:
            break;
        case waiting:
            break;
    }
}

// This is a synchronous state machine that is constantly updating
// the direction of navigation of north, east, south, and west
enum display_navigation{north, east, south, west} direction;
void updateNavigation(){
    if(A0){
       switch(direction){
           case north:
               if(isEast){
                   direction = east;
               }else if(isWest){
                   direction = west;
               }else if(isSouth){
                   direction = south;
               }else{
                   direction = north;
               }
               break;
           case east:
               if(isNorth){
                   direction = north;
               }else if(isWest){
                   direction = west;
               }else if(isSouth){
                   direction = south;
               }else{
                   direction = east;
               }
               break;
           case south:
               if(isEast){
                   direction = east;
               }else if(isWest){
                   direction = west;
               }else if(isNorth){
                   direction = north;
               }else{
                   direction = south;
               }
               break;
           case west:
               if(isEast){
                   direction = east;
               }else if(isNorth){
                   direction = north;
               }else if(isSouth){
                   direction = south;
               }else{
                   direction = west;
               }
               break;
       }
       switch(direction){
           case north:
               B4 = 1;
               B5 = 0;
               B6 = 0;
               B7 = 0;
               break;
           case east:
               B4 = 0;
               B5 = 1;
               B6 = 0;
               B7 = 0;
               break;
           case south:
               B4 = 0;
               B5 = 0;
               B6 = 1;
               B7 = 0;
               break;
           case west:
               B4 = 0;
               B5 = 0;
               B6 = 0;
               B7 = 1;
               break;
       }
   }
}

// This is a synchronous state machine which is the warning center
// This basically tells the captain what to avoid and which 
// direction to go in to prevent shipwreck. 
enum warning_control{init, searching, iceberg, storm, passingShip, maintaining, shipwreck} warning;

int maintained = 0;
int tenSecondTimer = 10;
int directionChanged = 0;
int incident = 0;

void warningControls(){
    switch(warning){
        case init:
            if(A0){
                warning = searching;
            }else if(!A0){
                warning = init;
            }
            break;
        case searching:
            maintained = 0;
            if(A0){
               if(warningTimer){
                   warningTimer = warningTimer - 1;
                   warning = searching;
               }else if(warningTimer == 0){
                   warningTimer = (rand() % 10) + 11;
                   incident = 0;//rand() % 3;
                   if(incident == 0){
                       warning = iceberg;
                   }else if(incident == 1){
                       warning = storm;
                   }else if(incident == 2){
                       warning = passingShip;
                   }
               }
            }
            break;
        case iceberg:
            if(A0){
               printf("---------------------------------------------------------------------\n");
               printf("\r\n   ICEBERG STRAIGHT AHEAD\r\n\n");
               printf("CHANGE DIRECTIONS IN 10 SECONDS OR FACE SHIPWRECK\r\n\n\n\n");
               printf("---------------------------------------------------------------------");
               warning = maintaining;
            }
            break;
        case storm:
            if(A0){
               printf("---------------------------------------------------------------------");
               printf("\r\n\n\n    STORM STRAIGHT AHEAD\r\n\n\n\n");
               printf("CHANGE DIRECTIONS IN 10 SECONDS OR FACE SHIPWRECK\r\n\n\n\n");
               printf("---------------------------------------------------------------------");
               warning = maintaining;
            }
            break;
        case passingShip:
            if(A0){
               printf("---------------------------------------------------------------------");
               printf("\r\n\n\n    SHIP STRAIGHT AHEAD\r\n\n\n\n");
               printf("CHANGE DIRECTIONS IN 10 SECONDS OR FACE SHIPWRECK\r\n\n\n\n");
               printf("---------------------------------------------------------------------");
               warning = maintaining;
            }
            break;
        case maintaining:
            B3 = 1;
            if(A0){
               directionChanged = checkNavigationShift();
               if(directionChanged){
                   directionChanged = 0;
                   printf("\r\n\n\n\n WARNING IS CLEARED \r\n\n\n\n");
                   maintained = 1;
               }
            }
            break;
        case shipwreck:
            break;
    }
    switch(warning){
        case init:
            if(A0){
                warning = searching;
            }else if(!A0){
                warning = init;
            }
            break;
        case searching:
            B3 = 0;
            break;
        case iceberg:
            break;
        case storm:
            break;
        case passingShip:
            break;
        case maintaining:
            if(tenSecondTimer == 0){
                warning = shipwreck;
            }else if(A0 && maintained){
               tenSecondTimer = 10;
               warning = searching;
            }else if(A0 && !maintained && tenSecondTimer){
                tenSecondTimer = tenSecondTimer - 1;
                warning = maintaining;
            }
            break;
        case shipwreck:
            B = (B & 0xFF);
            engineOn = 0;
            printf("\n\n\n\nYOU HAVE WRECKED YOUR SHIP\n\n\n\n\n\n\n");
            break;
    }
}

int main(){
    
    printf("\n"); // prints new line to start animation
 
    TimerSet(period); // set to period
    TimerOn(); // Turn timer on
    
    // initialize states
    navigation = engineOff;
    propellor = propellorsOff;
    direction = north;
    warning = init;
    
    while(1){ 
      if(A0){
          B0 = 1;
      }else if(!A0){
          printf("\nSHIP HAS TURNED OFF\n");
          B0 = 0;
      }
      TimerSet(period); // set to period
      TimerOn(); // Turn timer on
      
      // call functions synchronously
      updateNavigation();
      boatNavigation();
      propellorsControl();
      warningControls();
      
      while(!TimerFlag){
      }// Wait 1 sec
      TimerFlag = 0;
        
    }
    return 0;
}
