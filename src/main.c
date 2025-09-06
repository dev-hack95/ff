#include "include/ff.h"
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void search_for_filename(Buffer *buff, const char* currentWorkingDir, const char* searchTerm, int JSON) {
    struct dirent *dir;
    DIR* dp = opendir(currentWorkingDir);
    CHECK_ALLOC(dp);

    int found = 0;

    switch (JSON) {
        case  1:
            // TODO: Write code for json format
            printf("Demo JSON");
            break;

        default:
            while ((dir = readdir(dp)) != NULL) {
                char str[MAX_BUFFER];
                char fullpath[MAX_BUFFER];
                // TODO: Insted of realpath use stat
                realpath(dir->d_name, fullpath);
                long unsigned int written = snprintf(str, sizeof(str), "{\n file: %.4000s\n},\n", fullpath);
                if (written >= sizeof(str)) {
                    fprintf(stderr, "Output truncated");
                    fflush(stderr);
                }
                CHECK_BUFF(add_buffer(buff, str));
                found = 1;
            }

            if (!found) {
                char str[MAX_BUFFER];
                long unsigned int written = snprintf(str, sizeof(str), "{\n msg: no file found \n}");
                if (written >= sizeof(str)) {
                    fprintf(stderr, "Output truncated");
                    fflush(stderr);
                }
                CHECK_BUFF(add_buffer(buff, str));
            }
    }

    closedir(dp);
    return;    
}

void search_in_file_for_text(Buffer *buff, const char* filename, const char* searchTerm, int JSON) {

    // Error Type : Bug; Priority: High;
    // TODO: Check first input file path is present of not

    
    FILE* file = fopen(filename, "r+");
    CHECK_ALLOC(file);
    
    char buffer[MAX_BUFFER];
    int line_number = 0;
    int found = 0;

    switch (JSON) {
    case 1:
        // TODO; Write code for JSON format
        printf("Demo JSON");
        break;    
    default:
        while(fgets(buffer, sizeof(buffer), file)) {
            line_number++;  
            if(strstr(buffer, searchTerm)) {
                char str[MAX_BUFFER];
                long unsigned int written = snprintf(str, sizeof(str),"{\n  linenumber: %d,\n  line: %.4000s},\n", line_number, buffer);
                if (written >= sizeof(str)) {
                    fprintf(stderr, "Warning: Output truncated at line %d\n", line_number);
                    fflush(stderr);
                }
                CHECK_BUFF(add_buffer(buff, str));
                found = 1;
            }
        }
        
        
        if (!found) {
            char str[MAX_BUFFER];
            long unsigned int written = snprintf(str, sizeof(str), "{\n msg: match not found \n}");
            if (written >= sizeof(str)) {
                fprintf(stderr, "Warning: Output truncated \n");
                fflush(stderr);
            }
            CHECK_BUFF(add_buffer(buff, str));
        }
        break;
    }
    
    fclose(file);
    return;
}

void ff(int argc, char *argv[]) {
    Buffer* buff = new_buffer();
    
    if (!strcmp(argv[1], "-f")) {
        if (argc < 4) {
            printf("Usage: %s -f <search_term> <filename> [--json]\n", argv[0]);
            free_buffer(buff);
            return;
        }
        
        if (!file_exists(argv[3])) { 
            printf("File not found: %s\n", argv[3]);
            free_buffer(buff);
            return;
        }
        
        if (argc == 5 && !strcmp(argv[4], "--json")) {
            search_in_file_for_text(buff, argv[3], argv[2], 1);
        } else if (argc == 4) {
            search_in_file_for_text(buff, argv[3], argv[2], 0);
        } else {
            printf("Invalid number of arguments\n");
        }
    }

    printf("%s", buff->data);
    
    free_buffer(buff);
}

int main(int argc, char *argv[]) {

    ff(argc, argv);
    
    return 0;
}
