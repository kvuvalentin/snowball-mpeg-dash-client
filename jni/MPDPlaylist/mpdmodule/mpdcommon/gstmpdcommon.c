/**  TBD: add STM Header
 */


#include <string.h>
#include <stdlib.h>
#include "gstmpdcommon.h"
#include "debug.h"

/* make a copy of the MPD data structure (original data structure pointers will be set to NULL) */
GstMPDNode *
gst_mpdcommon_copy_mpd_node (GstMPDNode * mpd_node)
{
  GstMPDNode *mpd_copy;

  mpd_copy = g_slice_new0 (GstMPDNode);
  if (mpd_copy == NULL)
    {
      //GST_WARNING ("Allocation of MPD node failed!");
      return NULL;
    }

  /* copy data */
  mpd_copy->default_namespace = mpd_node->default_namespace;
  mpd_copy->namespace_xsi = mpd_node->namespace_xsi;
  mpd_copy->namespace_ext = mpd_node->namespace_ext;
  mpd_copy->schemaLocation = mpd_node->schemaLocation;
  mpd_copy->profiles = mpd_node->profiles;
  mpd_copy->type = mpd_node->type;
  mpd_copy->mediaPresentationDuration = mpd_node->mediaPresentationDuration;
  mpd_copy->minBufferTime = mpd_node->minBufferTime;
  mpd_copy->baseURL = mpd_node->baseURL;
  mpd_copy->ProgramInfo = mpd_node->ProgramInfo;
  mpd_copy->Periods = mpd_node->Periods;

  /* reset original data struct */
  mpd_node->default_namespace = NULL;
  mpd_node->namespace_xsi = NULL;
  mpd_node->namespace_ext = NULL;
  mpd_node->schemaLocation = NULL;
  mpd_node->profiles = NULL;
  mpd_node->baseURL = NULL;
  mpd_node->ProgramInfo = NULL;
  mpd_node->Periods = NULL;

  return mpd_copy;
}

