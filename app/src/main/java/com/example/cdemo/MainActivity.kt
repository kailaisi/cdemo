package com.example.cdemo

import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {


    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    private val TAG = "MainActivity"
    var testField = "this is test"

    private val threadDemo by lazy {
        var threadDemo1 = ThreadDemo()
        threadDemo1.onErrorListener = object : OnErrorListener {
            override fun onError(code: Int, msg: String) {
                println("code:$code,msg:$msg")
            }
        }
        threadDemo1
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        // Example of a call to a native method
        sample_text.text = stringFromJNI()
        testField()
        printPerson()
        Log.d(TAG, "onCreate:newField: $testField")
        dynamicJavaFunc1()
        dynamicJavaFunc2(16)
    }

    external fun testField(): Unit

    /*静态注册的方法*/
    external fun printPerson(): Unit

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun setArray(intArrayOf: IntArray): String

    /*动态注册方法*/
    external fun dynamicJavaFunc1()
    external fun dynamicJavaFunc2(i: Int): Int


    fun normal(view: View) {
        threadDemo.normalThread()
    }

    fun mutexThread(view: View) {
        threadDemo.mutexThread()
    }

    fun callJavaMethod(view: View) {
        threadDemo.callBack()
    }

    /**
     * 播放prm音频文件
     */
    fun playPrm(view: View) {
        val path = "/mnt/sdcard/audio_long8.pcm"
        playprm(path)
    }

    external fun playprm(path: String)

}
