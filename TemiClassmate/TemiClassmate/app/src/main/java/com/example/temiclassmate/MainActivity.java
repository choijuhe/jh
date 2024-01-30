package com.example.temiclassmate;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {
    Button btnFS, btnCA, btnAt;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Context ccontext = getApplicationContext();
        ActionBar actionBar = getSupportActionBar();
        actionBar.hide();
        btnAt = findViewById(R.id.body_btn1);
        btnCA = findViewById(R.id.body_btn2);
        btnFS = findViewById(R.id.body_btn3);
        btnAt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ccontext, AttendanceActivity.class);
                startActivity(intent);
            }
        });
        btnCA.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ccontext, ClassAssistantActivity.class);
                startActivity(intent);
            }
        });
        btnFS.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(ccontext, FacilityServiceActivity.class);
                startActivity(intent);
            }
        });
    }
}