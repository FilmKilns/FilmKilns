package com.filmkilns.preauto.entity;

import com.sun.tools.javac.code.Symbol;

import java.util.ArrayList;
import java.util.List;

public class CreationMethod {
    public String name;
    public Class<?> returnType;
    public final List<CreationArg> args = new ArrayList<>();

    public CreationMethod(Symbol.MethodSymbol symbol) {
        this.name = symbol.getSimpleName().toString();
        this.returnType = CreationClass.getClassFromType(symbol.getReturnType());
        for (Symbol.VarSymbol s : symbol.params) {
            this.args.add(new CreationArg(s));
        }
    }

    @Override
    public String toString() {
        return "CreationMethod{" +
                "name='" + name + '\'' +
                ", returnType=" + returnType +
                ", args=" + args +
                '}';
    }

    private static String getInternalArgName(CreationArg arg) {
        if (arg.type == boolean.class) {
            return arg.name + " = JNI_TRUE";
        } else if (arg.type == byte.class || arg.type == char.class || arg.type == short.class
                || arg.type == int.class || arg.type == long.class|| arg.type == float.class
                || arg.type == double.class || arg.type == Object.class) {
            return arg.name;
        } else if (arg.type == String.class) {
            return "FkJString::create(env, " + arg.name + ")";
        } else if (arg.type == byte[].class) {
            return "FkJBuffer::create(env, " + arg.name + ")";
        }
        return "_" + arg.name;
    }

    private static String[] getInternalReturnCast(Class<?> returnType, String returnJType) {
        String[] cast = new String[]{"", ""};
        if (returnType == boolean.class) {
            cast[1] = "? JNI_TRUE : JNI_FALSE";
        } else if (returnType == byte.class || returnType == char.class || returnType == short.class
                || returnType == int.class || returnType == long.class|| returnType == float.class
                || returnType == double.class) {
            cast[0] = "(" + returnJType + ")";
            cast[1] = "";
        }
        return cast;
    }

    public static class JMethodInfo {
        public String name;
        public String returnType;
        public String returnCType;
        public String returnSig;
        public String returnCastPrefix;
        public String returnCastSuffix;
        public String args;
        public String argsC;
        public String argsCOnlyName;
        public String argsSig;
        public boolean hasReturn;

        public JMethodInfo(CreationMethod method) {
            this.name = method.name;
            this.returnType = CreationClass.SIG_MAP.get(method.returnType).jniType;
            this.returnCType = CreationClass.SIG_MAP.get(method.returnType).cType;
            this.returnSig = CreationClass.SIG_MAP.get(method.returnType).sig;
            String[] cast = getInternalReturnCast(method.returnType, this.returnType);
            this.returnCastPrefix = cast[0];
            this.returnCastSuffix = cast[1];
            StringBuilder sb = new StringBuilder();
            StringBuilder sbC = new StringBuilder();
            StringBuilder sbCOnlyName = new StringBuilder();
            StringBuilder sbSig = new StringBuilder();
            for (int i = 0; i < method.args.size(); ++i) {
                CreationArg arg = method.args.get(i);
                sbSig.append(CreationClass.SIG_MAP.get(arg.type).sig);
                sb.append(CreationClass.SIG_MAP.get(arg.type).jniType);
                sb.append(" ");
                sb.append(arg.name);

                sbC.append(CreationClass.SIG_MAP.get(arg.type).cType);
                sbC.append(" ");
                sbC.append(arg.name);

                sbCOnlyName.append(getInternalArgName(arg));
                if (i != method.args.size() - 1) {
                    sb.append(", ");
                    sbC.append(", ");
                    sbCOnlyName.append(", ");
                }
            }
            this.args = sb.toString();
            this.argsC = sbC.toString();
            this.argsCOnlyName = sbCOnlyName.toString();
            this.argsSig = sbSig.toString();
            this.hasReturn = method.returnType != void.class;
        }
    }
}
