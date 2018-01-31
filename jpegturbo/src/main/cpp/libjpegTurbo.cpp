#include <jni.h>
#include <stdio.h>
#include <android/log.h>
#include "include/turbojpeg.h"
#include "include/libyuv.h"

#define LOG_TAG "turbojpeg"
#define LOGW(...)  __android_log_write(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void cnv212yuv420(unsigned char *Src_data, unsigned char *Dst_data,
                       int src_width, int src_height) {

    int NV12_Y_Size = src_width * src_height;

    //dst:YUV420 video size
    int I420_Y_Size = src_width * src_height;
    int I420_U_Size = (src_width >> 1) * (src_height >> 1);

    // video format transformation process
    unsigned char *Y_data_Src = Src_data;
    unsigned char *UV_data_Src = Src_data + NV12_Y_Size;
    int src_stride_y = src_width;
    int src_stride_uv = src_width;

    unsigned char *Y_data_Dst = Dst_data;
    unsigned char *U_data_Dst = Dst_data + I420_Y_Size;
    unsigned char *V_data_Dst = Dst_data + I420_Y_Size + I420_U_Size;

    int Dst_Stride_Y = src_width;
    int Dst_Stride_U = src_width >> 1;
    int Dst_Stride_V = Dst_Stride_U;
    libyuv::NV21ToI420(Y_data_Src, src_stride_y,
                       UV_data_Src, src_stride_uv,
                       Y_data_Dst, Dst_Stride_Y,
                       U_data_Dst, Dst_Stride_U,
                       V_data_Dst, Dst_Stride_V,
                       src_width, src_height);
}

extern "C"
JNIEXPORT jint JNICALL Java_com_cbly_jpegturbo_TurboJpegLib_yuv2jpeg
  (JNIEnv * env, jclass jclazz, jbyteArray jNv21,jbyteArray jI420, jint width, jint height,
		  jint subsample, jbyteArray jpeg,jint quality,jint flags){

	tjhandle handle = NULL;
	int padding = 1; // 1或4均可，但不能是0
	int ret = 0;

	if((handle=tjInitCompress())==NULL){
		LOGI("tjInitCompress error");
		return 1001;
	}

//	if(env->GetArrayLength(jpeg) < (jsize)tjBufSizeYUV2(width, padding, height, subsample)){
//		LOGI("Destination buffer is not large enough");
//		return 1002;
//	}

	unsigned char *srcBuf = (unsigned char *)env->GetPrimitiveArrayCritical(jNv21, 0);
    unsigned char *i420Buf = (unsigned char *)env->GetPrimitiveArrayCritical(jI420, 0);

    cnv212yuv420(srcBuf,i420Buf,width,height);

	unsigned char *jpegBuf = (unsigned char *)env->GetPrimitiveArrayCritical(jpeg, 0);
	unsigned long compressSize;

	ret = tjCompressFromYUV(handle, i420Buf, width, padding, height,
			subsample, &jpegBuf, &compressSize, quality, flags);

	tjDestroy(handle);
	env->ReleasePrimitiveArrayCritical(jNv21, srcBuf, 0);
	env->ReleasePrimitiveArrayCritical(jI420, i420Buf, 0);
	env->ReleasePrimitiveArrayCritical(jpeg, jpegBuf, 0);
//	tjFree(jpegBuf);
//	tjFree(srcBuf);
	jpegBuf=srcBuf=NULL;

	if (ret < 0) {
		LOGI("compress to jpeg failed: %d\n",ret);
		return ret;
	}
	ret = compressSize;
	return ret;
}
