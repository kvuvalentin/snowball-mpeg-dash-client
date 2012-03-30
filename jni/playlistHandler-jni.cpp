//MC: PlaylistInformations class - JNI interface
//this interface is useful to get informations 
//  1) about a M3U file (.m3u8) specified as the parameter of the setPlaylist function;
//  2) about a MPD file (.mpd) specified as the parameter of the setPlaylist function;
//the informations are obtained through the methods getM3U8Information and getMPDInformation. 


#include <string.h>
#include <jni.h>
#include <stdlib.h>

//String8 includes
#include <utils/String8.h>

//M3U8Playlist class include
#include "M3U8Playlist.h"
#include "MPDPlaylist.h"


//using namespace android;


#ifdef __cplusplus
extern "C" {
#endif


//global references used in methods

#define BUFF_SIZE 256    //buffer used to store representation bandwidths


//setPlaylist function
//passing a M3U8 or MPD playlist uri will obtain a proper Playlist pointer
JNIEXPORT int JNICALL 
Java_AndroidDashClient_com_InfoActivity_setPlaylist( JNIEnv* env, jobject thiz, 
														 jstring name, jstring type)
{
    //get the file URI and type from the jstring arguments
    const char *mUri, *mType;
    mUri = env->GetStringUTFChars(name, NULL);
	mType = env->GetStringUTFChars(type, NULL);

    //check if the given playlist is an m3u playlist
    if ((!strncasecmp(mType, "m3u8", 4)) && (
    (!strncasecmp(mUri, "httplive://", 11)) || (!strncasecmp(mUri, "httpdash://", 11)) ) ) 
    {
        String8 uri("http://");
        uri.append(mUri + 11);
        
        M3U8Playlist *mPlaylist = new M3U8Playlist(uri.string());
        
        return (int)mPlaylist;
    }
    else if ( (!strncasecmp(mType, "mpd", 3)) && (!strncasecmp(mUri, "httpdash://", 11) ) )
    {
        String8 uri("http://");
        uri.append(mUri + 11);
        
        MPDPlaylist *mPlaylist = new MPDPlaylist(uri.string());
        
        return (int)mPlaylist;
    }
    else
      return 0;
}


//freeM3U8Playlist function
//delete the M3U8Playlist object through the resource pointer
JNIEXPORT void JNICALL 
Java_AndroidDashClient_com_InfoActivity_freeM3U8Playlist( JNIEnv* env, jobject thiz, jint resource)
{
    M3U8Playlist* mPlaylist = (M3U8Playlist*) resource; 
    delete mPlaylist; 
}


//freeMPDPlaylist function
//delete the MPDPlaylist object through the resource pointer
JNIEXPORT void JNICALL 
Java_AndroidDashClient_com_InfoActivity_freeMPDPlaylist( JNIEnv* env, jobject thiz, jint resource)
{
    MPDPlaylist* mPlaylist = (MPDPlaylist*) resource; 
    delete mPlaylist; 
}


//getM3U8PlaylistInfo function
//passing the Playlist pointer and a info parameter (es SIZE..) will get the informations
JNIEXPORT jstring JNICALL 
Java_AndroidDashClient_com_InfoActivity_getM3U8PlaylistInfo( JNIEnv* env, jobject thiz, jint resource, jstring key)
{
    M3U8Playlist* mPlaylist = (M3U8Playlist*) resource; 
    
    //get the requested information from the jstring key argument
    const char *mKey;
    mKey = env->GetStringUTFChars(key, NULL);
    
    //switch on the key type and get the proper information
    if (!strncasecmp(mKey, "N_REPS", 5))
    {    
		char buffer[16]; 
		sprintf(buffer, "%d", mPlaylist->GetReprNumber());
		jstring	n_reps = (*env).NewStringUTF(buffer);
		
		return n_reps;	
    }
    else if (!strncasecmp(mKey, "REPS_BANDS", 10))
    {
      	long int bw;
      	int n = 0;
      	
      	char buffer[BUFF_SIZE]; 
      	
      	for (int i = 0; i < mPlaylist->GetReprNumber(); i++)
      	{
      	    bw = mPlaylist->GetReprBandwidth(i);
      	
      	    if (i != mPlaylist->GetReprNumber() -1)
				n += sprintf(buffer + n, "%d, ", bw);
			else
				n += sprintf(buffer + n, "%d", bw);
		}
		
		jstring	bandwidth = (*env).NewStringUTF(buffer);
    
        return bandwidth;
    }
    else if (!strncasecmp(mKey, "DURATION", 8))
    {
	  	char buffer[16]; 
		sprintf(buffer, "%d", mPlaylist->GetTotalDuration());
		jstring	duration = (*env).NewStringUTF(buffer);
		
		return duration;
    }
}


//getMPDPlaylistInfo function
//passing the Playlist pointer and a info parameter (es SIZE..) will get the informations
JNIEXPORT jstring JNICALL 
Java_AndroidDashClient_com_InfoActivity_getMPDPlaylistInfo( JNIEnv* env, jobject thiz, jint resource, jstring key)
{
    MPDPlaylist* mPlaylist = (MPDPlaylist*) resource; 
    
    //get the requested information from the jstring key argument
    const char *mKey;
    mKey = env->GetStringUTFChars(key, NULL);
    
    //switch on the key type and get the proper information
    if (!strncasecmp(mKey, "N_REPS", 5))
    {
        char buffer[16]; 

		sprintf(buffer, "%d", get_mpd_representations_number (mPlaylist->GetMPDMainNode(), 0));
		jstring	n_reps = (*env).NewStringUTF(buffer);
		
		return n_reps;	
    }
    else if (!strncasecmp(mKey, "MIME_TYPE", 9))
    {
        char buffer[32]; 
		char *str = get_mpd_representation_mimetype (mPlaylist->GetMPDMainNode(), 0, 0);
        
        if (str != NULL)
			sprintf(buffer, "%s", str);
		else
			sprintf(buffer, "no_mime");
			
		jstring	mime = (*env).NewStringUTF(buffer);
		
		return mime;	
    }
    else if (!strncasecmp(mKey, "REPS_BANDS", 10))
    {
      	long int bw;
      	int n = 0;
      	int n_reps = get_mpd_representations_number (mPlaylist->GetMPDMainNode(), 0);
      	
      	char buffer[BUFF_SIZE]; 
      	
      	//reorder the representations
      	reorder_mpd_representations (mPlaylist->GetMPDMainNode(), 0);
      	
      	for (int i = 0; i < n_reps; i++)
      	{
      	    bw = get_mpd_representation_bandwidth (mPlaylist->GetMPDMainNode(), 0, i);
      	
      	    if (i != n_reps-1)
				n += sprintf(buffer + n, "%d, ", bw);
			else
				n += sprintf(buffer + n, "%d", bw);
		}
		
		jstring	bandwidth = (*env).NewStringUTF(buffer);
    
        return bandwidth;
    }
    else if (!strncasecmp(mKey, "REPS_HEIGHT", 11))
    {
      	int n = 0;
      	int n_reps = get_mpd_representations_number (mPlaylist->GetMPDMainNode(), 0);
      	
      	char buffer[BUFF_SIZE]; 
      	
      	for (int i = 0; i < n_reps; i++)
      	{
      	    int h = get_mpd_representation_height (mPlaylist->GetMPDMainNode(), 0, i);
      	
      	    if (i != n_reps-1)
				n += sprintf(buffer + n, "%d, ", h);
			else
				n += sprintf(buffer + n, "%d", h);
		}
		
		jstring	height = (*env).NewStringUTF(buffer);
    
        return height;
    }
    else if (!strncasecmp(mKey, "REPS_WIDTH", 10))
    {
      	int n = 0;
      	int n_reps = get_mpd_representations_number (mPlaylist->GetMPDMainNode(), 0);
      	
      	char buffer[BUFF_SIZE]; 
      	
      	for (int i = 0; i < n_reps; i++)
      	{
      	    int w = get_mpd_representation_width (mPlaylist->GetMPDMainNode(), 0, i);
      	
      	    if (i != n_reps-1)
				n += sprintf(buffer + n, "%d, ", w);
			else
				n += sprintf(buffer + n, "%d", w);
		}
		
		jstring	width = (*env).NewStringUTF(buffer);
    
        return width;
    }
    else if (!strncasecmp(mKey, "DURATION", 8))
    {
	  	char buffer[16]; 
	  	
	  	//choosing the first representation available
	  	int n_segm = get_mpd_segments_number (mPlaylist->GetMPDMainNode(), 0, 0);
	  	int segm_duration = get_mpd_representation_minBufferTime (mPlaylist->GetMPDMainNode(), 0, 0);
	  	
	  	//return the duration in seconds
	  	if (n_segm > 0 && segm_duration > 0)
			sprintf(buffer, "%d", ((n_segm * segm_duration)/ 1000));
		else
		    sprintf(buffer, "%d", -1);
		    
		jstring	duration = (*env).NewStringUTF(buffer);
		
		return duration;
    }
    
}

#ifdef __cplusplus
}
#endif
