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

fun discoverServer(onDiscovered: (String) -> Unit) {
    Thread {
        try {
            val socket = DatagramSocket()
            socket.broadcast = true
            socket.soTimeout = 3000 // Give up after 3 seconds if no one answers

            // 1. Send the "Shout"
            val message = "WHO_IS_THE_PIGEON_MASTER?".toByteArray()
            val broadcastAddr = InetAddress.getByName("255.255.255.255")
            val packet = DatagramPacket(message, message.size, broadcastAddr, 6970)
            socket.send(packet)

            // 2. Listen for the "Reply"
            val buffer = ByteArray(64)
            val replyPacket = DatagramPacket(buffer, buffer.size)
            socket.receive(replyPacket)

            val response = String(replyPacket.data, 0, replyPacket.length)
            if (response == "I_AM_THE_PIGEON_MASTER") {
                onDiscovered(replyPacket.address.hostAddress)
            }
            socket.close()
        } catch (e: Exception) {
            e.printStackTrace() // Probably a timeout, which is fine
        }
    }.start()
}

