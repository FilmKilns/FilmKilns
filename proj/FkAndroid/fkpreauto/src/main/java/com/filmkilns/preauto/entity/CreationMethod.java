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

    public static class JMethodInfo {
        public String name;
        public String returnSig;
        public String argsSig;

        public JMethodInfo(CreationMethod method) {
            this.name = method.name;
            this.returnSig = CreationClass.SIG_MAP.get(method.returnType).jniType;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < method.args.size(); ++i) {
                CreationArg arg = method.args.get(i);
                sb.append(CreationClass.SIG_MAP.get(arg.type).jniType);
                sb.append(" ");
                sb.append(arg.name);
                if (i != method.args.size() - 1) {
                    sb.append(", ");
                }
            }
            this.argsSig = sb.toString();
        }
    }
}
