CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = \
	lcd_main \



all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o vgcore.* hybrid_main


# lcd clock problem

# lcd_main based only on assembly
lcd_main : lcd_main.o lcd_clock.o lcd_update_asm.o
	$(CC) -o $@ $^

lcd_main.o : lcd_main.c lcd.h
	$(CC) -c $<

lcd_clock.o : lcd_clock.c lcd.h
	$(CC) -c $<

# required assmebly implementation
lcd_update_asm.o : lcd_update_asm.s
	$(CC) -c $<

# C version of functions
lcd_update.o : lcd_update.c lcd.h
	$(CC) -c $<

# main which uses both assmebly and C update functions for incremental
# testing
hybrid_main : lcd_main.o lcd_clock.o lcd_update_asm.o lcd_update.o
	$(CC) -o $@ $^


# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test: test-p1

test-p1 : test_lcd lcd_main
	@printf "===TESTS for P1===\n"
	@printf "Running binary tests for lcd_update_asm.s\n"
	./test_lcd
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_lcd
	@printf "\n"
	@printf "Running shell tests for lcd_main\n"
	./test_lcd_main.sh lcd_main

test_lcd : test_lcd.o lcd_clock.o lcd_update_asm.o
	$(CC) -o $@ $^

test_lcd.o : test_lcd.c
	$(CC) -c $<

clean-tests : clean
	rm -f test-data/*.txt test-data/*.diff test_lcd test_hybrid


# test hybrid for incremental work
test-hybrid : test_hybrid hybrid_main
	@printf "===TESTS for Hybrid===\n"
	@printf "Running binary tests for hybrid\n"
	./test_hybrid
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_hybrid
	@printf "\n"
	@printf "Running shell tests for lcd_main\n"
	./test_lcd_main.sh hybrid_main


# hybrid test program
test_hybrid : test_lcd.o lcd_clock.o lcd_update_asm.o lcd_update.o
	$(CC) -o $@ $^
