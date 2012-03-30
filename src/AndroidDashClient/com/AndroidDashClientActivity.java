package AndroidDashClient.com;

import java.util.ArrayList;
import java.util.HashMap;
//import java.util.List;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

//import android.R.bool;
import android.app.ListActivity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.os.CountDownTimer;
//import android.os.Debug;
import android.util.Log;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewConfiguration;
import android.widget.AdapterView;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;


public class AndroidDashClientActivity extends ListActivity {

	private static final String TAG = "AndroidDashClientActivity";
	private NodeList nodes;
	private SourceDbAdapter dbHelper;
	private Cursor cursor;
	private CountDownTimer mCountdownTillNextEvent;
	private boolean mTimerRunning = false;
	private boolean mTakeEvent=false;

	// Handle xml
	private ArrayList<HashMap<String, String>> mylist = new ArrayList<HashMap<String, String>>();

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Debug.startMethodTracing("/mnt/sdcard/AndroidDashClientActivity");

		setContentView(R.layout.listplaceholder);

		String xml = XMLfunctions.getXML(this.getAssets(), "sourcelist.xml");
		Document doc = XMLfunctions.XMLfromString(xml);

		int numResults = XMLfunctions.numResults(doc);

		if ((numResults <= 0)) {
			Toast.makeText(AndroidDashClientActivity.this,
					"List" + "sourcelist.xml" + "is empty!!", Toast.LENGTH_LONG)
					.show();
			finish();
		}

		nodes = doc.getElementsByTagName("result");

		for (int i = 0; i < nodes.getLength(); i++) {
			HashMap<String, String> map = new HashMap<String, String>();

			Element e = (Element) nodes.item(i);
			// map.put("id", XMLfunctions.getValue(e, "id"));
			map.put("id", Integer.toString(-1));
			map.put("Fileurl", "Url: " + XMLfunctions.getValue(e, "fileurl"));
			map.put("Description",
					"Description: " + XMLfunctions.getValue(e, "description"));
			map.put("Type", "Type: " + XMLfunctions.getValue(e, "type"));
			
			mylist.add(map);
		}

		// Add items to the list from database
		dbHelper = new SourceDbAdapter(this);
		dbHelper.open();
		cursor = dbHelper.fetchAllSources();
		startManagingCursor(cursor);
		Toast.makeText(this, "Stabase size is " + cursor.getCount(),
				Toast.LENGTH_SHORT).show();

		displayListItems();
		dbHelper.close();

		ListAdapter adapter = new SimpleAdapter(this, mylist, R.layout.main,
				new String[] { "Fileurl", "Description" }, new int[] {
						R.id.item_title, R.id.item_subtitle, R.id.item_subtitle });

		setListAdapter(adapter);
		registerForContextMenu(getListView());
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.option_menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
/*		Toast.makeText(this, "You have chosen the " + item.getTitle(),
				Toast.LENGTH_SHORT).show();*/

		int menuItemIndex = item.getItemId();
		switch (menuItemIndex) {
		case R.id.new_link:
			Toast.makeText(this, "You have chosen the " + item.getTitle(),
					Toast.LENGTH_SHORT).show();
			Intent mIntent = new Intent(AndroidDashClientActivity.this,
					SourceAdderActivity.class);
			startActivityForResult(mIntent, 1);
			return true;
		case R.id.clean_cache:
			Toast.makeText(this, "You have chosen the " + item.getTitle(),
					Toast.LENGTH_SHORT).show();
			// Remove all items from database..
			cleanAllItemsFromDb();
			// TODO : add routine which remove all diplayed items form
			// database..and refresh listitem diplay

			return true;
		default:
			return super.onContextItemSelected(item);
		}
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
	 // TODO Auto-generated method stub
        if(mTakeEvent)
        {
        	Log.i("onTick", "Lupo");
            String selection = l.getItemAtPosition(position).toString();
    	    Toast.makeText(this, selection, Toast.LENGTH_LONG).show(); 
    	    playAction(position);
    		mCountdownTillNextEvent.cancel();
    		mCountdownTillNextEvent = null;
            mTimerRunning=false;
            mTakeEvent=false; 
    		return;
        }

