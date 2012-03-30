package AndroidDashClient.com;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Intent;
import android.content.res.Resources;
//import android.graphics.Color;

import android.media.MediaPlayer;
import android.media.MediaPlayer.OnBufferingUpdateListener;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.TrafficStats;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
//import android.os.Debug;
//import android.text.Html;
import android.text.Html;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

public class PlayerActivity extends Activity implements
		OnBufferingUpdateListener, OnCompletionListener, OnPreparedListener {

	private static final String TAG = "PlayerActivity";
	public static final String sourcepath = "";
	private VideoView mPreview;
	private String path;
	private Bundle extras;
	private Animation animShow, animHide;
	/*
	 * private Thread textViewThread; protected boolean _active = false; private
	 * int waited = 0; protected int _updateTime = 40000;
	 */
	protected boolean _active = false;
	protected TextView tv;
	/* protected TextView tv1; */
	private Handler mHandler = new Handler();
	private Timer time;
	private Resources r;

	// private long lastRxBytes = 0;

	/**
	 * 
	 * Called when the activity is first created.
	 */
	@Override
	public void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.mediaplayer_2);
		r = this.getResources();

		animShow = AnimationUtils.loadAnimation(this, R.anim.popup_show);
		animShow.setRepeatMode(Animation.RESTART);
		animHide = AnimationUtils.loadAnimation(this, R.anim.popup_hide);
		animHide.setRepeatMode(Animation.RESTART);

		mPreview = (VideoView) findViewById(R.id.videoPlayer);
		mPreview.setOnTouchListener(new View.OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {

				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					if (_active == true) {
						// System.out.println("active is true ");
						// startService(new Intent(this,
						// DashSystemService.class));
						animShow.reset();
						tv.startAnimation(animHide);
						// lastRxBytes = 0;
						doCleanUp();
						tv.setVisibility(View.GONE);
						// tv.destroyDrawingCache();
					} else {

						// lastRxBytes = TrafficStats.getTotalRxBytes();
						// stopService(new Intent(this,
						// DashSystemService.class));
						_active = true;
						time = new Timer();
						animHide.reset();
						time.scheduleAtFixedRate(new TestTask(r), 0, 1000);
						// System.out.println("active is false ");

					}
				}

				return true;
			}
		});

		this.registerForContextMenu(mPreview);
		extras = getIntent().getExtras();

		// get extras
		path = extras.getString("sourcepath");

		tv = (TextView) findViewById(R.id.textPlayer);
		tv.setLineSpacing(0, (float) 1.0);
		// tv.setBackgroundDrawable(r.getDrawable(R.layout.layout_gradient));
		tv.setVisibility(View.GONE);

		// System.setProperty("dashbw", "2000");
		// System.setProperty("dashreprbw", "1000")
		// SystemServer.addService( "Pippo", new DashSystemService());

		playVideo(extras.getInt("media"));
	}

	private void playVideo(Integer Media) {

		try {

			switch (Media) {
			case R.string.local_video:
				mPreview.setVideoPath(path);

				if (path == "") {
					// Tell the user to provide a media file URL.
					Toast.makeText(
							PlayerActivity.this,
							"Please edit MediaPlayerDemo_Video Activity, "
									+ "and set the path variable to your media file path."
									+ " Your media file must be stored on sdcard.",
							Toast.LENGTH_LONG).show();

				}
				break;
			case R.string.stream_video:
				mPreview.setVideoURI(Uri.parse(path));
				if (path == "") {
					// Tell the user to provide a media file URL.
					/*
					 * Toast.makeText( PlayerActivity.this,
					 * "Please edit MediaPlayerDemo_Video Activity," +
					 * " and set the path variable to your media file URL.",
					 * Toast.LENGTH_LONG).show();
					 */

				}

				break;

			}

			/*
			 * Toast.makeText(PlayerActivity.this, "Create Player : " + path,
			 * Toast.LENGTH_LONG).show();
			 */

			mPreview.setOnCompletionListener(this);
			mPreview.setOnPreparedListener(this);
			// mPreview.setOnClickListener(this);
			mPreview.setKeepScreenOn(true);

		} catch (Exception e) {
			Log.e(TAG, "error: " + e.getMessage(), e);
		}
	}

	public void onBufferingUpdate(MediaPlayer arg0, int percent) {
		Log.d(TAG, "onBufferingUpdate percent:" + percent);

	}

	public void onCompletion(MediaPlayer arg0) {
		Log.d(TAG, "onCompletion called");
		if (extras.getBoolean("looper")) {
			Log.d("STATISTICS", " 112345  123  254 \n");
			playVideo(extras.getInt("media"));
		} else {
			doCleanUp();
			this.finish();
		}
	}

	public void onPrepared(MediaPlayer mediaplayer) {
		Log.d(TAG, "onPrepared called");
		// TEst to handle seek settings : if(mPreview.canSeekForward())
		// mPreview.seekTo(mPreview.getDuration()/2);
		startVideoPlayback();
	}

	@Override
	protected void onPause() {
		super.onPause();
		releaseMediaPlayer();
		doCleanUp();
	}

	@Override
	protected void onDestroy() {
		Log.d(TAG, "onDestroy called");
		super.onDestroy();
		releaseMediaPlayer();
		doCleanUp();
	}

	private void releaseMediaPlayer() {

		if (mPreview != null) {
			mPreview = null;
		}
	}

	private void doCleanUp() {
		if (_active == true) {
			time.purge();
			time.cancel();
			_active = false;
		}
	}

	private void startVideoPlayback() {
		Log.v(TAG, "startVideoPlayback");
		mPreview.start();
	}

	class TestTask extends TimerTask {
		public TestTask(Resources r) {
			// tv.setBackgroundColor(Color.BLUE);
			Log.v(TAG, "TestTask");
			// tv.setVisibility(visibility);
			tv.clearAnimation();
			tv.setVisibility(View.VISIBLE);
			tv.startAnimation(animShow);
			// tv.startAnimation(animation)
			// tv.setVisibility(View.VISIBLE);
			// tv1.setBackgroundDrawable(r.getDrawable(R.layout.layout_gradient));
		}

		public void run() {
			mHandler.post(new Runnable() {
				public void run() {
					// System.out.println("time = " +
					// System.currentTimeMillis());
					long RxBytes = TrafficStats.getTotalRxBytes();
					long bw = RxBytes; // - lastRxBytes;

					// String value = System.getProperty("dashbandwith");
					// String bw = System.getenv("dashbw");
					// String representationbw = System.getenv("dashreprbw");

					if (_active) {
						// tv.setText("time = " + System.currentTimeMillis());

						/*
						 * tv.setText(Html.fromHtml("<b>  <u>Source Format</u> :"
						 * + "DASH <br />  " + "<b><u> Bandwith</u>:  " +
						 * RxBytes + "<br />" + "<b><u> Throupouth</u>:  " +
						 * bw));
						 */
						tv.setText(Html.fromHtml("<b>  <u>Source Format</u> :"
								+ "DASH <br />  "
								+ "<b><u> Throupouth Downlink</u>:  " + "TBD binding"));
						tv.setLineSpacing(0, (float) 1.0);
						tv.setPadding(30, 10, 10, 10);
					}
					// tv.setDrawingCacheEnabled(true);
					// lastRxBytes = RxBytes;
					/*
					 * tv1.setText(Html.fromHtml( "<b><u> Bandwith</u>:  "+
					 * System.currentTimeMillis() + "<br />" +
					 * "<b><u> Throupouth</u>:  " +
					 * System.currentTimeMillis())); //tv.setLineSpacing(0,
					 * (float) 1.0); tv1.setPadding(30, 270, 10, 10);
					 * tv1.setDrawingCacheEnabled(true);
					 */
				}
			});
		}
	}

}
