package com.projectvb;

import android.content.Context;
import android.util.TypedValue;

public class Utils {

    Context context;

    public Utils(Context globContext) {
        context = globContext;
    }

    public int FixDP(int i) {
        return (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, (float) i, context.getResources().getDisplayMetrics());
    }

}
