//
//  unhexdump.c
//  unhexdump
//
//  Created by Zhi-Qiang Zhou on 15/6/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char *ltrim(char *s)
{
    while (*s == ' ') s++;
    return s;
}

int main(int argc, char *argv[])
{
    if (argc == 1) {
        printf("usage: %s file\n\n", argv[0]);
        printf("This program reverses 'hexdump -C' output back to the original data.\n");
        exit(0);
    }

    FILE *ifile = fopen(argv[1], "r");
    int gap = 0;
    unsigned char ch;
    const char delims[] = " \n";
    char line[0x400];
    while (fgets(line, sizeof(line), ifile) && strlen(line) > 0)
    {
        char *result = ltrim(line);
        result = strtok(line, delims);

        if (result[0] == '*') {
            gap = 1;
            continue;
        } else {
            if (gap == 1) {
                ch = 0;
                size_t offset;
                sscanf(result, "%lx", &offset);
                fseek(stdout, offset - 1, SEEK_SET);
                fwrite(&ch, 1, 1, stdout);
            }
            gap = 0;
        }

        while ((result = strtok(NULL, delims)) != NULL) {
            if (result[0] == '|') break;
            sscanf(result, "%hhx", &ch);
            fwrite(&ch, 1, 1, stdout);
        }
    }
}
