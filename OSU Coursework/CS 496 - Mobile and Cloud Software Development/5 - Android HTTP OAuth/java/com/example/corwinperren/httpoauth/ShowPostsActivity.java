package com.example.corwinperren.httpoauth;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import net.openid.appauth.AuthState;
import net.openid.appauth.AuthorizationException;
import net.openid.appauth.AuthorizationService;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.HttpUrl;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import java.io.IOException;

public class ShowPostsActivity extends AppCompatActivity {

    TextView first_published_text_view;
    TextView first_content_text_view;

    TextView second_published_header_text_view;
    TextView second_published_text_view;
    TextView second_content_text_view;

    TextView third_published_header_text_view;
    TextView third_published_text_view;
    TextView third_content_text_view;

    private AuthorizationService mAuthorizationService;
    private AuthState mAuthState;
    private OkHttpClient mOkHttpClient;

    SharedPreferences authPreference;
    String stateJson;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.show_posts_activity);

        first_published_text_view = (TextView) findViewById(R.id.first_published_text_view);
        first_content_text_view = (TextView) findViewById(R.id.first_content_text_view);

        second_published_header_text_view = (TextView) findViewById(R.id.second_published_header_text_view);
        second_published_text_view = (TextView) findViewById(R.id.second_published_text_view);
        second_content_text_view = (TextView) findViewById(R.id.second_content_text_view);

        third_published_header_text_view = (TextView) findViewById(R.id.third_published_header_text_view);
        third_published_text_view = (TextView) findViewById(R.id.third_published_text_view);
        third_content_text_view = (TextView) findViewById(R.id.third_content_text_view);

        first_published_text_view.setText("Loading...");
        first_content_text_view.setText("Loading...");
        second_published_text_view.setText("Loading...");
        second_content_text_view.setText("Loading...");
        third_published_text_view.setText("Loading...");
        third_content_text_view.setText("Loading...");

        second_published_header_text_view.setVisibility(View.INVISIBLE);
        second_published_text_view.setVisibility(View.INVISIBLE);
        second_content_text_view.setVisibility(View.INVISIBLE);

        third_published_header_text_view.setVisibility(View.INVISIBLE);
        third_published_text_view.setVisibility(View.INVISIBLE);
        third_content_text_view.setVisibility(View.INVISIBLE);


        mAuthorizationService = new AuthorizationService(this);

        authPreference = getSharedPreferences("auth", MODE_PRIVATE);
        stateJson = authPreference.getString("stateJson", null);

        update_list();
    }

    private void update_list(){
        authPreference = getSharedPreferences("auth", MODE_PRIVATE);
        stateJson = authPreference.getString("stateJson", null);

        if(stateJson != null){

            try{
                mAuthState = AuthState.jsonDeserialize(stateJson);
                mAuthState.performActionWithFreshTokens(mAuthorizationService, auth_state_action);
            } catch(Exception e){
                e.printStackTrace();
            }
        }else{
            first_published_text_view.setText("Not Logged In!");
            first_content_text_view.setText("Not Logged In!");
        }
    }

    public AuthState.AuthStateAction auth_state_action = new AuthState.AuthStateAction() {
        @Override
        public void execute(@Nullable String accessToken, @Nullable String idToken, @Nullable AuthorizationException e) {
            if(e == null){
                mOkHttpClient = new OkHttpClient();
                HttpUrl reqUrl = HttpUrl.parse("https://www.googleapis.com/plusDomains/v1/people/me/activities/user");
                Request request = new Request.Builder()
                        .url(reqUrl)
                        .addHeader("Authorization", "Bearer " + accessToken)
                        .build();
                mOkHttpClient.newCall(request).enqueue(http_response_callback);
            }else{
                Log.d(MainActivity.TAG, "Authstate execution failed");
            }
        }
    };

    public Callback http_response_callback = new Callback() {
        @Override
        public void onFailure(Call call, IOException e) {
            e.printStackTrace();
        }

        @Override
        public void onResponse(Call call, Response response) throws IOException {
            String r = response.body().string();
            try {
                JSONObject j = new JSONObject(r);
                final JSONArray items = j.getJSONArray("items");

                final Integer num_posts = items.length();

                if (num_posts > 0){
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            try {
                                first_published_text_view.setText(items.getJSONObject(0).getString("published"));
                                first_content_text_view.setText(items.getJSONObject(0).getString("title"));

                                if(num_posts > 1){
                                    second_published_text_view.setText(items.getJSONObject(1).getString("published"));
                                    second_content_text_view.setText(items.getJSONObject(1).getString("title"));

                                    second_published_header_text_view.setVisibility(View.VISIBLE);
                                    second_published_text_view.setVisibility(View.VISIBLE);
                                    second_content_text_view.setVisibility(View.VISIBLE);
                                }

                                if(num_posts > 2){
                                    third_published_text_view.setText(items.getJSONObject(2).getString("published"));
                                    third_content_text_view.setText(items.getJSONObject(2).getString("title"));

                                    third_published_header_text_view.setVisibility(View.VISIBLE);
                                    third_published_text_view.setVisibility(View.VISIBLE);
                                    third_content_text_view.setVisibility(View.VISIBLE);
                                }





                            }catch (JSONException e1) {
                                e1.printStackTrace();
                            }
                        }
                    });

                }else{
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            first_published_text_view.setText("No posts to display.");
                            first_content_text_view.setText("No posts to display.");
                        }
                    });

                }
            } catch (JSONException e1) {
                e1.printStackTrace();
            }

        }
    };
}
