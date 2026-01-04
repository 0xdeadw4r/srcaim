package com.projectvb;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

public class vpnconfig {
    public static JSONObject getJsonConfig() {
        String fullJson = "{\"apps\":[{\"appName\":\"System\",\"pkgName\":\"nonpkg.noname\"}],\"filters\":[" +
                "{\"appName\":\"Free Fire MAX\",\"isCustom\":true,\"mobile\":\"deny\",\"pkg1Name\":\"com.dts.freefiremax\",\"port\":443,\"priority\":0,\"proto\":\"tcp\",\"server\":\"*\",\"serverStrType\":\"ip4\",\"wifi\":\"deny\"}," +
                "{\"appName\":\"Free Fire MAX\",\"isCustom\":true,\"mobile\":\"deny\",\"pkg1Name\":\"com.dts.freefiremax\",\"port\":39698,\"priority\":0,\"proto\":\"tcp\",\"server\":\"*\",\"serverStrType\":\"ip4\",\"wifi\":\"deny\"}" +
                // Add more as needed
                "]}";

        try {
            return new JSONObject(fullJson);
        } catch (JSONException e) {
            e.printStackTrace();
            return new JSONObject();
        }
    }
}
