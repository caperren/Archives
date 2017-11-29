package com.example.corwinperren.httpoauth;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import net.openid.appauth.AuthState;
import net.openid.appauth.AuthorizationException;
import net.openid.appauth.AuthorizationService;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.HttpUrl;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;


public class SubmitPostActivity extends AppCompatActivity {

    EditText submit_edit_text;
    Button submit_button;

    private AuthorizationService mAuthorizationService;
    private AuthState mAuthState;
    private OkHttpClient mOkHttpClient;

    SharedPreferences authPreference;
    String stateJson;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.submit_post_activity);

        submit_edit_text = (EditText) findViewById(R.id.submit_edit_text);
        submit_button = (Button) findViewById(R.id.submit_submit_button);

        mAuthorizationService = new AuthorizationService(this);

        authPreference = getSharedPreferences("auth", MODE_PRIVATE);
        stateJson = authPreference.getString("stateJson", null);

        setupButtonHandlers();
    }

    private void setupButtonHandlers(){
        if(stateJson != null){
            submit_button.setOnClickListener(new View.OnClickListener(){

                @Override
                public void onClick(View v){
                    post_to_gplus();
                }
            });
            submit_button.setEnabled(true);

        }else{
            submit_button.setEnabled(false);
            submit_edit_text.setHint("Not Logged In!");
        }

    }

    private void post_to_gplus(){

        try{
            mAuthState = AuthState.jsonDeserialize(stateJson);
            mAuthState.performActionWithFreshTokens(mAuthorizationService, auth_state_action);
        } catch(Exception e){
            e.printStackTrace();
        }
    }

    public AuthState.AuthStateAction auth_state_action = new AuthState.AuthStateAction() {
        @Override
        public void execute(@Nullable String accessToken, @Nullable String idToken, @Nullable AuthorizationException e) {
            if(e == null){
                String text_to_post = submit_edit_text.getText().toString();

                mOkHttpClient = new OkHttpClient();
                MediaType JSON = MediaType.parse("application/json; charset=utf-8");

                JSONObject json_google_object = new JSONObject();
                JSONObject json_google_access = new JSONObject();
                JSONObject jsonBody = new JSONObject();

                try {
                    json_google_object.put("originalContent", text_to_post);
                    json_google_access.put("domainRestricted", true);
                    jsonBody.put("object", json_google_object);
                    jsonBody.put("access", json_google_access);
                } catch(Exception e1){
                    e1.printStackTrace();
                }

                RequestBody body = RequestBody.create(JSON, jsonBody.toString());

                HttpUrl reqUrl = HttpUrl.parse("https://www.googleapis.com/plusDomains/v1/people/me/activities");
                Request request = new Request.Builder()
                        .url(reqUrl)
                        .post(body)
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
//            String r = response.body().string();
//            Log.d(MainActivity.TAG, r);
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    submit_edit_text.setText("");
                }
            });

            Intent intent = new Intent(SubmitPostActivity.this, MainActivity.class);
            startActivity(intent);


        }
    };

}
