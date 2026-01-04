package com.projectvb;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.ColorStateList;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.RippleDrawable;
import android.net.Uri;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.text.method.PasswordTransformationMethod;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.*;

import com.projectvb.KeyAuth;
import com.projectvb.UserData;
import com.projectvb.Utils;
import com.projectvb.ImageString;
import com.projectvb.ImageBase64;

import java.io.File;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Login {
    private static final String URL = "https://genzauth-tl0c.onrender.com/keyauth/";
    private static final String OWNER_ID = "9q1Zu-ccbS";
    private static final String APP_NAME = "AIMKILLL MAX";
    private static final String VERSION = "1.0.0";
    private static final String PREFS_NAME = "LunoxPrefs";

    private Context context;
    private Utils utils;
    private ImageString imageString;
    private int injectType;
    private EditText input_username, input_password;
    private RadioButton radioButton_root, radioButton_emulator;
    private Switch suToggle;
    private KeyAuth keyAuth;
    private final Handler mainHandler = new Handler(Looper.getMainLooper());
    private SharedPreferences prefs;

    public Login(Context glob_Context) {
        context = glob_Context;
        utils = new Utils(context);
        imageString = new ImageString();
        keyAuth = new KeyAuth(APP_NAME, OWNER_ID, VERSION, URL, context);
        prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        Init();
    }

    @TargetApi(Build.VERSION_CODES.M)
    private void Init() {
        // Background layout
        FrameLayout rootLayout = new FrameLayout(context);
        ((Activity) context).setContentView(rootLayout);

        // Black background
        LinearLayout bg = new LinearLayout(context);
        bg.setBackgroundColor(0xFF000000); // pure black background
        rootLayout.addView(bg, new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));

        // Container center (no scroll)
        LinearLayout container = new LinearLayout(context);
        container.setOrientation(LinearLayout.VERTICAL);
        container.setGravity(Gravity.CENTER);
        container.setPadding(utils.FixDP(0), utils.FixDP(0), utils.FixDP(0), utils.FixDP(0));
        rootLayout.addView(container, new FrameLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT
        ));

        // Login Card - Black with white outline
        LinearLayout container_login = new LinearLayout(context);
        LinearLayout.LayoutParams cardParams = new LinearLayout.LayoutParams(
                utils.FixDP(320), ViewGroup.LayoutParams.WRAP_CONTENT
        );
        cardParams.gravity = Gravity.CENTER_HORIZONTAL;
        container_login.setLayoutParams(cardParams);
        container_login.setOrientation(LinearLayout.VERTICAL);
        container_login.setGravity(Gravity.CENTER_HORIZONTAL);
        container_login.setPadding(utils.FixDP(20), utils.FixDP(18), utils.FixDP(20), utils.FixDP(18));

        GradientDrawable cardBg = new GradientDrawable();
        cardBg.setColor(0xFF1A1A1A); // dark gray card
        cardBg.setCornerRadius(utils.FixDP(16));
        cardBg.setStroke(utils.FixDP(2), 0xFFFFFFFF); // white outline
        container_login.setBackground(cardBg);
        container_login.setElevation(utils.FixDP(8));
        container.addView(container_login);

        // Logo section
        ImageBase64 image_icon = new ImageBase64(context);
        image_icon.setImageBase64(imageString.icon_image);
        LinearLayout.LayoutParams iconParams = new LinearLayout.LayoutParams(
                utils.FixDP(70), utils.FixDP(70)
        );
        iconParams.gravity = Gravity.CENTER_HORIZONTAL;
        iconParams.setMargins(0, 0, 0, utils.FixDP(16));
        image_icon.setLayoutParams(iconParams);
        container_login.addView(image_icon);

        // Username input
        input_username = new EditText(context);
        styleInput(input_username, "Username");
        container_login.addView(input_username);

        // Password input
        input_password = new EditText(context);
        styleInput(input_password, "Password");
        input_password.setTransformationMethod(PasswordTransformationMethod.getInstance());
        container_login.addView(input_password);

        // Injection type radio buttons (horizontal)
        RadioGroup radioGroup = new RadioGroup(context);
        LinearLayout.LayoutParams rgParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
        );
        rgParams.setMargins(0, utils.FixDP(9), 0, utils.FixDP(9));
        radioGroup.setLayoutParams(rgParams);
        radioGroup.setOrientation(LinearLayout.HORIZONTAL);
        radioGroup.setGravity(Gravity.CENTER_HORIZONTAL);

        radioButton_root = new RadioButton(context);
        styleRadio(radioButton_root, "VMOS");

        radioButton_emulator = new RadioButton(context);
        styleRadio(radioButton_emulator, "EMULATOR");

        radioGroup.addView(radioButton_root);
        radioGroup.addView(radioButton_emulator);
        container_login.addView(radioGroup);

        // Root Bypass toggle - Black/White theme
        suToggle = new Switch(context);
        suToggle.setText("Root Bypass");
        suToggle.setTextColor(0xFFFFFFFF); // white text
        suToggle.setTextSize(13);
        suToggle.setChecked(isSuRenamed());
        suToggle.setOnCheckedChangeListener((v, c) -> toggleSu(c));

        GradientDrawable switchBg = new GradientDrawable();
        switchBg.setCornerRadius(utils.FixDP(14));
        switchBg.setColor(0xFF2A2A2A); // dark background
        suToggle.setBackground(switchBg);
        suToggle.setThumbTintList(ColorStateList.valueOf(0xFFFFFFFF)); // white thumb
        suToggle.setTrackTintList(ColorStateList.valueOf(0xFF555555)); // gray track

        LinearLayout.LayoutParams suParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
        );
        suParams.gravity = Gravity.CENTER_HORIZONTAL;
        suParams.setMargins(0, 0, 0, utils.FixDP(8));
        suToggle.setLayoutParams(suParams);
        container_login.addView(suToggle);

        // Login button - White button with black text
        Button login = new Button(context);
        GradientDrawable btnBg = new GradientDrawable();
        btnBg.setCornerRadius(utils.FixDP(12));
        btnBg.setColor(0xFFFFFFFF); // white button
        btnBg.setStroke(utils.FixDP(2), 0xFFCCCCCC); // light gray border
        RippleDrawable ripple = new RippleDrawable(
                ColorStateList.valueOf(0x44CCCCCC), btnBg, null
        );
        login.setBackground(ripple);
        login.setText("LOGIN");
        login.setTextColor(0xFF000000); // black text
        login.setTextSize(15);
        login.setTypeface(null, Typeface.BOLD);
        login.setPadding(0, utils.FixDP(10), 0, utils.FixDP(10));

        LinearLayout.LayoutParams btnParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(44)
        );
        login.setLayoutParams(btnParams);

        login.setOnClickListener(v -> {
            String user = input_username.getText().toString().trim();
            String pass = input_password.getText().toString().trim();
            if (user.isEmpty() || pass.isEmpty()) {
                Toast.makeText(context, "Please enter username and password", Toast.LENGTH_SHORT).show();
                return;
            }
            if (!radioButton_root.isChecked() && !radioButton_emulator.isChecked()) {
                Toast.makeText(context, "Please select injection type", Toast.LENGTH_SHORT).show();
                return;
            }
            injectType = radioButton_root.isChecked() ? 0 : 1;
            checkKeyAuthLogin(user, pass, injectType);
        });
        container_login.addView(login);

        // Copyright text (bottom) - White text
        TextView copyright = new TextView(context);
        copyright.setText("© 2025 Dominus Corps");
        copyright.setTextColor(0xFFCCCCCC); // light gray text
        copyright.setTextSize(10);
        copyright.setGravity(Gravity.CENTER);
        copyright.setPadding(0, utils.FixDP(12), 0, 0);
        container.addView(copyright);

        // Credentials and KeyAuth logic untouched
        loadCredentials();
        initializeKeyAuth();
    }

    private void styleInput(EditText et, String hint) {
        et.setHint(hint);
        et.setTextColor(0xFFFFFFFF); // white text
        et.setHintTextColor(0xFF888888); // gray hint
        et.setTextSize(14);
        et.setSingleLine();
        et.setPadding(utils.FixDP(14), utils.FixDP(10), utils.FixDP(14), utils.FixDP(10));
        GradientDrawable bg = new GradientDrawable();
        bg.setColor(0xFF2A2A2A); // dark gray input background
        bg.setCornerRadius(utils.FixDP(8));
        bg.setStroke(utils.FixDP(1), 0xFFFFFFFF); // white border
        et.setBackground(bg);
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(44)
        );
        params.setMargins(0, utils.FixDP(8), 0, 0);
        et.setLayoutParams(params);
    }

    private void styleRadio(RadioButton rb, String text) {
        rb.setText(text);
        rb.setTextColor(0xFFFFFFFF); // white text
        rb.setTextSize(14);
        rb.setButtonTintList(ColorStateList.valueOf(0xFFFFFFFF)); // white radio button
        rb.setPadding(utils.FixDP(12), 0, utils.FixDP(12), 0);
    }

    private void saveCredentials(String username, String password) {
        prefs.edit()
                .putString("saved_user", username)
                .putString("saved_pass", password)
                .apply();
    }

    private void loadCredentials() {
        String user = prefs.getString("saved_user", "");
        String pass = prefs.getString("saved_pass", "");
        input_username.setText(user);
        input_password.setText(pass);
    }

    private void initializeKeyAuth() {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        executor.execute(() -> {
            try {
                keyAuth.init();
                mainHandler.post(() -> Toast.makeText(context, "Dominus Corps Initialized", Toast.LENGTH_SHORT).show());
            } catch (Exception e) {
                mainHandler.post(() -> Toast.makeText(context, "Initialization Failed: " + e.getMessage(), Toast.LENGTH_LONG).show());
            }
        });
    }

    private void checkKeyAuthLogin(String username, String password, int injectType) {
        ExecutorService executor = Executors.newSingleThreadExecutor();
        executor.execute(() -> {
            try {
                UserData userData = keyAuth.login(username, password);
                mainHandler.post(() -> {
                    saveCredentials(username, password);
                    showOfficialSellerDialog(injectType, userData.getUsername());
                });
            } catch (Exception e) {
                mainHandler.post(() -> Toast.makeText(context, "Login Error: " + e.getMessage(), Toast.LENGTH_LONG).show());
            }
        });
    }

    private void showOfficialSellerDialog(int injectType, String username) {
        Toast.makeText(context, "Welcome, " + username, Toast.LENGTH_SHORT).show();
        new Menu(context, injectType);
        if (context instanceof Activity) {
            ((Activity) context).finish();
        }
        final String PACKAGE = "com.dts.freefireth";
        PackageManager pm = context.getPackageManager();
        Intent intent = pm.getLaunchIntentForPackage(PACKAGE);
        if (intent != null) {
            intent.addCategory(Intent.CATEGORY_LAUNCHER);
            context.startActivity(intent);
        } else {
            context.startActivity(new Intent(Intent.ACTION_VIEW,
                    Uri.parse("https://play.google.com/store/apps/details?id=" + PACKAGE)));
        }
    }

    private void toggleSu(boolean enable) {
        String from = enable ? "/system/xbin/su" : "/system/xbin/su1";
        String to = enable ? "/system/xbin/su1" : "/system/xbin/su";
        try {
            Process p = Runtime.getRuntime().exec(enable ? "su" : "su1");
            p.getOutputStream().write(("mount -o remount,rw /system\n").getBytes());
            p.getOutputStream().write(("mv " + from + " " + to + "\n").getBytes());
            p.getOutputStream().write("exit\n".getBytes());
            p.getOutputStream().flush();
            p.waitFor();
            Toast.makeText(context, "Root Bypass " + (enable ? "Enabled" : "Disabled"), Toast.LENGTH_SHORT).show();
        } catch (Exception e) {
            Toast.makeText(context, "Root Bypass Failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }
    }

    private boolean isSuRenamed() {
        return !new File("/system/xbin/su").exists();
    }
}
