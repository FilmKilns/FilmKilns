package com.lmy.hwvcnative.processor

import android.graphics.PointF
import android.os.Handler
import android.os.Looper
import android.view.Surface
import androidx.annotation.Keep
import com.lmy.hwvcnative.CPPObject
import com.lmy.hwvcnative.entity.AlLayer
import com.lmy.hwvcnative.entity.AlRational
import com.lmy.hwvcnative.entity.AlResult

@Keep
class AlImageProcessor private constructor() : CPPObject() {
    private val mMainHandler = Handler(Looper.getMainLooper())
    private var onSaveListener: OnSaveListener? = null
    private var onLayerInfoListener: OnLayerInfoListener? = null

    init {
        handler = create()
    }

    fun release() {
        if (!isNativeNull()) {
            release(handler)
        }
        handler = 0L
    }

    fun updateWindow(surface: Surface) {
        if (!isNativeNull()) {
            updateWindow(handler, surface)
        }
        invalidate()
    }

    fun invalidate() {
        if (!isNativeNull()) {
            invalidate(handler)
        }
    }

    /**
     * 设置画布大小，图层也会等比例缩放
     * @param w     新的画布宽度
     * @param h     新的画布高度
     * @param loc   定位
     * @param color 画布颜色
     */
    fun setCanvas(w: Int, h: Int, loc: Int = 0, color: Int = 0) {
        if (!isNativeNull()) {
            setCanvas(handler, w, h, loc, color)
        }
    }

    /**
     * 添加新的图层
     * @param path 图片文件路径
     * @return     新图层的id，id < 0表示添加失败，可用于后续的图层操作
     */
    fun addLayer(path: String): Int {
        if (!isNativeNull()) {
            return addLayer(handler, path)
        }
        return AlResult.FAILED
    }

    /**
     * 改变图层层级
     * @param id    图层id
     * @param index 图层层级，index >= 0，if(index > layer size) index = layer size
     *              0为最底层，会被所有图层遮挡
     * @return      {@link AlResult}
     */
    fun moveLayerIndex(id: Int, index: Int): Int {
        if (!isNativeNull()) {
            return moveLayerIndex(handler, id, index)
        }
        return AlResult.FAILED
    }

    /**
     * 移除图层
     * @param id 图层id
     * @return   {@link AlResult}
     */
    fun removeLayer(id: Int): Int {
        if (!isNativeNull()) {
            return removeLayer(handler, id)
        }
        return AlResult.FAILED
    }

    /**
     * 缩放图层
     * @param id    图层id
     * @param scale 缩放倍数
     * @return      {@link AlResult}
     */
    fun setScale(id: Int, scale: AlRational): Int {
        if (!isNativeNull()) {
            return setScale(handler, id, scale.num, scale.den)
        }
        return AlResult.FAILED
    }

    /**
     * 累乘缩放图层
     * @param id     图层id
     * @param ds     缩放因子
     * @param anchor 缩放锚点
     * @return       {@link AlResult}
     */
    fun postScale(id: Int, ds: AlRational, anchor: PointF): Int {
        if (!isNativeNull()) {
            return postScale(handler, id, ds.num, ds.den, anchor.x, anchor.y)
        }
        return AlResult.FAILED
    }

    /**
     * 旋转图层
     * @param id 图层id
     * @param r  旋转弧度，分数表示，没有乘以PI，顺时针方向为正向(CW)
     * @return   {@link AlResult}
     */
    fun setRotation(id: Int, r: AlRational): Int {
        if (!isNativeNull()) {
            return setRotation(handler, id, r.num, r.den)
        }
        return AlResult.FAILED
    }

    /**
     * 累加旋转图层
     * @param id     图层id
     * @param dr     旋转弧度，分数表示，没有乘以PI，顺时针方向为正向(CW)
     * @param anchor 旋转锚点
     * @return       {@link AlResult}
     */
    fun postRotation(id: Int, dr: AlRational, anchor: PointF): Int {
        if (!isNativeNull()) {
            return postRotation(handler, id, dr.num, dr.den, anchor.x, anchor.y)
        }
        return AlResult.FAILED
    }

    /**
     * 移动图层
     * @param id 图层id
     * @param x  相对于画布的x轴位移，画布中心为0点，x∈[-1, 1]为画布可见范围，绝对值大于1时会超出画布范围而不可见
     * @param y  相对于画布的y轴位移，同x
     * @return   {@link AlResult}
     */
    fun setTranslate(id: Int, x: Float, y: Float): Int {
        if (!isNativeNull()) {
            return setTranslate(handler, id, x, y)
        }
        return AlResult.FAILED
    }

