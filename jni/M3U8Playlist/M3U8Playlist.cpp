// M3U8Playlist class
//This class is useful to obtain M3U8 Playlist file informations through the
//selectors provided

#include "M3U8Playlist.h"


M3U8Playlist::M3U8Playlist(const char* uri)
{
    mMainURL = (char*) uri;
    mValidInfo = false;
    mHTTPSource = new NuHTTPDataSource;
  
    //fetch the playlist informations
    if(ParsePlaylist() == true);
    {
        mValidInfo = true;
        LOGI("Fetch completed successfully");
    }
}


M3U8Playlist::~M3U8Playlist()
{
}


bool M3U8Playlist::ParsePlaylist()
{
  	//parse the .m3u8 file stored into mSourceBuffer
    status_t err = FetchM3U8(mMainURL);
    
    if (err != OK)
      	return false;
    
    //M3U8 parser instance    
    mParser = new M3UParser(mMainURL, mSourceBuffer->data(), mSourceBuffer->size());
    
    //verify parsing
    if (mParser->initCheck() != OK)
    	return false;
      
    //if mMainURL links others m3u8 files in order to identify more representations..
    if (mParser->isVariantPlaylist()) 
    {
        //for each representation..
        for (size_t i = 0; i < mParser->size(); ++i)
        {
            Representation item;
            sp<AMessage> meta;
            
            //...get url and bandwidth            
            mParser->itemAt(i, &item.mURI, &meta);

            unsigned long bandwidth;
            meta->findInt32("bandwidth", (int32_t *)&item.mBandwidth);

            mRepresentations.push(item);
        }
        mParser.clear();

        // fall through
        if (mRepresentations.size() == 0) {
            return false;
        }

        mRepresentations.sort(SortByBandwidth);           
       
        //in order to estimate the stream duration, choose the first representation
    	const Representation &item = mRepresentations.itemAt(0);
    	mURL = item.mURI.c_str();    
    }
	else
	{
	    //there isw only one level of playlist and the parsing is already done
		mURL = mMainURL;
	}
       
       
	//now start parsing the sub-playlist containing the segment infos
	if (mParser == NULL)
	{
		//fetch the .m3u8 playlist file into mSourceBuffer
    	status_t err = FetchM3U8(mURL);
      
    	if (err != OK)
      		return false;
      	
      	mParser = new M3UParser(mURL, mSourceBuffer->data(), mSourceBuffer->size());
      		
      	//verify parsing
    	if (mParser->initCheck() != OK)
    		return false;
    		
    	//verify that doesn't have sub-playlists at this level
    	if (mParser->isVariantPlaylist())
    	    return false;
    }
			
 	//get the segment number and the segment duration to compute total duration
 	int duration;
 	if(!mParser->meta()->findInt32("target-duration", &duration))
 	    return false;
 	
 	mTotalDuration = mParser->size() * duration;
 	
 	if (mTotalDuration <= 0)
 	   return false;
    
    return true;
}


int M3U8Playlist::SortByBandwidth(const Representation *a, const Representation *b) {
    if (a->mBandwidth < b->mBandwidth) {
        return -1;
    } else if (a->mBandwidth == b->mBandwidth) {
        return 0;
    }

    return 1;
}


status_t M3U8Playlist::FetchM3U8(const char *url)
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
