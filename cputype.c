#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint32_t uint32;

#define FAT_MAGIC 0xCAFEBABE

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <mach-o file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    uint32 magic;
    if (fread(&magic, sizeof(magic), 1, fp) != 1) {
        perror("fread");
        fclose(fp);
        return 1;
    }

    if (magic == FAT_MAGIC) {
        printf("Universal binary\n");
        // Handle universal binary (multiple architectures)
        // ...
    } else {
        // Handle single-architecture binary
        // ...
        uint32 cputype;
        if (fread(&cputype, sizeof(cputype), 1, fp) != 1) {
            perror("fread");
            fclose(fp);
            return 1;
        }

        switch (cputype) {
            case 0x07:
                printf("x86\n");
                break;
            case 0x01000007:
                printf("x86_64\n");
                break;
            case 0x0100000C:
                printf("arm64\n");
                break;
            default:
                printf("Unknown CPU type: 0x%x\n", cputype);
        }
    }

    fclose(fp);
    return 0;
}
