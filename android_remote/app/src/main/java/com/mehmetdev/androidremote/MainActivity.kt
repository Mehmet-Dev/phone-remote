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
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.unit.dp
import com.mehmetdev.androidremote.ui.theme.AndroidRemoteTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.*
import androidx.compose.foundation.lazy.grid.*
import androidx.compose.ui.unit.sp

class MainActivity : ComponentActivity() {
    private var activeClient: UdpClient? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            AndroidRemoteTheme {
                var serverIp by remember { mutableStateOf<String?>(null) }

                Surface(modifier = Modifier.fillMaxSize()) {
                    if (serverIp == null) {
                        DiscoveryScreen(onFound = { ip -> serverIp = ip })
                    } else {
                        // 2. Assign the client once the IP is found
                        val client = remember { UdpClient(serverIp!!) }
                        activeClient = client
                        RemoteScreen(client)
                    }
                }
            }
        }
    }
}

@Composable
fun DiscoveryScreen(onFound: (String) -> Unit) {
    Box(contentAlignment = Alignment.Center, modifier = Modifier.fillMaxSize()) {
        Button(onClick = {
            // Call the function we wrote in UdpClient.kt
            discoverServer { ip -> onFound(ip) }
        }) {
            Text("SEARCH FOR PC")
        }
    }
}

@Composable
fun RemoteScreen(client: UdpClient) {
    // Define our buttons: Label to Command ID
    val mediaButtons = listOf(
        "PREV" to 5, "PLAY/PAUSE" to 1, "NEXT" to 4,
        "VOL -" to 3, "L-CLICK" to 11, "VOL +" to 2,
        "R-CLICK" to 12, "SCROLL" to 13, "QUIT" to 99 // Just in case!
    )

    Column(modifier = Modifier.fillMaxSize()) {
        // --- THE TRACKPAD (Takes up 60% of screen) ---
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .weight(0.6f)
                .background(MaterialTheme.colorScheme.surfaceVariant)
                .pointerInput(Unit) {
                    detectDragGestures { change, dragAmount ->
                        change.consume()
                        client.sendCommand(10, dragAmount.x.toInt().toShort(), dragAmount.y.toInt().toShort())
                    }
                },
            contentAlignment = Alignment.Center
        ) {
            Text("TRACKPAD", style = MaterialTheme.typography.labelLarge, color = MaterialTheme.colorScheme.onSurfaceVariant)
        }

        // --- THE MEDIA GRID (Takes up 40% of screen) ---
        LazyVerticalGrid(
            columns = GridCells.Fixed(3), // 3 buttons per row
            modifier = Modifier
                .fillMaxWidth()
                .weight(0.4f)
                .padding(8.dp),
            verticalArrangement = Arrangement.spacedBy(8.dp),
            horizontalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            items(mediaButtons) { (label, cmdId) ->
                Button(
                    onClick = { client.sendCommand(cmdId.toByte()) },
                    modifier = Modifier.fillMaxHeight(),
                    shape = MaterialTheme.shapes.medium,
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (label.contains("CLICK"))
                            MaterialTheme.colorScheme.secondary
                        else MaterialTheme.colorScheme.primary
                    )
                ) {
                    Text(label, fontSize = 12.sp, maxLines = 1)
                }
            }
        }
    }
}