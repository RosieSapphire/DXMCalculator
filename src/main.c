#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

enum dose_range {
	DOSE_RANGE_LOW,
	DOSE_RANGE_MEDIUM,
	DOSE_RANGE_HIGH,
	DOSE_RANGE_COUNT,
};

enum flags {
	FLAG_NONE,
	FLAG_USE_POUNDS,
	FLAG_USE_ROBOTABS,
};

const char *usage = "Usage: DexCalc [-p plateu (1 2 3 4)]"
" [-w weight] [-l] [-r (0 1 2)] [-f]";
const char *warning = "WARNING: Taking more than 1000 mg of DXM Hbr "
"has been linked with permanent adverse rections to the mind and body. "
"Proceed with EXTREME caution!";
const char *danger = "DANGER: This is probably going to kill you. If you're "
"doing this because you're depressed or suicidal, trust me this is no way to "
"die. It's the reason I made this calculator in the first place. Please don't "
"potentially screw up the rest of your life for the sake of a trip.";

int plateu = 0;
int dose_range = 0;
double weight = 0;
enum flags flags = FLAG_NONE;

void handle_options(int opt)
{
	switch(opt) {
	case 'p':
		plateu = atoi(optarg);

		if(plateu >= 1 && plateu <= 4)
			break;

		fprintf(stderr, "There is no plateu %d; only 1, 2, 3, or 4.\n",
				plateu);
		exit(EXIT_FAILURE);

	case 'w':
		weight = atof(optarg);

		break;

	case 'l':
		flags |= FLAG_USE_POUNDS;

		if(!weight) {
			fprintf(stderr, "%s\n", usage);
			exit(EXIT_FAILURE);
		}

		weight /= 2.205;
		break;

	case 'r':
		dose_range = atoi(optarg);

		if(dose_range >= 0 && dose_range <= 2)
			break;

		fprintf(stderr, "%s\n", usage);
		exit(EXIT_FAILURE);

	case 'f':
		flags |= FLAG_USE_ROBOTABS;
		break;

	default:
		fprintf(stderr, "%s\n", usage);
		exit(EXIT_FAILURE);
	}
}

void print_warnings(double dose_mg_hbr)
{
	if(dose_mg_hbr > 2000.0) {
		printf("%s\n", danger);
		exit(EXIT_SUCCESS);
		return;
	}

	if(dose_mg_hbr > 1000.0) {
		printf("%s\n", warning);
		printf("But if you understand the "
				"risks and wish to proceed...\n");
		return;
	}
}

int main(int argc, char **argv)
{
	if(argc == 1) {
		fprintf(stderr, "%s\n", usage);
		exit(EXIT_FAILURE);
	}

	int opt;

	while((opt = getopt(argc, argv, "p:w:lr:f")) != -1) {
		handle_options(opt);
	}

	double plateu_mg_per_kg[DOSE_RANGE_COUNT][4] = {
		{1.5, 2.5,  7.5,  15.0},
		{2.0, 5.0, 11.25, 16.5},
		{2.5, 7.5, 15.0,  18.0},
	};

	if(!weight) {
		fprintf(stderr, "You must specify a weight "
				"using the -w flag.\n");
		exit(EXIT_FAILURE);
	}

	double dose_mg_hbr = plateu_mg_per_kg[dose_range][plateu - 1] * weight;

	if(flags & FLAG_USE_POUNDS) {
		weight *= 2.205;
	}

	char dose_string[128];

	if(flags & FLAG_USE_ROBOTABS) {
		double tmp = dose_mg_hbr;
		int dose_robotabs = 0;

		tmp /= 1.3;
		tmp /= 30.0;
		dose_robotabs = round(tmp);
		sprintf(dose_string, "%d Robotabs", dose_robotabs);
	} else {
		sprintf(dose_string, "%.2f mg DXM Hbr", dose_mg_hbr);
	}

	const char *dose_range_strings[DOSE_RANGE_COUNT] = {
		"low range", "mid range", "high range"
	};

	print_warnings(dose_mg_hbr);
	printf("For a %s plateu %d trip, weighing %.2f %s, take %s.\n",
			dose_range_strings[dose_range], plateu, weight,
			(flags & FLAG_USE_POUNDS) ? "lbs" : "kg", dose_string);

	exit(EXIT_SUCCESS);
}
