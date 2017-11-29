package com.example.corwinperren.cs496androidui;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button linear_vert_layout_button = (Button) findViewById(R.id.linear_vert_layout_button);
        linear_vert_layout_button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, LinearVertActivity.class);
                startActivity(intent);
            }
        });

        Button linear_horiz_layout_button = (Button) findViewById(R.id.linear_horiz_layout_button);
        linear_horiz_layout_button.setOnClickListener(new View.OnClickListener(){

                @Override
                public void onClick(View v){
                    Intent intent = new Intent(MainActivity.this, LinearHorizActivity.class);
                    startActivity(intent);
                }
        });

        Button grid_layout_button = (Button) findViewById(R.id.grid_layout_button);
        grid_layout_button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, GridActivity.class);
                startActivity(intent);
            }
        });

        Button relative_layout_button = (Button) findViewById(R.id.relative_layout_button);
        relative_layout_button.setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, RelativeActivity.class);
                startActivity(intent);
            }
        });
    }
}
