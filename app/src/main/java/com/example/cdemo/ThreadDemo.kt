package com.example.cdemo

/**
 * 描述:和线程相关的Demo文件
 * <p/>作者：wu
 * <br/>创建时间：2020/7/31 9:54
 */
class ThreadDemo {
    init {
        System.loadLibrary("native-lib")
    }

    var onErrorListener: OnErrorListener? = null

    /**
     * 普通线程
     */
    external fun normalThread()

    /**
     * 带线程锁
     */
    external fun mutexThread()

    /**
     * 用户测试线程回调的处理
     */
    external fun callBack()
    fun onError(code: Int, msg: String) {
        onErrorListener?.onError(code, msg)
    }
}

interface OnErrorListener {
    fun onError(code: Int, msg: String)
}