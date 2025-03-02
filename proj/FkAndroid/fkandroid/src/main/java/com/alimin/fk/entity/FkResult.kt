package com.alimin.fk.entity

class FkResult(val code: Int, val msg: String) {
    companion object {
        val OK = FkResult(0, "Success")
        val SUCCESS_END = FkResult(999, "Success end flag")

        /**
         **************** ERROR ****************
         */
        val FAIL = FkResult(-1, "Fail")
        val INVALID_STATE = FkResult(-2, "Invalid state")
        val PROTOCOL_NOT_ACCEPT = FkResult(-3, "Protocol not accept")
        val INVALID_DATA = FkResult(-4, "Invalid data")
        val EMPTY_DATA = FkResult(-5, "Empty data")
        val SKIP = FkResult(-6, "Skip")
        val NPE = FkResult(-7, "Null pointer error")
        val FILE_NOT_FOUND = FkResult(-8, "File not found")
        val ERROR_END = FkResult(-999, "Error end flag")

        /**
         **************** INFO ****************
         */
        val INFO_CAMERA_STOPPED = FkResult(1001, "Camera stopped")
        val INFO_CAMERA_FILL_SOME_FEATURES_FINISH = FkResult(1002, "Fill some features finish")
        val INFO_CAMERA_FILL_ALL_FEATURES_FINISH = FkResult(1003, "Fill all features finish")
        val INFO_CAMERA_TAKE_PICTURE_SUCCESS = FkResult(1004, "Take picture success")
        val INFO_CAMERA_SWITCH_DONE = FkResult(1005, "Switch camera done")
        val INFO_CAMERA_PREVIEW_SIZE_SELECTED = FkResult(1006, "Preview size selected")
        val INFO_CAMERA_PICTURE_SIZE_SELECTED = FkResult(1007, "Picture size selected")
        val INFO_END = FkResult(9999, "Info end flag")
    }

    constructor(code: Int) : this(code, if (code == OK.code) OK.msg else FAIL.msg) {}

    fun isSuccess(): Boolean = (this.code >= OK.code && this.code < SUCCESS_END.code)

    override fun equals(other: Any?): Boolean {
        return if (other != null && other is FkResult) other.code == code else false
    }

    override fun toString(): String {
        return code.toString()
    }

    override fun hashCode(): Int {
        var result = code
        result = 31 * result + msg.hashCode()
        return result
    }
}