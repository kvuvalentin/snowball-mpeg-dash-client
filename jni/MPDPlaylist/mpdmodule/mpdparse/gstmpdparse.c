/**  TBD: add STM Header
 */

#include <string.h>

/* including libxml2 library to parse XML MPD files */
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlstring.h>
#include "gstmpdparse.h"
#include "debug.h"


/* Filter signals and args */
enum
{
  SIGNAL_ON_NEW_MPD,
  LAST_SIGNAL
};

enum
{
  PROP_0,
  PROP_INPUT_MPD_FILENAME,
  PROP_OUTPUT_MPD_FILENAME
};

static const xmlChar xml_initialisation_string[] =
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
#define XML_INIT_LEN 38

static const xmlChar mpd_start_string[] = "<MPD";
#define MPD_START_LEN 4

static const xmlChar mpd_end_string[] = "</MPD>";
#define MPD_END_LEN 6

static gboolean
gst_mpdparse_detect_mpd_file (Gstmpdparse * mpdparse, char *buf)
{
  gint maxlen;
  const xmlChar *data, *pointer;
  xmlChar *search_block;

  data = (xmlChar *) NULL;	//TODO GST_BUFFER_DATA (buf);
  maxlen = 0;			//TODO GST_BUFFER_SIZE (buf);

  if (maxlen <= XML_INIT_LEN + MPD_START_LEN + 1)
    {
      /* FIXME: should we save it in the adapter for next iteration? */
      return FALSE;
    }

  if (xmlStrncmp (data, xml_initialisation_string, XML_INIT_LEN) == 0)
    {

      search_block =
	xmlStrndup (data, MIN (maxlen, MPD_PARSE_MAX_START_LENGTH));
      pointer = xmlStrstr (search_block, mpd_start_string);
      xmlFree (search_block);

      /* look for the MPD signature: "<MPD" */
      if (pointer)
	{
	  return TRUE;
	}
    }

  return FALSE;
}

/* functions to parse a node namespaces, content and properties */
static gchar *
gst_mpdparse_get_xml_prop_string (Gstmpdparse * mpdparse, xmlNode * a_node,
				  const gchar * property)
{
  xmlChar *prop_string = NULL;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  
  //MC: show contents if DEBUG is defined
  if (prop_string)
  {
    LOGAST ("Debug", " - %s: %s", property, prop_string);
  }

  return (gchar *) prop_string;
}

static gint
gst_mpdparse_get_xml_prop_integer (Gstmpdparse * mpdparse, xmlNode * a_node,
				   const gchar * property)
{
  xmlChar *prop_string;
  gint prop_integer = -1;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      if (sscanf ((gchar *) prop_string, "%d", &prop_integer))
	{
	  LOGAST ("Debug", " - %s: %d", property, prop_integer);
	}
      else
	{
	  LOGASTW ("Warning",
		"failed to parse integer property %s from xml string %s",
		property, prop_string);
	}
      xmlFree (prop_string);
    }

  return prop_integer;
}

static guint
gst_mpdparse_get_xml_prop_unsigned_integer (Gstmpdparse * mpdparse,
					    xmlNode * a_node,
					    const gchar * property,
					    guint default_val)
{
  xmlChar *prop_string;
  guint prop_unsigned_integer = default_val;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      if (sscanf ((gchar *) prop_string, "%u", &prop_unsigned_integer))
	{
	  LOGAST ("Debug", " - %s: %u", property, prop_unsigned_integer);
	}
      else
	{
	  LOGASTW ("Warning",
		"failed to parse unsigned integer property %s from xml string %s",
		property, prop_string);
	}
      xmlFree (prop_string);
    }

  return prop_unsigned_integer;
}

static gdouble
gst_mpdparse_get_xml_prop_double (Gstmpdparse * mpdparse, xmlNode * a_node,
				  const gchar * property)
{
  xmlChar *prop_string;
  gdouble prop_double = 0;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      if (sscanf ((gchar *) prop_string, "%lf", &prop_double))
	{
	  LOGAST ("Debug", " - %s: %lf", property, prop_double);
	}
      else
	{
	  LOGASTW ("Warning",
		"failed to parse double property %s from xml string %s",
		property, prop_string);
	}
      xmlFree (prop_string);
    }

  return prop_double;
}

