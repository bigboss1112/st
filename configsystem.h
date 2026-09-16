#ifndef CFGSYSTEM_H
#define CFGSYSTEM_H 1

#include <stdio.h>
#include <stdbool.h>

typedef struct {
	char** keys;
	char** vals;
	size_t used;
	size_t size;
} config;

extern config* configNew();
extern bool configAddPair(config* cfg, char* key, char* val);
extern bool configAddLine(config* cfg, char* line);
extern bool configLoadFile(config* cfg, char* path);
extern void configDelete(config* cfg);
extern size_t configMatchI(config* cfg, char* key);
extern char* configMatchS(config* cfg, char* key, bool dup);

#endif
