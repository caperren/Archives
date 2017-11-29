package com.example.corwinperren.httpoauth;

import android.app.PendingIntent;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import net.openid.appauth.AuthState;
import net.openid.appauth.AuthorizationRequest;
import net.openid.appauth.AuthorizationService;
import net.openid.appauth.AuthorizationServiceConfiguration;
import net.openid.appauth.ResponseTypeValues;

import org.json.JSONException;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = MainActivity.class.getSimpleName();

    private String mClientIDString = "106019908752-hkcvokpsq34vg0795mbhm42mbitqocub.apps.googleusercontent.com";

    public AuthorizationService mAuthorizationService;

    Button gplus_login_button;
    Button show_posts_button;
    Button submit_new_post_button;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mAuthorizationService = new AuthorizationService(this);

        gplus_login_button = (Button) findViewById(R.id.gplus_login_button);
        show_posts_button = (Button) findViewById(R.id.show_posts_button);
        submit_new_post_button = (Button) findViewById(R.id.submit_new_post_button);

        setupButtonHandlers();
    }

    private void setupButtonHandlers(){
        gplus_login_button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v){
                getOrCreateAuthState();
            }
        });

        show_posts_button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, ShowPostsActivity.class);
                startActivity(intent);
            }
        });

        submit_new_post_button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, SubmitPostActivity.class);
                startActivity(intent);
            }
        });
    }

    private void request_authorization(){
        Uri authEndpoint = new Uri.Builder().scheme("https").authority("accounts.google.com").path("/o/oauth2/v2/auth").build();
        Uri tokenEndpoint = new Uri.Builder().scheme("https").authority("www.googleapis.com").path("/oauth2/v4/token").build();
        Uri redirect = new Uri.Builder().scheme("com.example.corwinperren.httpoauth").path("oauth").build();

        AuthorizationServiceConfiguration config = new AuthorizationServiceConfiguration(authEndpoint, tokenEndpoint, null);
        AuthorizationRequest req = new AuthorizationRequest.Builder(config, mClientIDString, ResponseTypeValues.CODE, redirect)
                .setScopes("https://www.googleapis.com/auth/plus.me", "https://www.googleapis.com/auth/plus.stream.write", "https://www.googleapis.com/auth/plus.stream.read")
                .build();

        Intent authComplete = new Intent(this, GoogleAuthCompleteActivity.class);
        mAuthorizationService.performAuthorizationRequest(req, PendingIntent.getActivity(this, req.hashCode(), authComplete, 0));
    }

    AuthState getOrCreateAuthState() {
        AuthState auth = null;
        SharedPreferences authPreference = getSharedPreferences("auth", MODE_PRIVATE);
        String stateJson = authPreference.getString("stateJson", null);
        if (stateJson != null) {
            try {
                auth = AuthState.jsonDeserialize(stateJson);
            } catch (JSONException e) {
                e.printStackTrace();
                return null;
            }
        }
        if (auth != null && auth.getAccessToken() != null) {
            return auth;
        } else {
            request_authorization();
            return null;
        }
    }


}
