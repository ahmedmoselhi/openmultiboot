#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "omb_common.h"
#include "omb_log.h"
#include "omb_branding.h"

omb_device_item *omb_branding_read_info(const char* base_dir, const char *identifier)
{
	char version[255];
	char name[255];
	char distro_cmd[512];
	char version_cmd[512];

	version[0] = name[0] = '\0';

	sprintf(distro_cmd, "%s %s %s/usr/lib/enigma2/python image_distro", OMB_PYTHON_BIN, OMB_BRANDING_HELPER_BIN, base_dir);
	FILE *fd = popen(distro_cmd, "r");
	if (fd) {
		char buffer[255];
		char *line = fgets(buffer, sizeof(buffer), fd);
		if (line) {
			strtok(line, "\n");
			strncpy(name, line, sizeof(name));
		}
		pclose(fd);
	}
	
	sprintf(version_cmd, "%s %s %s/usr/lib/enigma2/python image_version", OMB_PYTHON_BIN, OMB_BRANDING_HELPER_BIN, base_dir);
	fd = popen(version_cmd, "r");
	if (fd) {
		char buffer[255];
		char *line = fgets(buffer, sizeof(buffer), fd);
		if (line) {
			strtok(line, "\n");
			strncpy(version, line, sizeof(version));
		}
		pclose(fd);
	}
	
	if (strlen(name) == 0)
		strcpy(name, identifier);
		
	omb_device_item *item = malloc(sizeof(omb_device_item));
	item->label = malloc(strlen(name) + strlen(version) + 2);
	item->directory = malloc(strlen(base_dir) + 1);
	item->identifier = malloc(strlen(identifier) + 1);
	item->next = NULL;
	sprintf(item->label, "%s %s", name, version);
	strcpy(item->directory, base_dir);
	strcpy(item->identifier, identifier);
	return item;
}

