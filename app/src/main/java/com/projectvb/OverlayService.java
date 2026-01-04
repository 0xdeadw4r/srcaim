package com.projectvb;
import android.app.Service;
import android.content.Intent;
import android.os.IBinder;


public class OverlayService extends Service {

    public static Menu overlayMenu;

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int injectType = intent.getIntExtra("injectType", 1);

        if (overlayMenu == null) {
            overlayMenu = new Menu(this, injectType);
        }

        return START_STICKY;
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        if (overlayMenu != null) {
            overlayMenu.remove();  // overlay cleanup
            overlayMenu = null;
        }
        stopSelf();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }
}
