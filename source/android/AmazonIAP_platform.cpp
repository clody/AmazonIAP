/*
 * android-specific implementation of the AmazonIAP extension.
 * Add any platform-specific functionality here.
 */
/*
 * NOTE: This file was originally written by the extension builder, but will not
 * be overwritten (unless --force is specified) and is intended to be modified.
 */
#include "AmazonIAP_internal.h"

#include "s3eEdk.h"
#include "s3eEdk_android.h"
#include <jni.h>
#include "IwDebug.h"

static jobject g_Obj;
static jmethodID g_AmazonIAPStartPurchase;

int g_Result = 0;

static char* jstringToNative(jstring str)
{
    JNIEnv* env=s3eEdkJNIGetEnv();
    jsize len=env->GetStringLength(str);
    const char* utf=env->GetStringUTFChars(str,NULL);
    char* retval=new char[len+1];
    memcpy(retval,utf,len+1); //faster strcpy
    env->ReleaseStringUTFChars(str,utf);
    return retval;
}

//----------------------------------------------------------------------------------------
// Purchase Result Callbacks
//----------------------------------------------------------------------------------------
void AmazonIAPPurchaseSuccessfulCallback(JNIEnv* env, jobject obj, jstring productID)
{
    s3eDebugOutputString("AmazonIAPPurchaseSuccessfulCallback <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    s3eEdkCallbacksEnqueue(S3E_EXT_AMAZONIAP_HASH, AMAZONIAP_PURCHASE_SUCCESSFUL, jstringToNative(productID), 0);
}

void AmazonIAPPurchaseFailedCallback(JNIEnv* env, jobject obj, jstring productID)
{
    s3eDebugOutputString("AmazonIAPPurchaseFailedCallback <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    s3eEdkCallbacksEnqueue(S3E_EXT_AMAZONIAP_HASH, AMAZONIAP_PURCHASE_FAILED, jstringToNative(productID), 0);
}


s3eResult AmazonIAPInit_platform()
{
    // Get the environment from the pointer
    JNIEnv* env = s3eEdkJNIGetEnv();
    jobject obj = NULL;
    jmethodID cons = NULL;

    const JNINativeMethod nativeMethodDefs[] =
    {
      { "AmazonIAPPurchaseSuccessfulCallback",				"(Ljava/lang/String;)V",			(void*)&AmazonIAPPurchaseSuccessfulCallback },
      { "AmazonIAPPurchaseFailedCallback",				"(Ljava/lang/String;)V",			(void*)&AmazonIAPPurchaseFailedCallback },
    };
    
    // Get the extension class
    jclass cls = s3eEdkAndroidFindClass("AmazonIAP");
    if (!cls)
        goto fail;

    // Get its constructor
    cons = env->GetMethodID(cls, "<init>", "()V");
    if (!cons)
        goto fail;

    // Construct the java class
    obj = env->NewObject(cls, cons);
    if (!obj)
        goto fail;

    // Get all the extension methods
    g_AmazonIAPStartPurchase = env->GetMethodID(cls, "AmazonIAPStartPurchase", "(Ljava/lang/String;)I");
    if (!g_AmazonIAPStartPurchase)
        goto fail;
        
    if(env->RegisterNatives(cls, nativeMethodDefs, sizeof(nativeMethodDefs)/sizeof(nativeMethodDefs[0])))
        goto fail;



    IwTrace(AMAZONIAP, ("AMAZONIAP init success"));
    g_Obj = env->NewGlobalRef(obj);
    env->DeleteLocalRef(obj);
    env->DeleteGlobalRef(cls);

    // Add any platform-specific initialisation code here
    return S3E_RESULT_SUCCESS;

fail:
    jthrowable exc = env->ExceptionOccurred();
    if (exc)
    {
        env->ExceptionDescribe();
        env->ExceptionClear();
        IwTrace(AmazonIAP, ("One or more java methods could not be found"));
    }
    return S3E_RESULT_ERROR;

}

void AmazonIAPTerminate_platform()
{
    // Add any platform-specific termination code here
}

s3eResult AmazonIAPStartPurchase_platform(const char* productID)
{
    JNIEnv* env = s3eEdkJNIGetEnv();
    jstring productID_jstr = env->NewStringUTF(productID);
    return (s3eResult)env->CallIntMethod(g_Obj, g_AmazonIAPStartPurchase, productID_jstr);
}
