package com.example.cdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private val TAG = "MainActivity"
    var testField="this is test";
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        // Example of a call to a native method
        sample_text.text = stringFromJNI()
        sample_text.text=setArray(intArrayOf(1,2))
        testFIeld()
        Log.d(TAG, "onCreate:newField: $testField")
    }

    external fun testFIeld():Unit
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun setArray(intArrayOf: IntArray):String
    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
