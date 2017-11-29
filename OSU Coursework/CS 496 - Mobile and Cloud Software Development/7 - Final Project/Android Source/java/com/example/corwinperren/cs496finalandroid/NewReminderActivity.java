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


public class NewReminderActivity extends AppCompatActivity {
    EditText new_edit_reminder_title_edit_text;
    EditText new_edit_reminder_date_edit_text;
    EditText new_edit_reminder_text_content_edit_text;
    EditText new_edit_reminder_users_add_edit_text;

    Button new_edit_reminder_add_edit_button;
    Button new_edit_reminder_delete_button;
    Button new_edit_reminder_user_add_button;
    Button new_edit_reminder_user_remove_button;

    ListView new_edit_reminder_user_list_view;

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

    private void submit_new_reminder(){
        String title = new_edit_reminder_title_edit_text.getText().toString();
        String reminder_date = new_edit_reminder_date_edit_text.getText().toString();
        String text_content = new_edit_reminder_text_content_edit_text.getText().toString();

        if(title.equals("")){
            show_toast_text("Reminder Name Required...");
            return;
        }

        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);

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

        HttpUrl reqUrl = HttpUrl.parse(APIUrls.post_projects_url + "/" + project_id + "/reminders");
        Request request = new Request.Builder()
                .url(reqUrl)
                .post(body)
                .build();
        mOkHttpClient.newCall(request).enqueue(post_reminder_callback);

    }

    public Callback post_reminder_callback = new Callback() {
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

            Intent intent = new Intent(NewReminderActivity.this, ReminderListingActivity.class);
            startActivity(intent);
        }
    };

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
                submit_new_reminder();
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
        new_edit_reminder_add_edit_button.setText("Submit");
        new_edit_reminder_delete_button.setVisibility(View.INVISIBLE);

        user_array_adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, user_string_list);
        new_edit_reminder_user_list_view.setAdapter(user_array_adapter);

        new_edit_reminder_user_list_view.setOnItemClickListener(user_list_click_listener);
    }
}
