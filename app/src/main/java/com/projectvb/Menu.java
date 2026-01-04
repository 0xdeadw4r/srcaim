package com.projectvb;

import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.content.Context;
import android.content.res.ColorStateList;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LinearGradient;
import android.graphics.PixelFormat;
import android.graphics.PorterDuff;
import android.graphics.Shader;
import android.graphics.Typeface;
import android.graphics.drawable.ClipDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.GradientDrawable;
import android.graphics.drawable.LayerDrawable;
import android.graphics.drawable.RippleDrawable;
import android.os.Build;
import android.text.Html;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import androidx.annotation.RequiresApi;
import com.topjohnwu.superuser.Shell;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Menu {
    // Native Functions
    public static native void Functions();
    public static native void ChangesID(int ID, int Value);
    public static native void Init();
    private String target = "com.dts.freefireth";
    private int injectType;

    // Variables Menu - Black and White Theme with box text black
    private int buttonClick = 0;
    public static int PrimaryColor = 0xFFFFFFFF; // White
    public static int TabSelectedColor = 0xFF888888; // Medium Gray for selected tab
    public static int BackgroundColor = 0xFF000000; // Black background
    public static int TextColor = 0xFFFFFFFF; // Default white text elsewhere
    public static int BoxTextColor = 0xFF000000; // Black text inside box elements
    private static Context context;
    private static Utils utils;
    private native String imageBase64();

    // Window system
    private WindowManager windowManager;
    private WindowManager.LayoutParams windowManagerParams;
    private FrameLayout frameLayout;

    // DrawView
    DrawView drawView;
    WindowManager.LayoutParams windowManagerDrawViewParams;
    public static native void OnDrawLoad(DrawView drawView, Canvas canvas);

    // Tab Management
    private static Map<String, LinearLayout> tabContentContainers = new HashMap<>();
    private static List<TextView> tabButtons = new ArrayList<>();
    private static String currentTab = "";
    private static HorizontalScrollView tabsScrollView;
    private static LinearLayout tabsContainer;
    private static ScrollView scrollView_center;
    private static LinearLayout featuresScrollContainer;

    public void DrawCanvas() {
        int LAYOUT_FLAG = Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ?
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY :
                WindowManager.LayoutParams.TYPE_PHONE;

        drawView = new DrawView(context);
        windowManagerDrawViewParams = new WindowManager.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT,
                ViewGroup.LayoutParams.MATCH_PARENT,
                LAYOUT_FLAG,
                WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL |
                        WindowManager.LayoutParams.FLAG_NOT_TOUCHABLE |
                        WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_FULLSCREEN,
                PixelFormat.TRANSPARENT
        );
        windowManagerDrawViewParams.gravity = Gravity.CENTER;
        windowManager.addView(drawView, windowManagerDrawViewParams);
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public Menu(Context globContext, int glob_injectType) {
        context = globContext;
        utils = new Utils(context);
        injectType = glob_injectType;
        System.loadLibrary("hawdawdawdawda");
        onCreate();
    }

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    public void onCreate() {
        onCreateSystemWindow();
        onCreateTemplate();
    }

    // ==================== BLACK AND WHITE UI TEMPLATE - NO BORDER ON LOGO ====================
    public void onCreateTemplate() {
        // Main Container Gradient (Black to Dark Gray)
        GradientDrawable gradientDrawable_container = new GradientDrawable(
                GradientDrawable.Orientation.TOP_BOTTOM,
                new int[]{BackgroundColor, 0xFF222222}
        );
        gradientDrawable_container.setCornerRadius(utils.FixDP(4));

        LinearLayout container = new LinearLayout(context);
        container.setOrientation(LinearLayout.VERTICAL);
        container.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));

        // Main Menu - Smaller & Clean Black Background
        final LinearLayout container_menu = new LinearLayout(context);
        container_menu.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(200), ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        container_menu.setBackground(gradientDrawable_container);
        container_menu.setVisibility(View.GONE);
        container_menu.setOrientation(LinearLayout.VERTICAL);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            container_menu.setElevation(utils.FixDP(6));
        }

        // Floating Icon (Smaller) - White Background NO BORDER
        final ImageBase64 icon_cheat = new ImageBase64(context);
        icon_cheat.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(45), utils.FixDP(45)
        ));
        icon_cheat.setImageBase64(imageBase64());

        GradientDrawable iconBg = new GradientDrawable();
        iconBg.setShape(GradientDrawable.OVAL);
        iconBg.setColor(PrimaryColor); // White
        // REMOVED BORDER - NO setStroke call
        icon_cheat.setBackground(iconBg);
        icon_cheat.setPadding(utils.FixDP(5), utils.FixDP(5), utils.FixDP(5), utils.FixDP(5));
        icon_cheat.setOnTouchListener(onTouchListener());
        icon_cheat.setOnClickListener(v -> {
            icon_cheat.setVisibility(View.GONE);
            container_menu.setVisibility(View.VISIBLE);
        });

        // Top Bar
        LinearLayout container_top = new LinearLayout(context);
        container_top.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        container_top.setPadding(utils.FixDP(8), utils.FixDP(8), utils.FixDP(8), utils.FixDP(6));
        container_top.setGravity(Gravity.CENTER);
        container_top.setOrientation(LinearLayout.HORIZONTAL);

        ImageBase64 icon_menu = new ImageBase64(context);
        icon_menu.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(40), utils.FixDP(40)
        ));
        icon_menu.setImageBase64(imageBase64());
        container_top.addView(icon_menu);

        // Tabs ScrollView
        tabsScrollView = new HorizontalScrollView(context);
        tabsScrollView.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(35)
        ));
        tabsScrollView.setHorizontalScrollBarEnabled(false);

        tabsContainer = new LinearLayout(context);
        tabsContainer.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.MATCH_PARENT
        ));
        tabsContainer.setOrientation(LinearLayout.HORIZONTAL);
        tabsContainer.setPadding(utils.FixDP(6), 0, utils.FixDP(6), 0);
        tabsScrollView.addView(tabsContainer);

        // Center Content (Reduced Height)
        LinearLayout container_center = new LinearLayout(context);
        container_center.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(180)
        ));
        container_center.setGravity(Gravity.CENTER);

        scrollView_center = new ScrollView(context);
        scrollView_center.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
        ));
        scrollView_center.setPadding(0, utils.FixDP(6), 0, utils.FixDP(6));

        featuresScrollContainer = new LinearLayout(context);
        featuresScrollContainer.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
        ));
        featuresScrollContainer.setOrientation(LinearLayout.VERTICAL);
        scrollView_center.addView(featuresScrollContainer);

        // Progress Bar - White color
        final ProgressBar progressBar = new ProgressBar(context);
        progressBar.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(40), utils.FixDP(40)
        ));
        progressBar.getIndeterminateDrawable().setColorFilter(TextColor, PorterDuff.Mode.SRC_IN);
        container_center.addView(progressBar);

        // Bottom Inject Button - White background, black text
        LinearLayout container_bottom = new LinearLayout(context);
        container_bottom.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        container_bottom.setPadding(utils.FixDP(8), utils.FixDP(8), utils.FixDP(8), utils.FixDP(8));
        container_bottom.setOrientation(LinearLayout.VERTICAL);
        container_bottom.setGravity(Gravity.RIGHT | Gravity.CENTER_VERTICAL);

        GradientDrawable injectBg = new GradientDrawable();
        injectBg.setColor(PrimaryColor); // White
        injectBg.setCornerRadius(utils.FixDP(4));
        injectBg.setStroke(utils.FixDP(1), TextColor); // White stroke

        RippleDrawable ripple = new RippleDrawable(
                ColorStateList.valueOf(0x33FFFFFF), injectBg, null
        );

        final Button inject_close = new Button(context);
        inject_close.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(32)
        ));
        inject_close.setText("INJECT");
        inject_close.setTextSize(12);
        inject_close.setTextColor(0xFF000000); // black text for good contrast
        inject_close.setBackground(ripple);
        inject_close.setOnClickListener(v -> {
            if (buttonClick == 0) {
                Toast.makeText(context, "Processing injection...", Toast.LENGTH_SHORT).show();
                boolean success = injectType == 0 ? false : InjectX86("libqqqqaaqwdqwd.so");
                if (success) {
                    Init();
                    progressBar.setVisibility(View.GONE);
                    inject_close.setText("MINIMIZE");
                    container_center.removeAllViews();
                    container_center.addView(scrollView_center);
                    buttonClick++;
                    Toast.makeText(context, "Injection successful!", Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(context, "Injection failed! Please try again.", Toast.LENGTH_LONG).show();
                }
            } else if (buttonClick == 1) {
                icon_cheat.setVisibility(View.VISIBLE);
                container_menu.setVisibility(View.GONE);
            }
        });

        container_bottom.addView(inject_close);

        // Add All Views
        frameLayout.addView(container);
        container.addView(icon_cheat);
        container.addView(container_menu);
        container_menu.addView(container_top);
        container_menu.addView(tabsScrollView);
        container_menu.addView(container_center);
        container_menu.addView(container_bottom);
    }

    public void onCreateSystemWindow() {
        int LAYOUT_FLAG = Build.VERSION.SDK_INT >= Build.VERSION_CODES.O ?
                WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY :
                WindowManager.LayoutParams.TYPE_PHONE;

        frameLayout = new FrameLayout(context);
        frameLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        frameLayout.setOnTouchListener(onTouchListener());
        frameLayout.setAlpha(0.95f);
        frameLayout.setBackgroundColor(BackgroundColor);

        windowManagerParams = new WindowManager.LayoutParams(
                WindowManager.LayoutParams.WRAP_CONTENT,
                WindowManager.LayoutParams.WRAP_CONTENT,
                LAYOUT_FLAG,
                WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE |
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_OVERSCAN |
                        WindowManager.LayoutParams.FLAG_LAYOUT_IN_SCREEN |
                        WindowManager.LayoutParams.FLAG_ALT_FOCUSABLE_IM |
                        WindowManager.LayoutParams.FLAG_SPLIT_TOUCH,
                PixelFormat.TRANSPARENT
        );
        windowManagerParams.gravity = Gravity.TOP | Gravity.LEFT;
        windowManagerParams.x = 30;
        windowManagerParams.y = 80;

        windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        DrawCanvas();
        windowManager.addView(frameLayout, windowManagerParams);
    }

    private View.OnTouchListener onTouchListener() {
        return new View.OnTouchListener() {
            private static final int TOUCH_MOVE_THRESHOLD = 8;
            private int x, y, initialX, initialY;
            private boolean isMoving = false;

            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch (event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        x = (int) event.getRawX();
                        y = (int) event.getRawY();
                        initialX = x; initialY = y;
                        isMoving = false;
                        frameLayout.setAlpha(0.80f);
                        return true;
                    case MotionEvent.ACTION_MOVE:
                        int nowX = (int) event.getRawX();
                        int nowY = (int) event.getRawY();
                        if (!isMoving && (Math.abs(nowX - initialX) > TOUCH_MOVE_THRESHOLD || Math.abs(nowY - initialY) > TOUCH_MOVE_THRESHOLD)) {
                            isMoving = true;
                        }
                        if (isMoving) {
                            int dx = nowX - x;
                            int dy = nowY - y;
                            x = nowX; y = nowY;
                            windowManagerParams.x += dx;
                            windowManagerParams.y += dy;
                            windowManager.updateViewLayout(frameLayout, windowManagerParams);
                        }
                        return true;
                    case MotionEvent.ACTION_UP:
                        if (!isMoving) v.performClick();
                        frameLayout.setAlpha(0.95f);
                        return true;
                    default:
                        return false;
                }
            }
        };
    }

    // ==================== CHAD TAB & UI METHODS ====================

    public static void addTab(final String tabName) {
        final boolean isFirstTab = tabButtons.isEmpty();

        GradientDrawable tabButtonDrawable = new GradientDrawable();
        tabButtonDrawable.setColor(isFirstTab ? TabSelectedColor : PrimaryColor);
        tabButtonDrawable.setCornerRadius(utils.FixDP(4));
        tabButtonDrawable.setStroke(utils.FixDP(1), BoxTextColor); // stroke with black to match box text

        final TextView tabButton = new TextView(context);
        LinearLayout.LayoutParams tabParams = new LinearLayout.LayoutParams(
                utils.FixDP(75), ViewGroup.LayoutParams.MATCH_PARENT
        );
        tabParams.setMargins(utils.FixDP(6), 0, utils.FixDP(6), 0);
        tabButton.setLayoutParams(tabParams);
        tabButton.setText(tabName);
        tabButton.setTextColor(BoxTextColor); // black text in boxes (tabs)
        tabButton.setTextSize(11);
        tabButton.setGravity(Gravity.CENTER);
        tabButton.setBackground(tabButtonDrawable);
        tabButton.setOnClickListener(v -> selectTab(tabName));

        tabButtons.add(tabButton);
        tabsContainer.addView(tabButton);

        LinearLayout tabContent = new LinearLayout(context);
        tabContent.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        tabContent.setOrientation(LinearLayout.VERTICAL);
        tabContent.setPadding(utils.FixDP(8), 0, utils.FixDP(8), 0);
        tabContent.setVisibility(isFirstTab ? View.VISIBLE : View.GONE);
        tabContentContainers.put(tabName, tabContent);
        featuresScrollContainer.addView(tabContent);

        if (isFirstTab) currentTab = tabName;
    }

    private static void selectTab(String tabName) {
        if (tabName.equals(currentTab)) return;
        for (TextView tabButton : tabButtons) {
            GradientDrawable drawable = new GradientDrawable();
            boolean isSelected = tabButton.getText().toString().equals(tabName);
            drawable.setColor(isSelected ? TabSelectedColor : PrimaryColor);
            drawable.setCornerRadius(utils.FixDP(4));
            drawable.setStroke(utils.FixDP(1), BoxTextColor);
            tabButton.setBackground(drawable);
            tabButton.setTextColor(BoxTextColor); // Always black text in boxes
        }
        for (Map.Entry<String, LinearLayout> entry : tabContentContainers.entrySet()) {
            entry.getValue().setVisibility(entry.getKey().equals(tabName) ? View.VISIBLE : View.GONE);
        }
        currentTab = tabName;
    }

    public static void addCategory(String name) {
        if (currentTab.isEmpty() || !tabContentContainers.containsKey(currentTab)) return;

        GradientDrawable gradientDrawable = new GradientDrawable();
        gradientDrawable.setColor(PrimaryColor);
        gradientDrawable.setCornerRadius(utils.FixDP(4));

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(26)
        ));
        linearLayout.setBackground(gradientDrawable);
        linearLayout.setGravity(Gravity.CENTER);
        linearLayout.setPadding(0, utils.FixDP(4), 0, utils.FixDP(4));
        LinearLayout.LayoutParams params = (LinearLayout.LayoutParams) linearLayout.getLayoutParams();
        params.setMargins(0, utils.FixDP(8), 0, 0);
        linearLayout.setLayoutParams(params);

        TextView textView = new TextView(context);
        textView.setText(name);
        textView.setTextSize(13);
        textView.setTextColor(BoxTextColor); // black text inside box category
        linearLayout.addView(textView);

        tabContentContainers.get(currentTab).addView(linearLayout);
    }

    public static void addSwitch(String name, final int ID) {
        if (currentTab.isEmpty() || !tabContentContainers.containsKey(currentTab)) return;

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        linearLayout.setPadding(0, utils.FixDP(6), 0, utils.FixDP(6));
        linearLayout.setOrientation(LinearLayout.HORIZONTAL);
        linearLayout.setGravity(Gravity.CENTER_VERTICAL);

        TextView textView = new TextView(context);
        textView.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT, 1
        ));
        textView.setGravity(Gravity.CENTER_VERTICAL);
        textView.setText(name);
        textView.setTextColor(TextColor); // normal white for switches
        textView.setTextSize(11);

        final SwitchStyle switchStyle = new SwitchStyle(context);
        switchStyle.setLayoutParams(new LinearLayout.LayoutParams(
                utils.FixDP(50), utils.FixDP(28)
        ));
        switchStyle.setOnCheckedChangeListener((view, isChecked) -> ChangesID(ID, isChecked ? 1 : 0));
        linearLayout.setOnClickListener(v -> switchStyle.setChecked(!switchStyle.isChecked()));

        linearLayout.addView(textView);
        linearLayout.addView(switchStyle);
        tabContentContainers.get(currentTab).addView(linearLayout);
    }

    public static void addSeekBar(final String name, int value, int max, final String type, final int ID) {
        if (currentTab.isEmpty() || !tabContentContainers.containsKey(currentTab)) return;

        LinearLayout linearLayout = new LinearLayout(context);
        linearLayout.setLayoutParams(new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT
        ));
        linearLayout.setPadding(0, utils.FixDP(6), 0, utils.FixDP(6));
        linearLayout.setOrientation(LinearLayout.VERTICAL);

        final TextView textView = new TextView(context);
        textView.setTextSize(11);
        textView.setTextColor(TextColor);
        updateSeekBarText(textView, name, value, type);

        SeekBar seekBar = new SeekBar(context);
        LinearLayout.LayoutParams seekBarParams = new LinearLayout.LayoutParams(
                ViewGroup.LayoutParams.MATCH_PARENT, utils.FixDP(30)
        );
        seekBarParams.setMargins(0, utils.FixDP(6), 0, 0);
        seekBar.setLayoutParams(seekBarParams);
        seekBar.getThumb().setColorFilter(TextColor, PorterDuff.Mode.SRC_IN);
        seekBar.getProgressDrawable().setColorFilter(TextColor, PorterDuff.Mode.SRC_IN);
        seekBar.setProgress(value);
        seekBar.setMax(type.equals("Color") ? 9 : type.equals("BoxType") || type.equals("LineType") ? 2 : max);

        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                updateSeekBarText(textView, name, i, type);
                ChangesID(ID, i);
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) { seekBar.setAlpha(0.85f); }
            @Override public void onStopTrackingTouch(SeekBar seekBar) { seekBar.setAlpha(1.0f); }
        });

        linearLayout.addView(textView);
        linearLayout.addView(seekBar);
        tabContentContainers.get(currentTab).addView(linearLayout);
    }

    private static void updateSeekBarText(TextView textView, String name, int value, String type) {
        if (type.equals("Color")) {
            String[] colors = {"White", "Green", "Blue", "Red", "Black", "Yellow", "Cyan", "Magenta", "Gray", "Purple"};
            String[] hex = {"#ffffff", "#00FF00", "#0000FF", "#FF0000", "#000000", "#FFFF00", "#00FFFF", "#FF00FF", "#808080", "#A020F0"};
            textView.setText(Html.fromHtml(name + ": <font color='" + hex[value] + "'>" + colors[value] + "</font>"));
        } else if (type.equals("BoxType")) {
            textView.setText(name + ": " + (value == 0 ? "Normal" : value == 1 ? "3D" : "Corner"));
        } else if (type.equals("LineType")) {
            textView.setText(name + ": " + (value == 0 ? "Top" : value == 1 ? "Center" : "Bottom"));
        } else {
            textView.setText(name + ": " + value + type);
        }
    }

    // ==================== INJECTION (UNCHANGED - projectvb) ====================
    public boolean InjectX86(String Lib) {
        try {
            String injector = context.getApplicationInfo().nativeLibraryDir + File.separator + "libinjectEmulator.so";
            String payload_source = context.getApplicationInfo().nativeLibraryDir + File.separator + Lib;
            String payload_dest = "/data/local/tmp/" + Lib;
            Shell.su("rm -f /data/local/tmp/libinject").exec();
            Shell.su("rm -f " + payload_dest).exec();
            Shell.su("rm -f /data/local/tmp/libmeuovo.so").exec();
            Shell.su("cat " + injector + " > /data/local/tmp/libinject").exec();
            Shell.su("cat " + payload_source + " > " + payload_dest).exec();
            Shell.su("cat " + payload_source + " > /data/local/tmp/libmeuovo.so").exec();
            Shell.su("chmod 777 /data/local/tmp/libinject").exec();
            Shell.su("chmod 777 " + payload_dest).exec();
            Shell.su("chmod 777 /data/local/tmp/libmeuovo.so").exec();
            String injectCmd = "su -c \"/data/local/tmp/libinject -pkg com.dts.freefireth -lib " + payload_dest + " -open -dl_memfd\"";
            Shell.su(injectCmd).exec();
            Shell.su("rm -f /data/local/tmp/libinject").exec();
            Shell.su("rm -f " + payload_dest).exec();
            Shell.su("rm -f /data/local/tmp/libmeuovo.so").exec();
            Functions();
            Toast.makeText(context, "Injection Successful", Toast.LENGTH_SHORT).show();
            return true;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public static void showNativeToast(String msg) {
        if (context != null) {
            new android.os.Handler(context.getMainLooper()).post(() -> Toast.makeText(context, msg, Toast.LENGTH_LONG).show());
        }
    }

    public void remove() {
        try {
            if (windowManager != null && frameLayout != null) {
                windowManager.removeViewImmediate(frameLayout);
                frameLayout = null;
            }
            if (drawView != null) {
                windowManager.removeViewImmediate(drawView);
                drawView = null;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
