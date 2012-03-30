// MPDPlaylist class
//This class is useful to obtain MPD Playlist file informations through the
//selectors provided

#include "MPDPlaylist.h"

//MC: GST module from stagefright>
#include <mpdmodule/glibchelper/gtypes.h>
#include <mpdmodule/glibchelper/glist.h>
#include <mpdmodule/glibchelper/glibchelper.h>
#include <mpdmodule/mpdcommon/gstmpdcommon.h>
#include <mpdmodule/mpdparse/gstmpdparse.h>

MPDPlaylist::MPDPlaylist(const char* uri)
{
    mURL = (char*) uri;
    mValidInfo = false;
    mHTTPSource = new NuHTTPDataSource;
  
    //fetch the playlist informations
    if(ParsePlaylist() == true)
        mValidInfo = true;
}


MPDPlaylist::~MPDPlaylist()
{
}


bool MPDPlaylist::ParsePlaylist()
{ 
    //fetch the playlist into mSourceBuffer
    status_t err = FetchMPD(mURL);
    
    if (err != OK)
      	return false;
    
    //MPD parsing instance    
    GstMPDFileStatus mpd_status = gst_mpdparse_parse_mpd_file (mMPDtree, 
                                       (const char *) mSourceBuffer->data(), 
                                       (int) mSourceBuffer->size());
    
    //mMPDtree->input_mpd_filename = g_value_set_string ("mnt/sdcard/sl.mpd");
    //gst_mpdparse_parse_local_mpd_file (mMPDtree); 
    
    //verify parsing
    if (mpd_status != GST_MPD_FILE_STATUS_COMPLETED)
    	return false;
    
    LOGE ("MC: parse done successfully"); 
    
    return true;
}


status_t MPDPlaylist::FetchMPD(const char *url)
{  
    mSourceBuffer = NULL;

    sp<DataSource> source;

    if (!strncasecmp(url, "file://", 7)) 
        source = new FileSource(url + 7);
    else 
    {   
        status_t err = mHTTPSource->connect(url);

        if (err != OK)
            return err;  
            
        source = mHTTPSource;
    }
    
    
    off_t size;
    status_t err = source->getSize(&size);

    if (err != OK)
        size = 65536;
        
    LOGI("Size is %d", size);

    sp<ABuffer> buffer = new ABuffer(size);
    buffer->setRange(0, 0);

    for (;;) 
    {
        size_t bufferRemaining = buffer->capacity() - buffer->size();

        if (bufferRemaining == 0) 
        {
            bufferRemaining = 32768;

            LOGV("increasing download buffer to %d bytes",
                 buffer->size() + bufferRemaining);

            sp<ABuffer> copy = new ABuffer(buffer->size() + bufferRemaining);
            memcpy(copy->data(), buffer->data(), buffer->size());
            copy->setRange(0, buffer->size());

            buffer = copy;
        }

        ssize_t n = source->readAt(
                buffer->size(), buffer->data() + buffer->size(),
                bufferRemaining);

        if (n < 0)
            return err;
        

        if (n == 0)
            break;

        buffer->setRange(0, buffer->size() + (size_t)n);
    } 

    mSourceBuffer = buffer;

    return OK;
}


/* MC: get the MPD main nodefrom the MPDPlaylist class.
   if the informations are valid returns the GstMPDNode pointer, 
   else it returns NULL
   */
GstMPDNode * MPDPlaylist::GetMPDMainNode()
{
  if (mValidInfo)
    return mMPDtree->mpd_node;
  else
    return NULL;
}



/*

int M3U8Playlist::GetReprNumber() const
{
    if (mValidInfo)
        return mRepresentations.size();
    else
        return 0;
}

int M3U8Playlist::GetTotalDuration() const
{
	if (mValidInfo)
        return mTotalDuration;
    else
        return 0;
}

unsigned long M3U8Playlist::GetReprBandwidth(int i) const
{
    if (mValidInfo && i >= 0 && i < mRepresentations.size())
    {
  		const Representation &item = mRepresentations.itemAt(i);		
  		return item.mBandwidth;
    }
    else
        return 0;
}

*/