GstRepresentationNode *
gst_mpdcommon_deep_copy_representation_node (const GstRepresentationNode *
					     rep)
{
  GList *list = NULL;

  if (rep == NULL)
    return NULL;

  GstRepresentationNode *new_rep = g_slice_dup (GstRepresentationNode, rep);

  new_rep->BaseURL = g_strdup (rep->BaseURL);
  new_rep->mimeType = g_strdup (rep->mimeType);
  new_rep->codecs = g_strdup (rep->codecs);
  new_rep->lang = g_strdup (rep->lang);
  new_rep->id = g_strdup (rep->id);
  new_rep->dependencyId = g_strdup (rep->dependencyId);

  new_rep->SegmentInfos = g_list_copy (rep->SegmentInfos);
  for (list = g_list_first (new_rep->SegmentInfos); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_segment_info_node (list->data);

  new_rep->SegmentBase = g_list_copy (rep->SegmentBase);
  for (list = g_list_first (new_rep->SegmentBase); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_segment_base_node (list->data);

  new_rep->SegmentList = g_list_copy (rep->SegmentList);
  for (list = g_list_first (new_rep->SegmentList); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_segment_list_node (list->data);

  return new_rep;
}

GstSegmentBaseNode *
gst_mpdcommon_deep_copy_segment_base_node (const GstSegmentBaseNode *
					   segment_base)
{
  GList *list = NULL;

  if (segment_base == NULL)
    return NULL;

  GstSegmentBaseNode *new_segment_base =
    g_slice_dup (GstSegmentBaseNode, segment_base);

  new_segment_base->Initialisation =
    g_list_copy (segment_base->Initialisation);
  for (list = g_list_first (new_segment_base->Initialisation); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_initialisation_node (list->data);

  return new_segment_base;
}

GstInitialisationNode *
gst_mpdcommon_deep_copy_initialisation_node (const GstInitialisationNode *
					     initialisation)
{
  if (initialisation == NULL)
    return NULL;

  GstInitialisationNode *new_initialisation =
    g_slice_dup (GstInitialisationNode, initialisation);

  new_initialisation->sourceURL = g_strdup (initialisation->sourceURL);
  new_initialisation->range = g_slice_dup (GstRange, initialisation->range);

  return new_initialisation;
}

GstSegmentListNode *
gst_mpdcommon_deep_copy_segment_list_node (const GstSegmentListNode *
					   segment_list)
{
  GList *list = NULL;

  if (segment_list == NULL)
    return NULL;

  GstSegmentListNode *new_segment_list =
    g_slice_dup (GstSegmentListNode, segment_list);

  new_segment_list->SegmentURL = g_list_copy (segment_list->SegmentURL);
  for (list = g_list_first (new_segment_list->SegmentURL); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_segment_url_node (list->data);

  return new_segment_list;
}

GstSegmentURLNode *
gst_mpdcommon_deep_copy_segment_url_node (const GstSegmentURLNode *
					  segment_url_node)
{
  if (segment_url_node == NULL)
    return NULL;

  GstSegmentURLNode *new_segment_url_node =
    g_slice_dup (GstSegmentURLNode, segment_url_node);

  new_segment_url_node->media = g_strdup (segment_url_node->media);
  new_segment_url_node->mediaRange =
    g_slice_dup (GstRange, segment_url_node->mediaRange);

  return new_segment_url_node;
}

GstUrlNode *
gst_mpdcommon_deep_copy_url_node (const GstUrlNode * url_node)
{
  if (url_node == NULL)
    return NULL;

  GstUrlNode *new_url_node = g_slice_dup (GstUrlNode, url_node);

  new_url_node->sourceURL = g_strdup (url_node->sourceURL);
  new_url_node->range = g_slice_dup (GstRange, url_node->range);

  return new_url_node;
}

GstSegmentInfoNode *
gst_mpdcommon_deep_copy_segment_info_node (const GstSegmentInfoNode *
					   segment_info)
{
  GList *list = NULL;

  if (segment_info == NULL)
    return NULL;

  GstSegmentInfoNode *new_segment_info =
    g_slice_dup (GstSegmentInfoNode, segment_info);

  new_segment_info->baseURL = g_strdup (segment_info->baseURL);
  new_segment_info->UrlTemplate =
    gst_mpdcommon_deep_copy_url_template (segment_info->UrlTemplate);

  new_segment_info->InitialisationSegmentURL =
    g_list_copy (segment_info->InitialisationSegmentURL);
  for (list = g_list_first (new_segment_info->InitialisationSegmentURL); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_url_node (list->data);

  new_segment_info->Urls = g_list_copy (segment_info->Urls);
  for (list = g_list_first (new_segment_info->Urls); list;
       list = g_list_next (list))
    list->data = gst_mpdcommon_deep_copy_url_node (list->data);

  return new_segment_info;
}

GstUrlTemplate *
gst_mpdcommon_deep_copy_url_template (const GstUrlTemplate * url_template)
{
  if (url_template == NULL)
    return NULL;

  GstUrlTemplate *new_url_template =
    g_slice_dup (GstUrlTemplate, url_template);

  new_url_template->sourceURL = g_strdup (url_template->sourceURL);
  new_url_template->id = g_strdup (url_template->id);

  return new_url_template;
}

/* comparison functions */
static int
strcmp_ext (const char *s1, const char *s2)
{
  if (s1 == NULL && s2 == NULL)
    return 0;
  if (s1 == NULL && s2 != NULL)
    return 1;
  if (s2 == NULL && s1 != NULL)
    return 1;
  return strcmp (s1, s2);
}

static int
gst_mpdcommon_compare_range (const GstRange * range1, const GstRange * range2)
{
  if (range1 == NULL && range2 == NULL)
    return 0;
  if (range1 == NULL && range2 != NULL)
    return 1;
  if (range2 == NULL && range1 != NULL)
    return 1;
  return range1->first_byte_pos != range2->first_byte_pos
    || range1->last_byte_pos != range2->last_byte_pos;
}

gint
gst_mpdcommon_compare_url_template_ext (const GstUrlTemplate * urlTemplate1,
					const GstUrlTemplate * urlTemplate2)
{
  if (urlTemplate1 == NULL && urlTemplate2 == NULL)
    return 0;
  if (urlTemplate1 == NULL && urlTemplate2 != NULL)
    return 1;
  if (urlTemplate2 == NULL && urlTemplate1 != NULL)
    return 1;
  return gst_mpdcommon_compare_url_template (urlTemplate1, urlTemplate2);
}

gint
gst_mpdcommon_compare_representation_nodes (const GstRepresentationNode *
					    rep1,
					    const GstRepresentationNode *
					    rep2)
{
  GList *list1, *list2;
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (rep1 != NULL && rep2 != NULL, 1);

  if (rep1->bandwidth != rep2->bandwidth)
    return 1;
  if (strcmp_ext (rep1->BaseURL, rep2->BaseURL) != 0)
    return 1;
  if (strcmp_ext (rep1->mimeType, rep2->mimeType) != 0)
    return 1;
  if (strcmp_ext (rep1->codecs, rep2->codecs) != 0)
    return 1;
  if (rep1->qualityRanking != rep2->qualityRanking)
    return 1;
  if (rep1->startWithRAP != rep2->startWithRAP)
    return 1;
  if (rep1->startWithSAP != rep2->startWithSAP)
    return 1;
  if (strcmp_ext (rep1->lang, rep2->lang) != 0)
    return 1;
  if (strcmp_ext (rep1->id, rep2->id) != 0)
    return 1;
  if (strcmp_ext (rep1->dependencyId, rep2->dependencyId) != 0)
    return 1;
  if (rep1->width != rep2->width)
    return 1;
  if (rep1->height != rep2->height)
    return 1;
  if (rep1->minBufferTime != rep2->minBufferTime)
    return 1;
  list1 = g_list_first (rep1->SegmentInfos);
  list2 = g_list_first (rep2->SegmentInfos);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_segment_info_nodes (list1->data, list2->data)
	  == 1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }
  list1 = g_list_first (rep1->SegmentBase);
  list2 = g_list_first (rep2->SegmentBase);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_segment_base_nodes (list1->data, list2->data)
	  == 1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }
  list1 = g_list_first (rep1->SegmentList);
  list2 = g_list_first (rep2->SegmentList);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_segment_list_nodes (list1->data, list2->data)
	  == 1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }
  return 0;
}

gint
gst_mpdcommon_compare_segment_base_nodes (const GstSegmentBaseNode * node1,
					  const GstSegmentBaseNode * node2)
{
  GList *list1, *list2;
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (node1 != NULL && node2 != NULL, 1);

  if (node1->timescale != node2->timescale)
    return 1;

  list1 = g_list_first (node1->Initialisation);
  list2 = g_list_first (node2->Initialisation);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_initialisation_node (list1->data, list2->data)
	  == 1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }

  return 0;
}

gint
gst_mpdcommon_compare_segment_list_nodes (const GstSegmentListNode * node1,
					  const GstSegmentListNode * node2)
{
  GList *list1, *list2;
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (node1 != NULL && node2 != NULL, 1);

  if (node1->duration != node2->duration)
    return 1;

  list1 = g_list_first (node1->SegmentURL);
  list2 = g_list_first (node2->SegmentURL);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_segment_url_node (list1->data, list2->data) ==
	  1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }

  return 0;
}

gint
gst_mpdcommon_compare_initialisation_node (const GstInitialisationNode *
					   node1,
					   const GstInitialisationNode *
					   node2)
{
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (node1 != NULL && node2 != NULL, 1);

  if (strcmp_ext (node1->sourceURL, node2->sourceURL) != 0)
    return 1;
  if (gst_mpdcommon_compare_range (node1->range, node2->range) != 0)
    return 1;
  return 0;
}

gint
gst_mpdcommon_compare_segment_url_node (const GstSegmentURLNode * node1,
					const GstSegmentURLNode * node2)
{
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (node1 != NULL && node2 != NULL, 1);

  if (strcmp_ext (node1->media, node2->media) != 0)
    return 1;
  if (gst_mpdcommon_compare_range (node1->mediaRange, node2->mediaRange) != 0)
    return 1;
  return 0;
}

gint
gst_mpdcommon_compare_segment_info_nodes (const GstSegmentInfoNode * node1,
					  const GstSegmentInfoNode * node2)
{
  GList *list1, *list2;
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (node1 != NULL && node2 != NULL, 1);

  if (node1->duration != node2->duration)
    return 1;
  if (strcmp_ext (node1->baseURL, node2->baseURL) != 0)
    return 1;
  if (gst_mpdcommon_compare_url_template_ext
      (node1->UrlTemplate, node2->UrlTemplate) == 1)
    return 1;
  list1 = g_list_first (node1->InitialisationSegmentURL);
  list2 = g_list_first (node2->InitialisationSegmentURL);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_url_node (list1->data, list2->data) == 1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }
  list1 = g_list_first (node1->Urls);
  list2 = g_list_first (node2->Urls);
  while (list1 != NULL || list2 != NULL)
    {
      if (list1 == NULL && list2 != NULL)
	return 1;
      if (list1 != NULL && list2 == NULL)
	return 1;
      if (gst_mpdcommon_compare_url_node (list1->data, list2->data) == 1)
	return 1;
      list1 = g_list_next (list1);
      list2 = g_list_next (list2);
    }

  return 0;
}

