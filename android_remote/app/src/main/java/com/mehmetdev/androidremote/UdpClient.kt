package com.mehmetdev.androidremote

import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import java.nio.ByteBuffer
import java.nio.ByteOrder

class UdpClient(private val pcIp: String, private val port: Int = 6969) {
    fun sendCommand(command: Byte, x: Short = 0, y: Short = 0) {
        Thread {
            try {
                val socket = DatagramSocket()
                val address = InetAddress.getByName(pcIp)
                val buffer = ByteBuffer.allocate(7).apply {
                    order(ByteOrder.BIG_ENDIAN)
                    putShort(0xAFAF.toShort()) // Magic
                    put(command)               // CMD_PLAY_PAUSE = 1
                    putShort(x)
                    putShort(y)
                }
                val packet = DatagramPacket(buffer.array(), buffer.limit(), address, port)
                socket.send(packet)
                socket.close()
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }.start()
    }
}