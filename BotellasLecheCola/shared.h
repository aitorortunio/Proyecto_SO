
struct message {
  long type;
};
typedef struct message tMessage;

#define FINISH "exit"
#define SIZE_MSG sizeof(tMessage) - sizeof(long)
#define FILE_PATH "./shared.h"
#define TYPE_ALGUIEN_COMPRANDO 1L
#define TYPE_HELADERA_LLENA 2L


void report_and_exit(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
