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

public class EditProjectActivity extends AppCompatActivity {

    EditText project_name_edit_text;
    EditText project_language_edit_text;
    EditText contributor_name_edit_text;

    Button project_submit_update_button;
    Button project_delete_button;
    Button contributor_add_button;
    Button contributor_remove_button;

    ListView contributor_list_view;

    List<String> contributor_string_list = new ArrayList<>();
    ArrayAdapter<String> contributor_array_adapter;

    int selected_contributor_position = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_or_edit_project);

        get_view_resources();
        setup_button_handlers();
        setup_view_resources();
    }

    @Override
    protected void onResume(){
        super.onResume();
        load_project();
    }

    private void load_project(){
        contributor_array_adapter.clear();

        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url + "/" + project_id);

        Request request = new Request.Builder().url(reqUrl).get().build();
        mOkHttpClient.newCall(request).enqueue(load_project_callback);
    }

    private void delete_project(){
        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);

        OkHttpClient mOkHttpClient = new OkHttpClient();
        HttpUrl reqUrl = HttpUrl.parse(APIUrls.get_projects_url + "/" + project_id);

        Request request = new Request.Builder().url(reqUrl).delete().build();
        mOkHttpClient.newCall(request).enqueue(delete_project_callback);
    }

    private void update_project(){

        String name = project_name_edit_text.getText().toString();
        String language = project_language_edit_text.getText().toString();

        SharedPreferences app_preferences = getSharedPreferences("app_prefs", MODE_PRIVATE);
        String project_id = app_preferences.getString("selected_project", null);

        if(name.equals("")){
            show_toast_text("Project Name Required...");
            return;
        }

        OkHttpClient mOkHttpClient = new OkHttpClient();
        MediaType JSON = MediaType.parse("application/json; charset=utf-8");

        JSONObject json_body = new JSONObject();

        try {
            json_body.put("name", name);
            json_body.put("language", language);
            json_body.put("contributors", new JSONArray(contributor_string_list));

        } catch(Exception e1){
            e1.printStackTrace();
        }

        RequestBody body = RequestBody.create(JSON, json_body.toString());

        HttpUrl reqUrl = HttpUrl.parse(APIUrls.post_projects_url + "/" + project_id);
        Request request = new Request.Builder()
                .url(reqUrl)
                .put(body)
                .build();
        mOkHttpClient.newCall(request).enqueue(update_project_callback);

    }


    public Callback load_project_callback = new Callback() {
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
                JSONObject http_response = new JSONObject(callback_response);

                String name = http_response.getString("name");
                String language = http_response.getString("language");
                JSONArray contributors = http_response.getJSONArray("contributors");

                update_ui_with_project(name, language, contributors);

            }catch (JSONException e1) {
                e1.printStackTrace();
            }
        }
    };

    public Callback delete_project_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            show_toast_text("Delete project failed!");
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            show_toast_text("Deleted project successfully!");

            String callback_response = response.body().string();

            Log.d(MainActivity.TAG, callback_response);

            Intent intent = new Intent(EditProjectActivity.this, MainActivity.class);
            startActivity(intent);
        }
    };

    public Callback update_project_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            show_toast_text("Update project failed!");
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            show_toast_text("Updated project successfully!");

            String callback_response = response.body().string();

            Log.d(MainActivity.TAG, callback_response);

            Intent intent = new Intent(EditProjectActivity.this, MainActivity.class);
            startActivity(intent);
        }
    };

    private void update_ui_with_project(final String name, final String language, final JSONArray contributors){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                project_name_edit_text.setText(name);

                if(!language.equals("null")) {
                    project_language_edit_text.setText(language);
                }

                for(int i = 0 ; i < contributors.length() ; i++){
                    try {
                        contributor_string_list.add(contributors.getString(i));
                    }catch (JSONException e1) {
                        e1.printStackTrace();
                    }
                }

                contributor_array_adapter.notifyDataSetChanged();
            }
        });
    }

    AdapterView.OnItemClickListener contributor_click_listener = new AdapterView.OnItemClickListener (){
        @Override
        public void onItemClick(AdapterView<?> parent, View current_list_item, int position, long id){
            clear_row_background_colors();
            current_list_item.setBackgroundColor(getColor(android.R.color.holo_orange_light));

            selected_contributor_position = position;

            contributor_remove_button.setEnabled(true);
        }

    };

    private void clear_row_background_colors(){
        for(Integer i = 0 ; i < contributor_list_view.getChildCount() ; i++){
            contributor_list_view.getChildAt(i).setBackground(null);

        }
    }

    private void get_view_resources(){
        project_name_edit_text = (EditText) findViewById(R.id.project_name_edit_text);
        project_language_edit_text = (EditText) findViewById(R.id.project_language_edit_text);
        contributor_name_edit_text = (EditText) findViewById(R.id.contributor_name_edit_text);

        project_submit_update_button = (Button) findViewById(R.id.project_submit_update_button);
        project_delete_button = (Button) findViewById(R.id.project_delete_button);
        contributor_add_button = (Button) findViewById(R.id.contributor_add_button);
        contributor_remove_button = (Button) findViewById(R.id.contributor_remove_button);

        contributor_list_view = (ListView) findViewById(R.id.contributor_list_view);
    }

    private void setup_button_handlers(){
        project_submit_update_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                update_project();
            }
        });

        project_delete_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                delete_project();
            }
        });

        contributor_add_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                contributor_string_list.add(contributor_name_edit_text.getText().toString());
                contributor_array_adapter.notifyDataSetChanged();

                contributor_name_edit_text.setText("");
            }
        });

        contributor_remove_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Log.d(MainActivity.TAG, String.valueOf(selected_contributor_position));
                contributor_string_list.remove(selected_contributor_position);
                contributor_array_adapter.notifyDataSetChanged();

                clear_row_background_colors();
                contributor_remove_button.setEnabled(false);
            }
        });
    }

    private void setup_view_resources(){
        project_submit_update_button.setText("Update");

        contributor_array_adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, contributor_string_list);
        contributor_list_view.setAdapter(contributor_array_adapter);

        contributor_list_view.setOnItemClickListener(contributor_click_listener);
    }

    private void show_toast_text(final String text){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(getApplicationContext(), text, Toast.LENGTH_SHORT).show();
            }
        });
    }
}
