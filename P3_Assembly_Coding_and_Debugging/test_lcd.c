#include <string.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "lcd.h"

// Struct to hold test information
typedef struct {
  char *id;
  int ret;
  int time_sec;
  tod_t tod;
  int bits;
} lcd_test_case;

int passed = 0;
int testn = 0;

#define TOLERANCE 1e-4

int compare_tod_t(tod_t x, tod_t y){
  return
    x.hours == y.hours &&
    x.minutes == y.minutes &&
    x.seconds == y.seconds &&
    x.ispm == y.ispm;
}

void print_tod_t(tod_t tod){
  printf("{\n"); 
  printf("  .hours   = %d,\n",tod.hours);
  printf("  .minutes = %d,\n",tod.minutes);
  printf("  .seconds = %d,\n",tod.seconds);
  printf("  .ispm    = %d,\n",tod.ispm);
  printf("}\n");
}

void print_test_description(char *function, char *id){
  printf("Test %2d: %-30s %-16s : ",testn,function,id);
}

void test_set_tod_from_secs(lcd_test_case test){
  testn++;
  print_test_description("set_tod_from_secs()",test.id);
  int time_sec = test.time_sec;
  tod_t expect = test.tod;
  int expect_ret = test.ret;
  
  TIME_OF_DAY_SEC = -1;
  LCD_DISPLAY_PORT = -1;

  tod_t actual = {};            // all 0s
  int actual_ret = set_tod_from_secs(time_sec, &actual);

  if(TIME_OF_DAY_SEC != -1 || LCD_DISPLAY_PORT != -1){
    printf("FAILED\n");
    printf("Do not modify TIME_OF_DAY_SEC or LCD_DISPLAY_PORT in this function\n");
    return;
  }
  else if( !compare_tod_t(actual,expect) || actual_ret!=expect_ret){
    printf("FAILED\n");
    printf("Actual and Expected tod_t differ or return value wrong\n");
    printf("time_sec: %d\n",time_sec);
    printf("EXPECT: "); print_tod_t(expect);
    printf("ACTUAL: "); print_tod_t(actual);
    printf("EXPECT return %d\n",expect_ret);
    printf("ACTUAL return %d\n",actual_ret);
    return;
  }
  passed++;
  printf("OK\n");
}    

void test_set_display_bits_from_tod(lcd_test_case test){
  tod_t tod = test.tod;
  int expect = test.bits;
  int expect_ret = test.ret;

  testn++;
  print_test_description("set_display_bits_from_tod()",test.id);
  TIME_OF_DAY_SEC = -1;
  LCD_DISPLAY_PORT = -1;

  int actual = 0;
  int actual_ret = set_display_bits_from_tod(tod, &actual);

  if(TIME_OF_DAY_SEC != -1 || LCD_DISPLAY_PORT != -1){
    printf("FAILED");
    printf("Do not modify TIME_OF_DAY_SEC or LCD_DISPLAY_PORT in this function\n");
    return;
  }
  else if(actual != expect || actual_ret!=expect_ret){
    printf("FAILED\n");
    printf("Actual and Expected bits differ\n");
    printf("tod = \n"); print_tod_t(tod);
    printf("         3         2         1         0\n");
    printf("index:  10987654321098765432109876543210\n");
    printf("EXPECT: "); showbits(expect); printf("\n");
    printf("ACTUAL: "); showbits(actual); printf("\n");
    printf("guide:     |      |      |      |      |\n");
    printf("\n");
    printf("EXPECT return %d\n",expect_ret);
    printf("ACTUAL return %d\n",actual_ret);
    return;
  }
  passed++;
  printf("OK\n");
}

void test_lcd_update(lcd_test_case test){
  testn++;
  print_test_description("lcd_update()",test.id);
  TIME_OF_DAY_SEC = test.time_sec;
  LCD_DISPLAY_PORT = -1;
  int expect_bits = test.bits;
  int expect_time = test.time_sec;
  int expect_ret  = test.ret;
  int actual_ret = lcd_update();
  if(LCD_DISPLAY_PORT != expect_bits || TIME_OF_DAY_SEC != expect_time || actual_ret!=expect_ret){
    printf("FAILED\n");
    printf("Expect TIME_OF_DAY_SEC: %d\n",expect_time);
    printf("Actual TIME_OF_DAY_SEC: %d\n",TIME_OF_DAY_SEC);
    printf("Bits of LCD_DISPLAY_PORT\n");
    printf("index:  10987654321098765432109876543210\n");
    printf("EXPECT: "); showbits(expect_bits); printf("\n");
    printf("ACTUAL: "); showbits(LCD_DISPLAY_PORT); printf("\n");
    printf("guide:     |      |      |      |      |\n");
    printf("EXPECT return %d\n",expect_ret);
    printf("ACTUAL return %d\n",actual_ret);
    printf("\n");
    return;
  }
  passed++;
  printf("OK\n");
  return;
}


