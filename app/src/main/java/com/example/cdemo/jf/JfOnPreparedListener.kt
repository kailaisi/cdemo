package com.example.cdemo.jf

/**
 * 描述：音频数据解码准备工作完成以后的回调方法。由C++层调用java层
 * <p/>作者：wu
 * <br/>创建时间：2020/8/2:22:13
 */
interface JfOnPreparedListener {
    /**
     * 准备完成
     */
    fun onPrepared();
}