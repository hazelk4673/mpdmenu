
build: ncless
	mkdir build; mv ncless build/; mv mpdcmd build/;

ncless: ncless.c
	$(CC) ncless.c -o ncless

install: ncless mpdcmd
	cp build.*
