package com.example.cdemo.jf

import kotlin.concurrent.thread

/**
 * 描述：
 * <p/>作者：wu
 * <br/>创建时间：2020/8/2:22:16
 */
class JfPlayer {
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

    var source: String? = null
    var jfOnPreparedListener: JfOnPreparedListener? = null

    fun prepared() {
        if (source.isNullOrBlank()) {
            return
        }
        thread {
            n_prepared(source!!)
        }.start()
    }

    fun start(){
        if(source.isNullOrBlank()){
            return
        }
        thread {
            n_start();
        }.start()
    }

    /**
     * c++层准备完成以后需要调用listener方法来进行通知处理
     */
    fun onCallPrepared() {
        jfOnPreparedListener?.onPrepared()
    }

    external fun n_prepared(source: String)
    external fun n_start();
}