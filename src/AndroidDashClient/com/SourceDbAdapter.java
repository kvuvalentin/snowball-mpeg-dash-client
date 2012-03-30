package AndroidDashClient.com;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;

public class SourceDbAdapter {
	// Database fields
	public static final String KEY_ROWID = "id";
	public static final String KEY_FORMAT = "format";
	public static final String KEY_FILEURL = "fileurl";
	public static final String KEY_DESCRIPTION = "description";
	public static final String KEY_MEDIA = "media";
	private static final String DATABASE_TABLE = "servers";
	private Context context;
	private SQLiteDatabase database;
	private SourceDatabaseHelper dbHelper;

	public SourceDbAdapter(Context context) {
		this.context = context;
	}

	public SourceDbAdapter open() throws SQLException {
		dbHelper = new SourceDatabaseHelper(context);
		database = dbHelper.getWritableDatabase();

		return this;
	}

	public void close() {
		dbHelper.close();
	}

	/**
	 * * Create a new todo If the todo is successfully created return the new *
	 * rowId for that note, otherwise return a -1 to indicate failure.
	 */

	public long createSource(String source, String format, String description,
			String media) {
		ContentValues initialValues = createContentValues(source, format,
				description, media);

		return database.insert(DATABASE_TABLE, null, initialValues);
	}

	/** * Update the todo */

	public boolean updateSource(long rowId, String source, String format,
			String description, String media) {
		ContentValues updateValues = createContentValues(source, format,
				description, media);

		return database.update(DATABASE_TABLE, updateValues, KEY_ROWID + "="
				+ rowId, null) > 0;
	}

	/** * Deletes todo */

	public boolean deleteSource(long rowId) {
		return database.delete(DATABASE_TABLE, KEY_ROWID + "=" + rowId, null) > 0;
	}

	/**
	 * * Return a Cursor over the list of all todo in the database * * @return
	 * Cursor over all notes
	 */

	public Cursor fetchAllSources() {
		return database.query(DATABASE_TABLE, new String[] { KEY_ROWID,
				KEY_FORMAT, KEY_FILEURL, KEY_DESCRIPTION, KEY_MEDIA }, null,
				null, null, null, null);
	}

	/** * Return a Cursor positioned at the defined todo */

	public Cursor fetchSource(long rowId) throws SQLException {
		Cursor mCursor = database.query(true, DATABASE_TABLE,
				new String[] { KEY_ROWID, KEY_FORMAT, KEY_FILEURL,
						KEY_DESCRIPTION, KEY_MEDIA }, KEY_ROWID + "=" + rowId,
				null, null, null, null, null);
		if (mCursor != null) {
			mCursor.moveToFirst();
		}
		return mCursor;
	}

	private ContentValues createContentValues(String format, String fileurl,
			String description, String media) {
		ContentValues values = new ContentValues();
		values.put(KEY_FORMAT, format);
		values.put(KEY_FILEURL, fileurl);
		values.put(KEY_DESCRIPTION, description);
		values.put(KEY_MEDIA, media);
		return values;
	}

}
