package com.alimin.fk_flutter.module.image

import android.content.Context
import android.graphics.Rect
import android.util.Size
import android.view.Surface
import com.alimin.fk.core.FkAbsImageSource
import com.alimin.fk.core.FkPaint
import com.alimin.fk.entity.FkRational
import com.alimin.fk.entity.FkResult
import com.alimin.fk.listener.FkDoStatusListener
import com.alimin.fk.listener.FkGetLayersListener
import com.alimin.fk.pb.FkImageLayerOuterClass
import com.lmy.mvp.BaseView
import com.lmy.mvp.ClosablePresenter

interface OnLayerUpdateListener {
    fun onLayers(layers: List<FkImageLayerOuterClass.FkImageLayer>)
    fun onLayer(layer: FkImageLayerOuterClass.FkImageLayer)
}

class ImageContract {
    interface View : BaseView<Presenter> {
        fun onImageSaved(file: String)
        fun onImageSaving()
        fun onPresenterInfo(result: FkResult, arg0 : Any? = null)
        fun getContext(): Context
    }

    abstract class Presenter : ClosablePresenter() {
        abstract fun create()
        abstract fun destroy()
        abstract fun notifyRender()
        abstract fun selectLayer(layerId: Int)
        abstract fun isSelectedLayer(layerId: Int) : Boolean
        abstract fun newLayerWithFile(path: String)
        abstract fun newLayerWithColor(size: Size, red: Int, green: Int, blue: Int, alpha: Int)
        abstract fun newLayerWithSource(source: FkAbsImageSource)
        abstract fun removeLayer(layerId: Int)
        abstract fun detachFromSurface(surface: Surface)
        abstract fun attachToSurface(surface: Surface)
        abstract fun postTranslate(dx: Int, dy: Int)
        abstract fun postRotate(dr: FkRational)
        abstract fun postScale(ds: FkRational)
        abstract fun getLayers(listener: FkGetLayersListener): Int
        abstract fun crop(rect: Rect)
        abstract fun save(file: String)
        abstract fun save(layerId: Int, file: String)
        abstract fun savePicture()
        abstract fun save()
        abstract fun load(listener: FkDoStatusListener)
        abstract fun addLayerUpdateListener(l: OnLayerUpdateListener)
        abstract fun removeLayerUpdateListener(l: OnLayerUpdateListener)
        abstract fun addLoadStatusListener(l: FkDoStatusListener)
        abstract fun removeLoadStatusListener(l: FkDoStatusListener)
        abstract fun drawPath(x: Int, y: Int)
        abstract fun drawPathFinish()
        abstract fun getPaint(): FkPaint
        abstract fun openCamera()
        abstract fun closeCamera()
        abstract fun switchCamera()
        abstract fun takePicture()
        abstract fun testAction()
    }
}