gint
gst_mpdcommon_compare_url_template (const GstUrlTemplate * urlTemplate1,
				    const GstUrlTemplate * urlTemplate2)
{
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (urlTemplate1 != NULL && urlTemplate2 != NULL, 1);

  if (strcmp_ext (urlTemplate1->sourceURL, urlTemplate2->sourceURL) != 0)
    return 1;
  if (strcmp_ext (urlTemplate1->id, urlTemplate2->id) != 0)
    return 1;
  if (urlTemplate1->startIndex != urlTemplate2->startIndex)
    return 1;
  if (urlTemplate1->endIndex != urlTemplate2->endIndex)
    return 1;

  return 0;
}

gint
gst_mpdcommon_compare_url_node (const GstUrlNode * urlNode1,
				const GstUrlNode * urlNode2)
{
  /* the result is not defined if any of the arguments is NULL */
  g_return_val_if_fail (urlNode1 != NULL && urlNode2 != NULL, 1);

  if (strcmp_ext (urlNode1->sourceURL, urlNode2->sourceURL) != 0)
    return 1;
  if (urlNode1->start != urlNode2->start)
    return 1;
  if (gst_mpdcommon_compare_range (urlNode1->range, urlNode2->range) != 0)
    return 1;

  return 0;
}

/* navigation functions */
GstPeriodNode *
gst_mpdcommon_get_next_period (GList * Periods, GstPeriodNode * prev_period)
{
  GList *list = NULL;

  if (Periods == NULL)
    return NULL;

  if (prev_period == NULL)
    {
      /* return the first period in the list */
      list = g_list_first (Periods);
    }
  else
    {
      /* found prev_period in the list */
      list = g_list_find (Periods, prev_period);
      /* next period */
      list = g_list_next (list);
    }

  return list ? (GstPeriodNode *) list->data : NULL;
}

