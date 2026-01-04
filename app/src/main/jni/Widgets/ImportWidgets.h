#include "Category.h"
#include "Switch.h"
#include "SeekBar.h"
#include "Images.h"
#include "Tab.h"
#include "Tab1.h"

class Widget {

private:
    JNIEnv* env;
    Category category;
    Switch aSwitch;
    SeekBar seekBar;
    Tab aTab;
    Tab1 aTab1;

public:
    Widget(JNIEnv* globEnv) {
        env = globEnv;
    }

    void Category(const char* name) {
        category.create(env, name);
    }

    void Switch(const char* name, jint ID) {
        aSwitch.create(env, name, ID);
    }

    void SeekBar(const char* name, jint value, jint max, const char* type, jint ID) {
        seekBar.create(env, name, value, max, type, ID);
    }
    void Tab(const char* name) {
        aTab.create(env, name);
    }
    void Tab1(const char* name) {
        aTab1.create(env, name);
    }
};