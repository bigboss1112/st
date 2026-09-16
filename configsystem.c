#include "configsystem.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

config* configNew() {
	return calloc(1, sizeof(config));
}

bool configAddPair(config* cfg, char* key, char* val) {
	if (cfg == NULL || key == NULL || val == NULL) {
		return false;
	}

	cfg->used++;

	if (cfg->used > cfg->size) {
		cfg->size *= 2;
		if (cfg->size == 0) cfg->size = 1;

		char** temp = realloc(cfg->keys, cfg->size * sizeof(char*));

		if (temp == NULL) {
			cfg->used--;
			cfg->size /= 2;

			return false;
		}

		cfg->keys = temp;

		temp = realloc(cfg->vals, cfg->size * sizeof(char*));

		if (temp == NULL) {
			cfg->used--;
			cfg->size /= 2;

			return false;
		}

		cfg->vals = temp;
	}

	cfg->keys[cfg->used - 1] = strdup(key);
	cfg->vals[cfg->used - 1] = strdup(val);

	return true;
}

bool configAddLine(config* cfg, char* line) {
	if (cfg == NULL || line == NULL) {
		return false;
	}

	size_t mid = strcspn(line, "=");

	if (mid == strlen(line)) {
		return false;
	}

	return configAddPair(cfg, strndup(line, mid), strdup(line + mid + 1));
}

bool configLoadFile(config* cfg, char* path) {
	if (cfg == NULL || path == NULL) {
		return false;
	}

	FILE* f = fopen(path, "r");

	if (f == NULL) {
		return false;
	}

	char* line = malloc(128);

	if (line == NULL) {
		return false;
	}

	while (fgets(line, 128, f)) {
		line[strcspn(line, "\n")] = '\0';

		configAddLine(cfg, line);
	}

	free(line);
	fclose(f);
}

void configDelete(config* cfg) {
	for (size_t i = 0; i < cfg->used; i++) {
		free(cfg->keys[i]);
		free(cfg->vals[i]);
	}

	free(cfg->keys);
	free(cfg->vals);
	free(cfg);
}

size_t configMatchI(config* cfg, char* key) {
	if (key == NULL || key == NULL) {
		return 0;
	}

	for (size_t i = 0; i < cfg->used; i++) {
		if (!strcmp(cfg->keys[i], key)) {
			return i;
		}
	}
}

char* configMatchS(config* cfg, char* key, bool dup) {
	if (cfg == NULL || key == NULL) {
		return NULL;
	}

	char* val = NULL;

	if (dup) {
		val = strdup(cfg->vals[configMatchI(cfg, key)]);
	} else {
		val = cfg->vals[configMatchI(cfg, key)];
	}

	return val;
}
