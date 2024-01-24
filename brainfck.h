#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct sbuilder {
	char *sp;
	size_t len;
};

// a lexer contains:
// - a pointer to an array of tokens
// - a position in that array
// - a token array size (currently not used)
// - a stack (which holds values for the data cells
// - a stack position to iterate through the stack
// - the size of the stack
struct lexer {
	char *toks;
	size_t tokssize;
	size_t toksp;
	int *stk;
	size_t stkp;
	size_t stksize;
};

void freesb(struct sbuilder* sb);
int freadfull(char *fname, struct sbuilder** sb);

struct lexer *createlexer(struct sbuilder* sb);
int next(struct lexer *l);
void freel(struct lexer *l);

int printt(void);