		if (!mTimerRunning){
            mTimerRunning=true;  //signaling that timer is running
            mTakeEvent=true;
            mCountdownTillNextEvent = new CountDownTimer(ViewConfiguration.getDoubleTapTimeout(), 1) //default time in milliseconds between single and double tap (see: repo of ViewConfiguration)
            {
                 @Override
            public void onTick(long millisUntilFinished) {
               // Log.i("onTick", "Entry: "+ millisUntilFinished);
            }
             @Override
            public void onFinish() {   //when time expires reverting to non-clicked state
                 mTimerRunning=false;
                 mTakeEvent=false; 
                 Log.i("onTick", "Ciccia");
                //after this point, means that single tap occured. Do something 
            }
		    };
		    mCountdownTillNextEvent.start();  //firing the countdown
		}
		

	}
	
	@Override
	public void onCreateContextMenu(ContextMenu menu, View view,
			ContextMenu.ContextMenuInfo menuInfo) {
		Log.v(TAG, "Creating context menu for view=" + view);
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.context_menu, menu);
		super.onCreateContextMenu(menu, view, menuInfo);
	}

	
	private void playAction(int position){
		HashMap<String, String> map = new HashMap<String, String>();
		Intent mIntent;
		boolean status = false;
		
		map = mylist.get(position);
		
		
		status = isHTMLFormatLink(position, Long.parseLong(map.get("id")));
		
		if (status)
			mIntent = new Intent(AndroidDashClientActivity.this,
					TestHTML5WebView.class);				
		else 
			mIntent = new Intent(AndroidDashClientActivity.this,
				PlayerActivity.class);

		// Retrieve file info from local database or internal xml
		if (Integer.parseInt(map.get("id")) < 0) {
			getInfoFromXML(position, mIntent);
		} else {
			// Retrieve info from database
			getInfoFromDb(Long.parseLong(map.get("id")), mIntent);
		}

		startActivity(mIntent);
	}

	@Override
	public boolean onContextItemSelected(MenuItem item) {

		AdapterView.AdapterContextMenuInfo info = (AdapterView.AdapterContextMenuInfo) item
				.getMenuInfo();
		int menuItemIndex = item.getItemId();
		boolean status = false;
		HashMap<String, String> map = new HashMap<String, String>();

		switch (menuItemIndex) {
		case R.id.play_loop:
		case R.id.play:
			// get item Id
			Intent mIntent;
			
			map = mylist.get(info.position);
			
			
			status = isHTMLFormatLink(info.position, Long.parseLong(map.get("id")));
			
			if (status)
				mIntent = new Intent(AndroidDashClientActivity.this,
						TestHTML5WebView.class);				
			else 
				mIntent = new Intent(AndroidDashClientActivity.this,
					PlayerActivity.class);

			// Retrieve file info from local database or internal xml
			if (Integer.parseInt(map.get("id")) < 0) {
				getInfoFromXML(info.position, mIntent);
			} else {
				// Retrieve info from database
				getInfoFromDb(Long.parseLong(map.get("id")), mIntent);
			}
			
			if (menuItemIndex == R.id.play_loop)
				mIntent.putExtra("looper", true);
			
			startActivity(mIntent);

			return true;
		case R.id.delete:
			Toast.makeText(
					this,
					"You have chosen the "
							+ getResources().getString(R.string.delete)
							+ " context menu option for ", Toast.LENGTH_SHORT)
					.show();

			// Update database
			map = mylist.get(info.position);
			String row_id = map.get("id");
			Log.v(TAG,
					"Row id deleted is" + row_id + " "
							+ Integer.parseInt(row_id));
			removeFromDb(Integer.parseInt(row_id));

			// Remove Item from list
			mylist.remove(info.position);
			ListAdapter adapter = new SimpleAdapter(this, mylist,
					R.layout.main, new String[] { "Fileurl", "Description" },
					new int[] { R.id.item_title, R.id.item_subtitle });

			setListAdapter(adapter);
			registerForContextMenu(getListView());

			return true;
		case R.id.viewinfo:

			Intent info_Intent = new Intent(AndroidDashClientActivity.this,
					InfoActivity.class);
			map = mylist.get(info.position);
			
			if ( isHTMLFormatLink(info.position, Long.parseLong(map.get("id"))) ){
				Toast.makeText(
						this,
						"Opps!! No Video info are available. It is an HTML link " , Toast.LENGTH_SHORT)
						.show();
				return true;
			}
			
			if (Integer.parseInt(map.get("id")) < 0) {
				Log.v(TAG, "Manuele XML : " + Integer.parseInt(map.get("id")));
				getInfoFromXML(info.position, info_Intent);
			} else {
				// Retrieve info from database
				getInfoFromDb(Long.parseLong(map.get("id")), info_Intent);
			}

			startActivity(info_Intent);

			return true;
		default:
			return super.onContextItemSelected(item);
		}
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		Log.v(TAG, "Return from activity!!!..check for db updates");

		if (resultCode == RESULT_OK && requestCode == 1) {
			String uri = data.getStringExtra("return_uri");
			String description = data.getStringExtra("return_description");
			String format = data.getStringExtra("return_format");
			int rowId = data.getIntExtra("return_rowId", 0);
			addListItem(uri, description, Integer.toString(rowId), format);
		}
	}


    
	@Override
	public void onResume() {
		super.onResume();
		ListAdapter adapter = new SimpleAdapter(this, mylist, R.layout.main,
				new String[] { "Fileurl", "Description" }, new int[] {
						R.id.item_title, R.id.item_subtitle });

		setListAdapter(adapter);
		registerForContextMenu(getListView());
	}

	private void removeFromDb(int rowId) {
		Log.v(TAG, "Remove RowFromDatabase" + rowId);
		dbHelper.open();
		dbHelper.deleteSource(rowId);
		dbHelper.close();
	}

	private void getInfoFromDb(long rowId, Intent mIntent) {
		Cursor item;
		Integer media = R.string.stream_video;

		dbHelper.open();
		item = dbHelper.fetchSource(rowId);

		if (item.getString(
				cursor.getColumnIndexOrThrow(SourceDbAdapter.KEY_MEDIA))
				.contentEquals("Local"))
			media = R.string.local_video;

		mIntent.putExtra("media", media);
		mIntent.putExtra("type", item.getString(cursor
				.getColumnIndexOrThrow(SourceDbAdapter.KEY_FORMAT)));
		mIntent.putExtra("sourcepath", item.getString(cursor
				.getColumnIndexOrThrow(SourceDbAdapter.KEY_FILEURL)));
		dbHelper.close();
		item.close();
	}

	private void cleanAllItemsFromDb() {
		Cursor cursor;

		dbHelper.open();
		cursor = dbHelper.fetchAllSources();
		startManagingCursor(cursor);

		if (cursor.moveToFirst()) {
			do {
				dbHelper.deleteSource(cursor.getLong(cursor
						.getColumnIndexOrThrow(SourceDbAdapter.KEY_ROWID)));
			} while (cursor.moveToNext());
		}
		if (cursor != null && !cursor.isClosed()) {
			cursor.close();
		}

		dbHelper.close();
	}

	private boolean isHTMLFormatLink(int position, long listId){
		boolean status = false;
		
		if (listId < 0){
			Element e = (Element) nodes.item(position);
			if (XMLfunctions.getValue(e, "media").contentEquals("Html"))
				status =  true;
		}else{
			Cursor item;

			dbHelper.open();
			item = dbHelper.fetchSource(listId);

			if (item.getString(
					cursor.getColumnIndexOrThrow(SourceDbAdapter.KEY_MEDIA))
					.contentEquals("Html"))
				status = true;
			
			dbHelper.close();
			item.close();
		}
		
		return status;
		
	}
	
	private void getInfoFromXML(int position, Intent mIntent) {
		Element e = (Element) nodes.item(position);
		Integer media = R.string.stream_video;

		if (XMLfunctions.getValue(e, "media").contentEquals("Local"))
			media = R.string.local_video;

		mIntent.putExtra("media", media);
		mIntent.putExtra("sourcepath", XMLfunctions.getValue(e, "fileurl"));
		mIntent.putExtra("type", XMLfunctions.getValue(e, "type"));
	}

	private void displayListItems() {
		Cursor cursor;

		cursor = dbHelper.fetchAllSources();
		startManagingCursor(cursor);

		if (cursor.moveToFirst()) {
			do {
				addListItem(
						cursor.getString(cursor
								.getColumnIndexOrThrow(SourceDbAdapter.KEY_FILEURL)),
						cursor.getString(cursor
								.getColumnIndexOrThrow(SourceDbAdapter.KEY_DESCRIPTION)),
						cursor.getString(cursor
								.getColumnIndexOrThrow(SourceDbAdapter.KEY_ROWID)),
								cursor.getString(cursor
										.getColumnIndexOrThrow(SourceDbAdapter.KEY_FORMAT)));
			} while (cursor.moveToNext());
		}
		if (cursor != null && !cursor.isClosed()) {
			cursor.close();
		}
	}

	private void addListItem(String uri, String description, String rowId, String type) {
		HashMap<String, String> map = new HashMap<String, String>();

		map.put("id", rowId);
		map.put("Fileurl", "Url: " + uri);
		map.put("Description", "Description: " + description);
		map.put("Type", "Type: " + type);
		mylist.add(map);
	}
}