GstAdaptationSetNode *
gst_mpdcommon_get_highest_adaptation_set (GList * AdaptationSets)
{
  GList *list = NULL;

  if (AdaptationSets == NULL)
    return NULL;

  list = g_list_last (AdaptationSets);

  return list ? (GstAdaptationSetNode *) list->data : NULL;
}

GstRepresentationNode *
gst_mpdcommon_get_highest_representation (GList * Representations)
{
  GList *list = NULL;

  if (Representations == NULL)
    return NULL;

  list = g_list_last (Representations);

  return list ? (GstRepresentationNode *) list->data : NULL;
}

GstRepresentationNode *
gst_mpdcommon_get_representation_with_max_bandwidth (GList * Representations,
						     gint max_bandwidth)
{
  GList *list = NULL;
  GstRepresentationNode *representation, *best_rep = NULL;

  if (Representations == NULL)
    return NULL;

  if (max_bandwidth <= 0)	/* 0 => get highest representation available */
    return gst_mpdcommon_get_highest_representation (Representations);

  for (list = g_list_first (Representations); list; list = g_list_next (list))
    {
      representation = (GstRepresentationNode *) list->data;
      if (representation && representation->bandwidth <= max_bandwidth)
	{
	  best_rep = representation;
	}
    }

  return best_rep;
}

/* old syntax */
GstSegmentInfoNode *
gst_mpdcommon_get_first_segment_info (GList * SegmentInfos)
{
  GList *list = NULL;

  if (SegmentInfos == NULL)
    return NULL;

  list = g_list_first (SegmentInfos);

  return list ? (GstSegmentInfoNode *) list->data : NULL;
}

GstUrlNode *
gst_mpdcommon_get_first_initialisation_segment_url (GList *
						    InitialisationSegmentURL)
{
  GList *list = NULL;

  if (InitialisationSegmentURL == NULL)
    return NULL;

  list = g_list_first (InitialisationSegmentURL);

  return list ? (GstUrlNode *) list->data : NULL;
}

/* end old syntax */

GstSegmentBaseNode *
gst_mpdcommon_get_first_segment_base (GList * SegmentBase)
{
  GList *list = NULL;

  if (SegmentBase == NULL)
    return NULL;

  list = g_list_first (SegmentBase);

  return list ? (GstSegmentBaseNode *) list->data : NULL;
}

GstInitialisationNode *
gst_mpdcommon_get_first_initialisation (GList * Initialisation)
{
  GList *list = NULL;

  if (Initialisation == NULL)
    return NULL;

  list = g_list_first (Initialisation);

  return list ? (GstInitialisationNode *) list->data : NULL;
}

GstSegmentListNode *
gst_mpdcommon_get_first_segment_list (GList * SegmentList)
{
  GList *list = NULL;

  if (SegmentList == NULL)
    return NULL;

  list = g_list_first (SegmentList);

  return list ? (GstSegmentListNode *) list->data : NULL;
}

GList *
gst_mpdcommon_get_first_segment_url (GList * Urls)
{
  if (Urls == NULL)
    return NULL;

  return g_list_first (Urls);
}

/* memory management functions */
void
gst_mpdcommon_free_mpd_node (GstMPDNode * mpd_node)
{
  if (mpd_node)
    {
      g_free (mpd_node->default_namespace);
      g_free (mpd_node->namespace_xsi);
      g_free (mpd_node->namespace_ext);
      g_free (mpd_node->schemaLocation);
      g_free (mpd_node->profiles);
      g_free (mpd_node->baseURL);
      gst_mpdcommon_free_prog_info_node (mpd_node->ProgramInfo);
      g_list_foreach (mpd_node->Periods,
		      (GFunc) gst_mpdcommon_free_period_node, NULL);
      g_list_free (mpd_node->Periods);
      g_slice_free (GstMPDNode, mpd_node);
    }
}

void
gst_mpdcommon_free_prog_info_node (GstProgramInformationNode * prog_info_node)
{
#if 0				//MANUELE
  if (prog_info_node)
    {
      g_free (prog_info_node->moreInformationURL);
      g_free (prog_info_node->Title);
      g_free (prog_info_node->Source);
      g_free (prog_info_node->Copyright);
      g_slice_free (GstProgramInformationNode, prog_info_node);
    }
#endif
}

