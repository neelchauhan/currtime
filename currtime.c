// currtime 0.1
// Copyright (c) 2015 Neel Chuahan <neel@neelc.org>.
// All Rights Reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE DISCLAIMED. IN
// NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGE (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void stnline(char *str) {
	int length;
	length = strlen(str);
	str[length-1] = '\0';
}

int main(int argc, char *argv[]) {
	char current[64];
	char output[64];
	time_t curr_time;
	struct tm *loc_time;

	for(;;) {
		curr_time = time(NULL);
		loc_time = localtime(&curr_time);
		strcpy(current, asctime(loc_time));
		stnline(current);

		switch (strcmp(current, output)) {
			case 0:
				break;
			default:
				strcpy(output, current);
				printf("\r%s", output);
				fflush(stdout);
				sleep(1);
				break;
		}
	}
	return 0;
}
