package com.example.cdemo

import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private val TAG = "MainActivity"
    var testField = "this is test";
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
        testFIeld()
        printPerson()
        Log.d(TAG, "onCreate:newField: $testField")
        dynamicJavaFunc1()
        dynamicJavaFunc2(16)
    }

    /*静态注册的方法*/
    external fun printPerson(): Unit

    external fun testFIeld(): Unit

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun setArray(intArrayOf: IntArray): String

    /*动态注册方法*/
    external fun dynamicJavaFunc1()
    external fun dynamicJavaFunc2(i: Int): Int

    companion object {
        init {
            System.loadLibrary("native-lib")
            //加载so包是有序的一定要那么写，不过这里我是复制过来的，所以没出现问题
            System.loadLibrary("avutil-56");
            System.loadLibrary("swresample-3");
            System.loadLibrary("avcodec-58");
            System.loadLibrary("avfilter-7");
            System.loadLibrary("swscale-5");
            System.loadLibrary("avdevice-58");
            System.loadLibrary("avformat-58");
            System.loadLibrary("postproc-55");
        }
    }

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
        val path="/mnt/sdcard/a.pcm"
        playprm(path)
    }

    external fun playprm(path: String)


}
