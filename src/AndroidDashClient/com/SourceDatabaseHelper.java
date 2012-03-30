package AndroidDashClient.com;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class SourceDatabaseHelper extends SQLiteOpenHelper {
	private static final String DATABASE_NAME = "serversource_ast3.sb";
	private static final String TABLE_NAME = "servers";
	private static final int DATABASE_VERSION = 1;
	//private Context context;

	public SourceDatabaseHelper(Context context) {
		super(context, DATABASE_NAME, null, DATABASE_VERSION);
		//this.context = context;

	}

	@Override
	public void onCreate(SQLiteDatabase db) {

		db.execSQL("CREATE TABLE "
				+ TABLE_NAME
				+ "(id INTEGER PRIMARY KEY, format TEXT, fileurl TEXT, description TEXT, media TEXT)");
		// db.execSQL(DATABASE_CREATE);
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		Log.w(SourceDatabaseHelper.class.getName(),
				"Upgrading database from version " + oldVersion + " to "
						+ newVersion + ", which will destroy all old data");
		db.execSQL("DROP TABLE IF EXISTS " + TABLE_NAME);
		onCreate(db);

	}

}
