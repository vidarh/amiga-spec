#ifndef __console_utils_H
#define __console_utils_H

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>

struct console_utils {
  struct Window * win;
  struct IOStdReq *writeReq;    /* IORequest block pointer */
  struct MsgPort  *writePort;   /* replyport for writes      */
  struct IOStdReq *readReq;     /* IORequest block pointer */
  struct MsgPort  *readPort;    /* replyport for reads       */
  BOOL OpenedConsole;
  BOOL StartedRead;             /* True if *any* reads have been started */
  BYTE error;  
};

void teardown(struct console_utils * u);
void setup(struct console_utils * u);

void ConPuts(struct console_utils * u,UBYTE *string);
#endif
