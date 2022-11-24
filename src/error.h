#ifndef IFJ_ERROR
#define IFJ_ERROR

void exit_program(int err_code, char* msg);

void warning_msg(const char* fmt, ...);

void error_exit(const char* fmt, ...);

#endif
