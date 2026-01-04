package com.projectvb;

import android.content.Intent;
import android.net.VpnService;
import android.os.Build;
import android.os.ParcelFileDescriptor;
import android.system.OsConstants;
import android.util.Log;

import androidx.annotation.RequiresApi;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.HashSet;
import java.util.Set;

public class MyVpnService extends VpnService {

    private ParcelFileDescriptor vpnInterface;
    private Thread vpnThread;
    private volatile boolean isRunning = false;

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        try {
            Builder builder = new Builder();
            builder.setSession("NC Blocker VPN")
                    .addAddress("10.0.0.2", 24)
                    .addDnsServer("8.8.8.8")
                    .addRoute("0.0.0.0", 0)  // We keep this to capture all traffic
                    .allowFamily(OsConstants.AF_INET)
                    .addAllowedApplication("com.dts.freefiremax")  // Target only Free Fire MAX
                    .addAllowedApplication("com.dts.freefireth");  // Also add classic Free Fire if needed


            vpnInterface = builder.establish();
            isRunning = true;

            JSONObject config = vpnconfig.getJsonConfig();
            JSONArray filters = config.getJSONArray("filters");
            final Set<Integer> blockedPorts = PacketParser.extractBlockedPorts(filters);

            vpnThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    runVpnLoop(vpnInterface, blockedPorts);
                }
            });

            vpnThread.start();

        } catch (Exception e) {
            e.printStackTrace();
        }

        return START_STICKY;
    }
    private int extractDestinationPort(byte[] packet, int length) {
        // Check if it's IPv4
        if ((packet[0] >> 4) == 4) {
            int ipHeaderLength = (packet[0] & 0x0F) * 4;
            int protocol = packet[9] & 0xFF;

            // Check for TCP (6) or UDP (17)
            if (protocol == 6 || protocol == 17) {
                // Extract destination port
                int destPort = ((packet[ipHeaderLength + 2] & 0xFF) << 8) |
                        (packet[ipHeaderLength + 3] & 0xFF);
                return destPort;
            }
        }
        return -1; // Unknown or unsupported packet
    }

    public void runVpnLoop(ParcelFileDescriptor vpnInterface, Set<Integer> blockedPorts) {
        try (
                FileInputStream in = new FileInputStream(vpnInterface.getFileDescriptor());
                FileOutputStream out = new FileOutputStream(vpnInterface.getFileDescriptor())
        ) {
            ByteBuffer packetBuffer = ByteBuffer.allocate(32767);

            while (isRunning) {
                int length = in.read(packetBuffer.array());
                if (length > 0) {
                    int destPort = extractDestinationPort(packetBuffer.array(), length);

                    if (blockedPorts.contains(destPort)) {
                        Log.d("VPN", "❌ Blocked packet to port: " + destPort);
                        // Don't write this packet to output, just drop it
                    } else {
                        // ✅ Allow this packet (forward it unchanged)
                        out.write(packetBuffer.array(), 0, length);
                        Log.d("VPN", "✅ Allowed packet to port: " + destPort);
                    }

                    // Clear buffer for next read
                    packetBuffer.clear();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }




    @Override
    public void onDestroy() {
        super.onDestroy();
        isRunning = false;
        try {
            if (vpnInterface != null) vpnInterface.close();
            if (vpnThread != null) vpnThread.interrupt();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
