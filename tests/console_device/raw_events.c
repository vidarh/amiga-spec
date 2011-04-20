
#include <exec/types.h>
#include <exec/io.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <libraries/dos.h>
#include <devices/console.h>

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>

#include "console_utils.h"

#include <stdio.h>
#include <stdlib.h>

void cleanexit(struct console_utils * u,UBYTE *s,LONG n);

void cleanup(struct console_utils * u);


int main(int argc, const char **argv)
{
  struct console_utils u;

  setup(&u);
  ConPuts(&u,"Hello World\n");
  Delay(50);
  teardown(&u);
  exit(RETURN_OK);
}


