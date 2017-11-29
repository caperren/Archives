package com.example.corwinperren.cs496finalandroid;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.HttpUrl;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;


public class ReminderEditActivity extends AppCompatActivity {
    EditText new_edit_reminder_title_edit_text;
    EditText new_edit_reminder_date_edit_text;
    EditText new_edit_reminder_text_content_edit_text;
    EditText new_edit_reminder_users_add_edit_text;

    Button new_edit_reminder_add_edit_button;
    Button new_edit_reminder_delete_button;
    Button new_edit_reminder_user_add_button;
    Button new_edit_reminder_user_remove_button;

    ListView new_edit_reminder_user_list_view;

    JSONObject http_response;

    List<String> user_string_list = new ArrayList<>();
    ArrayAdapter<String> user_array_adapter;

    int selected_user_position = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_or_edit_reminder);

        get_view_resources();
        setup_button_handlers();
        setup_view_resources();
    }

    @Override
    protected void onResume(){
        super.onResume();
        load_reminder();
    }

    private void load_reminder(){
        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);
        String reminder_id = app_preferences.getString("selected_reminder", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url + "/" + project_id + "/reminders/" + reminder_id);

        Request request = new Request.Builder().url(reqUrl).get().build();
        mOkHttpClient.newCall(request).enqueue(load_reminder_callback);
    }

    private void delete_reminder(){
        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);
        String reminder_id = app_preferences.getString("selected_reminder", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url + "/" + project_id + "/reminders/" + reminder_id);

        Request request = new Request.Builder().url(reqUrl).delete().build();
        mOkHttpClient.newCall(request).enqueue(delete_reminder_callback);
    }

    private void submit_edited_reminder(){
        String title = new_edit_reminder_title_edit_text.getText().toString();
        String reminder_date = new_edit_reminder_date_edit_text.getText().toString();
        String text_content = new_edit_reminder_text_content_edit_text.getText().toString();

        if(title.equals("")){
            show_toast_text("Reminder Name Required...");
            return;
        }

        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);
        String reminder_id = app_preferences.getString("selected_reminder", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        MediaType JSON = MediaType.parse("application/json; charset=utf-8");

        JSONObject json_body = new JSONObject();

        try {
            json_body.put("title", title);
            json_body.put("reminder_date", reminder_date);
            json_body.put("users_to_notify", new JSONArray(user_string_list));
            json_body.put("text_content", text_content);

        } catch(Exception e1){
            e1.printStackTrace();
        }

        RequestBody body = RequestBody.create(JSON, json_body.toString());

        HttpUrl reqUrl = HttpUrl.parse(APIUrls.post_projects_url + "/" + project_id + "/reminders/" + reminder_id);
        Request request = new Request.Builder()
                .url(reqUrl)
                .put(body)
                .build();
        mOkHttpClient.newCall(request).enqueue(put_reminder_callback);

    }

    public Callback load_reminder_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            show_toast_text("Load project failed!");
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            String callback_response = response.body().string();

            Log.d(MainActivity.TAG, callback_response);

            try {
                http_response = new JSONObject(callback_response);

                String title = http_response.getString("title");
                String reminder_date = http_response.getString("reminder_date");
                String text_content = http_response.getString("text_content");
                JSONArray users = http_response.getJSONArray("users_to_notify");

                set_gui_elements(title, reminder_date, text_content, users);

            }catch (JSONException e1) {
                e1.printStackTrace();
            }
        }
    };

    public Callback delete_reminder_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            show_toast_text("Deleting reminder failed!");
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            String callback_response = response.body().string();

            Log.d(MainActivity.TAG, callback_response);

            show_toast_text("Reminder deleted successfully!");

            Intent intent = new Intent(ReminderEditActivity.this, ReminderListingActivity.class);
            startActivity(intent);
        }
    };

    public Callback put_reminder_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            show_toast_text("Adding reminder failed!");
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            String callback_response = response.body().string();

            Log.d(MainActivity.TAG, callback_response);

            show_toast_text("Reminder added successfully!");

            Intent intent = new Intent(ReminderEditActivity.this, ReminderListingActivity.class);
            startActivity(intent);
        }
    };

    private void set_gui_elements(final String title, final String reminder_date, final String text_content, final JSONArray users){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                new_edit_reminder_title_edit_text.setText(title);
                new_edit_reminder_date_edit_text.setText(reminder_date);
                new_edit_reminder_text_content_edit_text.setText(text_content);

                for(int i = 0 ; i < users.length() ; i++){
                    try{
                        user_string_list.add(users.getString(i));
                    }catch (JSONException e1) {
                        e1.printStackTrace();
                    }
                }

                user_array_adapter.notifyDataSetChanged();

            }
        });
    }

    AdapterView.OnItemClickListener user_list_click_listener = new AdapterView.OnItemClickListener (){
        @Override
        public void onItemClick(AdapterView<?> parent, View current_list_item, int position, long id){
            clear_row_background_colors();
            current_list_item.setBackgroundColor(getColor(android.R.color.holo_orange_light));

            selected_user_position = position;

            new_edit_reminder_user_remove_button.setEnabled(true);
        }

    };

    private void clear_row_background_colors(){
        for(Integer i = 0 ; i < new_edit_reminder_user_list_view.getChildCount() ; i++){
            new_edit_reminder_user_list_view.getChildAt(i).setBackground(null);

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

    private void get_view_resources(){
        new_edit_reminder_title_edit_text = (EditText) findViewById(R.id.new_edit_reminder_title_edit_text);
        new_edit_reminder_date_edit_text = (EditText) findViewById(R.id.new_edit_reminder_date_edit_text);
        new_edit_reminder_text_content_edit_text = (EditText) findViewById(R.id.new_edit_reminder_text_content_edit_text);
        new_edit_reminder_users_add_edit_text = (EditText) findViewById(R.id.new_edit_reminder_users_add_edit_text);

        new_edit_reminder_add_edit_button = (Button) findViewById(R.id.new_edit_reminder_add_edit_button);
        new_edit_reminder_delete_button = (Button) findViewById(R.id.new_edit_reminder_delete_button);
        new_edit_reminder_user_add_button = (Button) findViewById(R.id.new_edit_reminder_user_add_button);
        new_edit_reminder_user_remove_button = (Button) findViewById(R.id.new_edit_reminder_user_remove_button);

        new_edit_reminder_user_list_view = (ListView) findViewById(R.id.new_edit_reminder_user_list_view);
    }

    private void setup_button_handlers(){
        new_edit_reminder_add_edit_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                submit_edited_reminder();
            }
        });

        new_edit_reminder_delete_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                delete_reminder();
            }
        });

        new_edit_reminder_user_add_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                user_string_list.add(new_edit_reminder_users_add_edit_text.getText().toString());
                user_array_adapter.notifyDataSetChanged();

                new_edit_reminder_users_add_edit_text.setText("");
            }
        });

        new_edit_reminder_user_remove_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                user_string_list.remove(selected_user_position);
                user_array_adapter.notifyDataSetChanged();

                clear_row_background_colors();
                new_edit_reminder_user_remove_button.setEnabled(false);
            }
        });
    }

    private void setup_view_resources(){
        new_edit_reminder_add_edit_button.setText("Update");

        user_array_adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, user_string_list);
        new_edit_reminder_user_list_view.setAdapter(user_array_adapter);

        new_edit_reminder_user_list_view.setOnItemClickListener(user_list_click_listener);
    }
}
