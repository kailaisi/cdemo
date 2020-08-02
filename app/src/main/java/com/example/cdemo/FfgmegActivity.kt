package com.example.cdemo

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import kotlin.concurrent.thread

/**
 * 描述：音视频使用的文件
 * <p/>作者：wu
 * <br/>创建时间：2020/7/30:22:46
 */
class FfgmegActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        thread {
        }
    }
}
