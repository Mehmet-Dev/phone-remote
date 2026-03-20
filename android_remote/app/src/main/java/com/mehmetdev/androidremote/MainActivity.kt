package com.mehmetdev.androidremote

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.gestures.detectDragGestures
import androidx.compose.foundation.layout.*
import androidx.compose.material3.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.input.pointer.PointerInputChange
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.unit.dp
import com.mehmetdev.androidremote.ui.theme.AndroidRemoteTheme
import androidx.compose.runtime.Composable

class MainActivity : ComponentActivity() {
    // CHANGE THIS TO YOUR PC'S IP ADDRESS!
    private val udpClient = UdpClient("192.168.1.242")

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            AndroidRemoteTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    RemoteScreen(udpClient)
                }
            }
        }
    }
}

@Composable
fun RemoteScreen(client: UdpClient) {
    Column(modifier = Modifier.fillMaxSize()) {
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .weight(1f)
                .background(MaterialTheme.colorScheme.surfaceVariant)
                .pointerInput(Unit) {
                    detectDragGestures { change: PointerInputChange, dragAmount: Offset ->
                        change.consume()
                        client.sendCommand(
                            command = 10,
                            x = dragAmount.x.toInt().toShort(),
                            y = dragAmount.y.toInt().toShort()
                        )
                    }
                },
            contentAlignment = Alignment.Center
        ) {
            Text("TRACKPAD", color = MaterialTheme.colorScheme.onSurfaceVariant)
        }

        Button(
            onClick = { client.sendCommand(1) },
            modifier = Modifier.fillMaxWidth().padding(16.dp)
        ) {
            Text("PLAY / PAUSE")
        }
    }
}