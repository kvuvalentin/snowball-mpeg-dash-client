package AndroidDashClient.com;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;

public class SourceAdderActivity extends Activity {

	public static final String TAG = "SourceAdder";

	private EditText mURI;
	private EditText mDescription;
	private Button mLinkSave;
	private Spinner mMedia;
	private Spinner mFormat;

	/**
	 * Called when the activity is first created. Responsible for initializing
	 * the UI.
	 */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		Log.v(TAG, "Activity State: onCreate()");
		super.onCreate(savedInstanceState);
		setContentView(R.layout.bookmark_adder);

		// Obtain handles to UI objects
		mURI = (EditText) findViewById(R.id.linkNameEditText);
		mDescription = (EditText) findViewById(R.id.linkDescription);
		mLinkSave = (Button) findViewById(R.id.contactSaveButton);
		mMedia = (Spinner) findViewById(R.id.selectMedia);
		mFormat = (Spinner) findViewById(R.id.selectFormat);

		mLinkSave.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				Log.v(TAG, "Activity State: onClick()");
				onSaveButtonClicked();
			}
		});

	}

	/**
	 * Actions for when the Save button is clicked. Creates a contact entry and
	 * terminates the activity.
	 */
	private void onSaveButtonClicked() {
		Log.v(TAG, "Save button clicked");
		createLinkEntry();
		// Setup here return values..

		// finish activity
		this.finish();
	}

	protected void createLinkEntry() {
		String uri_msg, description_msg, format_msg;
		// Get values from UI
		String uri = mURI.getText().toString();
		String description = mDescription.getText().toString();
		String format = mFormat.getSelectedItem().toString();
		String media = mMedia.getSelectedItem().toString();

		/*Toast.makeText(
				this,
				"Save URI: " + uri + "Descritpion :" + description + "Format :"
						+ format + "Media :" + media, Toast.LENGTH_SHORT)
				.show();*/

		// Update local database
		Long row_id = updateLocalDatabase(format, uri, description, media);

		Intent intent = new Intent();
		uri_msg = intent.getStringExtra("return_uri");
		uri_msg = uri;

		description_msg = intent.getStringExtra("return_description");
		description_msg = description;

		format_msg = intent.getStringExtra("return_format");
		format_msg = format;

		intent.putExtra("return_description", description_msg);
		intent.putExtra("return_uri", uri_msg);
		intent.putExtra("return_rowId", row_id.intValue());
		intent.putExtra("return_format", format_msg);

		setResult(RESULT_OK, intent);

	}

	protected long updateLocalDatabase(String source, String format,
			String description, String media) {
		long row_id;

		SourceDbAdapter dbHelper;
		dbHelper = new SourceDbAdapter(this);
		dbHelper.open();
		row_id = dbHelper.createSource(source, format, description, media);
		dbHelper.close();

		return row_id;
	}

}
