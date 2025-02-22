package com.filmkilns.preauto.processor;

import com.filmkilns.annotation.FkNativeAuto;
import com.filmkilns.preauto.entity.CreationClass;
import com.filmkilns.preauto.entity.CreationMethod;
import com.google.auto.service.AutoService;
import com.google.common.collect.ImmutableSet;
import com.sun.tools.javac.code.Symbol;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;
import org.apache.velocity.runtime.RuntimeConstants;
import org.apache.velocity.util.introspection.UberspectImpl;
import org.apache.velocity.util.introspection.UberspectPublicFields;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import javax.annotation.processing.ProcessingEnvironment;
import javax.annotation.processing.Processor;
import javax.annotation.processing.RoundEnvironment;
import javax.lang.model.element.Element;
import javax.lang.model.element.ElementKind;
import javax.lang.model.element.TypeElement;

@AutoService(Processor.class)
public class FkNativeAutoProcessor extends FkAbsProcessor {
    private final static String TAG = "FkNativeAutoProcessor";
    private VelocityEngine engine;

    @Override
    public synchronized void init(ProcessingEnvironment processingEnvironment) {
        super.init(processingEnvironment);
        logI(TAG, "init: projectDir=" + getProjectDir() + ", mainDir=" + getSourceMainDir());
        engine = new VelocityEngine();
        engine.setProperty(RuntimeConstants.FILE_RESOURCE_LOADER_PATH, new File(getProjectDir(), "templates/").getAbsolutePath());
        engine.setProperty(RuntimeConstants.UBERSPECT_CLASSNAME, UberspectImpl.class.getName() + ", " + UberspectPublicFields.class.getName());
        engine.init();
    }

    private List<CreationClass> getCreationItems(RoundEnvironment env) {
        Set<? extends Element> elements = env.getElementsAnnotatedWith(FkNativeAuto.class);
        List<CreationClass> items = new ArrayList<>();
        for (Element e : elements) {
            if (e.getKind() == ElementKind.CLASS) {
                Symbol.ClassSymbol symbol = (Symbol.ClassSymbol) e;
                FkNativeAuto ann = symbol.getAnnotation(FkNativeAuto.class);
                items.add(new CreationClass(symbol, ann.path()));
            }
        }
        return items;
    }

    private void generate(CreationClass item) {
        String dirStr = getSourceMainDir() + "/" + item.path;
        File dir = new File(dirStr);
        if (!dir.exists()) {
            dir.mkdirs();
        }
        generateHeader(item, new File(dir,  "Java_" + item.name + ".h"));
    }

    private Template createTemplate(String tempName) {
        return engine.getTemplate(tempName);
    }

    private void generateHeader(CreationClass item, File file) {
        if (file.exists()) {
            file.delete();
        }
        List<CreationMethod.JMethodInfo> classMethods = new ArrayList<>();
        for (CreationMethod it : item.methods) {
            classMethods.add(new CreationMethod.JMethodInfo(it));
        }
        VelocityContext ctx = new VelocityContext();
        ctx.put("className", item.name);
        ctx.put("classMethodCnt", item.methods.size());
        ctx.put("classMethods", classMethods);
        try {
            FileWriter writer = new FileWriter(file);
            Template template = createTemplate("FkNativeInterface.temp");
            template.merge(ctx, writer);
            writer.flush();
            writer.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    private void generateCPP() {

    }

    @Override
    public boolean process(Set<? extends TypeElement> set, RoundEnvironment env) {
        logI(TAG, "process");
        List<CreationClass> items = getCreationItems(env);
        logI(TAG, "items: " + items);
        for (CreationClass it : items) {
            generate(it);
        }
        return false;
    }

    @Override
    public Set<String> getSupportedAnnotationTypes() {
        return ImmutableSet.of(FkNativeAuto.class.getCanonicalName());
    }
}
