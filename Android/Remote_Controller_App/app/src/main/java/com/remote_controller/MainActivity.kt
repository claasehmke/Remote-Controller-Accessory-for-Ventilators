package com.remote_controller

import android.content.Context
import android.net.wifi.WifiManager
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.github.niqdev.mjpeg.DisplayMode
import com.github.niqdev.mjpeg.Mjpeg
import com.github.niqdev.mjpeg.MjpegInputStream
import kotlinx.android.synthetic.main.activity_main.*
import java.io.OutputStream
import java.net.ServerSocket
import java.net.Socket
import java.time.LocalDateTime
import java.util.concurrent.locks.ReentrantLock
import kotlin.concurrent.withLock

var pushUpdate: Boolean = false
var push: Boolean = false
var rotate: Int = 0
var rotateBef: Int = 0
val lock = ReentrantLock()
val condition = lock.newCondition()
//Thread handling from here: https://stackoverflow.com/questions/44589669/correctly-implementing-wait-and-notify-in-kotlin

class MainActivity : AppCompatActivity() {
    private val server = ServerSocket(8080)
    private val TIMEOUT: Int = 1 //seconds

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // https://developer.android.com/reference/kotlin/android/net/wifi/WifiManager.LocalOnlyHotspotReservation
        // https://stackoverflow.com/questions/45984345/how-to-turn-on-off-wifi-hotspot-programmatically-in-android-8-0-oreo?rq=1
        val wifiManager =
            applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager

        val callback = object : WifiManager.LocalOnlyHotspotCallback() {
            override fun onStarted(reservation: WifiManager.LocalOnlyHotspotReservation ) {
                super.onStarted(reservation);
                println("Wifi Hotspot is on now")
                //mReservation = reservation;
            }

            override fun onStopped() {
                super.onStopped();
                println("onStopped: ")
            }

            override fun onFailed(reason: Int) {
                super.onFailed(reason);
                println("onFailed: ")
            }
        }

        //wifiManager.startLocalOnlyHotspot(callback)

        val socketThread = SocketHandler(server)
        //initialize knob
        knob.setUserBehaviour {
            //text_view.text ="Pressed2"

            /*lock.withLock {           // like synchronized(lock)
                push = true
                condition.signal()
            }*/
        }

        knob.setUserPushed {
            lock.withLock {           // like synchronized(lock)
                push = true
                pushUpdate = true
                condition.signal()
            }
            Toast.makeText(this, "pushed", Toast.LENGTH_LONG).show()
        }

        knob.setUserReleased() {
            lock.withLock {           // like synchronized(lock)
                push = false
                pushUpdate = true
                condition.signal()
            }
            Toast.makeText(this, "released", Toast.LENGTH_LONG).show()
        }

        knob.setOnStateChanged {
            //text_view.text = knob.state.toString()
            var rotDiff = (knob.state-rotateBef) % knob.numberOfStates
            if (rotDiff < - knob.numberOfStates / 2){
                rotDiff +=  knob.numberOfStates
            }
            else if (rotDiff > knob.numberOfStates / 2){
                rotDiff -= knob.numberOfStates
            }
            rotateBef = knob.state

            lock.withLock {           // like synchronized(lock)
                rotate = rotDiff
                condition.signal()
            }
        }


        Mjpeg.newInstance()
            .open("http://192.168.49.140:8081/", TIMEOUT)
            .subscribe(
                { inputStream: MjpegInputStream? ->
                    mjpegView.setSource(inputStream)
                    mjpegView.setDisplayMode(DisplayMode.BEST_FIT)
                    mjpegView.showFps(true)
                })
                { throwable: Throwable? ->
                    Log.e(javaClass.simpleName, "mjpeg error", throwable)
                    Toast.makeText(this, "MJPEG stream Error", Toast.LENGTH_LONG).show()
                }



        println("Server is running on port ${server.localPort}")
        print("Server is running on ip ")
        //thread { SocketHandler(server).run() }
        socketThread.start()
}


    class SocketHandler(server: ServerSocket): Thread() {
    private val server: ServerSocket = server

    public override fun run() {

        try {
            while (true) {
                println("Try to accept client...")
                val client = server.accept()
                println("Client connected: ${client.inetAddress.hostAddress}")

                //Run client
                ClientHandler(client).run()
            }
        }finally {
            println("broke")
        }

    }

}

}


class ClientHandler(client: Socket) {
    private val client: Socket = client
    //private val reader: Scanner = Scanner(client.getInputStream())
    private val writer: OutputStream = client.getOutputStream()
    private var running: Boolean = false
    private val store = Store()

    fun run() {
        running = true
        println("Clienthandler started")
        // Welcome message
        //write("Welcome to the server!\n" +
        //        "To Exit, write: 'EXIT'.\n" +
        //        "To use the calculator, input two numbers separated with a space and an operation in the ending\n" +
        //        "Example: 5 33 multi\n" +
        //        "Available operations: 'add', 'sub', 'div', 'multi'")

        while (running) {
            println("Clienthandler running")
            lock.withLock {           // like synchronized(lock)
                condition.await()

                if (pushUpdate) {
                    store.setPushUpdate(1)
                    if (push){
                        store.setPush(1)
                    }else{
                        store.setPush(0)
                    }
                    pushUpdate = false
                   // Toast.makeText(this, "MJPEG stream Error", Toast.LENGTH_LONG).show()
                }
                if (rotate != 0) {
                    store.setRotationUpdate(1)
                    store.setRotation(rotate)
                    rotate = 0
                    //Toast.makeText(this, "MJPEG stream Error", Toast.LENGTH_LONG).show()
                }
            }
            try {
                    val curTime = LocalDateTime.now()
                    store.setTime(curTime.second + curTime.minute*60)
                    writer.write(store.getByteArray())
                    store.setPushUpdate(0)
                    store.setRotationUpdate(0)

                if (!client.isConnected){
                    throw NullPointerException()
                }

            } catch (ex: Exception) {
                // TODO: Implement exception handling
                shutdown()
            } finally {

            }

        }
    }

    private fun shutdown() {
        running = false
        client.close()
        println("${client.inetAddress.hostAddress} closed the connection")
        //when user leaves application
    }
}