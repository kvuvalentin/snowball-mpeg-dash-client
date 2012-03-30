package AndroidDashClient.com;



import android.app.Activity;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnBufferingUpdateListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;




public class MediaPlayerActivity extends Activity implements OnErrorListener, 
														  OnPreparedListener,
														  OnCompletionListener,
														  OnBufferingUpdateListener,
														  SurfaceHolder.Callback{

	private static final String TAG = "MediaPlayerActivity";
    private String path;
    private Bundle extras;
    
    private MediaPlayer mediaPlayer = new MediaPlayer();
    private SurfaceView surfaceView;
    private SurfaceHolder holder;

    
    
    
    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        setContentView(R.layout.mediaplayerlayout);

        // get extras 
        extras = getIntent().getExtras();
        path = extras.getString("sourcepath");
        
        
        //configure surface for video reproduction
        surfaceView = (SurfaceView) findViewById(R.id.surface);
        holder = surfaceView.getHolder();
        holder.addCallback(this);
        holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);      
    }
    
    
    private void configurePlayback() {
    	try {
    	  
    	    // Set the listeners
    		mediaPlayer.setOnErrorListener(this);
    		mediaPlayer.setOnPreparedListener(this);
    		mediaPlayer.setOnCompletionListener(this);
    	
    		
    		// Set the media display and data source
    		mediaPlayer.setDisplay(holder);
    		mediaPlayer.setDataSource(this, Uri.parse(path));
    		mediaPlayer.setScreenOnWhilePlaying(true);	
    		
    		
    		//prepare the video asynchronously and then start reproducing
    		mediaPlayer.prepareAsync();    
    		
    	}
    	
    	catch (Exception e){
    		Log.e(TAG, "error: " + e.getMessage(), e);
    		if (mediaPlayer != null){
    			mediaPlayer.stop();
    			mediaPlayer.release();
    			mediaPlayer = null;
    		}
    	}
    }


	public boolean onError(MediaPlayer mp, int what, int extra) {
		
		//Log the error info
		Log.e(TAG, "onError--->   what:" + what + "    extra:" + extra);
		
		//Release the media player resources
		if (mp != null){
			mp.stop();
			mp.release();
			mp = null;
		}
		
		return false;

	}


	public void onPrepared(MediaPlayer mp) {
		Log.d(TAG, "onPrepared called");
		
		//Get the dimensions of the video
	    int videoHeight = mp.getVideoHeight();
	    int videoWidth = mp.getVideoWidth();
	    
	    Log.v(TAG, "Width, Heigth = " + videoWidth + ", " + videoHeight);

	    //Get the width and heighth of the screen
	    int screenWidth = getWindowManager().getDefaultDisplay().getWidth();
	    int screenHeight = getWindowManager().getDefaultDisplay().getHeight();
	    
	    Log.v(TAG, "ScreenWidth = " + screenWidth);
	    
	    //Get the SurfaceView layout parameters
	    android.view.ViewGroup.LayoutParams lp = surfaceView.getLayoutParams();
	    
	    
	    //Set the dimensions of the SurfaceView according with the width of the screen
	    if (videoHeight > screenHeight && videoWidth <= screenWidth) {
	    	lp.height = screenHeight;
	    	lp.width = (int)(((float)videoWidth)*((float)screenHeight/(float)videoHeight)); 
	    }
	    else if (videoHeight <= screenHeight && videoWidth > screenWidth) {
	    	lp.width = screenWidth;
	    	lp.height = (int)(((float)videoHeight)*((float)screenWidth/(float)videoWidth));
	    }
	    else if (videoHeight > screenHeight && videoWidth > screenWidth) { 
	    	if (((float)videoHeight/(float)screenHeight) > ((float)videoWidth/(float)screenWidth)) {
	    		lp.height = screenHeight;
		   		lp.width = (int)(((float)videoWidth)*((float)screenHeight/(float)videoHeight));
	    	}
	    	else {
	    		lp.width = screenWidth;
		   		lp.height = (int)(((float)videoHeight)*((float)screenWidth/(float)videoWidth));
	    	}	    		    				    			    			
	    }
	    else {
	    	if (((float)screenHeight/(float)videoHeight) < ((float)screenWidth/(float)videoWidth)) {
	    		lp.height = screenHeight;
		   		lp.width = (int)(((float)videoWidth)*((float)screenHeight/(float)videoHeight));
	    	}
	    	else {
	    		lp.width = screenWidth;
		   		lp.height = (int)(((float)videoHeight)*((float)screenWidth/(float)videoWidth));
	    	}
	    }
	    
	    
	    //Commit the layout parameters
	    surfaceView.setLayoutParams(lp);  
		
		mp.start();		
	}


	public void onCompletion(MediaPlayer mp) {
		Log.d(TAG, "onCompletion called");
		this.finish();
		
	}
	
	
	public void onStop(MediaPlayer mp) {
		Log.d(TAG, "onStop called");
		if (mp != null){
		    mp.stop();
		    mp.release();
		    mp = null;
		}
		this.finish();
	}

	@Override
	protected void onPause() {
		super.onPause();
		mediaPlayer.stop();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		mediaPlayer.stop();
		mediaPlayer.release();
		// Debug.stopMethodTracing();
	}

	public void onBufferingUpdate(MediaPlayer mp, int percent) {
		Log.d(TAG, "onBufferingUpdate called --->   percent:" + percent);
		
	}
	
	
	public void surfaceCreated(SurfaceHolder surfaceholder) {
		Log.d(TAG, "surfaceCreated called");
		
		//configure mediaPlayer and start playback
		configurePlayback();
	}


	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		Log.d(TAG, "surfaceChanged called");
		
	}


	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.d(TAG, "surfaceDestroyed called");
		
	}
	
	

    
}
