package com.remote_controller

class Store {


    @Throws(IllegalArgumentException::class)
    external fun getByteArray(): ByteArray

    @Throws(IllegalArgumentException::class)
    external fun getTime(): Int

    @Throws(IllegalArgumentException::class)
    external fun getRotation(): Int

    external fun setTime(pInteger: Int)

    external fun setRotationUpdate(pInteger: Int)

    external fun setRotation(pInteger: Int)

    external fun setPushUpdate(pInteger: Int)

    external fun setPush(pInteger: Int)

    companion object {
        init {
            System.loadLibrary("Store")
        }
    }
}
