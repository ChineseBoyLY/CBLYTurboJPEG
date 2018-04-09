# CBLYTurboJPEG
一个快速的图片压缩库，各个版本Turbo jpeg 的jni实现，以及libyuv的转码库

建议编译的时候最新采用最新的版本libjpeg-turbo-1.5.2，默认mk文件里面已经采用了此版本，老的版本里面好像没有tjCompressFromYUV函数。


jni可以编译出libyuv.a 和 libturbo.a

jpegtubo 可以编译出在Android中使用的so库

真心快到没有朋友！！！

使用示例：
```
private byte[] mTempI420 = null;
private byte[] mMaxJpeg = null;

public void onPreviewFrame(byte[] nv21,xxxx) {

    if(mTempNv21 == null) {
	mTempNv21 = new byte[nv21.length]
    }

    if(mMaxJpeg == null) {
	mMaxJpeg = new byte[nv21.length * 2]
    }

    int compressSize = TurboJpegLib.yuv2jpeg(nv21, mTempI420, previewW, previewH, 
						TJ.SAMP_420, mMaxJpegData, 70, TJ.FLAG_BOTTOMUP);
     
    // 下方返回的结果就是最终把nv21帧数据压缩后的jpeg的二进制字节
    byte[] jpegDataOut = Arrays.copyOfRange(mMaxJpegData, 0, compressSize);

}
```
jni层还可以扩展很多函数，欢迎大家一起来挖掘完善学习。


