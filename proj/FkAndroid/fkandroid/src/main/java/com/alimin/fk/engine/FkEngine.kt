package com.alimin.fk.engine

import androidx.annotation.Keep
import com.alimin.fk.core.FkNativeObject
import com.alimin.fk.entity.FkResult

@Keep
abstract class FkEngine : FkNativeObject() {
    open fun start(): FkResult = FkResult.FAIL
    open fun stop(): FkResult = FkResult.FAIL
}