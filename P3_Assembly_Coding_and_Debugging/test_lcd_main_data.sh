#!/bin/bash
T=0                             # global test number


((T++))
tnames[T]="3:25am"
time[T]="12345"
read  -r -d '' output[$T] <<"ENDOUT"
TIME_OF_DAY_SEC set to: 12345
set_tod_from_secs( 12345, &tod );
tod is {       
  .hours   = 3
  .minutes = 25
  .seconds = 45
  .ispm    = 0
}              
Simulated time is: 03 : 25 : 45 am

Checking results for display bits
set_display_bits_from_tod(tod, &state);

state is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00010000000100111110110111101101
guide:    |      |      |      |      |

Running lcd_update()

LCD_DISPLAY_PORT is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00010000000100111110110111101101
guide:    |      |      |      |      |

LCD Clock Display:
         ~~   ~~   ~~          
           |o   | |            
         ~~   ~~   ~~          
AM         |o|       |         
         ~~   ~~   ~~          
ENDOUT

((T++))
tnames[T]="6:40am"
time[T]="24000"
read  -r -d '' output[$T] <<"ENDOUT"
TIME_OF_DAY_SEC set to: 24000
set_tod_from_secs( 24000, &tod );
tod is {       
  .hours   = 6
  .minutes = 40
  .seconds = 0
  .ispm    = 0
}              
Simulated time is: 06 : 40 : 00 am

Checking results for display bits
set_display_bits_from_tod(tod, &state);

state is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00010000000111110111001100111111
guide:    |      |      |      |      |

Running lcd_update()

LCD_DISPLAY_PORT is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00010000000111110111001100111111
guide:    |      |      |      |      |

LCD Clock Display:
         ~~        ~~          
        |   o|  | |  |         
         ~~   ~~               
AM      |  |o   | |  |         
         ~~        ~~          
ENDOUT

((T++))
tnames[T]="6:21pm"
time[T]="66091"
read  -r -d '' output[$T] <<"ENDOUT"
TIME_OF_DAY_SEC set to: 66091
set_tod_from_secs( 66091, &tod );
tod is {       
  .hours   = 6
  .minutes = 21
  .seconds = 31
  .ispm    = 1
}              
Simulated time is: 06 : 21 : 31 pm

Checking results for display bits
set_display_bits_from_tod(tod, &state);

state is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00100000000111110110110110000110
guide:    |      |      |      |      |

Running lcd_update()

LCD_DISPLAY_PORT is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00100000000111110110110110000110
guide:    |      |      |      |      |

LCD Clock Display:
         ~~   ~~               
        |   o   |    |         
         ~~   ~~               
        |  |o|       |         
PM       ~~   ~~               
ENDOUT

((T++))
tnames[T]="11:56am"
time[T]="42994"
read  -r -d '' output[$T] <<"ENDOUT"
TIME_OF_DAY_SEC set to: 42994
set_tod_from_secs( 42994, &tod );
tod is {       
  .hours   = 11
  .minutes = 56
  .seconds = 34
  .ispm    = 0
}              
Simulated time is: 11 : 56 : 34 am

Checking results for display bits
set_display_bits_from_tod(tod, &state);

state is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00010000110000011011011011111101
guide:    |      |      |      |      |

Running lcd_update()

LCD_DISPLAY_PORT is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00010000110000011011011011111101
guide:    |      |      |      |      |

LCD Clock Display:
              ~~   ~~          
      |    |o|    |            
              ~~   ~~          
AM    |    |o   | |  |         
              ~~   ~~          
ENDOUT


((T++))
tnames[T]="10:13pm"
time[T]="80000"
read  -r -d '' output[$T] <<"ENDOUT"
TIME_OF_DAY_SEC set to: 80000
set_tod_from_secs( 80000, &tod );
tod is {       
  .hours   = 10
  .minutes = 13
  .seconds = 20
  .ispm    = 1
}              
Simulated time is: 10 : 13 : 20 pm

Checking results for display bits
set_display_bits_from_tod(tod, &state);

state is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00100000110011111100001101001111
guide:    |      |      |      |      |

Running lcd_update()

LCD_DISPLAY_PORT is:
        3         2         1         0
index: 10987654321098765432109876543210
bits:  00100000110011111100001101001111
guide:    |      |      |      |      |

LCD Clock Display:
         ~~        ~~          
      | |  |o   |    |         
                   ~~          
      | |  |o   |    |         
PM       ~~        ~~          
ENDOUT
