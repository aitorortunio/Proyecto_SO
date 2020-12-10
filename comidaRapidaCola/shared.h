struct message {
  long type;
  int tipoMenu;
};
typedef struct message tMessage;

#define FINISH "exit"
#define SIZE_MSG sizeof(tMessage) - sizeof(long)
#define FILE_PATH "./shared.h"
#define TYPE_CANT_CARNE_VACIA 1L
#define TYPE_CANT_VEG_VACIA 2L
#define TYPE_PEDIDO_MENU 3L
#define TYPE_MESA_LIBRE 4L
#define TYPE_MESA_SUCIA 5L
#define TYPE_ENTREGADO_CARNE 6L
#define TYPE_ENTREGADO_VEG 7L
#define TYPE_CANT_CARNE_LLENO 8L
#define TYPE_CANT_VEG_LLENO 9L

#define MENU_DE_CARNE 1
#define MENU_VEGETARIANO 2


void report_and_exit(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}
