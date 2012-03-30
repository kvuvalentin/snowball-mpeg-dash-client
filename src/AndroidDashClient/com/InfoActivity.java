package AndroidDashClient.com;

import java.io.File;
import java.net.URI;
//import java.util.ArrayList;
import java.util.HashMap;

import android.app.Activity;
import android.os.Bundle;
import android.text.Html;
import android.util.Log;
import android.widget.TextView;

public class InfoActivity extends Activity {

	private static final String TAG = "InfoActivity";
	private Bundle extras;
	private String path;
	private String type;
	private int location;
	private int playlistPtr = 0;

	/**
	 * Called when the activity is first created.
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.info_layout);

		extras = getIntent().getExtras();
		path = extras.getString("sourcepath");
		location = extras.getInt("media");
		type = extras.getString("type");
		
		// structure useful to contain resource informations
		HashMap<String, String> resInfo = new HashMap<String, String>();

		// get media informations depending on the file type
		if (!getResourceInfo(resInfo))
			resInfo.put("error", "No information on this resource!");

		// show media informations depending on the file type
		showResourceInfo(resInfo);
	}

	private boolean getResourceInfo(HashMap<String, String> map) {
		// switch on file location (local or stream file)
		
		//LOCAL
		if (location == R.string.local_video) {

			map.put("location", "local");

			// switch on file type - Type MP4
			if (type.compareToIgnoreCase("MP4") == 0) {

				// get filename and size
				try {
					URI fileURI = new URI(path);
					File f = new File(fileURI.getPath());

					map.put("filename", f.getName());
					map.put("filesize", Long.toString(f.length()));
				} catch (Exception e) {
					Log.e(TAG, "error: " + e.getMessage(), e);
					return false;
				}

			}

			else
				return false;
			
		//REMOTE	
		} else if (location == R.string.stream_video) {
			map.put("location", "remote");

			// switch on the playlist type
			if (path.endsWith("m3u8")) {
				try{		
					String pathAux = path;
				
					if (pathAux.startsWith("httplive") || pathAux.startsWith("httpdash") ) {
						pathAux = pathAux.substring(8);
						pathAux = "http" + pathAux;
					}
				
					URI fileURI = new URI(pathAux);
					File f = new File(fileURI.getPath());
					map.put("filename", f.getName());

					playlistPtr = setPlaylist(path, "m3u8");

					map.put("reprNumber", getM3U8PlaylistInfo(playlistPtr, "N_REPS"));
					map.put("reprBandwidths", getM3U8PlaylistInfo(playlistPtr, "REPS_BANDS"));
					map.put("totalDuration", getM3U8PlaylistInfo(playlistPtr, "DURATION"));

					//free JNI objects
					freeM3U8Playlist(playlistPtr);
				}
				catch (Exception e) {
					Log.e(TAG, "error: " + e.getMessage(), e);
					return false;
				}
			}
			else if (path.endsWith("mpd")) {
				try{		
					String pathAux = path;
				
					if (pathAux.startsWith("httplive")) {
						pathAux = pathAux.substring(8);
						pathAux = "http" + pathAux;
					}
				
					URI fileURI = new URI(pathAux);
					File f = new File(fileURI.getPath());
					map.put("filename", f.getName());

					playlistPtr = setPlaylist(path, "mpd");

					map.put("reprNumber", getMPDPlaylistInfo(playlistPtr, "N_REPS"));
					map.put("mimeType", getMPDPlaylistInfo(playlistPtr, "MIME_TYPE"));
					map.put("reprBandwidths", getMPDPlaylistInfo(playlistPtr, "REPS_BANDS"));
					map.put("reprHeights", getMPDPlaylistInfo(playlistPtr, "REPS_HEIGHT"));
					map.put("reprWidths", getMPDPlaylistInfo(playlistPtr, "REPS_WIDTH"));
					map.put("totalDuration", getMPDPlaylistInfo(playlistPtr, "DURATION"));

					//free JNI objects
					freeMPDPlaylist(playlistPtr);
				}
				catch (Exception e) {
					Log.e(TAG, "error: " + e.getMessage(), e);
					return false;
				}
			}
					

			else
				return false;

		}

		return true;
	}

	private void showResourceInfo(HashMap<String, String> map) {
		// check if there's an error message and show it
		if (map.containsKey("error")) {

			TextView t2 = (TextView) findViewById(R.id.general_info);
			t2.setText(map.get("error"));
			return;
		}
		// show general informations first
		TextView t1 = (TextView) findViewById(R.id.general_info_title);
		t1.setText("General informations:");
		t1.setPadding(30, 30, 10, 10);

		TextView t2 = (TextView) findViewById(R.id.general_info);
		t2.setText(Html.fromHtml("<b><u> File name</u>: </b>  "
				+ map.get("filename") + "<br />"
				+ "<b><u> File type</u>: </b>  " + map.get("location") + " "
				+ type + " "));

		if (map.containsKey("filesize")
				&& (Long.parseLong(map.get("filesize")) > 0))
			t2.append(Html.fromHtml("<br />" + "<b><u> File size</u>: </b>  "
					+ map.get("filesize") + " byte"));

		t2.setLineSpacing(0, (float) 1.5);
		t2.setPadding(30, 10, 10, 10);

		
		
		// now show specific informations depending on the source type

		// Type M3U8
		if (path.endsWith("m3u8"))  {
			TextView t3 = (TextView) findViewById(R.id.spec_info_title);
			t3.setText("M3U8 playlist informations:");
			t3.setPadding(30, 30, 10, 10);

			TextView t4 = (TextView) findViewById(R.id.spec_info);
			if (map.containsKey("totalDuration")
					&& (Integer.parseInt(map.get("totalDuration")) > 0)) {
				int duration = Integer.parseInt(map.get("totalDuration"));
				int hh = (int) duration / 3600;
				int mm = (int) (duration - hh * 3600) / 60;
				int ss = (int) duration % 60;

				t4.setText(Html.fromHtml("<b><u> Duration </u>: </b> " + " "
						+ Integer.toString(hh) + "h " + Integer.toString(mm)
						+ "m " + Integer.toString(ss) + "s "));
			}
			else
				t4.setText(Html.fromHtml("No informations on stream duration!"));
			
			if (map.containsKey("reprNumber")) {
				String bw = map.get("reprBandwidths");
				String[] bandwidths = bw.split(",");

				if (Integer.parseInt(map.get("reprNumber")) != -1) {
					if (Integer.parseInt(map.get("reprNumber")) > 1) {
						t4.append(Html.fromHtml("<br />"
								+ "<b><u> Representations </u>: </b> "
								+ " "
								+ map.get("reprNumber")
								+ "<br />"
								+ "<b>\t\tfrom</u>:</b>\t  "
								+ bandwidths[0]
							    + " b/s"
							    + "<br />"
							    + "<b>\t\tto</u>:   </b>\t\t  "
							    + bandwidths[Integer.parseInt(map.get("reprNumber")) - 1]
							    + " b/s"));
					} else {
						t4.append(Html.fromHtml("<br />"
								+ "<b><u> Representations</u>: </b>  "
								+ map.get("reprNumber") + "  at  " + bandwidths[0]
								+ " b/s"));
					}
				}
			}
			else
				t4.append(Html.fromHtml("No informations on representations!"));
			
			t4.setLineSpacing(0, (float) 1.5);
			t4.setPadding(30, 10, 10, 10);
		}
		
		// Type MPD
		if (path.endsWith("mpd"))  {
			TextView t3 = (TextView) findViewById(R.id.spec_info_title);
			t3.setText("MPD playlist informations:");
			t3.setPadding(30, 30, 10, 10);
			
			TextView t4 = (TextView) findViewById(R.id.spec_info);
			
			if (map.containsKey("totalDuration")
					&& (Integer.parseInt(map.get("totalDuration")) > 0)) {
				int duration = Integer.parseInt(map.get("totalDuration"));
				int hh = (int) duration / 3600;
				int mm = (int) (duration - hh * 3600) / 60;
				int ss = (int) duration % 60;

				t4.setText(Html.fromHtml("<b><u> Duration </u>: </b> " + " "
						+ Integer.toString(hh) + "h " + Integer.toString(mm)
						+ "m " + Integer.toString(ss) + "s "));
			}
			else
				t4.setText(Html.fromHtml("No informations on stream duration!"));
			
			if (map.containsKey("mimeType")) {
				String mime = map.get("mimeType");
				
				if (mime.equalsIgnoreCase("no_mime") == false) {
				
				t4.append(Html.fromHtml("<br />"
						+ "<b><u> File format </u>: </b> "
						+ " "
						+ mime));
				}
			}
			
			if (map.containsKey("reprNumber")) {
				String bw = map.get("reprBandwidths");
				String[] bandwidths = bw.split(", ");
				String[] Heights, Widths;
				
				if (map.containsKey("reprHeights"))
					Heights = map.get("reprHeights").split(", ");
				else
					Heights = null;
					
				if (map.containsKey("reprWidths"))
					Widths = map.get("reprWidths").split(", ");
				else
					Widths = null;
				
				if (Integer.parseInt(map.get("reprNumber")) != -1) {
					
					t4.append(Html.fromHtml("<br />"
							+ "<b><u> Representations </u>: </b> "
							+ " "
							+ map.get("reprNumber")));
					
					//for each representation
					for (int i = 0; i < Integer.parseInt(map.get("reprNumber")); i++) {
						
						t4.append(Html.fromHtml("<br />"
								+ "<b>\t\tat</b>  "
								+ bandwidths[i]
								+ " b/s"
						));
						
						//the check is done only on one component
						//suppose that height and width come in pairs
						int h;
						
						if (Heights != null) {
						    h = Integer.parseInt(Heights[i].toString());

						    if (h != -1){
						    	t4.append(Html.fromHtml("\t["
						    			+ Widths[i]
						    			+ " x "
						    			+ Heights[i]
						    			+ "] px" ));
						    }     	
						}
					}					
				}
			}
			else
				t4.append(Html.fromHtml("No informations on representations!"));
			
			t4.setLineSpacing(0, (float) 1.5);
			t4.setPadding(30, 10, 10, 10);
			
		}

	}

	// native functions prototypes
	public native int setPlaylist(String name, String type);

	public native String getM3U8PlaylistInfo(int resource, String key);
	public native String getMPDPlaylistInfo(int resource, String key);

	public native int freeM3U8Playlist(int resource);
	public native int freeMPDPlaylist(int resource);

	// Load JNI lib
	static {
		System.loadLibrary("playlisthandler-jni");
	}

}