package com.example.cdemo

/**
 * 描述：
 * <p/>作者：wu
 * <br/>创建时间：2020/7/30:22:42
 */
object JniUtils{
    init {
        System.loadLibrary("native-lib")
    }

    /**
     * 定义native方法。
     * @return true:封装格式转换成功
     */
    external fun remuxe(inpath:String,outPath:String):Boolean
}