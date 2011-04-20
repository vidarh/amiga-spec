
#include "console_utils.h"

#include <devices/console.h>

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <clib/intuition_protos.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct NewWindow nw =
    {
    10, 10,                           /* starting position (left,top) */
    620,180,                          /* width, height */
    -1,-1,                            /* detailpen, blockpen */
    IDCMP_CLOSEWINDOW,                /* flags for idcmp */
    WFLG_DEPTHGADGET|WFLG_SIZEGADGET|
    WFLG_DRAGBAR|WFLG_CLOSEGADGET|
    WFLG_SMART_REFRESH|WFLG_ACTIVATE, /* window flags */
    NULL,                             /* no user gadgets */
    NULL,                             /* no user checkmark */
    "Console Test",                   /* title */
    NULL,                             /* pointer to window screen */
    NULL,                             /* pointer to super bitmap */
    100,45,                           /* min width, height */
    640,200,                          /* max width, height */
    WBENCHSCREEN                      /* open on workbench screen */
    };



/* Attach console device to an open Intuition window.
 * This function returns a value of 0 if the console
 * device opened correctly and a nonzero value (the error
 * returned from OpenDevice) if there was an error.
 */
static BYTE OpenConsole(struct IOStdReq * writereq, 
		 struct IOStdReq * readreq, 
		 struct Window * window)
{
    BYTE error;

    writereq->io_Data = (APTR) window;
    writereq->io_Length = sizeof(struct Window);
    error = OpenDevice("console.device", 0, writereq, 0);
    readreq->io_Device = writereq->io_Device; /* clone required parts */
    readreq->io_Unit   = writereq->io_Unit;
    return(error);
}

static void CloseConsole(struct IOStdReq *writereq)
{
    CloseDevice(writereq);
}

/* Output a stream of known length to a console
 */
static void ConWrite(struct IOStdReq *writereq, UBYTE *string, LONG length)
{
    writereq->io_Command = CMD_WRITE;
    writereq->io_Data = (APTR)string;
    writereq->io_Length = length;
    DoIO(writereq);
    /* command works because DoIO blocks until command is done
     * (otherwise ptr to string could become invalid in the meantime)
     */
}


/* Output a NULL-terminated string of characters to a console
 */
void ConPuts(struct console_utils * u,UBYTE *string)
{
    ConWrite(u->writeReq,string, -1);
}

/* Queue up a read request to console, passing it pointer
 * to a buffer into which it can read the character
 */
static void QueueRead(struct IOStdReq *readreq, UBYTE *whereto)
{
   readreq->io_Command = CMD_READ;
   readreq->io_Data = (APTR)whereto;
   readreq->io_Length = 1;
   SendIO(readreq);
}


/* Check if a character has been received.
 * If none, return -1
 */
static LONG ConMayGetChar(struct MsgPort *msgport, UBYTE *whereto)
{
    register int temp;
    struct IOStdReq *readreq;

    if (!(readreq = (struct IOStdReq *)GetMsg(msgport))) return(-1);
    temp = *whereto;                /* get the character */
    QueueRead(readreq,whereto);     /* then re-use the request block */
    return(temp);
}

/* Wait for a character
 */
static UBYTE ConGetChar(struct MsgPort *msgport, UBYTE *whereto)
{
    register int temp;
    struct IOStdReq *readreq;

    WaitPort(msgport);
    readreq = (struct IOStdReq *)GetMsg(msgport);
    temp = *whereto;               /* get the character */
    QueueRead(readreq,whereto);    /* then re-use the request block*/
    return((UBYTE)temp);
}

static void cleanup(struct console_utils * u)
{
    if(u->OpenedConsole) CloseConsole(u->writeReq);
    if(u->readReq)       DeleteExtIO(u->readReq);
    if(u->readPort)      DeletePort(u->readPort);
    if(u->writeReq)      DeleteExtIO(u->writeReq);
    if(u->writePort)     DeletePort(u->writePort);
    if(u->win)           CloseWindow(u->win);
}

static void cleanexit(struct console_utils * u,UBYTE *s,LONG n)
{
    cleanup(u);
    exit(n);
}


void teardown(struct console_utils * u)
{
    /* We always have an outstanding queued read request
     * so we must abort it if it hasn't completed,
     * and we must remove it.
     */
    fprintf(stderr,"Teardown\n");
	/* FIXME: On AROS, an AbortIO results in a crash if called on a readReq that has not
	   yet been used for a request. Is that also the case for AmigaOS? */
    if(u->readReq && u->readReq->io_Data) {
	  if (!(CheckIO(u->readReq)))  {
		fprintf(stderr,"Aborting waiting IO\n");
		AbortIO(u->readReq);
	  }
	  fprintf(stderr,"WaitIO\n");
	  WaitIO(u->readReq);     /* clear it from our replyport */
	}
	fprintf(stderr,"Cleanup\n");
    cleanup(u);
}

void setup(struct console_utils * u)
{
  memset(u,sizeof(struct console_utils),1);

    /* Create reply port and io block for writing to console */
    if(!(u->writePort = CreatePort("console_utils.console.write",0)))
      cleanexit(u,"Can't create write port\n",RETURN_FAIL);

    if(!(u->writeReq = (struct IOStdReq *)
                    CreateExtIO(u->writePort,(LONG)sizeof(struct IOStdReq))))
      cleanexit(u,"Can't create write request\n",RETURN_FAIL);

    /* Create reply port and io block for reading from console */
    if(!(u->readPort = CreatePort("console_utils.console.read",0)))
      cleanexit(u,"Can't create read port\n",RETURN_FAIL);

    if(!(u->readReq = (struct IOStdReq *)
                   CreateExtIO(u->readPort,(LONG)sizeof(struct IOStdReq))))
      cleanexit(u,"Can't create read request\n",RETURN_FAIL);

    /* Open a window */
    if(!(u->win = OpenWindow(&nw)))
      cleanexit(u,"Can't open window\n",RETURN_FAIL);

    /* Now, attach a console to the window */
    if(u->error = OpenConsole(u->writeReq,u->readReq,u->win))
      cleanexit(u,"Can't open console.device\n",RETURN_FAIL);
    else u->OpenedConsole = TRUE;
}

