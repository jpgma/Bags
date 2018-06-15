
typedef signed long long gint64;
typedef float gfloat;
typedef double gdouble;
typedef unsigned int gsize;
typedef signed int gssize;
typedef gint64 goffset;
typedef unsigned int guint32;
typedef signed int gint32;
typedef unsigned short guint16;
typedef signed short gint16;
typedef unsigned char guint8;
typedef signed char gint8;
typedef int gint;
typedef char gchar;
typedef long glong;
typedef unsigned long gulong;
typedef short gshort;
typedef unsigned int guint;
typedef unsigned char guchar;
typedef unsigned short gushort;
typedef void* gpointer;
typedef const void *gconstpointer;
typedef gint gboolean;

typedef enum
{
  GIMP_PDB_INT32,
  GIMP_PDB_INT16,
  GIMP_PDB_INT8,
  GIMP_PDB_FLOAT,
  GIMP_PDB_STRING,
  GIMP_PDB_INT32ARRAY,
  GIMP_PDB_INT16ARRAY,
  GIMP_PDB_INT8ARRAY,
  GIMP_PDB_FLOATARRAY,
  GIMP_PDB_STRINGARRAY,
  GIMP_PDB_COLOR,
  GIMP_PDB_ITEM,
  GIMP_PDB_DISPLAY,
  GIMP_PDB_IMAGE,
  GIMP_PDB_LAYER,
  GIMP_PDB_CHANNEL,
  GIMP_PDB_DRAWABLE,
  GIMP_PDB_SELECTION,
  GIMP_PDB_COLORARRAY,
  GIMP_PDB_VECTORS,
  GIMP_PDB_PARASITE,
  GIMP_PDB_STATUS,
  GIMP_PDB_END
} GimpPDBArgType;

typedef enum
{
  GIMP_PDB_EXECUTION_ERROR,
  GIMP_PDB_CALLING_ERROR,
  GIMP_PDB_PASS_THROUGH,
  GIMP_PDB_SUCCESS,
  GIMP_PDB_CANCEL
} GimpPDBStatusType;

struct GimpRGB
{
  gdouble r, g, b, a;
};

struct GimpParamDef
{
  GimpPDBArgType  type;
  gchar          *name;
  gchar          *description;
};

struct GimpParamRegion
{
  gint32 x;
  gint32 y;
  gint32 width;
  gint32 height;
};

struct GimpParasite
{
  gchar    *name;   /* The name of the parasite. USE A UNIQUE PREFIX! */
  guint32   flags;  /* save Parasite in XCF file, etc.                */
  guint32   size;   /* amount of data                                 */
  gpointer  data;   /* a pointer to the data.  plugin is              *
                     * responsible for tracking byte order            */
};

union GimpParamData
{
  gint32            d_int32;
  gint16            d_int16;
  guint8            d_int8;
  gdouble           d_float;
  gchar            *d_string;
  gint32           *d_int32array;
  gint16           *d_int16array;
  guint8           *d_int8array;
  gdouble          *d_floatarray;
  gchar           **d_stringarray;
  GimpRGB          *d_colorarray;
  GimpRGB           d_color;
  gint32            d_display;
  gint32            d_image;
  gint32            d_item;
  gint32            d_layer;
  gint32            d_layer_mask;
  gint32            d_channel;
  gint32            d_drawable;
  gint32            d_selection;
  gint32            d_boundary;
  gint32            d_vectors;
  gint32            d_unit;
  GimpParasite      d_parasite;
  gint32            d_tattoo;
  GimpPDBStatusType d_status;
};

struct GimpParam
{
  GimpPDBArgType type;
  GimpParamData  data;
};

typedef void (* GimpInitProc)  (void);
typedef void (* GimpQuitProc)  (void);
typedef void (* GimpQueryProc) (void);
typedef void (* GimpRunProc)   (const gchar      *name,
                                gint              n_params,
                                const GimpParam  *param,
                                gint             *n_return_vals,
                                GimpParam       **return_vals);

struct GimpPlugInInfo
{
  GimpInitProc  init_proc;
  GimpQuitProc  quit_proc;
  GimpQueryProc query_proc;
  GimpRunProc   run_proc;
};

// #  define MAIN()                                        \
//    struct HINSTANCE__;                                  \
//                                                         \
//    int _stdcall                                         \
//    WinMain (struct HINSTANCE__ *hInstance,              \
//             struct HINSTANCE__ *hPrevInstance,          \
//             char *lpszCmdLine,                          \
//             int   nCmdShow);                            \
//                                                         \
//    int _stdcall                                         \
//    WinMain (struct HINSTANCE__ *hInstance,              \
//             struct HINSTANCE__ *hPrevInstance,          \
//             char *lpszCmdLine,                          \
//             int   nCmdShow)                             \
//    {                                                    \
//      return gimp_main (&PLUG_IN_INFO, __argc, __argv);  \
//    }                                                    \
//                                                         \
//    int                                                  \
//    main (int argc, char *argv[])                        \
//    {                                                    \
//      /* Use __argc and __argv here, too, as they work   \
//       * better with mingw-w64.                          \
//       */                                                \
//      return gimp_main (&PLUG_IN_INFO, __argc, __argv);  \
//    }
// #else
#  define MAIN()                                        \
   int                                                  \
   main (int argc, char *argv[])                        \
   {                                                    \
     return gimp_main (&PLUG_IN_INFO, argc, argv);      \
   }
