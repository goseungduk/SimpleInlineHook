package com.sdssd.sdaad

import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.sdssd.sdaad.ui.theme.AOS_Simple_Inline_HookTheme

class MainActivity : ComponentActivity() {
    init {
        System.loadLibrary("tester");
    }
    external fun hookcheck();
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        var buttonView = findViewById<Button>(R.id.button);
        buttonView.setOnClickListener(View.OnClickListener {
            hookcheck();
        });
    }
}
