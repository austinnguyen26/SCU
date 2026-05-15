#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char buffer[10000];
    FILE *fp = fopen(argv[1], "rb");
    FILE *fp2;
    fp2 = fopen("copy.out", "wb");

    if (!fp)
    {
        perror("Error opening file");
        return 1;
    }

    while (fread(buffer, sizeof(buffer), 1, fp))
    {
        fwrite(&buffer, sizeof(buffer), 1, fp2);
    }

    fclose(fp);
    fclose(fp2);
    return 0;
}
