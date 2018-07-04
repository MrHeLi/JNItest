#include <jni.h>
#include <iostream>
#include <string>
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "In C/C++", __VA_ARGS__);
using namespace std;

extern "C"
JNIEXPORT jstring JNICALL
Java_com_dali_jnitest_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jdouble JNICALL
Java_com_dali_jnitest_MainActivity_average(JNIEnv *env, jobject instance, jint arg1,
                                           jint arg2) {

    jdouble result;//基本数据类型无需变化，在jni.h中已经设置了类型别名
    //默认情况下，下面的打印无效
//    printf("in c, the numbers is %d and %d\n", arg1, arg2);
//    cout << "In C++, the numbers are " << arg1 << " and " << arg2 << endl;
    LOGI("arg1: %d, ar2: %d", arg1, arg2);
    result = (arg1 + arg2) / 2;
    return result;

}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_dali_jnitest_MainActivity_testString(JNIEnv *env, jobject instance, jstring str_) {
    //在C中的String是以NULL结尾的字符串数组，需要通过特定方法转换，但在C++中有对应的String，是否可以不转换
    const char *str = env->GetStringUTFChars(str_, 0);
    LOGI("str: %s", str);
    char returnValue[128] = "jianli ,wo lai le";
    env->ReleaseStringUTFChars(str_, str);

    return env->NewStringUTF(returnValue);
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_dali_jnitest_MainActivity_sumAndAverage(JNIEnv *env, jobject instance,
                                                               jintArray arr_) {
    //从JNI参数接收数组数据，转换为C代码的本地数据（例如，jint[]）。
    jint *arr = env->GetIntArrayElements(arr_, NULL);
    if (NULL == arr) return NULL;
    jsize length = env->GetArrayLength(arr_);
    // 执行程序
    jint sum = 0;
    int i;
    for (i = 0; i < length; i++) {
        sum += arr[i];
    }
    jdouble average = (jdouble)sum / length;
    env->ReleaseIntArrayElements(arr_, arr, 0); // release resources

    jdouble outCArray[] = {(jdouble)sum, average};
    // 将本地C代码的数据（例如，jdouble[]）转换为JNI数组（例如，jdoubleArray）,并且返回JNI数据。
    jdoubleArray outJNIArray = env->NewDoubleArray(2);  // allocate
    if (NULL == outJNIArray) return NULL;
    env->SetDoubleArrayRegion(outJNIArray, 0 , 2, outCArray);  // copy
    return outJNIArray;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_dali_jnitest_MainActivity_modifyInstanceVariable(JNIEnv *env, jobject instance) {

    // 获得调用java层对象的引用
    jclass thisClass = env->GetObjectClass(instance);

    // int
    // 获取调用java层对象中number变量的fieldID
    jfieldID fidNumber = env->GetFieldID(thisClass, "number", "I");
    if (NULL == fidNumber) return;

    // 通过fieldID获取number变量中的值
    jint number = env->GetIntField(instance, fidNumber);
    LOGI("In C, the int is %d\n", number);

    // 修改变量
    number = 99;
    env->SetIntField(instance, fidNumber, number);

    // 获取调用java层对象中message变量的fieldID
    jfieldID fidMessage = env->GetFieldID(thisClass, "message", "Ljava/lang/String;");
    if (NULL == fidMessage) return;

    // String
    // 通过fieldID获取object变量中的值
    jstring message = static_cast<jstring>(env->GetObjectField(instance, fidMessage));

    // 通过JNI字符串创建C代码字符串=
    const char *cStr = env->GetStringUTFChars(message, NULL);
    if (NULL == cStr) return;

    LOGI("In C, the string is %s\n", cStr);
    env->ReleaseStringUTFChars(message, cStr);

    //创建C代码字符串，并分配给JNI字符串
    message = env->NewStringUTF("C：不你很蠢");
    if (NULL == message) return;

    // 修改实例变量
    env->SetObjectField(instance, fidMessage, message);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_dali_jnitest_MainActivity_modifyStaticVariable(JNIEnv *env, jobject instance) {

    jclass cls = env->GetObjectClass(instance);
    jfieldID fieldID = env->GetStaticFieldID(cls, "number", "D");
    if (fieldID == NULL) return;
    jdouble number = env->GetStaticDoubleField(cls, fieldID);
    LOGI("In C, the double is %f\n", number);
    number = 77.88;
    env->SetStaticDoubleField(cls, fieldID, number);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_dali_jnitest_MainActivity_nativeMethod(JNIEnv *env, jobject instance) {

    // 获取java层调用的class对象的引用
    jclass thisClass = env->GetObjectClass(instance);

    // 获取Method ID 用于调用“callback”函数，该函数没有参数，无返回值
    jmethodID midCallBack = env->GetMethodID(thisClass, "callback", "()V");
    if (NULL == midCallBack) return;
    LOGI("In C, call back Java's callback()\n");
    // 根据Method ID 调用该函数（无返回值）
    env->CallVoidMethod(instance, midCallBack);

    jmethodID midCallBackStr = env->GetMethodID(thisClass, "callback", "(Ljava/lang/String;)V");
    if (NULL == midCallBackStr) return;
    LOGI("In C, call back Java's called(String)\n");
    jstring message = env->NewStringUTF("Hello from C");
    env->CallVoidMethod(instance, midCallBackStr, message);

    jmethodID midCallBackAverage = env->GetMethodID(thisClass, "callbackAverage", "(II)D");
    if (NULL == midCallBackAverage) return;
    jdouble average = env->CallDoubleMethod(instance, midCallBackAverage, 2, 3);
    LOGI("In C, the average is %f\n", average);

    jmethodID midCallBackStatic = env->GetStaticMethodID(thisClass, "callbackStatic", "()Ljava/lang/String;");
    if (NULL == midCallBackStatic) return;
    jstring resultJNIStr = static_cast<jstring>(env->CallStaticObjectMethod(thisClass, midCallBackStatic));
    const char *resultCStr = env->GetStringUTFChars(resultJNIStr, NULL);
    if (NULL == resultCStr) return;
    LOGI("In C, the returned string is %s\n", resultCStr);
    env->ReleaseStringUTFChars(resultJNIStr, resultCStr);

}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_dali_jnitest_MainActivity_getIntegerObject(JNIEnv *env, jobject instance, jint number) {

    // 获取java.lang.Integer类的引用
    jclass cls = env->FindClass("java/lang/Integer");

    // 获取Integer的构造函数Method ID,使用int参数
    jmethodID midInit = env->GetMethodID(cls, "<init>", "(I)V");
    if (NULL == midInit) return NULL;
    // 使用jint参数调用构造函数，分配新的实例
    jobject newObj = env->NewObject(cls, midInit, number);

    // 调用String的toString()方法调用打印
    jmethodID midToString = env->GetMethodID(cls, "toString", "()Ljava/lang/String;");
    if (NULL == midToString) return NULL;
    jstring resultStr = static_cast<jstring>(env->CallObjectMethod(newObj, midToString));
    const char *resultCStr = env->GetStringUTFChars(resultStr, NULL);
    LOGI("In C: the number is %s\n", resultCStr);

    return newObj;

}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_dali_jnitest_MainActivity_sumAndAverage2(JNIEnv *env,jobject instance,jobjectArray numbers) {
    // 获取java.lang.Integer类的引用
    jclass classInteger = env->FindClass("java/lang/Integer");
    // 使用Integer.intValue() 函数
    jmethodID midIntValue = env->GetMethodID(classInteger, "intValue", "()I");
    if (NULL == midIntValue) return NULL;

    // 获取数据中每个元素Get the value of each Integer object in the array
    jsize length = env->GetArrayLength(numbers);
    jint sum = 0;
    int i;
    for (i = 0; i < length; i++) {
        jobject objInteger = env->GetObjectArrayElement(numbers, i);
        if (NULL == objInteger) return NULL;
        jint value = env->CallIntMethod(objInteger, midIntValue);
        sum += value;
    }
    double average = (double)sum / length;
    LOGI("In C, the sum is %d\n", sum);
    LOGI("In C, the average is %f\n", average);

    // 获取java.lang.Double类的引用
    jclass classDouble = env->FindClass("java/lang/Double");

    // 创建一个长度为2的java.lang.Double类型数组（）jobjectArray
    jobjectArray outJNIArray = env->NewObjectArray(2, classDouble, NULL);

    // 通过构造函数构造两个Double类型对象
    jmethodID midDoubleInit = env->GetMethodID(classDouble, "<init>", "(D)V");
    if (NULL == midDoubleInit) return NULL;
    jobject objSum = env->NewObject(classDouble, midDoubleInit, (double)sum);
    jobject objAve = env->NewObject(classDouble, midDoubleInit, average);
    // 初始化jobjectArray
    env->SetObjectArrayElement(outJNIArray, 0, objSum);
    env->SetObjectArrayElement(outJNIArray, 1, objAve);

    return outJNIArray;
}

// java类"java.lang.Integer"的全局引用
static jclass classInteger;
static jmethodID midIntegerInit;

jobject getInteger(JNIEnv *env, jobject instance, jint number) {

    // 如果classInteger为NULL，获取java.lang.Integer的类引用
    if (NULL == classInteger) {
        LOGI("Find java.lang.Integer\n");
        classInteger = env->FindClass("java/lang/Integer");
        // 不加下面这行会出现报错：jni error (app bug): accessed stale local reference
        classInteger = static_cast<jclass>(env->NewGlobalRef(classInteger));
//        env->DeleteLocalRef(classInteger);

    }
    if (NULL == classInteger) return NULL;

    // 如果midIntegerInit为空，获取Integer构造函数的method ID
    if (NULL == midIntegerInit) {
        LOGI("Get Method ID for java.lang.Integer's constructor\n");
        midIntegerInit = env->GetMethodID(classInteger, "<init>", "(I)V");
    }
    if (NULL == midIntegerInit) return NULL;

    // 调用构造函数以使用int参数分配新实例
    jobject newObj = env->NewObject(classInteger, midIntegerInit, number);
    LOGI("In C, constructed java.lang.Integer with number %d\n", number);
    return newObj;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_dali_jnitest_MainActivity_getIntegerObject2(JNIEnv *env, jobject instance, jint number) {
    return getInteger(env, instance, number);
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_dali_jnitest_MainActivity_anotherGetIntegerObject(JNIEnv *env, jobject instance,jint number) {
    return getInteger(env, instance, number);
}
