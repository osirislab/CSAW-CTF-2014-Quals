all:
	gcc -c -Wall -Werror -fpic challengeresponse.c
	gcc -shared -o libchallengeresponse.so challengeresponse.o
	rm challengeresponse.o
	gcc -L$(CURDIR) -Wall -o saturn saturn.c -lchallengeresponse
intel:
	gcc -m32 -c -Wall -Werror -fpic challengeresponse.c
	gcc -m32 -shared -o libchallengeresponse.so challengeresponse.o
	rm challengeresponse.o
	gcc -m32 -L$(CURDIR) -Wall -o saturn saturn.c -lchallengeresponse
	strip saturn
stripped: all
	strip saturn
clean:
	rm *.so saturn