void
gst_mpdcommon_free_period_node (GstPeriodNode * period_node)
{
  if (period_node)
    {
      gst_mpdcommon_free_segment_info_default_node (period_node->
						    SegmentInfoDefault);
      g_list_foreach (period_node->AdaptationSets,
		      (GFunc) gst_mpdcommon_free_adaptation_set_node, NULL);
      g_list_free (period_node->AdaptationSets);
      g_list_foreach (period_node->Representations,
		      (GFunc) gst_mpdcommon_free_representation_node, NULL);
      g_list_free (period_node->Representations);
      g_slice_free (GstPeriodNode, period_node);
    }
}

void
gst_mpdcommon_free_segment_info_default_node (GstSegmentInfoDefaultNode *
					      segment_info_default_node)
{
  if (segment_info_default_node)
    {
      g_free (segment_info_default_node->sourceUrlTemplatePeriod);
      g_free (segment_info_default_node->baseURL);
      g_slice_free (GstSegmentInfoDefaultNode, segment_info_default_node);
    }
}

void
gst_mpdcommon_free_adaptation_set_node (GstAdaptationSetNode *
					adaptation_set_node)
{
  if (adaptation_set_node)
    {
      g_free (adaptation_set_node->mimeType);
      g_free (adaptation_set_node->lang);
      g_list_foreach (adaptation_set_node->Representations,
		      (GFunc) gst_mpdcommon_free_representation_node, NULL);
      g_list_free (adaptation_set_node->Representations);
      g_slice_free (GstAdaptationSetNode, adaptation_set_node);
    }
}

void
gst_mpdcommon_free_representation_node (GstRepresentationNode *
					representation_node)
{
  if (representation_node)
    {
      g_free (representation_node->BaseURL);
      g_free (representation_node->mimeType);
      g_free (representation_node->codecs);
      g_free (representation_node->lang);
      g_free (representation_node->id);
      g_free (representation_node->dependencyId);
      g_list_foreach (representation_node->SegmentInfos,
		      (GFunc) gst_mpdcommon_free_segment_info_node, NULL);
      g_list_free (representation_node->SegmentInfos);
      g_list_foreach (representation_node->SegmentBase,
		      (GFunc) gst_mpdcommon_free_segment_base_node, NULL);
      g_list_free (representation_node->SegmentBase);
      g_list_foreach (representation_node->SegmentList,
		      (GFunc) gst_mpdcommon_free_segment_list_node, NULL);
      g_list_free (representation_node->SegmentList);
      g_slice_free (GstRepresentationNode, representation_node);
    }
}

void
gst_mpdcommon_free_segment_info_node (GstSegmentInfoNode * segment_info_node)
{
  if (segment_info_node)
    {
      g_free (segment_info_node->baseURL);
      gst_mpdcommon_free_url_template_node (segment_info_node->UrlTemplate);
      g_list_foreach (segment_info_node->InitialisationSegmentURL,
		      (GFunc) gst_mpdcommon_free_url_node, NULL);
      g_list_free (segment_info_node->InitialisationSegmentURL);
      g_list_foreach (segment_info_node->Urls,
		      (GFunc) gst_mpdcommon_free_url_node, NULL);
      g_list_free (segment_info_node->Urls);
      g_slice_free (GstSegmentInfoNode, segment_info_node);
    }
}

void
gst_mpdcommon_free_segment_base_node (GstSegmentBaseNode * segment_base_node)
{
  if (segment_base_node)
    {
      g_list_foreach (segment_base_node->Initialisation,
		      (GFunc) gst_mpdcommon_free_initialisation_node, NULL);
      g_list_free (segment_base_node->Initialisation);
      g_slice_free (GstSegmentBaseNode, segment_base_node);
    }
}

void
gst_mpdcommon_free_segment_list_node (GstSegmentListNode * segment_list_node)
{
  if (segment_list_node)
    {
      g_list_foreach (segment_list_node->SegmentURL,
		      (GFunc) gst_mpdcommon_free_segment_url_node, NULL);
      g_list_free (segment_list_node->SegmentURL);
      g_slice_free (GstSegmentListNode, segment_list_node);
    }
}

void
gst_mpdcommon_free_initialisation_node (GstInitialisationNode *
					initialisation_node)
{
  if (initialisation_node)
    {
      g_free (initialisation_node->sourceURL);
      g_slice_free (GstRange, initialisation_node->range);
      g_slice_free (GstInitialisationNode, initialisation_node);
    }
}

void
gst_mpdcommon_free_segment_url_node (GstSegmentURLNode * segment_url)
{
  if (segment_url)
    {
      g_free (segment_url->media);
      g_slice_free (GstRange, segment_url->mediaRange);
      g_slice_free (GstSegmentURLNode, segment_url);
    }
}