static gboolean
gst_mpdparse_get_xml_prop_boolean (Gstmpdparse * mpdparse, xmlNode * a_node,
				   const gchar * property)
{
  xmlChar *prop_string;
  gboolean prop_bool = FALSE;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      if (xmlStrcmp (prop_string, (xmlChar *) "false") == 0)
	{
	  //GST_LOG_OBJECT (mpdparse, " - %s: false", property);
	}
      else if (xmlStrcmp (prop_string, (xmlChar *) "true") == 0)
	{
	  //GST_LOG_OBJECT (mpdparse, " - %s: true", property);
	  prop_bool = TRUE;
	}
      else
	{
	  LOGASTW ("Warning",
		"failed to parse boolean property %s from xml string %s",
		property, prop_string);
	}
      xmlFree (prop_string);
    }

  return prop_bool;
}

static GstMPDFileType
gst_mpdparse_get_xml_prop_type (Gstmpdparse * mpdparse, xmlNode * a_node,
				const gchar * property)
{
  xmlChar *prop_string;
  GstMPDFileType prop_type = GST_MPD_FILE_TYPE_ONDEMAND;	/* default */

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      if (xmlStrcmp (prop_string, (xmlChar *) "OnDemand") == 0
	  || xmlStrcmp (prop_string, (xmlChar *) "static") == 0)
	{
	  //GST_LOG_OBJECT (mpdparse, " - %s: OnDemand", property);
	  prop_type = GST_MPD_FILE_TYPE_ONDEMAND;
	}
      else if (xmlStrcmp (prop_string, (xmlChar *) "Live") == 0
	       || xmlStrcmp (prop_string, (xmlChar *) "dynamic") == 0)
	{
	  //GST_LOG_OBJECT (mpdparse, " - %s: Live", property);
	  prop_type = GST_MPD_FILE_TYPE_LIVE;
	}
      else
	{
	  LOGASTW ("Warning",
		"failed to parse MPD type property %s from xml string %s",
		property, prop_string);
	}
      xmlFree (prop_string);
    }

  return prop_type;
}

static guint
gst_mpdparse_get_xml_prop_SAP_type (Gstmpdparse * mpdparse, xmlNode * a_node,
				    const gchar * property)
{
  xmlChar *prop_string;
  guint prop_SAP_type = 0;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      if (sscanf ((gchar *) prop_string, "%u", &prop_SAP_type)
	  && prop_SAP_type <= 6)
	{
	  //GST_LOG_OBJECT (mpdparse, " - %s: %u", property, prop_SAP_type);
	}
      else
	{
	  LOGASTW ("Warning",
		"failed to parse unsigned integer property %s from xml string %s",
		property, prop_string);
	}
      xmlFree (prop_string);
    }

  return prop_SAP_type;
}

static GstRange *
gst_mpdparse_get_xml_prop_range (Gstmpdparse * mpdparse, xmlNode * a_node,
				 const gchar * property)
{
  xmlChar *prop_string;
  GstRange *prop_range = NULL;
  guint64 first_byte_pos = 0, last_byte_pos = 0;
  guint len, pos;
  gchar *str;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      len = xmlStrlen (prop_string);
      str = (gchar *) prop_string;

      /* read "-" */
      pos = strcspn (str, "-");
      if (pos >= len)
	{
	  goto error;
	}
      /* read first_byte_pos */
      if (pos != 0)
	{
	  if (sscanf (str, "%llu", &first_byte_pos) != 1)
	    {
	      goto error;
	    }
	}
      /* read last_byte_pos */
      if (pos < (len - 1))
	{
	  if (sscanf (str + pos + 1, "%llu", &last_byte_pos) != 1)
	    {
	      goto error;
	    }
	}
      /* malloc return data structure */
      prop_range = g_slice_new0 (GstRange);
      if (prop_range == NULL)
	{
	  LOGASTW ("Warning", "%s", "Allocation of GstRange failed!");
	  goto error;
	}
      prop_range->first_byte_pos = first_byte_pos;
      prop_range->last_byte_pos = last_byte_pos;
      xmlFree (prop_string);
    }

  return prop_range;

error:
  LOGASTW ("Warning", "%s", "failed to parse property %s from xml string %s",
	property, prop_string);
  return NULL;
}


