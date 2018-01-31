package com.cbly.jpegturbo;

public class TurboJpegLib {

	static{
		System.loadLibrary("jni_jpeg");
	}
	
	public static native int yuv2jpeg(byte[] nv21,byte[] i420,int w,int h,int subsample,byte[] jpeg ,int quality,int flags);
}
