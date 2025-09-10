#ifndef __FF_H__
#define __FF_H__

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>

#ifdef _WIN32

#include <io.h>
#define F_OK 0

// Realpath for windows
#define realpath(src, dst) _fullpath((dst), (src), _MAX_PATH)

#else

#include <unistd.h>

#endif


#define MAX_BUFFER 1024 
#define INITIAL_CAPACITY 64

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
} Buffer;


/*
* Enum - buffError_t
* @info - The enum shows different buffer allocation errors 
*/
typedef enum {
  buffSuccess = 0, // Successful Completion
  buffErrorNull = -1, // Check pointer is null or not after allocation of buffer 
  buffReallocMemErr = -2, // Check for buffer realloaction error 
} buffError_t;

#define clean_errno() (errno == 0 ? "None": strerror(errno))

/*
* Macro - CHECK_BUFF
* @info - A buffer check macro which will return buffer check error and exit program from that point to execute
*/
#define CHECK_BUFF(exp) do {                                                                                    \
    const buffError_t err = exp;                                                                                \
    if (err != buffSuccess) {                                                                                   \
        fprintf(stderr, "%s:%d Buffer Error: %s\n", __FILE__, __LINE__, buffGetErrorString(err));               \
        fflush(stderr);                                                                                         \
        exit(EXIT_FAILURE);                                                                                     \
    }                                                                                                           \
} while(0)


/*
* Macro - Check file allocation macro
* @info - This function is used to check the memory allocation of varible if it's allocation is null return error and exit program from that point
*/
#define CHECK_ALLOC(exp) do {                                                                                    \
  if ((exp) == NULL) {                                                                                           \
    fprintf(stderr, "%s:%d %s : ", __FILE__, __LINE__, clean_errno());                                           \
    fflush(stderr);                                                                                              \
    exit(EXIT_FAILURE);                                                                                          \
   }                                                                                                             \
 } while(0) 


/*
* Inline Function - buffGetStringError
* @info - The function will return const char error based on that error
* @param - buffError_t is enum specified with multiple error can be occured in dynamic string buffer allocation
*/
static inline const char* buffGetErrorString(buffError_t buffError) {
  switch (buffError) {
    case buffSuccess:
      return "Succcessful compeltion";
    case buffErrorNull:
      return "Memory allocation failed";
    case buffReallocMemErr:
      return "Memory reallocation failed";
    default:
      return "Unknown buffer error";
    }
}

/*
* Inline Function - new_buffer
* @info - Dynmic buffer creation for with inital capaicty of 64 bytes of data;
* @info - Returns data with inital capacity
*/
static inline Buffer* new_buffer() {
  Buffer* buff = (Buffer*)malloc(sizeof(Buffer));
  if (buff == NULL) {
    return NULL;
  }

  buff->data = (char*)malloc(INITIAL_CAPACITY);
  if (buff->data == NULL) {
    free(buff->data);
    return NULL;
  }

  buff->data[0] = '\0';
  buff->length = 0;
  buff->capacity = INITIAL_CAPACITY;

  return buff;
}

/*
* Inline Function - add_buffer
* @info - This will new buffer string to existing buffer init at heap by using concept of dynamic data allocation
* @param - Buffer* buff --> Initialized buffer
* @param - str ---> constant char* str which is the new sting to append the data
*/
static inline buffError_t add_buffer(Buffer* buff, const char* str) {
  if (buff == NULL || str == NULL) {
    return buffErrorNull;
  }

  size_t str_len = strlen(str);
  size_t needed = buff->length + str_len + 1;

  if (needed > buff->capacity) {
    size_t new_capacity = buff->capacity;
    while (new_capacity < needed) {
      new_capacity *= 2;
    }

    char* new_data = (char*)realloc(buff->data, new_capacity);

    if (new_data == NULL) {
      return buffReallocMemErr;;
    }

    buff->data = new_data;
    buff->capacity = new_capacity;
  }

  strcat(buff->data, str);
  buff->length += str_len;
  return buffSuccess;
}

/*
* Inline Function - free buffer
* @info - Free allocated buffer memory
* @param - Buffer memory
*/
static inline void free_buffer(Buffer *buff) {
  if (buff) {
    free(buff->data);
    free(buff);
  } 
}

/*
* Function - search in file for text 
* @info - The function finds given text in the file
* @param - filename --> Its a file read location in which you have stored a file stream to read on
* @param - searchTerm ---> The text search term which scans and match the data
*/
void search_in_file_for_text(Buffer* buff, const char* filename, const char* searchTerm, int JSON);

/*
* Function - search for fileneame
* @info - Search for filename in current working direcitory
* @param - Current working directory
* @param - "search fiel name"
*/
void search_for_filename(const char* currentWorkingDir, const char* fileSearchTerm, int JSON);


/*
* Function - help
* @info - The function is used to show all command line --help properties
*/
void help(const char* exec_name);

/*
* Function - ff
* @info - A ff runner with logic flow control of cli
* @param - argc is number of input counts
* @param - argv[] strores inputs of cli
*/
void ff(int argc, char* argv[]);

/*
* Function - file_exists
* @info - Check file exist on path or not
* @param - filename 
*/
static inline int file_exists(const char* filename) {

#ifdef _WI32
  if (_access(filename, F_OK) == 0) {
    return 1;
  } else {
    return 0;
  }
#else
  if (access(filename, F_OK) == 0) {
    return 1; // File exist
  } else {
    return 0; // File doesnot exists
  }
#endif

}

/*
* Function(inline function): get_last
* @info - A function is used to get last element after spliting the path
* @param - const char* path is a file path input
*/
static inline char* get_last(const char* path) {
  const char *last_char = strrchr(path, '/');
  if (last_char != NULL ) {
    return (char*)last_char + 1;
  }

  return (char*)path;  
}

/*
* Function(inline function): CHECK_IS_DIR
* @info - The function is used to check the given path is dir or not
* @param - const char* filepath is a path of dir or a file
*/
static inline int CHECK_IS_DIR(const char* filepath) {
  struct stat filestats;
  if (stat(filepath, &filestats) != 0) {
    return 0;
  }

  return S_ISDIR(filestats.st_mode);
}

/*
* Function(inline function) : CHECK_IS_SYMLINK
* @info - Check the path is symlink or not
* @param - const char* filepath is a path of dir 
*/ 
static inline int CHECK_IS_SYMLINK(const char* filepath) {
  struct stat filestats;
  
  if (stat(filepath, &filestats) != 0) {
    return 0;
  }

  return S_ISLNK(filestats.st_mode);
}


static inline void append_buffer(Buffer *buff, const char *fmt, ...) {
    char str[MAX_BUFFER];
    va_list args;
    va_start(args, fmt);
    long unsigned int written = vsnprintf(str, sizeof(str), fmt, args);
    va_end(args);

    if (written <= 0) {
      fprintf(stderr, "Error formatting output\n");
      fflush(stderr);
      return;
    }
    if ((size_t)written >= sizeof(str)) {
      fprintf(stderr, "Warning: output truncated\n");
      fflush(stderr);
      return;    
    }
    CHECK_BUFF(add_buffer(buff, str));
}


#endif //__FF_H__
