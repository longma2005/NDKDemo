#Android Studio NDK开发步骤

用Android Studio进行NDK开发比Eclipse更加灵活方便。

## 环境搭建
下载安装Android NDK
地址：http://developer.android.com/sdk/ndk/index.html
Android Studio里面设置NDK路径：

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-db54db12d59f3d42.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


## 新建NDKDemo项目

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-2da83b5e223280cc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

新建一个类JniUtils用来实现native方法：

```
public class JniUtils {
    public static native String getStringFromC();
}
```

Rebuild Project

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-9c66e9c875e70ba6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

此时会生成JniUtils.class

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-5d079554c7ae14d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

然后在命令窗口定位到classes\debug目录下，执行命令**（注意区分大小写）**

```
javah -jni com.smile.sample.ndkdemo.JniUtils
```

此时会在debug目录下生成文件 com_smile_sample_ndkdemo_JniUtils.h

在app模块下，新建jni目录，并把头文件放到这个目录下。

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-ff0e3e8fa52964d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

新建一个cpp文件JniUtils.cpp来实现方法。

此时需要做几个操作：
1. 下载和配置NDK路径
2. 修改build.gradle配置，在defaultConfig里面新增一下代码
```
   ndk {
        moduleName "JniDemo"
        abiFilters "armeabi", "armeabi-v7a", "x86"
    }
```
3. 在gradle.properties里面配置

```
android.useDeprecatedNdk=true
```
4. Jni目录下，新建一个cpp的空文件
> 解决方案：
> 这是NDK在Windows下一个bug，当只编译一个文件时出现，解决方法就是再添加一个空的文件即可。
> 原文见http://ph0b.com/android-studio-gradle-and-ndk-integration/：

5. 在JniUtils.cpp里面输入以下代码：

```
#include "com_smile_sample_ndkdemo_JniUtils.h"

JNIEXPORT jstring JNICALL Java_com_smile_sample_ndkdemo_JniUtils_getStringFromC
        (JNIEnv * env, jclass obj){
    return env->NewStringUTF("这里是C++代码");
}
```

Rebuild Project生成.so文件

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-4c10a3c8cf44c6db.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在JniUtils里面，加入加载so文件代码。

```
static {
    System.loadLibrary("JniDemo");
}
```

到此，代码已经编写完成，运行程序看看吧。运行 

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-5d85bdb76a983479.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在app\main下新建jniLibs目录，然后把刚才生成的.so文件夹拷贝到这里，删除jni目录，Rebuild Project 运行依然正常。


##常见问题
1. 如果是JniUtils.c，则代码不一样；

```
#include "com_smile_sample_ndkdemo_JniUtils.h"

JNIEXPORT jstring JNICALL Java_com_smile_sample_ndkdemo_JniUtils_getStringFromC
        (JNIEnv * env, jclass obj){
    return (*env)->NewStringUTF(env, "这里是C代码");
}
```
  
2. 编译错误no rule to make target

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-8d88929a2c5d2582.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
解决办法：
这是因为我们jni目录下只有个cpp文件，新建一个空的cpp文件即可。这个是NDK在Windows下的一个bug.
  
3. 编译错误Error: NDK integration is deprecated in the current plugin.

![Paste_Image.png](http://upload-images.jianshu.io/upload_images/1616478-289d42606d9fe3e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

解决办法：
在gradle.properties里面新增
>android.useDeprecatedNdk=true

4. 运行错误java.lang.UnsatisfiedLinkError: Native method not found
找不到native方法，检查下是否忘记加System.loadLibrary或者库名写错了。
库名称要跟gradle里面配置的一致。

```
    static {
        System.loadLibrary("JniDemo");
    }
```
```
   ndk {
            moduleName "JniDemo"
            abiFilters "armeabi", "armeabi-v7a", "x86"
    }
```