// Global array of test data; terminated by a struct with time_sec=-1 
lcd_test_case tests[] = {
  { .id = "midnight",
    .ret = 0,
    .time_sec = 0,
    .tod = {
      .hours   = 12,
      .minutes = 0,
      .seconds = 0,
      .ispm    = 0,
    },
    .bits=0b00010000110101101101111110111111,
  },
  { .id = "after-midnight",
    .ret = 0,
    .time_sec = 101,
    .tod = {
      .hours   = 12,
      .minutes = 1,
      .seconds = 41,
      .ispm    = 0,
    },
    .bits=0b00010000110101101101111110000110,
  },
  { .id = "1am",
    .ret = 0,
    .time_sec = 4170,
    .tod = {
      .hours   = 1,
      .minutes = 9,
      .seconds = 30,
      .ispm    = 0,
    },
    .bits=0b00010000000000011001111111101111,
  },
  { .id = "nearly-noon",
    .ret = 0,
    .time_sec = 43199,
    .tod = {
      .hours   = 11,
      .minutes = 59,
      .seconds = 59,
      .ispm    = 0,
    },
    .bits=0b00010000110000011011011011101111,
  },
  { .id = "noon",
    .ret = 0,
    .time_sec = 43200,
    .tod = {
      .hours   = 12,
      .minutes = 0,
      .seconds = 0,
      .ispm    = 1,
    },
    .bits=0b00100000110101101101111110111111,
  },
  { .id = "one-pm",
    .ret = 0,
    .time_sec = 47089,
    .tod = {
      .hours   = 1,
      .minutes = 4,
      .seconds = 49,
      .ispm    = 1,
    },
    .bits=0b00100000000000011001111111100110,
  },
  { .id = "six-pm",
    .ret = 0,
    .time_sec = 67089,
    .tod = {
      .hours   = 6,
      .minutes = 38,
      .seconds = 9,
      .ispm    = 1,
    },
    .bits=0b00100000000111110110011111111111,
  },
  { .id = "neary-midnight",
    .ret = 0,
    .time_sec = 86399,
    .tod = {
      .hours   = 11,
      .minutes = 59,
      .seconds = 59,
      .ispm    = 1,
    },
    .bits=0b00100000110000011011011011101111,
  },
  { .id="afternoon",
    .ret = 0,
    .time_sec = 46348,
    .tod = {
      .hours   = 12,
      .minutes = 52,
      .seconds = 28,
      .ispm    = 1,
    },
    .bits=0b00100000110101101111011011011011,
  },


  { .time_sec = -1,  },
};

void test_error_conditions(){
  lcd_test_case neg_time = {
    .id="err-neg-time",
    .ret = 1,
    .time_sec = -5,
    .tod = {
      .hours   = 0,
      .minutes = 0,
      .seconds = 0,
      .ispm    = 0,
    },
    .bits=0,
  };
  lcd_test_case tod_off = {
    .id="err-tod-off",
    .ret = 1,
    .time_sec = -1,
    .tod = {
      .hours   = 8,
      .minutes = 72,
      .seconds = 23,
      .ispm    = 1,
    },
    .bits=0,
  };
  lcd_test_case big_time = {
    .id="err-big-time",
    .ret = 1,
    .time_sec = 90000,
    .tod = {
      .hours   = 0,
      .minutes = 0,
      .seconds = 0,
      .ispm    = 0,
    },
    .bits=-1,
  };

  test_set_tod_from_secs(neg_time);
  test_set_display_bits_from_tod(tod_off);
  test_lcd_update(big_time);
}  



int main(int argc, char **argv){
  int i;

  // Run set_tod_from_secs() tests
  for(i=0; tests[i].time_sec != -1; i++){
    test_set_tod_from_secs(tests[i]);
  }
    
  // Run display_bits_from_tod() tests
  for(i=0; tests[i].time_sec != -1; i++){
    test_set_display_bits_from_tod(tests[i]);
  }

  // Run lcd_update() tests
  for(i=0; tests[i].time_sec != -1; i++){
    test_lcd_update(tests[i]);
  }

  test_error_conditions();

  printf("Overall Results: %d / %d\n",passed,testn);

  return 0;
}