/*
DateTime Data Type

The dateTime data type is used to specify a date and a time.

The dateTime is specified in the following form "YYYY-MM-DDThh:mm:ss" where:

    * YYYY indicates the year
    * MM indicates the month
    * DD indicates the day
    * T indicates the start of the required time section
    * hh indicates the hour
    * mm indicates the minute
    * ss indicates the second

Note: All components are required!
*/

/*
Duration Data Type

The duration data type is used to specify a time interval.

The time interval is specified in the following form "-PnYnMnDTnHnMnS" where:

    * - indicates the negative sign (optional)
    * P indicates the period (required)
    * nY indicates the number of years
    * nM indicates the number of months
    * nD indicates the number of days
    * T indicates the start of a time section (required if you are going to specify hours, minutes, or seconds)
    * nH indicates the number of hours
    * nM indicates the number of minutes
    * nS indicates the number of seconds
*/

/* this function computes decimals * 10 ^ (3 - pos) */
static gint
convert_to_millisecs (gint decimals, gint pos)
{
  gint num = 1, den = 1, i = 3 - pos;

  while (i < 0)
    {
      den *= 10;
      i++;
    }
  while (i > 0)
    {
      num *= 10;
      i--;
    }
  /* if i == 0 we have exactly 3 decimals and nothing to do */
  return decimals * num / den;
}

static gint64
gst_mpdparse_get_xml_prop_duration (Gstmpdparse * mpdparse, xmlNode * a_node,
				    const gchar * property)
{
  xmlChar *prop_string;
  gchar *str;
  gint64 prop_duration = -1;
  gint ret, read, len, pos, posT;
  gint years = 0, mounts = 0, days = 0, hours = 0, minutes = 0, seconds =
    0, decimals = 0;
  gint sign = 1;
  gboolean have_ms = FALSE;

  prop_string = xmlGetProp (a_node, (const xmlChar *) property);
  if (prop_string)
    {
      len = xmlStrlen (prop_string);
      str = (gchar *) prop_string;

      /* read "-" for sign, if present */
      pos = strcspn (str, "-");
      if (pos < len)
	{			/* found "-" */
	  if (pos != 0)
	    {
	      LOGASTW ("Warning", "%s",
		    "sign \"-\" non at the beginning of the string");
	      return -1;
	    }

	  sign = -1;
	  str++;
	  len--;
	}
      /* read "P" for period */
      pos = strcspn (str, "P");
      if (pos != 0)
	{
	  LOGASTW ("Warning", "%s",
		"P not found at the beginning of the string!");
	  return -1;
	}
      str++;
      len--;
      /* read "T" for time (if present) */
      posT = strcspn (str, "T");
      len -= posT;
      if (posT > 0)
	{
	  /* there is some room between P and T, so there must be a period section */
	  /* read years, mounts, days */
	  do
	    {
	      pos = strcspn (str, "YMD");
	      ret = sscanf (str, "%d", &read);
	      if (ret != 1)
		{
		  LOGASTW ("Warning", "%s",
			"can not read integer value from string %s!", str);
		  return -1;
		}
	      switch (str[pos])
		{
		case 'Y':
		  years = read;
		  break;
		case 'M':
		  mounts = read;
		  break;
		case 'D':
		  days = read;
		  break;
		default:
		  LOGASTW ("Warning", "unexpected char %c!", str[pos]);
		  return -1;
		  break;
		}

	      str += (pos + 1);
	      posT -= (pos + 1);
	    }
	  while (posT > 0);

	}
      /* read "T" for time (if present) */
      /* here T is at pos == 0 */
      str++;
      len--;
      pos = 0;
      if (pos < len)
	{
	  /* T found, there is a time section */
	  /* read hours, minutes, seconds, cents of second */
	  do
	    {
	      pos = strcspn (str, "HMS,.");
	      ret = sscanf (str, "%d", &read);
	      if (ret != 1)
		{
		  LOGASTW ("Warning",
			"can not read integer value from string %s!", str);
		  return -1;
		}
	      switch (str[pos])
		{
		case 'H':
		  hours = read;
		  break;
		case 'M':
		  minutes = read;
		  break;
		case 'S':
		  if (have_ms)
		    {
		      /* we have read the decimal part of the seconds */
		      decimals = convert_to_millisecs (read, pos);
		    }
		  else
		    {
		      /* no decimals */
		      seconds = read;
		    }
		  break;
		case '.':
		case ',':
		  /* we have read the integer part of a decimal number in seconds */
		  seconds = read;
		  have_ms = TRUE;
		  break;
		default:
		  LOGASTW ("Warning", "unexpected char %c!", str[pos]);
		  return -1;
		  break;
		}

	      str += pos + 1;
	      len -= (pos + 1);
	    }
	  while (len > 0);

	}

      xmlFree (prop_string);
      prop_duration =
	sign *
	((((((gint64) years * 365 + mounts * 30 + days) * 24 + hours) * 60 +
	   minutes) * 60 + seconds) * 1000 + decimals);
    }

  return prop_duration;
}

