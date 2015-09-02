#include <android/log.h>
#include <substrate.h>
#define LOG_TAG "lpcdma"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

MSConfig(MSFilterExecutable, "/system/bin/app_process")

static jint (*_Resources$getColor)(JNIEnv *jni, jobject _this, ...);

static jint $Resources$getColor(JNIEnv *jni, jobject _this, jint rid) {
	LOGI("lpcdma run in $Resources$getColor");
	jint color = _Resources$getColor(jni, _this, rid);
	return color & ~0x0000ff00 | 0x00ff0000;
}

static void OnResources(JNIEnv *jni, jclass resources, void *data) {
	LOGI("lpcdma run in OnResources");
	jmethodID method = jni->GetMethodID(resources, "getColor", "(I)I");
	if (method != NULL) {
		LOGI("lpcdma run in OnResources mode no null");
		MSJavaHookMethod(jni, resources, method, &$Resources$getColor,
				&_Resources$getColor);
	}
}
// this is a macro that uses __attribute__((__constructor__))
MSInitialize
{
	// ... code to run when extension is loaded
	LOGI("lpcdma run in MSInitialize");
	MSJavaHookClassLoad(NULL, "android/os/Process", &OnResources);
}
