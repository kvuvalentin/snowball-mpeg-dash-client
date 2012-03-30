/**  TBD: add STM Header
 */

#ifndef __GST_MPDCOMMON_H__
#define __GST_MPDCOMMON_H__

#include <stdio.h>
#include <mpdmodule/glibchelper/glibchelper.h>
#include <mpdmodule/glibchelper/glist.h>

#ifdef __cplusplus
extern "C" {
#endif

//G_BEGIN_DECLS

typedef struct _GstRange GstRange;
typedef struct _GstMPDNode GstMPDNode;
typedef struct _GstPeriodNode GstPeriodNode;
typedef struct _GstSegmentInfoDefaultNode GstSegmentInfoDefaultNode;
typedef struct _GstProgramInformationNode GstProgramInformationNode;
typedef struct _GstAdaptationSetNode GstAdaptationSetNode;
typedef struct _GstRepresentationNode GstRepresentationNode;
typedef struct _GstSegmentInfoNode GstSegmentInfoNode;
typedef struct _GstSegmentBaseNode GstSegmentBaseNode;
typedef struct _GstSegmentListNode GstSegmentListNode;
typedef struct _GstInitialisationNode GstInitialisationNode;
typedef struct _GstSegmentURLNode GstSegmentURLNode;
typedef struct _GstUrlTemplate GstUrlTemplate;
typedef struct _GstUrlNode GstUrlNode;

typedef enum
{
  GST_MPD_FILE_TYPE_ONDEMAND,
  GST_MPD_FILE_TYPE_LIVE,
  GST_MPD_FILE_TYPE_UNKNOWN
} GstMPDFileType;

struct _GstRange
{
  guint64 first_byte_pos;
  guint64 last_byte_pos;
};

struct _GstUrlNode
{
  gchar *sourceURL;
  gint64 start;			/* in milliseconds */
  GstRange *range;
};

struct _GstUrlTemplate
{
  gchar *sourceURL;
  gchar *id;
  gint startIndex;
  gint endIndex;
};

struct _GstSegmentInfoNode
{
  gint64 duration;		/* in milliseconds */
  gchar *baseURL;
  GstUrlTemplate *UrlTemplate;
  /* list of InitialisationSegmentURL nodes */
  GList *InitialisationSegmentURL;
  /* list of Url nodes */
  GList *Urls;
};

struct _GstSegmentBaseNode
{
  guint timescale;
  /* list of Initialisation nodes */
  GList *Initialisation;
};

struct _GstSegmentListNode
{
  gint64 duration;		/* in milliseconds */
  /* list of SegmentURL nodes */
  GList *SegmentURL;
};

struct _GstInitialisationNode
{
  gchar *sourceURL;
  GstRange *range;
};

struct _GstSegmentURLNode
{
  gchar *media;
  GstRange *mediaRange;
};

struct _GstRepresentationNode
{
  gint bandwidth;
  gchar *BaseURL;
  gchar *mimeType;
  gchar *codecs;
  gint qualityRanking;
  gboolean startWithRAP;
  guint startWithSAP;
  gchar *lang;			/* FIXME: LangVectorType RFC 5646 */
  gchar *id;
  gchar *dependencyId;
  guint width;
  guint height;
  guint minBufferTime;
  /* list of SegmentInfo nodes */
  GList *SegmentInfos;
  /* list of SegmentBase nodes */
  GList *SegmentBase;
  /* list of SegmentList nodes */
  GList *SegmentList;
};

struct _GstAdaptationSetNode
{
  gboolean bitstreamSwitching;
  gboolean segmentAlignmentFlag;
  guint width;
  guint height;
  gdouble frameRate;
  gchar *lang;			/* FIXME: LangVectorType RFC 5646 */
  gchar *mimeType;
  gint minBandwidth;
  /* list of Representation nodes */
  GList *Representations;
};

struct _GstSegmentInfoDefaultNode
{
  gchar *sourceUrlTemplatePeriod;
  gchar *baseURL;
};

struct _GstPeriodNode
{
  gboolean segmentAlignmentFlag;
  gint64 start;			/* in milliseconds */
  gint64 duration;		/* in milliseconds */
  GstSegmentInfoDefaultNode *SegmentInfoDefault;
  /* list of Adaptation Set nodes */
  GList *AdaptationSets;
  /* list of Representation nodes */
  GList *Representations;
};

struct _GstProgramInformationNode
{
  gchar *moreInformationURL;
  /* children nodes */
  gchar *Title;
  gchar *Source;
  gchar *Copyright;
};

struct _GstMPDNode
{
  gchar *default_namespace;
  gchar *namespace_xsi;
  gchar *namespace_ext;
  gchar *schemaLocation;
  gchar *profiles;
  GstMPDFileType type;
  gint64 mediaPresentationDuration;	/* in milliseconds */
  gint64 minBufferTime;		/* in milliseconds */
  gchar *baseURL;
  /* ProgramInformation node */
  GstProgramInformationNode *ProgramInfo;
  /* list of Periods nodes */
  GList *Periods;
};

/* make a copy of the MPD data structure (original data structure pointers will be set to NULL) */
GstMPDNode *gst_mpdcommon_copy_mpd_node (GstMPDNode * mpd_node);

/* deep copy functions */
GstRepresentationNode *gst_mpdcommon_deep_copy_representation_node (const
								    GstRepresentationNode
								    * rep);
GstSegmentBaseNode *gst_mpdcommon_deep_copy_segment_base_node (const
							       GstSegmentBaseNode
							       *
							       segment_base);
GstInitialisationNode *gst_mpdcommon_deep_copy_initialisation_node (const
								    GstInitialisationNode
								    *
								    initialisation);
GstSegmentListNode *gst_mpdcommon_deep_copy_segment_list_node (const
							       GstSegmentListNode
							       *
							       segment_list);
GstSegmentURLNode *gst_mpdcommon_deep_copy_segment_url_node (const
							     GstSegmentURLNode
							     *
							     segment_url_node);
GstSegmentInfoNode *gst_mpdcommon_deep_copy_segment_info_node (const
							       GstSegmentInfoNode
							       * segmentInfo);
GstUrlTemplate *gst_mpdcommon_deep_copy_url_template (const GstUrlTemplate *
						      url_template);
GstUrlNode *gst_mpdcommon_deep_copy_url_node (const GstUrlNode * url_node);

/* comparison functions */
gint gst_mpdcommon_compare_representation_nodes (const GstRepresentationNode *
						 rep1,
						 const GstRepresentationNode *
						 rep2);
gint gst_mpdcommon_compare_segment_base_nodes (const GstSegmentBaseNode *
					       node1,
					       const GstSegmentBaseNode *
					       node2);
gint gst_mpdcommon_compare_segment_list_nodes (const GstSegmentListNode *
					       node1,
					       const GstSegmentListNode *
					       node2);
gint gst_mpdcommon_compare_initialisation_node (const GstInitialisationNode *
						node1,
						const GstInitialisationNode *
						node2);
gint gst_mpdcommon_compare_segment_url_node (const GstSegmentURLNode * node1,
					     const GstSegmentURLNode * node2);
gint gst_mpdcommon_compare_segment_info_nodes (const GstSegmentInfoNode *
					       node1,
					       const GstSegmentInfoNode *
					       node2);
gint gst_mpdcommon_compare_url_template (const GstUrlTemplate * urlTemplate1,
					 const GstUrlTemplate * urlTemplate2);
gint gst_mpdcommon_compare_url_node (const GstUrlNode * urlNode1,
				     const GstUrlNode * urlNode2);

/* navigation functions */
GstPeriodNode *gst_mpdcommon_get_next_period (GList * Periods,
					      GstPeriodNode * prev_period);
GstAdaptationSetNode *gst_mpdcommon_get_highest_adaptation_set (GList *
								AdaptationSets);
GstRepresentationNode *gst_mpdcommon_get_highest_representation (GList *
								 Representations);
GstRepresentationNode
  *gst_mpdcommon_get_representation_with_max_bandwidth (GList *
							Representations,
							gint max_bandwidth);
/* old syntax */
GstSegmentInfoNode *gst_mpdcommon_get_first_segment_info (GList *
							  SegmentInfos);
GstUrlNode *gst_mpdcommon_get_first_initialisation_segment_url (GList *
								InitialisationSegmentURL);
/* end old syntax */
GstSegmentBaseNode *gst_mpdcommon_get_first_segment_base (GList *
							  SegmentBase);
GstInitialisationNode *gst_mpdcommon_get_first_initialisation (GList *
							       Initialisation);
GstSegmentListNode *gst_mpdcommon_get_first_segment_list (GList *
							  SegmentList);
GList *gst_mpdcommon_get_first_segment_url (GList * Urls);

/* memory management functions */
void gst_mpdcommon_free_mpd_node (GstMPDNode * mpd_node);
void gst_mpdcommon_free_prog_info_node (GstProgramInformationNode *
					prog_info_node);
void gst_mpdcommon_free_period_node (GstPeriodNode * period_node);
void gst_mpdcommon_free_segment_info_default_node (GstSegmentInfoDefaultNode *
						   segment_info_default_node);
void gst_mpdcommon_free_adaptation_set_node (GstAdaptationSetNode *
					     adaptation_set_node);
void gst_mpdcommon_free_representation_node (GstRepresentationNode *
					     representation_node);
void gst_mpdcommon_free_segment_info_node (GstSegmentInfoNode *
					   segment_info_node);
void gst_mpdcommon_free_segment_base_node (GstSegmentBaseNode *
					   segment_base_node);
void gst_mpdcommon_free_segment_list_node (GstSegmentListNode *
					   segment_list_node);
void gst_mpdcommon_free_initialisation_node (GstInitialisationNode *
					     initialisation_node);
void gst_mpdcommon_free_segment_url_node (GstSegmentURLNode * segment_url);
void gst_mpdcommon_free_url_template_node (GstUrlTemplate * url_template);
void gst_mpdcommon_free_url_node (GstUrlNode * url_node);

/* debug output functions */
const char *representations_to_string (GList * representations);
void dump_mpdnode (GstMPDNode * new_mpd);


/*MC: API to get informations about the MPD tree*/
int get_mpd_periods_number (GstMPDNode * new_mpd);
int get_mpd_representations_number (GstMPDNode * new_mpd, int period_idx);
int get_mpd_representation_bandwidth (GstMPDNode * new_mpd, int period_idx, int rep_idx);
int get_mpd_representation_width (GstMPDNode * new_mpd, int period_idx, int rep_idx);
int get_mpd_representation_height (GstMPDNode * new_mpd, int period_idx, int rep_idx);
int get_mpd_representation_minBufferTime (GstMPDNode * new_mpd, int period_idx, int rep_idx);
int get_mpd_segments_number (GstMPDNode * new_mpd, int period_idx, int rep_idx);  //supposing segment info index = 0

void reorder_mpd_representations (GstMPDNode * new_mpd, int period_idx);

char * get_mpd_segments_URL (GstMPDNode * new_mpd, int period_idx, int rep_idx, int segm_idx);
char * get_mpd_representation_mimetype (GstMPDNode * new_mpd, int period_idx, int rep_idx);


//G_END_DECLS

#ifdef __cplusplus
}
#endif

#endif /* __GST_MPDCOMMON_H__ */
