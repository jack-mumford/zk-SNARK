CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11
all: student_snark
student_snark: main.o field.o snark_sim.o
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o student_snark
