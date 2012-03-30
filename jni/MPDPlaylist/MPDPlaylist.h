//MPDPlaylist class header file

#ifndef MPD_PLAYLIST
#define MPD_PLAYLIST

#include <include/utils/TypeHelpers.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <include/media/stagefright/FileSource.h>
#include <include/media/stagefright/DataSource.h>
#include <include/NuHTTPDataSource.h>
//#include <include/M3UParser.h>

//MC: GST module from stagefright>
#include <mpdmodule/glibchelper/gtypes.h>
#include <mpdmodule/glibchelper/glist.h>
#include <mpdmodule/glibchelper/glibchelper.h>
#include <mpdmodule/mpdcommon/gstmpdcommon.h>
#include <mpdmodule/mpdparse/gstmpdparse.h>


using namespace android;

class MPDPlaylist
{
    public:
        MPDPlaylist(const char* uri);
        ~MPDPlaylist();
        
        status_t FetchMPD(const char *url);
        bool ParsePlaylist();
        
        //selectors
        GstMPDNode * GetMPDMainNode();
        //int GetReprNumber() const;
        //int GetTotalDuration() const;
        //unsigned long GetReprBandwidth(int i) const;
        

    private:
    	const char* mURL;
        sp<ABuffer> mSourceBuffer;       
        sp<NuHTTPDataSource> mHTTPSource;
        int mTotalDuration;
        
        bool mValidInfo;
        
        //structure where the parsed tree is stored
        Gstmpdparse * mMPDtree;
};


#endif
 
