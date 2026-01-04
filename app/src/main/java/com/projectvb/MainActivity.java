package com.projectvb;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.view.Gravity;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

    private static final int OVERLAY_PERMISSION_REQUEST_CODE = 100;
    private static final int STORAGE_PERMISSION_REQUEST_CODE = 101;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Force true fullscreen — no status bar, no nav bar
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE |
                        View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN |
                        View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                        View.SYSTEM_UI_FLAG_FULLSCREEN |
                        View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
        );

        // Show splash always
        showChadSplash();

        // Start permission checks after splash fade-in
        new Handler().postDelayed(this::checkOverlayPermission, 1400);
    }

    // Shows the animated splash
    private void showChadSplash() {
        LinearLayout layout = new LinearLayout(this);
        layout.setOrientation(LinearLayout.VERTICAL);
        layout.setGravity(Gravity.CENTER);
        layout.setBackgroundColor(Color.BLACK);
        layout.setPadding(50, 100, 50, 100);

        // === LUNOX - Bold, Red, Huge ===
        TextView lunoxText = new TextView(this);
        lunoxText.setText("\uD835\uDC03\uD835\uDC0E\uD835\uDC0C\uD835\uDC08\uD835\uDC0D\uD835\uDC14\uD835\uDC12"); // Fancy LUNOX
        lunoxText.setTextColor(Color.parseColor("#FFFFFFFF"));
        lunoxText.setTextSize(82);
        lunoxText.setGravity(Gravity.CENTER);
        lunoxText.setTypeface(null, android.graphics.Typeface.BOLD);
        lunoxText.setAlpha(0f);

        // === CHEATS - White, Smaller, Below ===
        TextView cheatsText = new TextView(this);
        cheatsText.setText("\uD835\uDC02\uD835\uDC0E\uD835\uDC11\uD835\uDC0F\uD835\uDC12"); // Fancy CHEATS
        cheatsText.setTextColor(Color.WHITE);
        cheatsText.setTextSize(35);
        cheatsText.setGravity(Gravity.CENTER);
        cheatsText.setTypeface(null, android.graphics.Typeface.BOLD);
        cheatsText.setAlpha(0f);

        layout.addView(lunoxText);
        layout.addView(cheatsText);
        setContentView(layout);

        // --- Animation ---
        lunoxText.animate()
                .alpha(1f)
                .setDuration(900)
                .setStartDelay(100)
                .start();

        cheatsText.animate()
                .alpha(1f)
                .setDuration(900)
                .setStartDelay(520)
                .start();
    }

    // === Permissions Flow ===
    private void checkOverlayPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Overlay permission is required!", Toast.LENGTH_SHORT).show();
                Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION,
                        Uri.parse("package:" + getPackageName()));
                startActivityForResult(intent, OVERLAY_PERMISSION_REQUEST_CODE);
                return;
            }
        }
        checkStoragePermission();
    }

    private void checkStoragePermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ||
                    checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this, "Storage permission is required!", Toast.LENGTH_SHORT).show();
                requestPermissions(new String[]{
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE
                }, STORAGE_PERMISSION_REQUEST_CODE);
                return;
            }
        }
        startLogin();
    }

    private void startLogin() {
        new Handler().postDelayed(() -> new Login(this), 300);
    }

    // === Permission Results ===
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == OVERLAY_PERMISSION_REQUEST_CODE) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !Settings.canDrawOverlays(this)) {
                Toast.makeText(this, "Overlay permission denied! Exiting...", Toast.LENGTH_SHORT).show();
                finish();
            } else {
                checkStoragePermission();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == STORAGE_PERMISSION_REQUEST_CODE) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                startLogin();
            } else {
                Toast.makeText(this, "Storage permission denied! Exiting...", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}
