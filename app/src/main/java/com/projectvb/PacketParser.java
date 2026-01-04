package com.projectvb;

import android.util.Log;
import org.json.JSONArray;
import org.json.JSONObject;

import java.util.HashSet;
import java.util.Set;

public class PacketParser {

    public static Set<Integer> extractBlockedPorts(JSONArray filters) {
        Set<Integer> ports = new HashSet<>();
        try {
            for (int i = 0; i < filters.length(); i++) {
                JSONObject rule = filters.getJSONObject(i);

                String pkg = rule.optString("pkg1Name");
                String mobile = rule.optString("mobile");
                int port = rule.optInt("port", -1);

                boolean isTargetApp =
                        pkg.equals("com.dts.freefiremax") ||
                                pkg.equals("com.dts.freefireth") ||
                                pkg.equals("com.vphonegaga.titan") ||
                                pkg.equals("com.f1player") ||
                                pkg.equals("com.vmos.pro") ||
                                pkg.equals("com.x8zs.sandbox");

                if (isTargetApp && "deny".equalsIgnoreCase(mobile) && port > 0) {
                    ports.add(port);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return ports;
    }


    public static int getDestinationPort(byte[] packet) {
        if (packet.length < 20) return -1;

        int protocol = packet[9] & 0xFF;
        if (protocol != 6 && protocol != 17) return -1; // TCP(6), UDP(17)

        int ipHeaderLength = (packet[0] & 0x0F) * 4;
        int portOffset = ipHeaderLength;

        if (packet.length < portOffset + 4) return -1;

        int destPort = ((packet[portOffset + 2] & 0xFF) << 8) | (packet[portOffset + 3] & 0xFF);
        return destPort;
    }
}
