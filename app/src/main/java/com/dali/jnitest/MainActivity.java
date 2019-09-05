package com.dali.jnitest;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    // 在程序开始时使用静态代码块加载'native-lib'库
    static {
        System.loadLibrary("native-lib");
    }

    // 实例的内部变量
//    private int number = 88;
//    private String message = "java：帅呆了！";

    // 静态变量
    private static double number = 55.66;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
//        Log.i("In java", testString("hello"));

//        int[] numbers = {22, 33, 33};
////        double[] results = sumAndAverage(numbers);
////        Log.i("In java, the int is ", "" + results[0]);
////        Log.i("In java, the String is", " " + results[1]);

//        modifyStaticVariable();
//        Log.i("In Java, double is ", "" + number);
//        Log.i("In Java, String is ", message);
//        nativeMethod();
//        Log.i("In Java,the number is :", "" + getIntegerObject(9999));

//        Integer[] numbers = {11, 22, 32};  // auto-box
//        Double[] results = sumAndAverage2(numbers); // auto-unbox
//        Log.i("In Java,the sum is ","" + results[0]);
//        Log.i("In Java,the average is ","" + results[1]);

        Log.i("java:", ""+getIntegerObject2(1));
        Log.i("java:", ""+getIntegerObject2(2));
        Log.i("java:", ""+anotherGetIntegerObject(11));
        Log.i("java:", ""+anotherGetIntegerObject(12));
        Log.i("java:", ""+getIntegerObject2(3));
        Log.i("java:", ""+anotherGetIntegerObject(13));
        dynamicMethod();

    }

    private native void dynamicMethod();

    // 返回给定int值的java.lang.Integer的本地方法。
    private native Integer getIntegerObject2(int number);

    // 另一个本地方法也返回带有给定int值的java.lang.Integer。
    private native Integer anotherGetIntegerObject(int number);

    // 本地函数：接收一个Integer[], 返回Double[2]，[0]为和，[1]为平均值
    private native Double[] sumAndAverage2(Integer[] numbers);

    // 本地函数，调用之后返回创建完毕的java对象实例
    // 使用给定的int返回Integer对象
    private native Integer getIntegerObject(int number);

    // 申明本地函数
    private native void nativeMethod();

    // 用于测试C代码调用
    private void callback() {
        Log.i("In Java", "");
    }

    private void callback(String message) {
        Log.i("In Java with ", message);
    }
    // 用于测试C代码调用
    private double callbackAverage(int n1, int n2) {
        return ((double)n1 + n2) / 2.0;
    }

    // Static method to be called back
    private static String callbackStatic() {
        return "From static Java method";
    }

    private native void modifyStaticVariable();

    public native void modifyInstanceVariable();

    /**
     * 本地'native-lib'库中实现了的本地方法，共享库会打包到本应用中
     */
    public native String stringFromJNI();

    //基本数据类型在c/java之间的传递
    public native double average(int arg1, int arg2);

    //字符串在c/java之间的传递
    public native String testString(String str);

    //基础数据类型的数组传递
    public native double[] sumAndAverage(int arr[]);
}