static gchar *
gst_mpdparse_get_xml_node_content (Gstmpdparse * mpdparse, xmlNode * a_node)
{
  xmlChar *content = NULL;

  content = xmlNodeGetContent (a_node);
  if (content)
    {
      LOGAST ("Debug", " - %s: %s", a_node->name, content);
    }

  return (gchar *) content;
}

static gchar *
gst_mpdparse_get_xml_node_namespace (Gstmpdparse * mpdparse, xmlNode * a_node,
				     const gchar * prefix)
{
  xmlNs *curr_ns;
  gchar *namespace = NULL;

  if (prefix == NULL)
    {
      /* return the default namespace */
      namespace = g_strdup ((gchar *) a_node->ns->href);
      if (namespace)
	{
	  LOGAST ("Debug", " - default namespace: %s", namespace);
	}
    }
  else
    {
      /* look for the specified prefix in the namespace list */
      for (curr_ns = a_node->ns; curr_ns; curr_ns = curr_ns->next)
	{
	  if (xmlStrcmp (curr_ns->prefix, (xmlChar *) prefix) == 0)
	    {
	      namespace = g_strdup ((gchar *) curr_ns->href);
	      if (namespace)
		{
		  LOGAST ("Debug", " - %s namespace: %s", curr_ns->prefix,
			curr_ns->href);
		}
	    }
	}
    }

  return namespace;
}

static void
gst_mpdparse_parse_url_node (Gstmpdparse * mpdparse, GList ** list,
			     xmlNode * a_node)
{
  GstUrlNode *new_url;

  new_url = g_slice_new0 (GstUrlNode);
  if (new_url == NULL)
    {
      //GST_WARNING_OBJECT (mpdparse, "Allocation of Url node failed!");
      return;
    }
  *list = g_list_append (*list, new_url);

  //GST_LOG_OBJECT (mpdparse, "attributes of Url node:");
  new_url->sourceURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "sourceURL");
  new_url->start =
    gst_mpdparse_get_xml_prop_duration (mpdparse, a_node, "start");
  new_url->range =
    gst_mpdparse_get_xml_prop_range (mpdparse, a_node, "range");
}

static void
gst_mpdparse_parse_url_template_node (Gstmpdparse * mpdparse,
				      GstUrlTemplate ** pointer,
				      xmlNode * a_node)
{
  GstUrlTemplate *new_url_template;

  gst_mpdcommon_free_url_template_node (*pointer);
  *pointer = new_url_template = g_slice_new0 (GstUrlTemplate);
  if (new_url_template == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of UrlTemplate node failed!");
      return;
    }

  new_url_template->sourceURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "sourceURL");
  new_url_template->id =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "id");
  new_url_template->startIndex =
    gst_mpdparse_get_xml_prop_integer (mpdparse, a_node, "startIndex");
  new_url_template->endIndex =
    gst_mpdparse_get_xml_prop_integer (mpdparse, a_node, "endIndex");
}

