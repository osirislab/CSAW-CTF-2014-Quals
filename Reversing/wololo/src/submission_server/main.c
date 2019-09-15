#include "common.h"
#include <err.h>
#include "../validate/validate.c"

const int LPORT = 2510;
const char *USER = "wololo";

int handleConnection(int sd) 
{

	sendMsg(sd, "I'm ready to accept your input file!\n\n"
			"Run this with: python wololo_x.py hostname port file_to_submit\n\n"
			"#!/usr/bin/env python\n\n"
			"import sys, socket, struct\n"
			"s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
			"s.connect((sys.argv[1], int(sys.argv[2])))\n"
			"print s.recv(1024)\n\n"
			"contents = open(sys.argv[3], \"rb\").read()\n"
			"s.send(struct.pack(\"<I\", len(contents)) + contents)\n\n"
			"print \"The challenge server says: \", s.recv(1024)\n");

	uint32_t input_file_size = 0;
	unsigned char *input_file = NULL;

	readAll(sd, (char *)&input_file_size, sizeof(uint32_t));

	if (input_file_size <= 0x4000)
	{

		input_file = calloc(input_file_size, sizeof(unsigned char));
		if (!input_file)
			err(1, "Could not allocate memory");

		if (input_file_size != readAll(sd, (char *)input_file, input_file_size))
			err(2, "Did not read all the bytes");

		if (!validate_database(input_file, input_file_size)) 
		{
			sendMsg(sd, "Sorry, your file did not pass all the checks.\n");
			return 0;
		}

		if (!check_login(input_file, input_file_size))
		{
			sendMsg(sd, "Sorry, login data is incorrect.\n");
			return 0;
		}

		sendMsg(sd, "Valid! key{Small Group of Helpless Villages? Call in the Trebuchets.}\n");

	} 
	else
	{
		sendMsg(sd, "Error! Your file size exceeds 0x4000 bytes!\n");
	}


	return 0;
}