    /**
     * 移动图层，会累加前一次的位移
     * @param id 图层id
     * @param dx  相对于画布的x轴位移，画布中心为0点
     * @param dy  相对于画布的y轴位移，同dx
     * @return   {@link AlResult}
     */
    fun postTranslate(id: Int, dx: Float, dy: Float): Int {
        if (!isNativeNull()) {
            return postTranslate(handler, id, dx, dy)
        }
        return AlResult.FAILED
    }

    /**
     * 设置图层透明度
     * @param id    图层id
     * @param alpha 图层透明度，范围[0, 1]，1为完全不透明
     * @return      {@link AlResult}
     */
    fun setAlpha(id: Int, alpha: Float): Int {
        if (!isNativeNull()) {
            return setAlpha(handler, id, alpha)
        }
        return AlResult.FAILED
    }

    /**
     * 根据View坐标获取layer id
     * 用于点击选择图层
     * @param x 相对于View的x轴坐标
     * @param y 相对于View的y轴坐标
     * @return  图层id，负数表示没找到
     */
    fun getLayer(x: Float, y: Float): Int {
        if (!isNativeNull()) {
            return getLayer(handler, x, y)
        }
        return AlResult.FAILED
    }

    /**
     * 对图层进行裁剪，目前一个图层只能有一个裁剪操作
     * 如果要更新裁剪区域，请先进行cancelCropLayer
     * 该操作会重置图层的缩放旋转位移
     * @param id      图层id
     * @param left   左, left∈[0, 1]
     * @param top    上, top∈[0, 1]
     * @param right  右, right∈[0, 1]
     * @param bottom 下, bottom∈[0, 1]
     * @return       {@link AlResult}
     */
    fun ensureCropLayer(id: Int, left: Float, top: Float, right: Float, bottom: Float): Int {
        if (!isNativeNull()) {
            return ensureCropLayer(handler, id, left, top, right, bottom)
        }
        return AlResult.FAILED
    }

    /**
     * 裁剪画布，并保持各图层在画布中的位置不变
     * @param left   左, left∈[-1, 1]
     * @param top    上, top∈[-1, 1]
     * @param right  右, right∈[-1, 1]
     * @param bottom 下, bottom∈[-1, 1]
     */
    fun cropCanvas(left: Float, top: Float, right: Float, bottom: Float) {
        if (!isNativeNull()) {
            cropCanvas(handler, left, top, right, bottom)
        }
    }

    /**
     * 取消图层裁剪
     * @param id      图层id
     * @return       {@link AlResult}
     */
    fun cancelCropLayer(id: Int): Int {
        if (!isNativeNull()) {
            return cancelCropLayer(handler, id)
        }
        return AlResult.FAILED
    }

    /**
     * 校直裁剪
     * @param id 图层id
     * @param r  旋转角度
     * @return   {@link AlResult}
     */
    fun ensureAlignCrop(id: Int, r: AlRational): Int {
        if (!isNativeNull()) {
            return ensureAlignCrop(handler, id, r.num, r.den)
        }
        return AlResult.FAILED
    }

    /**
     * 取消校直裁剪
     * @param id 图层id
     * @return   {@link AlResult}
     */
    fun cancelAlignCrop(id: Int): Int {
        if (!isNativeNull()) {
            return cancelAlignCrop(handler, id)
        }
        return AlResult.FAILED
    }

    /**
     * 保存图片
     * @return {@link AlResult}
     */
    fun save(path: String): Int {
        if (!isNativeNull()) {
            return save(handler, path)
        }
        return AlResult.FAILED
    }

    /**
     * 导出到.alx文件
     */
    fun export(path: String): Int {
        if (!isNativeNull()) {
            return export(handler, path)
        }
        return AlResult.FAILED
    }

    /**
     * 从.alx文件导入
     */
    fun import(path: String): Int {
        if (!isNativeNull()) {
            return import(handler, path)
        }
        return AlResult.FAILED
    }

    fun redo(): Int {
        if (!isNativeNull()) {
            return redo(handler)
        }
        return AlResult.FAILED
    }

    fun undo(): Int {
        if (!isNativeNull()) {
            return undo(handler)
        }
        return AlResult.FAILED
    }