static void
gst_mpdparse_parse_segment_info_node (Gstmpdparse * mpdparse, GList ** list,
				      xmlNode * a_node)
{
  xmlNode *cur_node;
  GstSegmentInfoNode *new_segment_info;

  new_segment_info = g_slice_new0 (GstSegmentInfoNode);
  /* list of InitialisationSegmentURL nodes */
  new_segment_info->InitialisationSegmentURL = NULL;
  /* list of Url nodes */
  new_segment_info->Urls = NULL;

  if (new_segment_info == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of SegmentInfo node failed!");
      return;
    }
  *list = g_list_append (*list, new_segment_info);

  new_segment_info->duration =
    gst_mpdparse_get_xml_prop_duration (mpdparse, a_node, "duration");
  new_segment_info->baseURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "baseURL");

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "Url") == 0)
	    {
	      gst_mpdparse_parse_url_node (mpdparse, &new_segment_info->Urls,
					   cur_node);
	    }
	  else
	    if (xmlStrcmp
		(cur_node->name, (xmlChar *) "InitialisationSegmentURL") == 0)
	    {
	      gst_mpdparse_parse_url_node (mpdparse,
					   &new_segment_info->
					   InitialisationSegmentURL,
					   cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "UrlTemplate") == 0)
	    {
	      gst_mpdparse_parse_url_template_node (mpdparse,
						    &new_segment_info->
						    UrlTemplate, cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "BaseURL") == 0)
	    {
	      new_segment_info->baseURL =
		gst_mpdparse_get_xml_node_content (mpdparse, cur_node);
	    }
	}
    }
}

static void
gst_mpdparse_parse_initialisation_node (Gstmpdparse * mpdparse, GList ** list,
					xmlNode * a_node)
{
  GstInitialisationNode *new_intialisation;

  new_intialisation = g_slice_new0 (GstInitialisationNode);
  if (new_intialisation == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of Initialisation node failed!");
      return;
    }
  *list = g_list_append (*list, new_intialisation);

  new_intialisation->sourceURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "sourceURL");
  new_intialisation->range =
    gst_mpdparse_get_xml_prop_range (mpdparse, a_node, "range");
}

static void
gst_mpdparse_parse_segment_url_node (Gstmpdparse * mpdparse, GList ** list,
				     xmlNode * a_node)
{
  GstSegmentURLNode *new_segment_url;

  new_segment_url = g_slice_new0 (GstSegmentURLNode);
  if (new_segment_url == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of SegmentURL node failed!");
      return;
    }
  *list = g_list_append (*list, new_segment_url);

  new_segment_url->media =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "media");
  new_segment_url->mediaRange =
    gst_mpdparse_get_xml_prop_range (mpdparse, a_node, "mediaRange");
}

static void
gst_mpdparse_parse_segment_base_node (Gstmpdparse * mpdparse, GList ** list,
				      xmlNode * a_node)
{
  xmlNode *cur_node;
  GstSegmentBaseNode *new_segment_base;

  new_segment_base = g_slice_new0 (GstSegmentBaseNode);
  if (new_segment_base == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of SegmentBase node failed!");
      return;
    }
  *list = g_list_append (*list, new_segment_base);

  new_segment_base->timescale =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node, "timescale",
						1);

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "Initialisation") == 0)
	    {
	      gst_mpdparse_parse_initialisation_node (mpdparse,
						      &new_segment_base->
						      Initialisation,
						      cur_node);
	    }
	}
    }
}

static void
gst_mpdparse_parse_segment_list_node (Gstmpdparse * mpdparse, GList ** list,
				      xmlNode * a_node)
{
  xmlNode *cur_node;
  GstSegmentListNode *new_segment_list;

  new_segment_list = g_slice_new0 (GstSegmentListNode);
  if (new_segment_list == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of SegmentList node failed!");
      return;
    }
  *list = g_list_append (*list, new_segment_list);

  new_segment_list->duration =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node, "duration",
						0);

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "SegmentURL") == 0)
	    {
	      gst_mpdparse_parse_segment_url_node (mpdparse,
						   &new_segment_list->
						   SegmentURL, cur_node);
	    }
	}
    }
}

