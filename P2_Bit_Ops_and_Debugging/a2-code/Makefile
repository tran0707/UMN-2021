CFLAGS = -Wall -g
CC     = gcc $(CFLAGS)

PROGRAMS = \
	lcd_main \
	puzzlebox \


all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o


# lcd clock problem
lcd_main : lcd_main.o lcd_clock.o lcd_update.o
	$(CC) -o $@ $^

lcd_main.o : lcd_main.c lcd.h
	$(CC) -c $<

lcd_clock.o : lcd_clock.c lcd.h
	$(CC) -c $<

lcd_update.o : lcd_update.c lcd.h
	$(CC) -c $<


# debugging problem
#puzzlebox.c : puzzlebox_commented.c
#	./strip-comments.awk $< > $@

puzzlebox.o : puzzlebox.c
	$(CC) -c $<

puzzlebox : puzzlebox.o
	$(CC) -o $@ $^



# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test: test-p1 test-p2

test-p1 : test_lcd lcd_main
	@printf "===TESTS for P1===\n"
	@printf "Running binary tests for lcd_update.c\n"
	./test_lcd
	@printf "\n"
	@printf "Running tests in Valgrind\n"
	$(VALGRIND) ./test_lcd
	@printf "\n"
	@printf "Running shell tests for lcd_main\n"
	./test_lcd_main.sh

test_lcd : test_lcd.o lcd_clock.o lcd_update.o
	$(CC) -o $@ $^

test_lcd.o : test_lcd.c
	$(CC) -c $<


test-p2 : puzzlebox
	./puzzlebox input.txt


clean-tests : clean
	rm -f test-data/*.txt