    /**
     * 画笔绘制接口
     * @param point 屏幕像素坐标，中心为原点
     */
    fun paint(id: Int, point: PointF, painting: Boolean): Int {
        if (!isNativeNull()) {
            return paint(handler, id, point.x, point.y, painting)
        }
        return AlResult.FAILED
    }

    fun queryLayerInfo(l: OnLayerInfoListener? = null) {
        if (null != l) {
            onLayerInfoListener = l
        }
        if (!isNativeNull()) {
            return queryLayerInfo(handler)
        }
    }

    /***************************/
    /**      Listener         **/
    /***************************/
    interface OnSaveListener {
        fun onSave(code: Int, msg: String?, path: String?)
    }

    fun setOnSaveListener(listener: OnSaveListener) {
        this.onSaveListener = listener
    }

    fun setOnSaveListener(listener: (code: Int, msg: String?, path: String?) -> Unit) {
        setOnSaveListener(object : OnSaveListener {
            override fun onSave(code: Int, msg: String?, path: String?) {
                listener(code, msg, path)
            }
        })
    }

    interface OnLayerInfoListener {
        fun onInfo(layers: Array<AlLayer>)
    }

    fun setOnLayerInfoListener(l: OnLayerInfoListener) {
        this.onLayerInfoListener = l
    }

    fun setOnLayerInfoListener(l: (layers: Array<AlLayer>) -> Unit) {
        setOnLayerInfoListener(object : OnLayerInfoListener {
            override fun onInfo(layers: Array<AlLayer>) {
                l(layers)
            }
        })
    }

    /***************************/
    /** Callback from native  **/
    /***************************/

    /**
     * 图像保存完成回调
     * @param code {@link AlResult}
     * @param msg  状态信息
     * @param path 保存路径
     */
    fun onSave(code: Int, msg: String?, path: String?) {
        mMainHandler.post {
            onSaveListener?.onSave(code, msg, path)
        }
    }

    /**
     * 查询所有图层信息回调
     * @param ids layer id
     * @param ws  layer width
     * @param hs  layer height
     */
    fun onLayerInfo(ids: IntArray, ws: IntArray, hs: IntArray) {
        val layers = Array(ids.size) {
            return@Array AlLayer(ids[it], ws[it], hs[it])
        }
        mMainHandler.post {
            onLayerInfoListener?.onInfo(layers)
        }
    }


    companion object {
        fun create(): AlImageProcessor = AlImageProcessor()
    }

    private external fun create(): Long
    private external fun release(handler: Long)
    private external fun updateWindow(handler: Long, surface: Surface)
    private external fun invalidate(handler: Long)
    private external fun setCanvas(handler: Long, w: Int, h: Int, loc: Int, color: Int)
    private external fun addLayer(handler: Long, path: String): Int
    private external fun moveLayerIndex(handler: Long, id: Int, index: Int): Int
    private external fun removeLayer(handler: Long, id: Int): Int
    private external fun setScale(handler: Long, id: Int, scaleNum: Int, scaleDen: Int): Int
    private external fun postScale(handler: Long, id: Int, dsNum: Int, dsDen: Int,
                                   anchorX: Float, anchorY: Float): Int

    private external fun setRotation(handler: Long, id: Int, rNum: Int, rDen: Int): Int
    private external fun postRotation(handler: Long, id: Int, drNum: Int, drDen: Int,
                                      anchorX: Float, anchorY: Float): Int

    private external fun setTranslate(handler: Long, id: Int, x: Float, y: Float): Int
    private external fun postTranslate(handler: Long, id: Int, dx: Float, dy: Float): Int
    private external fun setAlpha(handler: Long, id: Int, alpha: Float): Int
    private external fun getLayer(handler: Long, x: Float, y: Float): Int
    private external fun ensureCropLayer(handler: Long, id: Int, left: Float, top: Float,
                                         right: Float, bottom: Float): Int

    private external fun cropCanvas(handler: Long, left: Float, top: Float,
                                    right: Float, bottom: Float)

    private external fun cancelCropLayer(handler: Long, id: Int): Int

    private external fun ensureAlignCrop(handler: Long, id: Int, rNum: Int, rDen: Int): Int

    private external fun cancelAlignCrop(handler: Long, id: Int): Int

    private external fun save(handler: Long, path: String): Int
    private external fun export(handler: Long, path: String): Int
    private external fun import(handler: Long, path: String): Int
    private external fun redo(handler: Long): Int
    private external fun undo(handler: Long): Int
    private external fun paint(handler: Long, id: Int, x: Float, y: Float, painting: Boolean): Int
    private external fun queryLayerInfo(handler: Long)
}