static void
gst_mpdparse_parse_representation_node (Gstmpdparse * mpdparse, GList ** list,
					xmlNode * a_node)
{
  xmlNode *cur_node;
  GstRepresentationNode *new_representation;

  new_representation = g_slice_new0 (GstRepresentationNode);
  /* list of SegmentInfo nodes */
  new_representation->SegmentInfos = NULL;
  /* list of SegmentBase nodes */
  new_representation->SegmentBase = NULL;
  /* list of SegmentList nodes */
  new_representation->SegmentList = NULL;
  if (new_representation == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of Representation node failed!");
      return;
    }
  *list = g_list_append (*list, new_representation);

  new_representation->bandwidth =
    gst_mpdparse_get_xml_prop_integer (mpdparse, a_node, "bandwidth");
  new_representation->mimeType =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "mimeType");
  new_representation->codecs =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "codecs");
  new_representation->qualityRanking =
    gst_mpdparse_get_xml_prop_integer (mpdparse, a_node, "qualityRanking");
  new_representation->startWithRAP =
    gst_mpdparse_get_xml_prop_boolean (mpdparse, a_node, "startWithRAP");
  new_representation->startWithSAP =
    gst_mpdparse_get_xml_prop_SAP_type (mpdparse, a_node, "startWithSAP");
  new_representation->lang =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "lang");
  new_representation->id =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "id");
  new_representation->dependencyId =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "dependencyId");
  new_representation->width =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node, "width", 0);
  new_representation->height =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node, "height",
						0);
  new_representation->minBufferTime =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node,
						"minBufferTime", 0);

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "SegmentInfo") == 0)
	    {
	      gst_mpdparse_parse_segment_info_node (mpdparse,
						    &new_representation->
						    SegmentInfos, cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "SegmentBase") == 0)
	    {
	      gst_mpdparse_parse_segment_base_node (mpdparse,
						    &new_representation->
						    SegmentBase, cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "SegmentList") == 0)
	    {
	      gst_mpdparse_parse_segment_list_node (mpdparse,
						    &new_representation->
						    SegmentList, cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "BaseURL") == 0)
	    {
	      new_representation->BaseURL =
		gst_mpdparse_get_xml_node_content (mpdparse, cur_node);
	    }
	}
    }

}

static void
gst_mpdparse_parse_adaptation_set_node (Gstmpdparse * mpdparse, GList ** list,
					xmlNode * a_node)
{
  xmlNode *cur_node;
  GstAdaptationSetNode *new_group;

  new_group = g_slice_new0 (GstAdaptationSetNode);
  if (new_group == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of AdaptationSet node failed!");
      return;
    }
  *list = g_list_append (*list, new_group);

  new_group->bitstreamSwitching =
    gst_mpdparse_get_xml_prop_boolean (mpdparse, a_node,
				       "bitstreamSwitching");
  new_group->segmentAlignmentFlag =
    gst_mpdparse_get_xml_prop_boolean (mpdparse, a_node,
				       "segmentAlignmentFlag");
  new_group->width =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node, "width", 0);
  new_group->height =
    gst_mpdparse_get_xml_prop_unsigned_integer (mpdparse, a_node, "height",
						0);
  new_group->frameRate =
    gst_mpdparse_get_xml_prop_double (mpdparse, a_node, "frameRate");
  new_group->lang =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "lang");
  new_group->mimeType =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "mimeType");
  new_group->minBandwidth =
    gst_mpdparse_get_xml_prop_integer (mpdparse, a_node, "minBandwidth");

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "Representation") == 0)
	    {
	      gst_mpdparse_parse_representation_node (mpdparse,
						      &new_group->
						      Representations,
						      cur_node);
	    }
	}
    }
}

static void
gst_mpdparse_parse_segment_info_default_node (Gstmpdparse * mpdparse,
					      GstSegmentInfoDefaultNode **
					      pointer, xmlNode * a_node)
{
  GstSegmentInfoDefaultNode *new_segment_info;

  gst_mpdcommon_free_segment_info_default_node (*pointer);
  *pointer = new_segment_info = g_slice_new0 (GstSegmentInfoDefaultNode);
  if (new_segment_info == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of SegmentInfoDefault node failed!");
      return;
    }

  //GST_LOG_OBJECT (mpdparse, "attributes of SegmentInfoDefault node:");
  new_segment_info->sourceUrlTemplatePeriod =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node,
				      "sourceUrlTemplatePeriod");
  new_segment_info->baseURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "baseURL");
}

