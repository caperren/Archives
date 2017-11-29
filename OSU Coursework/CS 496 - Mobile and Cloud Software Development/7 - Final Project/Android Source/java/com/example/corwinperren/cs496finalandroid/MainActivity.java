package com.example.corwinperren.cs496finalandroid;

import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
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

public class MainActivity extends AppCompatActivity {
    public static final String TAG = MainActivity.class.getSimpleName();

    TableLayout projects_table_layout;

    Button new_project_button;
    Button edit_project_button;
    Button edit_reminders_button;

    JSONArray http_response;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        get_view_resources();
        setup_button_handlers();
    }

    @Override
    protected void onResume(){
        super.onResume();
        load_projects();
    }

    private void get_view_resources(){
        projects_table_layout = (TableLayout) findViewById(R.id.projects_table_layout);

        new_project_button = (Button) findViewById(R.id.new_project_button);
        edit_project_button = (Button) findViewById(R.id.edit_project_button);
        edit_reminders_button = (Button) findViewById(R.id.edit_reminders_button);
    }

    private void load_projects(){
        clear_projects();

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url);

        Request request = new Request.Builder().url(reqUrl).get().build();
        mOkHttpClient.newCall(request).enqueue(load_projects_callback);
    }

    public Callback load_projects_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            String callback_response = response.body().string();

            Log.d(TAG, callback_response);

            try {
                http_response = new JSONArray(callback_response);

                Integer num_projects = http_response.length();

                if(num_projects == 0){
                    edit_project_button.setEnabled(false);
                    edit_reminders_button.setEnabled(false);
                    return;
                }

                for (Integer index = 0 ; index < num_projects ; index++){
                    JSONObject current_project = http_response.getJSONObject(index);

                    String name = current_project.getString("name");
                    String language = current_project.getString("language");
                    JSONArray contributors = current_project.getJSONArray("contributors");
                    String project_id = current_project.getString("id");

                    add_row_to_table(name, language, contributors, project_id);
                }

            }catch (JSONException e1) {
                e1.printStackTrace();
            }

            select_first_project();
        }

        private void add_row_to_table(final String name, final String language, final JSONArray contributors, final String project_id){
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    TableRow newRow = new TableRow(MainActivity.this);


                    TextView nameTextView = new TextView(MainActivity.this);
                    TextView languageTextView = new TextView(MainActivity.this);
                    TextView contributorsTextView = new TextView(MainActivity.this);

                    nameTextView.setText(name);

                    if(!language.equals("null")) {
                        languageTextView.setText(language);
                    }

                    String contributors_string = "";

                    for(Integer i = 0 ; i < contributors.length() ; i++){
                        if(i != 0 ){ contributors_string += "\n"; }

                        try {
                            contributors_string += contributors.getString(i);
                        }catch (JSONException e1) {
                            e1.printStackTrace();
                        }
                    }

                    contributorsTextView.setText(contributors_string);

                    newRow.addView(nameTextView);
                    newRow.addView(languageTextView);
                    newRow.addView(contributorsTextView);

                    newRow.setTag(project_id);
                    newRow.setClickable(true);
                    newRow.setOnClickListener(row_click_listener);

                    projects_table_layout.addView(newRow, new TableLayout.LayoutParams(TableLayout.LayoutParams.MATCH_PARENT, TableLayout.LayoutParams.MATCH_PARENT));
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
            app_preferences.edit().putString("selected_project", current_row.getTag().toString()).apply();

            edit_project_button.setEnabled(true);
            edit_reminders_button.setEnabled(true);
        }

    };

    private void clear_projects(){
        int numTableRows = projects_table_layout.getChildCount();
        if (numTableRows > 1){
            projects_table_layout.removeViews(1, numTableRows - 1);
        }
    }


    private void select_first_project(){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if(projects_table_layout.getChildCount() > 1){
                    TableRow row = (TableRow) projects_table_layout.getChildAt(1);
                    row.performClick();
                }
            }
        });

    }

    private void clear_row_background_colors(){
        for(Integer i = 0 ; i < projects_table_layout.getChildCount() ; i++){
            TableRow row = (TableRow) projects_table_layout.getChildAt(i);
            row.setBackground(null);

        }
    }

    private void show_toast_text(final String text){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), text, Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void setup_button_handlers(){
        new_project_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, NewProjectActivity.class);
                startActivity(intent);
            }
        });

        edit_project_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, EditProjectActivity.class);
                startActivity(intent);

            }
        });

        edit_reminders_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, ReminderListingActivity.class);
                startActivity(intent);
            }
        });
    }
}