// #endif

/* The main procedure that must be called with the PLUG_IN_INFO structure
 * and the 'argc' and 'argv' that are passed to "main".
 */
gint           gimp_main                (const GimpPlugInInfo *info,
                                         gint                  argc,
                                         gchar                *argv[]);

/* Forcefully causes the gimp library to exit and
 *  close down its connection to main gimp application.
 */
void           gimp_quit                (void) G_GNUC_NORETURN;


/* Install a procedure in the procedure database.
 */
void           gimp_install_procedure   (const gchar        *name,
                                         const gchar        *blurb,
                                         const gchar        *help,
                                         const gchar        *author,
                                         const gchar        *copyright,
                                         const gchar        *date,
                                         const gchar        *menu_label,
                                         const gchar        *image_types,
                                         GimpPDBProcType     type,
                                         gint                n_params,
                                         gint                n_return_vals,
                                         const GimpParamDef *params,
                                         const GimpParamDef *return_vals);

/* Install a temporary procedure in the procedure database.
 */
void           gimp_install_temp_proc   (const gchar        *name,
                                         const gchar        *blurb,
                                         const gchar        *help,
                                         const gchar        *author,
                                         const gchar        *copyright,
                                         const gchar        *date,
                                         const gchar        *menu_label,
                                         const gchar        *image_types,
                                         GimpPDBProcType     type,
                                         gint                n_params,
                                         gint                n_return_vals,
                                         const GimpParamDef *params,
                                         const GimpParamDef *return_vals,
                                         GimpRunProc         run_proc);

/* Uninstall a temporary procedure
 */
void           gimp_uninstall_temp_proc (const gchar        *name);

/* Notify the main GIMP application that the extension is ready to run
 */
void           gimp_extension_ack       (void);

/* Enable asynchronous processing of temp_procs
 */
void           gimp_extension_enable    (void);

/* Process one temp_proc and return
 */
void           gimp_extension_process   (guint            timeout);

/* Run a procedure in the procedure database. The parameters are
 *  specified via the variable length argument list. The return
 *  values are returned in the 'GimpParam*' array.
 */
GimpParam    * gimp_run_procedure       (const gchar     *name,
                                         gint            *n_return_vals,
                                         ...);

/* Run a procedure in the procedure database. The parameters are
 *  specified as an array of GimpParam.  The return
 *  values are returned in the 'GimpParam*' array.
 */
GimpParam    * gimp_run_procedure2      (const gchar     *name,
                                         gint            *n_return_vals,
                                         gint             n_params,
                                         const GimpParam *params);

/* Destroy the an array of parameters. This is useful for
 *  destroying the return values returned by a call to
 *  'gimp_run_procedure'.
 */
void           gimp_destroy_params      (GimpParam       *params,
                                         gint             n_params);

/* Destroy the an array of GimpParamDef's. This is useful for
 *  destroying the return values returned by a call to
 *  'gimp_procedural_db_proc_info'.
 */
void           gimp_destroy_paramdefs   (GimpParamDef    *paramdefs,
                                         gint             n_params);

/* Retrieve the error message for the last procedure call.
 */
const gchar  * gimp_get_pdb_error       (void);

/* Retrieve the return status for the last procedure call.
 */
GimpPDBStatusType gimp_get_pdb_status   (void);

/* Return various constants given by the GIMP core at plug-in config time.
 */
guint          gimp_tile_width          (void) G_GNUC_CONST;
guint          gimp_tile_height         (void) G_GNUC_CONST;
gint           gimp_shm_ID              (void) G_GNUC_CONST;
guchar       * gimp_shm_addr            (void) G_GNUC_CONST;
gdouble        gimp_gamma               (void) G_GNUC_CONST;
gboolean       gimp_show_tool_tips      (void) G_GNUC_CONST;
gboolean       gimp_show_help_button    (void) G_GNUC_CONST;
gboolean       gimp_export_exif         (void) G_GNUC_CONST;
gboolean       gimp_export_xmp          (void) G_GNUC_CONST;
gboolean       gimp_export_iptc         (void) G_GNUC_CONST;
GimpCheckSize  gimp_check_size          (void) G_GNUC_CONST;
GimpCheckType  gimp_check_type          (void) G_GNUC_CONST;
gint32         gimp_default_display     (void) G_GNUC_CONST;
const gchar  * gimp_wm_class            (void) G_GNUC_CONST;
const gchar  * gimp_display_name        (void) G_GNUC_CONST;
gint           gimp_monitor_number      (void) G_GNUC_CONST;
guint32        gimp_user_time           (void) G_GNUC_CONST;

const gchar  * gimp_get_progname        (void) G_GNUC_CONST;