static void
gst_mpdparse_parse_period_node (Gstmpdparse * mpdparse, GList ** list,
				xmlNode * a_node)
{
  xmlNode *cur_node;
  GstPeriodNode *new_period;
  new_period = g_slice_new0 (GstPeriodNode);
  new_period->Representations = NULL;
  new_period->AdaptationSets = NULL;
  if (new_period == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of Period node failed!");
      return;
    }

  *list = g_list_append (*list, new_period);

  new_period->segmentAlignmentFlag =
    gst_mpdparse_get_xml_prop_boolean (mpdparse, a_node,
				       "segmentAlignmentFlag");
  new_period->start =
    gst_mpdparse_get_xml_prop_duration (mpdparse, a_node, "start");
  new_period->duration =
    gst_mpdparse_get_xml_prop_duration (mpdparse, a_node, "duration");

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "Representation") == 0)
	    {
	      gst_mpdparse_parse_representation_node (mpdparse,
						      &new_period->
						      Representations,
						      cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "Group") == 0 ||
		   xmlStrcmp (cur_node->name,
			      (xmlChar *) "AdaptationSet") == 0)
	    {
	      gst_mpdparse_parse_adaptation_set_node (mpdparse,
						      &new_period->
						      AdaptationSets,
						      cur_node);
	    }
	  else
	    if (xmlStrcmp (cur_node->name, (xmlChar *) "SegmentInfoDefault")
		== 0)
	    {
	      gst_mpdparse_parse_segment_info_default_node (mpdparse,
							    &new_period->
							    SegmentInfoDefault,
							    cur_node);
	    }
	}
    }
}

static void
gst_mpdparse_parse_program_info_node (Gstmpdparse * mpdparse,
				      GstProgramInformationNode ** pointer,
				      xmlNode * a_node)
{
  xmlNode *cur_node;
  GstProgramInformationNode *new_prog_info;

  gst_mpdcommon_free_prog_info_node (*pointer);
  *pointer = new_prog_info = g_slice_new0 (GstProgramInformationNode);
  if (new_prog_info == NULL)
    {
      LOGASTW ("Warning", "%s", "Allocation of ProgramInfo node failed!");
      return;
    }

  new_prog_info->moreInformationURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "moreInformationURL");

  /* explore children nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "Title") == 0)
	    {
	      new_prog_info->Title =
		gst_mpdparse_get_xml_node_content (mpdparse, cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "Source") == 0)
	    {
	      new_prog_info->Source =
		gst_mpdparse_get_xml_node_content (mpdparse, cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "Copyright") == 0)
	    {
	      new_prog_info->Copyright =
		gst_mpdparse_get_xml_node_content (mpdparse, cur_node);
	    }
	}
    }
}

static void
gst_mpdparse_parse_root_node (Gstmpdparse * mpdparse, xmlNode * a_node)
{
  xmlNode *cur_node;
  GstMPDNode *new_mpd;

  //gst_mpdcommon_free_mpd_node (mpdparse->mpd_node);
  mpdparse->mpd_node = new_mpd = g_slice_new0 (GstMPDNode);
  mpdparse->mpd_node->Periods = NULL;
  if (new_mpd == NULL)
    {

      LOGASTW ("Warning", "%s", "Allocation of MPD node failed!");
      return;
    }

  new_mpd->default_namespace =
    gst_mpdparse_get_xml_node_namespace (mpdparse, a_node, NULL);
  new_mpd->namespace_xsi =
    gst_mpdparse_get_xml_node_namespace (mpdparse, a_node, "xsi");
  new_mpd->namespace_ext =
    gst_mpdparse_get_xml_node_namespace (mpdparse, a_node, "ext");
  new_mpd->schemaLocation =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "schemaLocation");
  new_mpd->profiles =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "profiles");
  new_mpd->type = gst_mpdparse_get_xml_prop_type (mpdparse, a_node, "type");
  new_mpd->mediaPresentationDuration =
    gst_mpdparse_get_xml_prop_duration (mpdparse, a_node,
					"mediaPresentationDuration");
  new_mpd->minBufferTime =
    gst_mpdparse_get_xml_prop_duration (mpdparse, a_node, "minBufferTime");
  new_mpd->baseURL =
    gst_mpdparse_get_xml_prop_string (mpdparse, a_node, "baseURL");
  /* explore children Period nodes */
  for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
    {
      if (cur_node->type == XML_ELEMENT_NODE)
	{
	  if (xmlStrcmp (cur_node->name, (xmlChar *) "Period") == 0)
	    {
	      gst_mpdparse_parse_period_node (mpdparse, &new_mpd->Periods,
					      cur_node);
	    }
	  else
	    if (xmlStrcmp (cur_node->name, (xmlChar *) "ProgramInformation")
		== 0)
	    {
	      gst_mpdparse_parse_program_info_node (mpdparse,
						    &new_mpd->ProgramInfo,
						    cur_node);
	    }
	  else if (xmlStrcmp (cur_node->name, (xmlChar *) "BaseURL") == 0)
	    {
	      new_mpd->baseURL =
		gst_mpdparse_get_xml_node_content (mpdparse, cur_node);
	    }
	}
    }

}

