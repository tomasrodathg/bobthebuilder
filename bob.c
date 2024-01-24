#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAXOPTS 30
#define MAXBUILD 30

struct bobcmd { 
	char *exect;
	char *buildcmd;
};

struct bobcmd *create_bob_cmd(char **opts, ssize_t optc, char *progname, char **buildfs, ssize_t buildc) {
	int i;

	assert(buildc >= 1);

	if (buildfs == NULL) {
		printf("Pointer to array of build file names cannot be null\n");
		return NULL;
	}

	if (progname == NULL) {
		printf("Program name cannot be empty\n");
		return NULL;
	}

	struct bobcmd *bob = malloc(1 * sizeof(struct bobcmd));
	if (bob == NULL) {
		printf("Failed to allocate memory to 'bob' the builder!\n");
		return NULL;
	}

	bob->exect = malloc(30 * sizeof(char));
	strcpy(bob->exect, "./");
	strcat(bob->exect, progname);

	bob->buildcmd = malloc(100 * sizeof(char));
	strcpy(bob->buildcmd, "cc ");
	for (i = 0; i < optc; i++) {
		strcat(bob->buildcmd, " ");
		strcat(bob->buildcmd, opts[i]);
	}

	strcat(bob->buildcmd, " -o ");
	strcat(bob->buildcmd, progname);

	for (i = 0; i < buildc; i++) {
		strcat(bob->buildcmd, " ");
		strcat(bob->buildcmd, buildfs[i]);
	}
	
	return bob;
}

void freebob(struct bobcmd *bob) {
	if (bob != NULL) {
		if (bob->buildcmd != NULL)
			free(bob->buildcmd);
	
		if (bob->exect != NULL)
			free(bob->exect);
		free(bob);
	}
}

void printheader(void) {
	printf("\n\n======================\n\n");
}

void printheaderwm(char *msg) {
	printheader();
	printf("%s\n", msg);
}

int run_build(struct bobcmd *bob) {
	printheaderwm("Compiling...\n");
	return system(bob->buildcmd);
}

int execute(struct bobcmd *bob) {
	printheaderwm("Executing program...\n");
	return system(bob->exect);
}

int argidx = 2;
char *nextarg(int argc, char **argv) {
	return argidx < argc ? argv[argidx++] : '\0';
}

void printmenu(void) {
	printheader();
	printf("Press any of the following commands followed by <ENTER>\n");
	printf("Press (q) to quit\n");
	printf("Press (e) to execute\n");
	printf("Press (c) to recompile\n");
	printf("Press (h) to hot reload (i.e. compile and execute)\n");
	printf("Press (m) to see the commands again\n\n");
	printf("Command: ");
}

int main(int argc, char **argv) {
	char in;
	ssize_t optc, buildc;
	struct bobcmd* bob;
	char *opts[MAXOPTS], *buildfs[MAXBUILD];
	
	assert(argc >= 1); 
	optc = buildc = 0;
	char *progname = argv[1];

	char *arg = nextarg(argc, argv);
	while(arg) {
		if (strcmp(arg, "--opts") == 0) { 
			while((arg = nextarg(argc, argv)) && optc < MAXOPTS && strcmp(arg, "--build") != 0) opts[optc++] = arg;
		} else if (optc < MAXOPTS && strcmp(arg, "--hot") == 0) {
			opts[optc++] = "hot";
		}
	
		if (strcmp(arg, "--build") == 0)
			while((arg = nextarg(argc, argv)) && buildc < MAXBUILD) buildfs[buildc++] = arg;

		arg = nextarg(argc, argv);
	}

	bob = create_bob_cmd(opts, optc, progname, buildfs, buildc);
	printf("Bob the builder!\n\n");
	printf("|| build cmd: %s\n", bob->buildcmd);
	printf("||exec cmd: %s\n", bob->exect);

	if(run_build(bob) != 0) {
		printf("Failed to build...\n\n");
	} else {
		printf("Successfully compiled!...\n\n");
	}

	printmenu();

	while((in = getchar()) != EOF) {
		getchar();
		switch(in) {
			case 'q':
				printheaderwm("Goodbye!");
				goto cleanexit;
				break;
			case 'e':
				if(execute(bob) != 0)
					printf("Failed to execute!\nPress (h) to recompile.\n");
				break;
			case 'h':
				if(run_build(bob) != 0)
					printf("Failed to build!\nFix your code and press (h) to recompile.\n");
				printf("Successfully compiled!\n");
				if(execute(bob) != 0)
					printf("Failed to execute!\nPress (h) to recompile.\n");
				break;
			case 'm':
				printmenu();
				break;
			case 'c':
				if(run_build(bob) != 0)
					printf("Failed to build!\nFix your code and press (h) to recompile.\n");
				printf("Successfully compiled!\n");
				break;
			default:
				printf("Try a different command please.\n");
				break;
		}
		printmenu();
	}

	cleanexit:
		freebob(bob);
	
	return 0;
}
