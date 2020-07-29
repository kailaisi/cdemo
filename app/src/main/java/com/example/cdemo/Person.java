package com.example.cdemo;

import android.util.Log;

/**
 * 描述:
 * <p/>作者：wu
 * <br/>创建时间：2020/7/28 16:45
 */
public class Person {
    private  int age;
    private String name;

    public Person(int age, String name) {
        this.age = age;
        this.name = name;
    }

    public void print(){
        Log.e("Person",name+age+"岁了");
    }

    @Override
    public String toString() {
        return "Person{" +
                "age=" + age +
                ", name='" + name + '\'' +
                '}';
    }
}
