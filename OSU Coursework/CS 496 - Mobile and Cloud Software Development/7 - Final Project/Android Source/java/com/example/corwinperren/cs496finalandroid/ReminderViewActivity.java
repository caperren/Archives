package com.example.corwinperren.cs496finalandroid;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class ReminderViewActivity extends AppCompatActivity {

    TextView view_reminder_title_text_view;
    TextView view_reminder_date_text_view;
    TextView view_reminder_content_text_view;

    ListView view_reminder_users_list_view;

    JSONObject http_response;

    List<String> user_string_list = new ArrayList<>();
    ArrayAdapter<String> user_array_adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_view_reminder);

        get_view_resources();
        setup_view_resources();
    }

    @Override
    protected void onResume(){
        super.onResume();
        load_reminder();
    }

    private void load_reminder(){
        user_array_adapter.clear();

        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);
        String reminder_id = app_preferences.getString("selected_reminder", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url + "/" + project_id + "/reminders/" + reminder_id);

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

    private void set_gui_elements(final String title, final String reminder_date, final String text_content, final JSONArray users){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
            view_reminder_title_text_view.setText(title);
            view_reminder_date_text_view.setText(reminder_date);
            view_reminder_content_text_view.setText(text_content);

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

    private void setup_view_resources(){
        user_array_adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, user_string_list);
        view_reminder_users_list_view.setAdapter(user_array_adapter);
    }

    private void get_view_resources(){
        view_reminder_title_text_view = (TextView) findViewById(R.id.view_reminder_title_text_view);
        view_reminder_date_text_view = (TextView) findViewById(R.id.view_reminder_date_text_view);
        view_reminder_content_text_view = (TextView) findViewById(R.id.view_reminder_content_text_view);

        view_reminder_users_list_view = (ListView) findViewById(R.id.view_reminder_users_list_view);
    }

}
