/*#include <stdio.h>
#include "lcd.h"

int set_tod_from_secs (int time_of_day_sec, tod_t *tod){
    if (time_of_day_sec  < 0 || time_of_day_sec > 86400){
        return 1;
    }
    else{
       int result_hours = tod->hours = (time_of_day_sec / 3600);
       tod->minutes = (time_of_day_sec -(tod->hours *3600))/60;
       tod->seconds = (time_of_day_sec -(tod->hours *3600)-(tod->minutes *60));
       /////////// 3 Cases For Hours////////
       if(tod->hours > 12){
           tod->hours = tod->hours - 12;
       }
       else if(tod->hours == 0){
           tod->hours = 12;
       }
       else{
           tod->hours = (time_of_day_sec / 3600);
       }
       //////////////// AM-PM ///////////////
       if(result_hours > 11 && result_hours !=24){
           tod->ispm = 1;
       }
       else{
           tod->ispm = 0;
       }
    }
    return 0;
}

int set_display_bits_from_tod(tod_t tod, int *display){
    if((tod.hours < 0 || tod.minutes <0 || tod.seconds < 0)||(tod.hours > 12) || tod.minutes > 59 ||tod.seconds >59 ){
        return 1;
    }
    int bit_array[10]= {0b0111111,0b0000110,0b1011011,0b1001111,0b1100110,0b1101101,0b1111101,0b0000111,0b1111111,0b1101111};
    int min_one, min_tens , hour_one, hour_tens;
    min_one = (tod.minutes) % 10;
    min_tens = (tod.minutes/10) % 10;
    hour_one = (tod.hours) % 10;
    hour_tens = (tod.hours /10) % 10;
    int state = 0;
    /////////min_one////////////
    min_one = bit_array[min_one];
    state = state|min_one;
    /////////min_tens////////////
    min_tens = bit_array[min_tens];
    state = (min_tens<<7)|state;
    /////////hour_one////////////
    hour_one = bit_array[hour_one];
    state = (hour_one<<14)|state;
    /////////hour_tens///////////
    if(hour_tens == 1){
        hour_tens = bit_array[hour_tens];
        state = (hour_tens<<21)|state;
    }
    else{//Blank case for the hour_tens
        hour_tens = 0b0000000;
        state = (hour_tens <<21)|state;
    }
    ///////////AM-PM/////////////
    if (tod.ispm == 0 ){
        tod.ispm = 0b0001;
        state = (tod.ispm << 28)|state;
    }
    else{
        tod.ispm = 0b0010;
        state = (tod.ispm<<28)|state;
    }
    *display = state;
    return 0;
}

int lcd_update(){
    if(TIME_OF_DAY_SEC > 86400||TIME_OF_DAY_SEC <0){
        return 1;
    }
    else{
        tod_t tod;
        set_tod_from_secs(TIME_OF_DAY_SEC , &tod);
        set_display_bits_from_tod(tod,&LCD_DISPLAY_PORT);
    }
    return 0;
}
*/
