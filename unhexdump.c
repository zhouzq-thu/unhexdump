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
#include <getopt.h>
#include <string.h>

void print_usage(FILE* stream, char *program_name)
{
    fprintf(stream, "Usage:  %s -i inputfile -o outputfile\n", program_name);
    fprintf(stream,
            "  -h            Display this usage information.\n"
            "  -i  filename  Input file written by 'hexdump -C'.\n"
            "  -o  filename  Output file.\n"
            );
    exit(EXIT_FAILURE);
}

char *ltrim(char *s)
{
    while (*s == ' ') s++;
    return s;
}

int main(int argc, char *argv[])
{
    char *ifilename = NULL;
    char *ofilename = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i': ifilename = optarg; break;
            case 'o': ofilename = optarg; break;
            case 'h':
            default:
                print_usage(stdout, argv[0]);
        }
    }
    
    if (!ifilename || !ofilename) {
        print_usage(stdout, argv[0]);
    }

    FILE *ifile = fopen(ifilename, "r");
    FILE *ofile = fopen(ofilename, "wb");

    unsigned char ch;
    int gap = 0;
    const char delims[] = " ";
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
                fseek(ofile, offset - 1, SEEK_SET);
                fwrite(&ch, 1, 1, ofile);
            }
            gap = 0;
        }

        while ((result = strtok(NULL, delims)) != NULL) {
            if (result[0] == '|') break;
            sscanf(result, "%hhx", &ch);
            fwrite(&ch, 1, 1, ofile);
        }
    }
}