void
gst_mpdcommon_free_url_template_node (GstUrlTemplate * url_template)
{
  if (url_template)
    {
      g_free (url_template->sourceURL);
      g_free (url_template->id);
      g_slice_free (GstUrlTemplate, url_template);
    }
}

void
gst_mpdcommon_free_url_node (GstUrlNode * url_node)
{
  if (url_node)
    {
      g_free (url_node->sourceURL);
      g_slice_free (GstRange, url_node->range);
      g_slice_free (GstUrlNode, url_node);
    }
}


/**************************
 * debug output functions *
 **************************/
void
url_to_string (GList * urllist)
{
  unsigned int count = 0;
  GList *list = NULL;
  for (list = g_list_first (urllist); list; list = g_list_next (list))
    {
      const GstUrlNode *url = (GstUrlNode *) list->data;
      LOGASTW ("dump_mpdnode", "   %d : Start : %lu [ms] , URL = %s", count++,
	     (unsigned long) url->start, url->sourceURL);
    }
}



void
segment_info_to_string (GList * segmentinfos)
{
  unsigned int count = 0;
  GList *list = NULL;
  for (list = g_list_first (segmentinfos); list; list = g_list_next (list))
    {
      const GstSegmentInfoNode *info = (GstSegmentInfoNode *) list->data;

      LOGASTW ("dump_mpdnode", "   %d : Duration : %lu [ms] , Base URL = %s", count++,
	    (unsigned long) info->duration, info->baseURL);
      LOGASTW ("dump_mpdnode", "%s ",
	    "Start Initialization Segment -------------------------------------------");
      url_to_string (info->InitialisationSegmentURL);
      LOGASTW ("dump_mpdnode", "%s ",
	    "End Initialization Segment -------------------------------------------");
      LOGASTW ("dump_mpdnode", "%s ",
	    "Start Fragment List        -------------------------------------------");
      url_to_string (info->Urls);
      LOGASTW ("dump_mpdnode", "%s",
	    "End Fragment List        -------------------------------------------");
    }
}

const char *
representations_to_string (GList * representations)
{
  static char buf[2048];
  sprintf (buf, "Representation : ( ");
  GList *list = NULL;
  for (list = g_list_first (representations); list; list = g_list_next (list))
    {
      const GstRepresentationNode *representation =
	(GstRepresentationNode *) list->data;
      if (representation->id && 0 == strcmp (representation->id, "2D"))
	sprintf (buf + strlen (buf),
		 "BW=%d/2D [kb/s], Resolution=[%d x %d], Mime=%s",
		 representation->bandwidth / 1000, representation->width,
		 representation->height, representation->mimeType);
      else if (representation->id && 0 == strcmp (representation->id, "3D"))
	sprintf (buf + strlen (buf),
		 "BW=%d/3D [kb/s], Resolution=[%d x %d], Mime=%s",
		 representation->bandwidth / 1000, representation->width,
		 representation->height, representation->mimeType);
      else
	sprintf (buf + strlen (buf), "BW=%d [kb/s], Resolution=[%d x %d], Mime=%s",
		 representation->bandwidth / 1000, representation->width,
		 representation->height, representation->mimeType);
      if (g_list_next (list))
	sprintf (buf + strlen (buf), ", ");
    }
  sprintf (buf + strlen (buf), ")");

  return buf;
}

const char *
representations_to_string2 (GList * representations)
{
  static char buf[2048];
  GList *list = NULL;
  for (list = g_list_first (representations); list; list = g_list_next (list))
    {
      const GstRepresentationNode *representation =
	(GstRepresentationNode *) list->data;
      if (representation->id && 0 == strcmp (representation->id, "2D"))
	sprintf (buf + strlen (buf),
		 "Representation Info : BW=%d/2D [kb/s], Resolution=[%d x %d], Mime=%s",
		 representation->bandwidth / 1000, representation->width,
		 representation->height, representation->mimeType);
      else if (representation->id && 0 == strcmp (representation->id, "3D"))
	sprintf (buf + strlen (buf),
		 "Representation Info : BW=%d/3D [kb/s], Resolution=[%d x %d], Mime=%s",
		 representation->bandwidth / 1000, representation->width,
		 representation->height, representation->mimeType);
      else
	sprintf (buf + strlen (buf),
		 "Representation Info : BW=%d [kb/s], Resolution=[%d x %d], Mime=%s",
		 representation->bandwidth / 1000, representation->width,
		 representation->height, representation->mimeType);

      LOGASTW ("dump_mpdnode", "%s", buf);
      LOGASTW ("dump_mpdnode", "%s", "Start Segment Info:");
      segment_info_to_string (representation->SegmentInfos);
      LOGASTW ("dump_mpdnode", "%s",
	    "End Segment Info   -----------------------------------------------");

    }
  //sprintf(buf + strlen(buf), ")");

  return buf;
}

