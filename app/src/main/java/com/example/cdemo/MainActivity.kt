package com.example.cdemo

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Environment
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.example.cdemo.jf.JfOnPreparedListener
import com.example.cdemo.jf.JfPlayer
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File


class MainActivity : AppCompatActivity() {
    lateinit var jfPlayer:JfPlayer

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
        requestPermission()
        // Example of a call to a native method
        sample_text.text = stringFromJNI()
        testField()
        printPerson()
        Log.d(TAG, "onCreate:newField: $testField")
        dynamicJavaFunc1()
        dynamicJavaFunc2(16)
        jfPlayer= JfPlayer();
        jfPlayer.jfOnPreparedListener=object:JfOnPreparedListener{
            override fun onPrepared() {
                jfPlayer.start()
            }

        }
    }

    external fun testField(): Unit

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
        var sd = Environment.getExternalStorageDirectory().path + "/queue/audio_long8.pcm"
        var file = File("/storage/sdcard0/audio_long8.pcm")
        println("path:$sd, ${file.exists()}")
        playprm(file.absolutePath)
    }

    external fun playprm(path: String)
    fun requestPermission() {
        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            ) != PackageManager.PERMISSION_GRANTED ||
            ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.READ_EXTERNAL_STORAGE
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            Toast.makeText(this, "申请权限", Toast.LENGTH_SHORT).show()

            // 申请 相机 麦克风权限
            ActivityCompat.requestPermissions(
                this, arrayOf(
                    Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.READ_EXTERNAL_STORAGE
                ), 100
            )
        }
    }

    /**
     * 调用ffm解析文件然后播放的功能
     */
    fun ffmplay(view: View) {
        jfPlayer.source="${Environment.getExternalStorageDirectory()}/queue/jjdk.mp4"
        Log.d(TAG,"Environment.getExternalStorageDirectory() + File.separator  === " + Environment.getExternalStorageDirectory() + File.separator + "");
        jfPlayer.prepared();
    }
}
