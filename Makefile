all: claw claw-cli
claw: claw.c
	gcc -o claw claw.c -L/usr/include -lusb-1.0 -Og
claw-cli: claw-cli.c
	gcc -o claw-cli claw-cli.c -L/usr/include -Og
