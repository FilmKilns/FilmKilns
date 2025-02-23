package com.filmkilns.preauto.entity;

import com.sun.tools.javac.code.Symbol;

public class CreationArg {
    public String name;
    public Class<?> type;
    public String sig;

    public CreationArg(Symbol.VarSymbol symbol) {
        this.name = symbol.name.toString();
        this.type = CreationClass.getClassFromType(symbol.type);
        if (this.type == Object.class) {
            this.sig = "L" + CreationClass.getCanonicalName(symbol.type).replaceAll("\\.", "/") + ";";
        } else {
            this.sig = CreationClass.SIG_MAP.get(this.type).sig;
        }
    }

    @Override
    public String toString() {
        return "CreationArg{" +
                "name='" + name + '\'' +
                ", type=" + type.getSimpleName() +
                '}';
    }
}
