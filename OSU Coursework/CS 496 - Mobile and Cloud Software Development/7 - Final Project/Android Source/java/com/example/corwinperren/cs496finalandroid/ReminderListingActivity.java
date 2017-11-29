package com.example.corwinperren.cs496finalandroid;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.Toast;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;

public class ReminderListingActivity extends AppCompatActivity {

    TableLayout reminders_table_layout;

    Button reminders_list_home_button;
    Button new_reminder_button;
    Button edit_reminder_button;
    Button view_reminder_button;

    JSONArray http_response;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_reminder_list);

        get_view_resources();
        setup_button_handlers();
    }

    @Override
    protected void onResume(){
        super.onResume();
        load_reminders();
    }

    private void load_reminders(){
        clear_reminders();

        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url + "/" + project_id + "/reminders");

        Request request = new Request.Builder().url(reqUrl).get().build();
        mOkHttpClient.newCall(request).enqueue(load_reminders_callback);
    }

    public Callback load_reminders_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            String callback_response = response.body().string();

            Log.d(MainActivity.TAG, callback_response);

            try {
                http_response = new JSONArray(callback_response);

                Integer num_reminders = http_response.length();

                if(num_reminders == 0){
                    set_buttons_enabled(false);
                    return;
                }

                for (Integer index = 0 ; index < num_reminders ; index++){
                    JSONObject current_reminder = http_response.getJSONObject(index);

                    String title = current_reminder.getString("title");
                    String reminder_date = current_reminder.getString("reminder_date");
                    String reminder_id = current_reminder.getString("id");

                    add_row_to_table(title, reminder_date, reminder_id);
                }

            }catch (JSONException e1) {
                e1.printStackTrace();
            }

            select_first_reminder();
        }

        private void add_row_to_table(final String title, final String reminder_date, final String reminder_id){
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    TableRow newRow = new TableRow(ReminderListingActivity.this);


                    TextView titleTextView = new TextView(ReminderListingActivity.this);
                    TextView reminderDateTextVew = new TextView(ReminderListingActivity.this);

                    titleTextView.setText(title);

                    if(!reminder_date.equals("null")) {
                        reminderDateTextVew.setText(reminder_date);
                    }

                    newRow.addView(titleTextView);
                    newRow.addView(reminderDateTextVew);

                    newRow.setTag(reminder_id);
                    newRow.setClickable(true);
                    newRow.setOnClickListener(row_click_listener);

                    reminders_table_layout.addView(newRow, new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.MATCH_PARENT));
                }
            });
        }
    };

    View.OnClickListener row_click_listener = new View.OnClickListener(){
        @Override
        public void onClick(View current_row){
            clear_row_background_colors();
            current_row.setBackgroundColor(getColor(android.R.color.holo_orange_light));

            SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
            app_preferences.edit().putString("selected_reminder", current_row.getTag().toString()).apply();

            set_buttons_enabled(true);
        }

    };

    private void set_buttons_enabled(final boolean enabled){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                edit_reminder_button.setEnabled(enabled);
                view_reminder_button.setEnabled(enabled);
            }
        });
    }

    private void select_first_reminder(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if(reminders_table_layout.getChildCount() > 1){
                    TableRow row = (TableRow) reminders_table_layout.getChildAt(1);
                    row.performClick();
                }
            }
        });

    }

    private void clear_row_background_colors(){
        for(Integer i = 0 ; i < reminders_table_layout.getChildCount() ; i++){
            TableRow row = (TableRow) reminders_table_layout.getChildAt(i);
            row.setBackground(null);

        }
    }

    private void clear_reminders(){
        int numTableRows = reminders_table_layout.getChildCount();
        if (numTableRows > 1){
            reminders_table_layout.removeViews(1, numTableRows - 1);
        }
    }

    private void get_view_resources(){
        reminders_table_layout = (TableLayout) findViewById(R.id.reminders_table_layout);

        reminders_list_home_button = (Button) findViewById(R.id.reminders_list_home_button);
        new_reminder_button = (Button) findViewById(R.id.new_reminder_button);
        edit_reminder_button = (Button) findViewById(R.id.edit_reminder_button);
        view_reminder_button = (Button) findViewById(R.id.view_reminder_button);
    }

    private void setup_button_handlers(){
        reminders_list_home_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(ReminderListingActivity.this, MainActivity.class);
                startActivity(intent);
            }
        });

        new_reminder_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(ReminderListingActivity.this, NewReminderActivity.class);
                startActivity(intent);
            }
        });

        edit_reminder_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(ReminderListingActivity.this, ReminderEditActivity.class);
                startActivity(intent);

            }
        });

        view_reminder_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(ReminderListingActivity.this, ReminderViewActivity.class);
                startActivity(intent);
            }
        });
    }
}
