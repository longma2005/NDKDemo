package com.smile.sample.ndkdemo;

/**
 * Created by xuzhiming02 on 2016/2/18.
 */
public class JniUtils {
    static {
        System.loadLibrary("JniDemo");
    }
    public static native String getStringFromC();
}
