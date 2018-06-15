
#include <stdio.h>

#include "..\..\..\gimp.h"

static void query (void);
static void run   (const gchar *name,
                   gint nparams,
                   const GimpParam *param,
                   gint *nreturn_vals,
                   GimpParam **return_vals);

GimpPlugInInfo PLUG_IN_INFO = 
{
        NULL,
        NULL,
        query,
        run
};

MAIN()

void main()
{
    printf("ok\n");
}

static void 
query (void)
{

}

static void 
run (const gchar *name,
     gint nparams,
     const GimpParam *param,
     gint *nreturn_vals,
     GimpParam **return_vals)
{

}