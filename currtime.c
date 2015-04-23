// currtime 0.2.3
// Copyright (c) 2015 Neel Chuahan <neel@neelc.org>
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

#define APP_NAME "currtime"
#define APP_VER "0.2.3"
#define APP_COPYYEAR "2015"
#define APP_AUTHOR "Neel Chauhan"
#define APP_AUTHOR_EMAIL "neel@neelc.org"

#define T_MINUTE 60
#define T_HOUR 60 * T_MINUTE
#define T_DAY 24 * T_HOUR

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int nflag = 0;
int tflag = 0;
int Tflag = 0;
int eflag = 0;
char *efexec;
long int tickno = 0;

void calcticks(char *str);
void runclock(void);
void showusage(char *binname);

void stnline(char *str) {
	int length;
	length = strlen(str);
	str[length-1] = '\0';
}


int main(int argc, char *argv[]) {
	int carg;
	opterr = 0;
	while ((carg = getopt(argc, argv, "ntT:e:h")) != -1) {
		switch (carg) {
			case 'n':
				nflag = 1;
				break;
			case 't':
				tflag = 1;
				break;
			case 'T':
				Tflag = 1;
				//tickno = strtol(optarg, NULL, 10);
				//putchar(optarg[strlen(optarg)-1]);
				//putchar('\n');
				calcticks(optarg);
				break;
			case 'e':
				eflag = 1;
				efexec = (char*) malloc(strlen(optarg)+1);
				strcpy(efexec, optarg);
				break;
			case 'h':
				showusage(argv[0]);
				break;
			case '?':
				if (optopt == 'c')
					fprintf(stderr, "Option -%c requires an argument\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'\n", optopt);
				else
					fprintf(stderr, "Unknown option character `\\x%x'\n", optopt);
				return 1;
				break;
			default:
				abort();
				break;
		}
	}
	if ((eflag == 1) && (Tflag == 0)) {
		printf("Option -e requires -T to be set\n");
		exit(-1);
	}
	runclock();
}

void calcticks(char *str) {
	int endch;
	tickno = strtol(optarg, NULL, 10);
	endch = optarg[strlen(optarg)-1];
	switch (toupper(endch)) {
		case 'M':
			tickno = tickno * T_MINUTE;
			break;
		case 'H':
			tickno = tickno * T_HOUR;
			break;
		case 'D':
			tickno = tickno * T_DAY;
			break;
		default:
			break;
	}
}

void runclock(void) {
	char current[64];
	char output[64];
	long int currticks = 0;
	time_t curr_time;
	struct tm *loc_time;

	for (;;) {
		curr_time = time(NULL);
		loc_time = localtime(&curr_time);
		strcpy(current, asctime(loc_time));
		stnline(current);

		switch (strcmp(current, output)) {
			case 0:
				break;
			default:
				strcpy(output, current);
				putchar('\r');
				switch (tflag) {
					case 1:
						printf("%ld\t", currticks);
						break;
					default:
						break;
				}
				printf("%s", output);
				switch (nflag) {
					case 1:
						putchar('\n');
						break;
					case 0:
					default:
						fflush(stdout);
						break;
				}
				sleep(1);
				currticks++;
				break;
		}

		if (Tflag == 1) {
			if (currticks == tickno) {
				putchar('\n');
				if (eflag == 1) {
					system(efexec);
					free(efexec);
				}
				exit(0);
			}
		}
	}
}

void showusage(char *binname) {
	printf("%s %s\n", APP_NAME, APP_VER);
	printf("Copyright %s %s <%s>\n", APP_COPYYEAR, APP_AUTHOR, APP_AUTHOR_EMAIL);
	printf("Usage: %s [-tnh] [-T TICKS] [-e EXECNAME]\n", binname);
	printf("Options include:\n");
	printf("  -n\t\tDisplay the time on a new line for every tick (second)\n");
	printf("  -t\t\tPrint tick number \n");
	printf("  -T TICKS\tRun for 'TICKS' number of ticks, or for 'TICKS'\n");
	printf("\t\tnumber of minutes, hours, or days with a suffix of M, H, or D\n");
	printf("\t\trespectively (suffixes are not case sensitive, but only one \n");
	printf("\t\tsuffix can be used at runtime)\n");
	printf("  -e\t\tExecute 'EXECNAME' after 'TICKS' number of ticks (requires -T\n");
	printf("\t\tto be set)\n");
	printf("  -h\t\tDisplay this help\n");
	exit(0);
}
