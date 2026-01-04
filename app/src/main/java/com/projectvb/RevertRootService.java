package com.projectvb;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.widget.Toast;

import java.io.OutputStream;

public class RevertRootService extends Service {

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        // This method is called when the app is swiped away or killed from recent apps
        try {
            String from = "/system/xbin/su1";
            String to = "/system/xbin/su";

            Process process = Runtime.getRuntime().exec("su");
            OutputStream os = process.getOutputStream();
            os.write(("mount -o remount,rw /system\n").getBytes());
            os.write(("mv " + from + " " + to + "\n").getBytes());
            os.write("exit\n".getBytes());
            os.flush();
            process.waitFor();

            Toast.makeText(getApplicationContext(), "Root Bypass Reverted", Toast.LENGTH_SHORT).show();
        } catch (Exception e) {
            Toast.makeText(getApplicationContext(), "Failed to revert root: " + e.getMessage(), Toast.LENGTH_SHORT).show();
        }

        stopSelf(); // Stop the service
        super.onTaskRemoved(rootIntent);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