void
gst_mpdparse_parse_local_mpd_file (Gstmpdparse * mpdparse)
{
  xmlDocPtr doc;
  xmlNode *root_element = NULL;
  
  /* this initialize the library and check potential ABI mismatches  *
   * between the version it was compiled for and the actual shared   *
   * library used.                                                   */
  //LIBXML_TEST_VERSION
  
    /* parse "data" into a document (which is a libxml2 tree structure xmlDoc) */
    doc = xmlReadFile (mpdparse->input_mpd_filename, NULL, 0);
  if (doc == NULL)
    {
      LOGASTE ("Error", "%s", "failed to parse the MPD file");
      return;
    }
    
  if (mpdparse->output_mpd_filename != NULL)
    {
      LOGASTE ("Error", "%s", "MC-parse: salvataggio file in uscita");
      /* save the MPD file for checking to the specified output file location */
      xmlSaveFormatFileEnc (mpdparse->output_mpd_filename, doc, "UTF-8", 1);
    }

  /* get the root element node */
  root_element = xmlDocGetRootElement (doc);

  if (root_element->type != XML_ELEMENT_NODE
      || xmlStrcmp (root_element->name, (xmlChar *) "MPD") != 0)
    {
      LOGASTE ("Error", "%s",
	    "can not find the root element MPD, failed to parse the MPD file");
    }
  else
    {
      /* now we can parse the MPD root node and all children nodes, recursively */
      gst_mpdparse_parse_root_node (mpdparse, root_element);
      //GST_LOG_OBJECT (mpdparse, "successfully parsed the MPD file, firing the on-new-mpd signal...");
      /* trigger the "on-new-mpd" signal */
      //g_signal_emit (G_OBJECT (mpdparse), mpd_parse_signals[SIGNAL_ON_NEW_MPD], 0, mpdparse->mpd_node);
      /* now we can reset the adapter and the status, and wait for an eventual MPD file update */
      //gst_mpdparse_reset (mpdparse);
    }
  
  /* free the document */
  xmlFreeDoc (doc);
  /* cleanup function for the XML library */
  xmlCleanupParser ();
  /* dump XML library memory for debugging */
  //xmlMemoryDump ();
}

GstMPDFileStatus gst_mpdparse_parse_mpd_file (Gstmpdparse * mpdparse, const char * buffer, int size) 
{
  /*MANUEL CODE*/
  xmlDocPtr doc;
  xmlNode *root_element = NULL;

  /* this initialize the library and check potential ABI mismatches  *
   * between the version it was compiled for and the actual shared   *
   * library used.                                                   */
  //LIBXML_TEST_VERSION
  
  doc = xmlReadMemory (buffer, size, "noname.xml", NULL, 0);
  
  if (doc == NULL)
    {
      LOGASTE ("Error", "%s", "failed to parse the MPD file");
      return GST_MPD_FILE_STATUS_NOT_VALID;
    }
    
  //Don't need to save the MPD file, it generates errors.  
  //if (mpdparse->output_mpd_filename != NULL)
  //  {
  //    /* save the MPD file for checking to the specified output file location */
  //    xmlSaveFormatFileEnc (mpdparse->output_mpd_filename, doc, "UTF-8", 1);
  //  }
  
    /* get the root element node */
  root_element = xmlDocGetRootElement (doc);

  if (root_element->type != XML_ELEMENT_NODE
      || xmlStrcmp (root_element->name, (xmlChar *) "MPD") != 0)
    {
      LOGASTE ("Error", "%s",
	    "can not find the root element MPD, failed to parse the MPD file");
      return GST_MPD_FILE_STATUS_NOT_VALID;
    }
  else
    {
      /* now we can parse the MPD root node and all children nodes, recursively */
      gst_mpdparse_parse_root_node (mpdparse, root_element);
    }
  
  /* free the document */
  xmlFreeDoc (doc);
  /* cleanup function for the XML library */
  xmlCleanupParser ();
  /* dump XML library memory for debugging */
  //xmlMemoryDump ();
  
  return GST_MPD_FILE_STATUS_COMPLETED;
}
