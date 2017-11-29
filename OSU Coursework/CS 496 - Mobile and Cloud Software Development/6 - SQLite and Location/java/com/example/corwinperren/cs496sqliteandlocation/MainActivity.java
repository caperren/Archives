package com.example.corwinperren.cs496sqliteandlocation;

import android.Manifest;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.location.Location;
import android.os.Bundle;
import android.os.Looper;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import android.content.ContentValues;
import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.provider.BaseColumns;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.LocationSettingsRequest;
import com.google.android.gms.location.LocationSettingsResponse;
import com.google.android.gms.location.SettingsClient;
import com.google.android.gms.tasks.OnSuccessListener;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = MainActivity.class.getSimpleName();

    private static final int REQUEST_PERMISSIONS_REQUEST_CODE = 41;

    private FusedLocationProviderClient mFusedLocationClient;
    private SettingsClient mSettingsClient;
    private LocationRequest mLocationRequest;
    private LocationSettingsRequest mLocationSettingsRequest;
    private LocationCallback mLocationCallback;
    private Location mCurrentLocation;

    private Button mDeleteAllButton;
    private Button mRequestPermissionsButton;
    private Button mAddEntryButton;

    private TextView mLocationPermissionsTextView;
    private TextView mLatitudeTextView;
    private TextView mLongitudeTextView;

    private EditText mCommentEditText;

    private TableLayout mCommentTableLayout;

    SQLiteDatabase mWriteGeoCommDB;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ////////// UI /////////
        // Get UI widgets.
        mDeleteAllButton = (Button) findViewById(R.id.delete_all_button);
        mRequestPermissionsButton = (Button) findViewById(R.id.request_permissions_button);
        mAddEntryButton = (Button) findViewById(R.id.add_entry_button);

        mLocationPermissionsTextView = (TextView) findViewById(R.id.location_permissions_text_view);
        mLatitudeTextView = (TextView) findViewById(R.id.latitude_text_view);
        mLongitudeTextView = (TextView) findViewById(R.id.longitude_text_view);

        mCommentEditText = (EditText) findViewById(R.id.comment_edit_text_box);

        mCommentTableLayout = (TableLayout) findViewById(R.id.comment_table_layout);

        ////////// Location//////////
        // Get Location and Location Settings Clients
        mFusedLocationClient = LocationServices.getFusedLocationProviderClient(this);
        mSettingsClient = LocationServices.getSettingsClient(this);

        // Fully set up location services
        createLocationCallback();
        createLocationRequest();
        buildLocationSettingsRequest();

        // Connect Buttons to their methods
        mDeleteAllButton.setOnClickListener(deleteAllHandler);
        mRequestPermissionsButton.setOnClickListener(requestPermissionsHandler);
        mAddEntryButton.setOnClickListener(addEntryHandler);

        ////////// SQLite //////////
        GeoCommentSQLiteDB mGeoCommentDB = new GeoCommentSQLiteDB(this);
        mWriteGeoCommDB = mGeoCommentDB.getWritableDatabase();

        ////////// Start Needed Services ////////
        if (checkPermissions()){
            mLocationPermissionsTextView.setText("Permissions granted!!!");
            startLocationUpdates();
        }

        // Show any saved content from database if present
        updateTableLayoutFromDatabase();
    }

    private void createLocationCallback() {
        mLocationCallback = new LocationCallback() {
            @Override
            public void onLocationResult(LocationResult locationResult) {
                super.onLocationResult(locationResult);

                mCurrentLocation = locationResult.getLastLocation();
                mLatitudeTextView.setText(String.valueOf(mCurrentLocation.getLatitude()));
                mLongitudeTextView.setText(String.valueOf(mCurrentLocation.getLongitude()));
                // TODO: call update gui method updateLocationUI();
            }
        };
    }

    private void createLocationRequest() {
        mLocationRequest = new LocationRequest();
        mLocationRequest.setInterval(500);
        mLocationRequest.setFastestInterval(500);
        mLocationRequest.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);
    }

    private void buildLocationSettingsRequest() {
        LocationSettingsRequest.Builder builder = new LocationSettingsRequest.Builder();
        builder.addLocationRequest(mLocationRequest);
        mLocationSettingsRequest = builder.build();
    }

    private boolean checkPermissions() {
        int permissionState = ActivityCompat.checkSelfPermission(this,
                Manifest.permission.ACCESS_FINE_LOCATION);
        return permissionState == PackageManager.PERMISSION_GRANTED;
    }

    private void requestPermissions() {
            ActivityCompat.requestPermissions(MainActivity.this,
                    new String[]{Manifest.permission.ACCESS_FINE_LOCATION},
                    REQUEST_PERMISSIONS_REQUEST_CODE);

    }

    private void startLocationUpdates() {
        // Begin by checking if the device has the necessary location settings.
        mSettingsClient.checkLocationSettings(mLocationSettingsRequest)
                .addOnSuccessListener(this, new OnSuccessListener<LocationSettingsResponse>() {
                    @Override
                    public void onSuccess(LocationSettingsResponse locationSettingsResponse) {
                        //noinspection MissingPermission
                        mFusedLocationClient.requestLocationUpdates(mLocationRequest,
                                mLocationCallback, Looper.myLooper());
                    }
                });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == REQUEST_PERMISSIONS_REQUEST_CODE) {
            if (grantResults.length > 0) {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    mLocationPermissionsTextView.setText("Permissions granted!!!");
                    startLocationUpdates();
                    return;
                }
            }
        }
        mLocationPermissionsTextView.setText("No permissions. OSU Defaults.");
    }

    private View.OnClickListener deleteAllHandler = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            // Delete entries from database
            emptyDatabase();

            // Delete all entries in table
            clearCommentTable();
        }
    };

    private View.OnClickListener requestPermissionsHandler = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            if (!checkPermissions()) {
                requestPermissions();
            }
        }
    };

    private View.OnClickListener addEntryHandler = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            // Add entry to database
            ContentValues newRowContent = new ContentValues();
            newRowContent.put(DBContract.GeoCommentTable.COLUMN_NAME_LATITUDE_STRING, mLatitudeTextView.getText().toString());
            newRowContent.put(DBContract.GeoCommentTable.COLUMN_NAME_LONGITUDE_STRING, mLongitudeTextView.getText().toString());
            newRowContent.put(DBContract.GeoCommentTable.COLUMN_NAME_COMMENT_STRING, mCommentEditText.getText().toString());
            mWriteGeoCommDB.insert(DBContract.GeoCommentTable.TABLE_NAME, null, newRowContent);

            // Have GUI show updated database
            updateTableLayoutFromDatabase();

            // Clear out our comment entry box
            mCommentEditText.setText("");
        }
    };

    public void clearCommentTable(){
        int numTableRows = mCommentTableLayout.getChildCount();
        if (numTableRows > 1){
            mCommentTableLayout.removeViews(1, numTableRows - 1);
        }
    }

    ////////// Database Related Classes and Methods /////////

    public void updateTableLayoutFromDatabase(){
        // Delete all entries in table
        clearCommentTable();

        // Get and update tablelayout with all entries
        Cursor geoCommCursor = mWriteGeoCommDB.rawQuery("SELECT * FROM " + DBContract.GeoCommentTable.TABLE_NAME, null);

        int num_entries = geoCommCursor.getCount();

        if(num_entries > 0) {
            geoCommCursor.moveToFirst();

            do {
                String latitude = geoCommCursor.getString(geoCommCursor.getColumnIndex(DBContract.GeoCommentTable.COLUMN_NAME_LATITUDE_STRING));
                String longitude = geoCommCursor.getString(geoCommCursor.getColumnIndex(DBContract.GeoCommentTable.COLUMN_NAME_LONGITUDE_STRING));
                String comment = geoCommCursor.getString(geoCommCursor.getColumnIndex(DBContract.GeoCommentTable.COLUMN_NAME_COMMENT_STRING));

                TableRow newRow = new TableRow(MainActivity.this);

                TextView latTextView = new TextView(MainActivity.this);
                TextView lonTextView = new TextView(MainActivity.this);
                TextView commentTextView = new TextView(MainActivity.this);

                latTextView.setText(latitude);
                lonTextView.setText(longitude);
                commentTextView.setText(comment);

                newRow.addView(latTextView);
                newRow.addView(lonTextView);
                newRow.addView(commentTextView);

                mCommentTableLayout.addView(newRow, new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.MATCH_PARENT));
            } while (geoCommCursor.moveToNext());
        }

        // Needed to avoid memory leaks
        geoCommCursor.close();
    }

    public void emptyDatabase(){
        mWriteGeoCommDB.delete(DBContract.GeoCommentTable.TABLE_NAME, null, null);
    }

    class GeoCommentSQLiteDB extends SQLiteOpenHelper {

    public GeoCommentSQLiteDB(Context context) {
            super(context, DBContract.GeoCommentTable.DB_NAME, null, DBContract.GeoCommentTable.DB_VERSION);
        }

        @Override
        public void onCreate(SQLiteDatabase db) {
            db.execSQL(DBContract.GeoCommentTable.SQL_CREATE_DEMO_TABLE);

            ContentValues testValues = new ContentValues();
            testValues.put(DBContract.GeoCommentTable.COLUMN_NAME_LATITUDE_STRING, "42");
            testValues.put(DBContract.GeoCommentTable.COLUMN_NAME_LONGITUDE_STRING, "-123");
            testValues.put(DBContract.GeoCommentTable.COLUMN_NAME_COMMENT_STRING, "This is a comment test!!!!");
            db.insert(DBContract.GeoCommentTable.TABLE_NAME,null,testValues);
        }

        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
            db.execSQL(DBContract.GeoCommentTable.SQL_DROP_DEMO_TABLE);
            onCreate(db);
        }

    }

    final class DBContract {
        private DBContract(){}

        public final class GeoCommentTable implements BaseColumns {
            public static final String DB_NAME = "geo_comment_db";
            public static final String TABLE_NAME = "geo_comments";
            public static final String COLUMN_NAME_LATITUDE_STRING = "latitude_string";
            public static final String COLUMN_NAME_LONGITUDE_STRING = "longitude_string";
            public static final String COLUMN_NAME_COMMENT_STRING = "comment_string";
            public static final int DB_VERSION = 4;


            public static final String SQL_CREATE_DEMO_TABLE = "CREATE TABLE " +
                    GeoCommentTable.TABLE_NAME + "(" + GeoCommentTable._ID + " INTEGER PRIMARY KEY NOT NULL," +
                    GeoCommentTable.COLUMN_NAME_LATITUDE_STRING + " VARCHAR(255)," +
                    GeoCommentTable.COLUMN_NAME_LONGITUDE_STRING + " VARCHAR(255),"+
                    GeoCommentTable.COLUMN_NAME_COMMENT_STRING + " VARCHAR(255));";

            public static final String SQL_DROP_DEMO_TABLE = "DROP TABLE IF EXISTS " + GeoCommentTable.TABLE_NAME;
        }
    }

    class GeoComment {
        public String latitude;
        public String longitude;
        public String comment;
    }
}