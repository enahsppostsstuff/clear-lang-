#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void trim_newline(char *line) {
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source.clr>\n", argv[0]);
        return 1;
    }

    printf("[DEBUG] Reading from: %s\n", argv[1]);

    FILE *src = fopen(argv[1], "r");
    if (!src) {
        perror("Could not open source file");
        return 1;
    }

    FILE *out = fopen("out.c", "w");
    fprintf(out, "#include <stdio.h>\nint main() {\n");

    char line[256];
    while (fgets(line, sizeof(line), src)) {
        trim_newline(line);

        if (strncmp(line, "say ", 4) == 0) {
            fprintf(out, "  printf(\"%s\\n\");\n", line + 4);
        } else if (strncmp(line, "set ", 4) == 0) {
            char var[64];
            int val;
            if (sscanf(line, "set %s to %d", var, &val) == 2) {
                fprintf(out, "  int %s = %d;\n", var, val);
            }
        } else if (strncmp(line, "add ", 4) == 0) {
            char var[64];
            int val;
            if (sscanf(line, "add %s and %d", var, &val) == 2) {
                fprintf(out, "  printf(\"%%d\\n\", %s + %d);\n", var, val);
            }
        } else if (strncmp(line, "subtract ", 9) == 0) {
            char var[64];
            int val;
            if (sscanf(line, "subtract %s and %d", var, &val) == 2) {
                fprintf(out, "  printf(\"%%d\\n\", %s - %d);\n", var, val);
            }
        } else if (strncmp(line, "multiply ", 9) == 0) {
            char var[64];
            int val;
            if (sscanf(line, "multiply %s and %d", var, &val) == 2) {
                fprintf(out, "  printf(\"%%d\\n\", %s * %d);\n", var, val);
            }
        } else if (strncmp(line, "divide ", 7) == 0) {
            char var[64];
            int val;
            if (sscanf(line, "divide %s by %d", var, &val) == 2) {
                fprintf(out, "  if (%d != 0) {\n", val);
                fprintf(out, "    printf(\"%%d\\n\", %s / %d);\n", var, val);
                fprintf(out, "  } else {\n");
                fprintf(out, "    printf(\"Division by zero error\\n\");\n");
                fprintf(out, "  }\n");
            }
        }
    }

    fprintf(out, "  return 0;\n}\n");
    fclose(src);
    fclose(out);

    system("gcc out.c -o program && ./program");
    return 0;
}

