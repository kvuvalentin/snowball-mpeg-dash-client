package AndroidDashClient.com;

import android.app.Activity;
import android.content.Intent;
//import android.net.Uri;
import android.os.Bundle;
import android.view.MotionEvent;
//import android.widget.Toast;

public class SplashScreen extends Activity {
	
	protected boolean _active = true;
	protected int _splashTime = 80000;
	
	/** Called when first activity will be created */
	@Override
	public void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		setContentView(R.layout.splash);
		
		// Thread for displaying splash screen
		Thread splashThread = new Thread(){
			@Override
			public void run(){
				try {
					int waited = 0;
					while(_active && (waited<_splashTime)) {
						sleep(1);
						if (_active)
							waited += 100;
					}
				} catch (InterruptedException e) {
					//do nothing
				} finally {
					finish();
					startActivity(new Intent("android.stm.splash.AndroidDashClientActivity"));
					stop();
				}
			}
		};
		splashThread.start();
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event){
		if (event.getAction() == MotionEvent.ACTION_DOWN){
			_active = false;
		}
		return true;
	}
}
