package com.filmkilns.preauto.entity;

import com.sun.tools.javac.code.Flags;
import com.sun.tools.javac.code.Symbol;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class CreationClass {
    public static class Type {
        public Class<?> javaType;
        public String jniType;
        public String sig;
        public String cType;

        public Type(Class<?> javaType, String jniType, String sig, String cType) {
            this.javaType = javaType;
            this.jniType = jniType;
            this.sig = sig;
            this.cType = cType;
        }
    }

    public String name;
    public String path;
    public final List<CreationMethod> methods = new ArrayList<>();

    public CreationClass(Symbol.ClassSymbol symbol, String path) {
        this.name = getSimpleClassName(symbol);
        this.path = path;
        for (Symbol s : symbol.members().getSymbols()) {
            if (s instanceof Symbol.MethodSymbol && isNativeMethod((Symbol.MethodSymbol) s)) {
                this.methods.add(new CreationMethod((Symbol.MethodSymbol) s));
            }
        }
    }

    private String getSimpleClassName(Symbol.ClassSymbol symbol) {
        String name = symbol.className();
        return name.substring(name.lastIndexOf(".") + 1);
    }

    private boolean isNativeMethod(Symbol.MethodSymbol symbol) {
        return (symbol.flags() & Flags.NATIVE) != 0;
    }

    @Override
    public String toString() {
        return "CreationClass{" +
                "name='" + name + '\'' +
                ", path='" + path + '\'' +
                ", methods=" + methods +
                '}';
    }

    public static Class<?> getClassFromType(com.sun.tools.javac.code.Type type) {
        String className = type.toString().replaceAll("<.*>", "");
        try {
            if (className.equals("boolean")) {
                return boolean.class;
            } else if (className.equals("byte")) {
                return byte.class;
            } else if (className.equals("char")) {
                return char.class;
            } else if (className.equals("short")) {
                return short.class;
            } else if (className.equals("int")) {
                return int.class;
            } else if (className.equals("long")) {
                return long.class;
            } else if (className.equals("float")) {
                return float.class;
            } else if (className.equals("double")) {
                return double.class;
            } else if (className.equals("void")) {
                return void.class;
            } else if (className.equals("byte[]")) {
                return byte[].class;
            }
            return Class.forName(className);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            return Object.class;
        }
    }

    public final static Map<Class<?>, Type> SIG_MAP = Map.ofEntries(
            Map.entry(boolean.class, new Type(boolean.class, "jboolean", "Z", "bool")),
            Map.entry(byte.class, new Type(byte.class, "jbyte", "B", "uint8_t")),
            Map.entry(char.class, new Type(char.class, "jchar", "C", "char")),
            Map.entry(short.class, new Type(short.class, "jshort", "S", "uint16_t")),
            Map.entry(int.class, new Type(int.class, "jint", "I", "uint32_t")),
            Map.entry(long.class, new Type(long.class, "jlong", "J", "uint64_t")),
            Map.entry(float.class, new Type(float.class, "jfloat", "F", "float")),
            Map.entry(double.class, new Type(double.class, "jdouble", "D", "double")),
            Map.entry(void.class, new Type(void.class, "void", "V", "void")),
            Map.entry(String.class, new Type(String.class, "jstring", "Ljava/lang/String;", "std::string")),
            Map.entry(Object.class, new Type(Object.class, "jobject", "Ljava/lang/Object;", "std::any")),
            Map.entry(byte[].class, new Type(byte[].class, "jbyteArray", "[B", "uint8_t[]"))
    );
}