/* debug output functions */
void
periods_to_string (GList * periods)
{
  GList *list = NULL;

  for (list = g_list_first (periods); list; list = g_list_next (list))
    {
      const GstPeriodNode *period = (GstPeriodNode *) list->data;
      LOGASTW ("dump_mpdnode", "%s",
	    "START Period -----------------------------------------------");
      LOGASTW ("dump_mpdnode", "Period Start=%lu ; Duration=%lu [ms]", (unsigned long)period->start,
	    (unsigned long) period->duration);
      LOGASTW ("dump_mpdnode", "%s", "Display Representation List:");
      representations_to_string2 (period->Representations);
      LOGASTW ("dump_mpdnode", "%s",
	    "END Period   -----------------------------------------------");
    }
  return;

}

void
dump_mpdnode (GstMPDNode * new_mpd)
{
  GList *list = NULL;
  const char *bufRepr;
  GstPeriodNode *periods;

  LOGASTW ("dump_mpdnode", "namespace_xsi : %s %x", new_mpd->namespace_xsi,
	(unsigned int) new_mpd);
  LOGASTW ("dump_mpdnode", "namespace_ext : %s", new_mpd->namespace_ext);
  LOGASTW ("dump_mpdnode", "schemaLocation : %s", new_mpd->schemaLocation);
  LOGASTW ("dump_mpdnode", "profiles : %s", new_mpd->profiles);
  LOGASTW ("dump_mpdnode", "baseURL : %s", new_mpd->baseURL);
  LOGASTW ("dump_mpdnode", "minBufferTime : %ld", new_mpd->minBufferTime);
  LOGASTW ("dump_mpdnode", "ProgramInfo : MoreInfo=%s",
	new_mpd->ProgramInfo->moreInformationURL);

  periods_to_string (new_mpd->Periods);

  return;
}



//MC: API to get informations from the mpd structure

/**
 * get_mpd_periods_number:
 * 
 * @new_mpd: a GstMPDNode structure pointer
 * 
 * Gets the number of periods within a MPD tree.
 *
 * Returns: the number of periods, or -1 if new_mpd is NULL
 */
int get_mpd_periods_number (GstMPDNode * new_mpd)
{ 
  if (new_mpd)
    return (int) g_list_length (new_mpd->Periods);
  else
    return -1;
}


/**
 * get_mpd_representations_number:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 *
 * Gets the number of representations within a given period index.
 *
 * Returns: the number of representations, or -1 if the index is not valid
 */
int get_mpd_representations_number (GstMPDNode * new_mpd, int period_idx)
{ 
  if (new_mpd == NULL)
    return -1;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
 
  //MC: if index is valid..
  if (period)
    return (int) g_list_length (period->Representations);
  else 
    return -1;
}


/**
 * get_mpd_representation_bandwidth:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 *
 * Gets the bandwidth [b/s] of a representation identified by period and representation indexes.
 *
 * Returns: the bandwidth in [b/s], or -1 if one or both the indexes are not valid
 */
int get_mpd_representation_bandwidth (GstMPDNode * new_mpd, int period_idx, int rep_idx)
{
  if (new_mpd == NULL)
    return -1;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  if (period)
  {
    GList * list = g_list_nth (period->Representations, rep_idx );
    GstRepresentationNode * representation = (GstRepresentationNode *) list->data;
    
    if (representation)
      return (int) representation->bandwidth;
    else
      return -1;
  }
  else
    return -1;
}


/**
 * reorder_mpd_representations:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 *
 * Reorder the representations within a period according to the representation bandwidth [b/s].
 */
void reorder_mpd_representations (GstMPDNode * new_mpd, int period_idx)
{
  if (new_mpd == NULL)
    return;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  if (period)
  {
    int changes, i;
    
    //bubble sort reordering
    do
    {  
      changes = 0;
      
      for (i = 0; i < (int) g_list_length (period->Representations) -1; i++)
      {
	//get two representations and compare their bandwidth
        GList * list_1 = g_list_nth (period->Representations, i );
	GstRepresentationNode * representation_1 = (GstRepresentationNode *) list_1->data;
	GList * list_2 = g_list_nth (period->Representations, i + 1 );
	GstRepresentationNode * representation_2 = (GstRepresentationNode *) list_2->data;
	
	if ((int) representation_1->bandwidth > (int) representation_2->bandwidth) 
	{
	  //swap the representations
	  changes++;
	  
	  g_list_remove (period->Representations, (gconstpointer) representation_2);
	  period->Representations = g_list_insert (period->Representations, (gpointer) representation_2, (gint) i);

	}
      }
    } while (changes != 0);

  }
  return;
}


/**
 * get_mpd_representation_width:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 *
 * Gets the width [px] of a representation identified by period and representation indexes.
 *
 * Returns: the width in [px], or -1 if one or both the indexes are not valid
 */
