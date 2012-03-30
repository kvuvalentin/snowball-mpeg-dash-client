#ifndef __GST_MPDPARSE_H__
#define __GST_MPDPARSE_H__

#include <mpdmodule/mpdcommon/gstmpdcommon.h>

#ifdef __cplusplus
extern "C" {
#endif

/* define to set the max length of the trail after the </MPD> tag */
#define MPD_PARSE_MAX_TRAIL_LENGTH 100
/* define to set the max number of bytes to check for the <MPD tag */
#define MPD_PARSE_MAX_START_LENGTH 100

/* #defines don't like whitespacey bits */
#define GST_TYPE_MPDPARSE \
  (gst_mpdparse_get_type())
#define GST_MPDPARSE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MPDPARSE,Gstmpdparse))
#define GST_MPDPARSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MPDPARSE,GstmpdparseClass))
#define GST_IS_MPDPARSE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MPDPARSE))
#define GST_IS_MPDPARSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MPDPARSE))

typedef struct _Gstmpdparse Gstmpdparse;
typedef struct _GstmpdparseClass GstmpdparseClass;

typedef enum
{
  GST_MPD_PARSE_STATUS_PASSTHROUGH,
  GST_MPD_PARSE_STATUS_PARSING
} GstMPDParseStatus;

typedef enum
{
  GST_MPD_FILE_STATUS_PARSING,
  GST_MPD_FILE_STATUS_COMPLETED,
  GST_MPD_FILE_STATUS_NOT_VALID
} GstMPDFileStatus;

struct _Gstmpdparse
{
  GstMPDParseStatus mpdparse_status;	/* MPD parser status */
  gchar *input_mpd_filename;	/* input MPD local file name (NULL if not available) */
  gchar *output_mpd_filename;	/* output file name where the current MPD will be saved */

  guint64 total_mpd_size;	/* Accumulated size of the MPD files downloaded */

  GstMPDNode *mpd_node;		/* the MPD data structure */
};


void gst_mpdparse_parse_local_mpd_file (Gstmpdparse * mpdparse);
GstMPDFileStatus gst_mpdparse_parse_mpd_file (Gstmpdparse * mpdparse, const char * buffer, int size);

//GType gst_mpdparse_get_type (void);


#ifdef __cplusplus
}
#endif

#endif /* __GST_MPDPARSE_H__ */
