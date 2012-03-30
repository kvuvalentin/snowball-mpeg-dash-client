package AndroidDashClient.com;

import android.app.Activity;
import android.os.Bundle;
//import android.util.Log;
import android.view.KeyEvent;

public class TestHTML5WebView extends Activity {
	
	//private static final String TAG = "TestHTML5WebView";
	HTML5WebView mWebView;
	public String uri = "";
	private Bundle extras;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mWebView = new HTML5WebView(this);
        extras = getIntent().getExtras();
        
        if (savedInstanceState != null) {
        	mWebView.restoreState(savedInstanceState);
        } else {	
        	//mWebView.loadUrl("http://192.168.1.1/myvideo.html");
        	uri = extras.getString("sourcepath");
        	//Log.v(TAG, "URI is : " + uri);
        	mWebView.loadUrl(uri);
        }
        
        setContentView(mWebView.getLayout());
    }
    
    @Override
    public void onSaveInstanceState(Bundle outState) {
    	super.onSaveInstanceState(outState);
    	mWebView.saveState(outState);
    }
    
    @Override
    public void onStop() {
    	super.onStop();
    	mWebView.stopLoading();
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (mWebView.inCustomView()) {
            	mWebView.hideCustomView();
            	return true;
            }
    	}
    	return super.onKeyDown(keyCode, event);
    }
}