int get_mpd_representation_width (GstMPDNode * new_mpd, int period_idx, int rep_idx)
{
  if (new_mpd == NULL)
    return -1;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  if (period)
  {
    GstRepresentationNode * representation = (GstRepresentationNode *) g_list_nth (period->Representations, rep_idx)->data;
    
    if (representation)
      return (int) representation->width;
    else
      return -1;
  }
  else
    return -1;
}


/**
 * get_mpd_representation_height:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 *
 * Gets the height [px] of a representation identified by period and representation indexes.
 *
 * Returns: the height in [px], or -1 if one or both the indexes are not valid
 */
int get_mpd_representation_height (GstMPDNode * new_mpd, int period_idx, int rep_idx)
{
  if (new_mpd == NULL)
    return -1;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  if (period)
  {
    GstRepresentationNode * representation = (GstRepresentationNode *) g_list_nth (period->Representations, rep_idx)->data;
    
    if (representation)
      return (int) representation->height;
    else
      return -1;
  }
  else
    return -1;
}


/**
 * get_mpd_representation_minBufferTime:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 *
 * Gets the minBufferTime [ms] of a representation identified by period and representation indexes.
 *
 * Returns: the minBufferTime in [ms], or -1 if one or both the indexes are not valid
 */
int get_mpd_representation_minBufferTime (GstMPDNode * new_mpd, int period_idx, int rep_idx)
{
  if (new_mpd == NULL)
    return -1;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  if (period)
  {
    GList * list = g_list_nth (period->Representations, rep_idx );
    GstRepresentationNode * representation = (GstRepresentationNode *) list->data;
    
    if (representation)
      return (int) representation->minBufferTime;
    else
      return -1;
  }
  else
    return -1;
}


/**
 * get_mpd_segments_number:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 *
 * Gets the number of segments within a given representation identified by period and representation indexes.
 *
 * Returns: the number of segments, or -1 if one or both the indexes are not valid
 */
int get_mpd_segments_number (GstMPDNode * new_mpd, int period_idx, int rep_idx) //supposing segment info index = 0
{ 
  if (new_mpd == NULL)
    return -1;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  //MC: if index is valid..
  if (period)
  {
    GList * list = g_list_nth (period->Representations, rep_idx );
    GstRepresentationNode * representation = (GstRepresentationNode *) list->data;
    
    if (representation)
    {
      GList * list2 = g_list_nth (representation->SegmentInfos, 0 );
      GstSegmentInfoNode *segment_info = (GstSegmentInfoNode *) list2->data;
      
      if (segment_info)
        return (int) g_list_length (segment_info->Urls);
    }
  }
  
  return -1;
}


/**
 * get_mpd_segments_URL:
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 * @segm_idx: 	the position of the segment, counting from 0
 *
 * Gets the number of segments within a given representation identified by period and representation indexes.
 *
 * Returns: the number of segments, or NULL if one or both the indexes are not valid
 */
char * get_mpd_segments_URL (GstMPDNode * new_mpd, int period_idx, int rep_idx, int segm_idx) //supposing segment info index = 0
{ 
  if (new_mpd == NULL)
    return NULL;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  //MC: if index is valid..
  if (period)
  {
    GList * list = g_list_nth (period->Representations, rep_idx );
    GstRepresentationNode * representation = (GstRepresentationNode *) list->data;
    
    if (representation)
    {
      GList * list2 = g_list_nth (representation->SegmentInfos, 0 );
      GstSegmentInfoNode *segment_info = (GstSegmentInfoNode *) list2->data;
      
      if (segment_info)
      {
	GList * list3 = g_list_nth (segment_info->Urls, segm_idx);
	GstUrlNode *url = (GstUrlNode *) list3->data;
	
	if (url) 
	  return (char *) url->sourceURL;
      }
    }
  }
  
  return NULL;
}


/**
 * get_mpd_representation_mimetype :
 * 
 * @new_mpd: 	a GstMPDNode structure pointer
 * @period_idx: the position of the period, counting from 0
 * @rep_idx: 	the position of the representation, counting from 0
 *
 * Gets the mime type of a given representation identified by period and representation indexes.
 *
 * Returns: the mime type of the representation, or NULL if one or both the indexes are not valid
 */
char * get_mpd_representation_mimetype (GstMPDNode * new_mpd, int period_idx, int rep_idx) //supposing segment info index = 0
{ 
  if (new_mpd == NULL)
    return NULL;
  
  GstPeriodNode * period = (GstPeriodNode *) (g_list_nth(new_mpd->Periods, period_idx))->data;
  
  //MC: if index is valid..
  if (period)
  {
    GList * list = g_list_nth (period->Representations, rep_idx );
    GstRepresentationNode * representation = (GstRepresentationNode *) list->data;
    
    if (representation)
      return (char *) representation->mimeType;
  }
  
  return NULL;
}


