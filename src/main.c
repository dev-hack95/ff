#include "include/ff.h"
#include <dirent.h>
#include <string.h>

/*
* TODO: 1) Multi thread implementation with queue, try this with single core (i.e 2 threads)
* TODO: 2) Json implmentation 
*/
void search_for_filename(const char* currentWorkingDir, const char* searchTerm, int JSON) {
    struct dirent *dir;
    
    char fullpath[MAX_BUFFER];
    char temp_path[MAX_BUFFER];
    
    DIR* dp = opendir(currentWorkingDir);
    CHECK_ALLOC(dp);
    
    switch (JSON) {
        case 1:
            printf("Demo JSON");
            break;
        default:
            while ((dir = readdir(dp)) != NULL) {
                
                if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                    continue;
                }
                
                long unsigned int path_len = snprintf(temp_path, sizeof(temp_path), "%s/%s", currentWorkingDir, dir->d_name);
                if (path_len >= sizeof(temp_path)) {
                    continue;
                }
                
                if (realpath(temp_path, fullpath)) {
                    if (!CHECK_IS_SYMLINK(fullpath)) {
                        if (CHECK_IS_DIR(fullpath)) {
                            search_for_filename(fullpath, searchTerm, JSON);
                        } else if (strlen(searchTerm) == 0 || strstr(temp_path, searchTerm)) {
                            // append_buffer(buff, "{\n  file_path: %s\n},\n", fullpath);
                            printf("{\n file_path: %s \n},\n", fullpath);
                        }
                    }
                }
            }
            break;
    }
    closedir(dp);
    return;
}


/*
* TODO: Json implmentation
*/
void search_in_file_for_text(Buffer *buff, const char* filename, const char* searchTerm, int JSON) {
    
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
                append_buffer(buff, "{\n linenumber: %d,\n line: %s},\n", line_number, buffer);
                found = 1;
            }
        }
        
        if (!found) {
            append_buffer(buff, "{\n msg: match not found \n}");
        }
        break;
    }
    
    fclose(file);
    return;
}

void help(const char *program_name) {
    printf("Usage:\n");
    printf("  %s -f <search_term> <filename> [--json]  Search for text in a file\n", program_name);
    printf("  %s -d <search_term> <directory>          Search for filenames in a directory\n", program_name);
    printf("\nParameters:\n");
    printf("  -f           Search inside a file\n");
    printf("  -d           Search for files in a directory\n");
    printf("  <search_term> Text string to search for\n");
    printf("  <filename>   File to search within\n");
    printf("  <directory>  Directory to search files in\n");
    printf("  --json       Optional output format (JSON) when searching in a file\n");
    printf("\nExamples:\n");
    printf("  %s -f hello file.txt\n", program_name);
    printf("  %s -d report ./Documents\n", program_name);
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
    } else if (!strcmp(argv[1], "-d")) {
        
        if (!file_exists(argv[3])) { 
            printf("Dir not found: %s\n", argv[3]);
            free_buffer(buff);
            return;
        }
        
        search_for_filename(argv[3], argv[2], 0);
    } else {
        help(argv[0]);
    }
    
    printf("%s", buff->data);
    free_buffer(buff);
}

int main(int argc, char *argv[]) {

    if (!strcmp(argv[1], "--help")) {
        help(argv[0]);
        return 0;
    }

    ff(argc, argv);
    
    return 0;
}
