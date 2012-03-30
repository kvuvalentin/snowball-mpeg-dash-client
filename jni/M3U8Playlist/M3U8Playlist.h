//M3U8Playlist class header file

#ifndef M3U8_PLAYLIST
#define M3U8_PLAYLIST

#include <include/utils/TypeHelpers.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <include/media/stagefright/FileSource.h>
#include <include/media/stagefright/DataSource.h>
#include <include/NuHTTPDataSource.h>
#include <include/M3UParser.h>



using namespace android;


class M3U8Playlist
{
    public:
        M3U8Playlist(const char* uri);
        ~M3U8Playlist();
        
        status_t FetchM3U8(const char *url);
        bool ParsePlaylist();
        
        //selectors
        int GetReprNumber() const;
        int GetTotalDuration() const;
        unsigned long GetReprBandwidth(int i) const;
        

    private:
    	const char* mMainURL;
    	const char* mURL;
        sp<ABuffer> mSourceBuffer;       
        sp<NuHTTPDataSource> mHTTPSource;
        sp<M3UParser> mParser;
        int mTotalDuration;
        
        bool mValidInfo;
        
        //structure useful to collect representation informations
        struct Representation 
        {
        	AString mURI;
        	unsigned long mBandwidth;
    	};

        Vector<Representation> mRepresentations;
        
        //private methods
        static int SortByBandwidth(const Representation *a, const Representation *b);
};


#endif
 
