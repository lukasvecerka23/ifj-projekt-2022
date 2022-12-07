/**
 * Project - IFJ Projekt 2022
 *
 * @author Lukas Vecerka xvecer30
 * @author Jachym Dolezal xdolez0c
 * @author Andrej Nespor xnespo10
 * @author Matej Tomko xtomko06
 *
 * @brief Header file for error.c
 */

#ifndef IFJ_ERROR
#define IFJ_ERROR

#include <stdio.h>
#include <stdlib.h>

/**
 * Exit program with given error code
 *
 * @param err_code error code
 * @param msg describing message
 */
void exit_program(int err_code, char* msg);

#endif
/*END OF FILE*/
