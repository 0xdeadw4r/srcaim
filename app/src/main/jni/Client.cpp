#include <list>
#include <vector>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include "Tools/Includes/Logger.h"
#include "Tools/Includes/obfuscate.h"
#include "Tools/Includes/Utils.h"
#include "Hack/Memory.h"
#include "Tools/SOCKET/client.h"
#include "Tools/SOCKET/IncludeClient.h"
#include "Widgets/ImportWidgets.h"
#include "Tools/DrawTools/Draw.h"
#include "Tools/KittyMemory/KittyInclude.hpp"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_projectvb_Menu_imageBase64(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF(OBFUSCATE("iVBORw0KGgoAAAANSUhEUgAAAJYAAACWCAMAAAAL34HQAAAAaVBMVEVHcEwEBAEEBAEEBAEEBAEEBAEEBAEEBAEEBAEDBAEDBAEDAwEDAwEEBAIAAAD7/Pz////9/v4aGhrV1tYvLy8PDw/q6+tAQUHz9PWwsLG6u7pzc3Pf4N9YWFihoaGPj4/KysppaWmBgYGiZVfsAAAADHRSTlMAjDT1bArn1r8ZTK4qlxjFAAAKd0lEQVR42tWch4KiMBBARbGtCU26VP//I29KQlMUOze7d7IQ8DGZTGYSyGLxiqz3yz/TOOy2m9VKgqxWm+3uYJh/y/168RvZL00DeaQlLNmKQJFAZ5jL/ZeR1kvzsF3JO7LaHszl17S2/zN2d5EatJ3xt/+GnoztZCatNOPDOtubuweZiAt0Zn5OZaAo+aygyj4EtZEvyeYDYC9DfQJs/w4odGpb4302tja38m2yNd/UKpcH+VY5LN+iqo18s2xeV9hyJz8gu+XcVPUGhe2NlfyQrJ5vkp+pQCXWsxX5t5Ufle3fM1SfMquugT1h7Cv5cVk9avhr4wtUaPjrx6jkl+QRru9RPcT1RaoHuMzVN7FW5hyppnL9beSXZTPBry638uuyvdsP7XfyB7Lbz8c1PNAcv23u08x+uZE/ks0N81rv5M9kN16N5u+opDDnV4W3qnF9+CWVtA7r2VUhydVq3G9/jbXd/zqaGXGqc7P3UaufgbKuqGsOyrqirlko60Jd81DWhbpmoqyBun7vs676LlPORsx5BDQ3Apzlaj5Yq+X8DL5n9PMx+J7R/72vDgX+sNwtObJz9ffuOhQDeakWR9rhM1eGEyySF7As1RZ1OxQjcqlwMY4lfRLr+slCTqha1RbNlsoaykVt3FQBXMCrQpAkGOMaP59K0mH2qIfmhLxs5Yw/fq862JpHjJfLWV7m2LZTBe0N9c+4jqXoBWMfOu4Br+lXjqvEQXGrhmukfnsfuMVYaTA4r2uw3c8rxqNcROPimcsGcekX/nPucQ3EQm2BgLYeOq9/A2RcZscykMthTYHeJnBp+2uMMahPILlsLXMSoNXTq9n1WoK5wjDL0GxDu+GyLttC2yR4A89XW+pfI9joe6dcnNxpX9pzdbwW1wIJNvLEpXosWyWcTucItnPe9mFvURQ5Ho+KJCnhBryaJFdl6sDyzkmSoPYk7ao9vkCNx5pr0dWjxuLAc3U7RNwd5ShwsuUlaGhOgWeXNlWr6xyBKw+plnOviOEjrIWVH3FHCqAlFMKWWMdQ3Mk8P7XhMz5JS6ZoHiGyH7FZZB5d1+Wbd5wssnQtgs33onjYSSxxhCqo6YyCzrb5fJu4zrRZQLvF/WFehzYdgwr3Mqz6KghSuI6bnY7choBL0h7GwjJHj69rc+MK87YWIaLv9NO0t4tls7aCMuYbQgzkqvlasIM+w5iR4RYkfSVoi69jx67Dh0Kf9xBWZROW1FR4obNolIW9tdnxwhrL7WCBtk50blKWRYzfmQg+4MRppvxImJK+nJSxXI0FZaojnR1H4gKLNOukeX4OnSLomLwwF8agaxliOYUotALVdRjLqaIgyuAAVKcvT7HGsrvaSvPAPw6xAsSyNRY2qbzwui0RmuKh3zdc0dYFlmSsE7SuRNuLH17BoiPaLEa05WbnKLBEz0GIw2J3B8sp+1ju41juEEuZPGGhCVRIxlwqz1hsr2HZiCWpJYKSO1j2fW25V7UVMhbtCY5aW442+TA9+UGLtV1sxrV1GsEST2DFHSxPYQXnKlYNFRpsWLe1uFmshLjAcnqV2GDJqVjubaxA+y3Ly4ujJnPObS2urmK5l1iu1lZrW+c+ljtZW9Q2HcSyFNkllryC1XGnboOVe14U4g7wW2Sq41haW04Pi7x8fIrKo93BAhavHGLJhRjFCgqXsXL8ShsDC7qcz73SU9qCjZAdP2MFtQ++IVdY4h4WqMbnLifDvpm9edMndrFcjQVbhOVobUEX3WChDUAPry5BfibEWz/bIQQeHSzFsRhkAMo7g2rQjpgKuSh+gH4RqayaIgjESnEjZiyMIaSOTtsjagviA+IC2/RpD14qhz7Tyc5nFdh1sFZXHATFC47WFXFV1RElxaBQ5hlsZjVsFdkxyzBS9CvYl0FXneJnEsARkKNPZdQWxD+YFXl4FhSqoggcF95rzNFHW4fQEuUIVqsrigOV8B8UKgbNFgZ5HD42nyqcxLi03bIwvASvyXugt2wzmqNv9bA2g8SIFczS6IrD0VNdc8T5tPSiZ8wbtGgqhbVZbAdYpySBSLhI8JepRB3qpAONpBbWi8KJh6zhm/BHfVM3c9sudsNxhzZPFZoKHYXLmRCGv5w28IABbAa4EWBFBgH+8h8B1Jf2TKqIsgLRy4h1Ot9NJ8VucRhiDfVehyo0pRztTCR1kpKU8MURb54srzhWUcn7pZ+AdVesbr9IG+HQWIrBNw2GFA6DUaSL/I3ju7hE0wJHVqJhwT7ONxw3wYCcoAtZohvIY3IQOcXw5E6gcUJRdWNQvptYjww7Wcb4IDNjkfd2uaPI7UJRuU1mAN/D+X1RQ0QfRwG50zjmMB8bjZeqcN1tkuEJw823hgIJy26w6sRjKlsNBqDJJZ5y7Oi6EYtM0IlDV7lu6sKyNK1sbtkTqCDFuDWtMsAK6P+Io3ZIJivuVYjEpkrVWHER+Sn3UJSHOYmU3vmYW9OwICG7NZ6Lo2gtFpuo1B2YpeMwhQUhZlIHuqvmci5gUZd4rMGNemIiFqSvN6cKetpSjWcEyz6efanidBqx0VipSiZTDtinmBYOUxpCTNKWHqK5jkV+2lKRZ4PFlcjhp+tiKiGncOGgrimm2Za40FbZxaq4T9KZT1suTzK7idgzfwqWeWdm5TZWV1uujoEHWJCcSp8idpXzT8CiYbcbNn+hLes2Fo6dXmJRXJyXmc3h6n0snsc4TDR5tBw5rMTGQXSxqDyVcxErOnm69CSsw73JxAGWn+JYFrpSp4CeOOUsotGW0Fg49uCnGt+vXGylZ5Vz3Mcy703bKSyb+kLo3DCs8ajFxxBWxzxw5V3BAi9vq3yEu8QwAfdKPf1dLDVdcMNzARY7JbuMoihVGKlDY3Au/6kG9/pYdjNGR/kTpc0YfwQTlKUnOo1bNh+prCeOOcmMcxy9VBKfyYNSfKmxyNDKWCURQdlEoC7pfJrXuj1xh22PuVw14IcDpMB1ZKGY0Etxk+dTdEv0TrALw62cC1KCcg6mOPlm4m69vRXcSMhPOA+gbMprcwyVN6gcQk2uKN8qOAeRVDLA38CT1pR5s3Zu37g9ExdlTUWk3mCUXvTn6jp9onU5nj9pOs+YNIVOXCct3p0piS7W9ZmTW12d6E+h344ixJWZvEGoK5pUgbGOgdXMj3HoPmkyUfQfOLj5eMbkid4Gy8UZsqcmX+Ecc+rDLNNnnwkLovjCnxTt3X8Qz3hiGn4MKzwFlvXsTLXx/gelePa1ii7zvqefKzPehXX2noe6fArvXerC/P/5RyAun1l8yzMaLz6YYRkfesDzFajrjzXP83HYMd8lvkd19eHhnz+JN/Liw48fTJcjD6bP9DH+H1ej+b+9IjLTF2rm+vrRXF/WmumrbXN9EXCur03O9SXTr3NNfUV+Pc8XmL8ae83zNfTHXtpfzHIpgbkuvDDXZSpmu6jHh5dAeWEtm3kuGPPI8jriUbN6bT2iqRX5WCq9e3n1pnku3TTXha5muyzYGxdRkxvjvQv1zXLJudku0PfqcobyU8sZPr/4o/zs4o+zXSqTyWa4sOh8l2EdLlp7Deg3i9Y2WvvUEr//AH3+1LtBMuRzAAAAAElFTkSuQmCC"));
}

struct {
    bool ActivateAll = false;
    bool speedhackjoy = false;
    bool medikitrun = false;
    bool ghoston = false;
    bool aimkillbrutal = false;
    bool ghostrun = false;
    bool Aimkilltpv2 = false;
    bool Aimkilltpv3 = false;
    bool AutoSwitchEnabled = false;
    bool ultraswitch = false;
    bool fastswitch = false;
    bool SpeedTimer = false;
    bool cameraup = false;
    bool AutoSwitchEnabled3 = false;
    bool rootbyapss = false;
    bool YflyPower = false;
    bool flySpeedPower = false;
    bool flyhackk = false;
} MasterBool;

struct {
    bool Aimkill = false;
    bool Aimkill360 = false;
    bool Aimkilltp = false;
    bool AimkillUp = false;
    bool shakekillultra = false;
    bool Aimkillrotatev2 = false;
    bool Aimkillrotate = false;
    bool AimSilent = false;
    bool AimSilent360 = false;
    float aimbotFOV = 0.0f;
    bool downplayer = false;
    bool dwonplayerv2 = false;
    bool telehack = false;
    bool resetguest = false;

} pAimbotPlayer;

struct {
    bool espLine = false;
    bool espBox = false;
    bool espDistance = false;
    bool espHealth = false;
    bool espLineTracker = false;
    bool espDrawFov = false;
    Color espColor = Color::White();
    bool espNickName = false;
    int lineType = 0;
    int boxType = 0;
} pEspPlayer;

struct {

    bool UpPlayer = false;

} pMemoryTools;

using namespace std;

extern "C"
JNIEXPORT void JNICALL
Java_com_projectvb_Menu_Init(JNIEnv *env, jclass thiz) {
    startClient();
}


#include <iostream>
#include <cstdio>
#include <cstdlib>

float fovRadius = 104;

extern "C"
JNIEXPORT void JNICALL
Java_com_projectvb_Menu_Functions(JNIEnv *env, jclass clazz) {
    Widget widget = Widget(env);

    widget.Tab("Aim Functions");
    widget.Tab("Advanced Functions");
    widget.Tab("ESP Functions");
    widget.Tab("Other Functions");

    widget.Tab1("Aim Functions");
    widget.Category(OBFUSCATE("Aim"));
    widget.Switch(OBFUSCATE("Active Funtions"), 102);
    widget.Switch(OBFUSCATE("Aimkill"), 100);
    widget.Switch(OBFUSCATE("Aimkill 360"), 101);
    widget.Switch(OBFUSCATE("Aimkill 360 MAX"), 198);
    widget.Switch(OBFUSCATE("Aimkill Brutal"), 920);
    widget.Switch(OBFUSCATE("Aim Silent"), 891);
    widget.Switch(OBFUSCATE("Aimkill Closest"), 105);



    widget.Tab1("Advanced Functions");
    widget.Category(OBFUSCATE("Misc"));
    widget.Switch(OBFUSCATE("ShakeKill V1"), 153);
    widget.Switch(OBFUSCATE("ShakeKill V2"), 154);
    widget.Switch(OBFUSCATE("ShakeKill V3"), 10110);
    widget.Switch(OBFUSCATE("Teleport Tyre"), 161);
    widget.Switch(OBFUSCATE("Down Kill Ultra"), 162);
    widget.Switch(OBFUSCATE("Auto Switch"), 922);

    widget.Tab1("ESP Functions");
    widget.Category(OBFUSCATE("ESP"));
    widget.Switch(OBFUSCATE("ESP Line"), 1);
    widget.Switch(OBFUSCATE("ESP Distance"), 4);
    widget.Switch(OBFUSCATE("ESP Box"), 2);
    widget.Switch(OBFUSCATE("ESP Health"), 3);
    widget.Switch(OBFUSCATE("ESP Name"), 9);
    widget.Category(OBFUSCATE("Config"));
    widget.Switch(OBFUSCATE("Show Fov"), 16);
    widget.SeekBar(OBFUSCATE("Fov Size"), 0, 1200, "x", 104);
    widget.SeekBar(OBFUSCATE("Draw Color"), 0, 0, OBFUSCATE("Color"), 5);
    widget.SeekBar(OBFUSCATE("Line Varity"), 0, 0, OBFUSCATE("LineType"), 6);

    widget.Tab1("Other Functions");
    widget.Category(OBFUSCATE("Other"));
    widget.Switch(OBFUSCATE("Joystic Speed"), 155);
    widget.Switch(OBFUSCATE("Reset Guest"), 1002);
    widget.Switch(OBFUSCATE("Camera Up"), 1154);
    widget.Switch(OBFUSCATE("Ultra Switch V1"), 199);
    widget.Switch(OBFUSCATE("Ultra Switch V2{Risk}"), 5440);

}



extern "C"
JNIEXPORT void JNICALL
Java_com_projectvb_Menu_ChangesID(JNIEnv *env, jclass clazz, jint id, jint value) {
    switch (id) {

        case 102:

            MasterBool.ActivateAll = !MasterBool.ActivateAll;
            SendFeatuere(3, MasterBool.ActivateAll);
            break;


        case 100:

            pAimbotPlayer.Aimkill = !pAimbotPlayer.Aimkill;
            SendFeatuere(100, pAimbotPlayer.Aimkill);

            break;

        case 101:

            pAimbotPlayer.Aimkill360 = !pAimbotPlayer.Aimkill360;
            SendFeatuere(198, pAimbotPlayer.Aimkill360);

            break;
        case 161:

            pAimbotPlayer.telehack = !pAimbotPlayer.telehack;
            SendFeatuere(161, pAimbotPlayer.telehack);

            break;
        case 162:

            pAimbotPlayer.downplayer = !pAimbotPlayer.downplayer;
            SendFeatuere(162, pAimbotPlayer.downplayer);

            break;
        case 1002:

            pAimbotPlayer.resetguest = !pAimbotPlayer.resetguest;
            SendFeatuere(1002, pAimbotPlayer.resetguest);

            break;

        case 153:

            pAimbotPlayer.Aimkillrotate = !pAimbotPlayer.Aimkillrotate;
            SendFeatuere(153, pAimbotPlayer.Aimkillrotate);

            break;
        case 154:

            pAimbotPlayer.Aimkillrotatev2 = !pAimbotPlayer.Aimkillrotatev2;
            SendFeatuere(154, pAimbotPlayer.Aimkillrotatev2);

            break;

        case 920:

            MasterBool.aimkillbrutal = !MasterBool.aimkillbrutal;
            SendFeatuere(920, MasterBool.aimkillbrutal);

            break;
        case 909:

            MasterBool.Aimkilltpv2 = !MasterBool.Aimkilltpv2;
            SendFeatuere(909, MasterBool.Aimkilltpv2);

            break;

        case 111:

            pAimbotPlayer.AimkillUp = !pAimbotPlayer.AimkillUp;
            SendFeatuere(111, pAimbotPlayer.AimkillUp);

            break;
        case 10110:

            pAimbotPlayer.shakekillultra = !pAimbotPlayer.shakekillultra;
            SendFeatuere(10110, pAimbotPlayer.shakekillultra);

            break;
        case 105:

            pAimbotPlayer.Aimkilltp = !pAimbotPlayer.Aimkilltp;
            SendFeatuere(105, pAimbotPlayer.Aimkilltp);

            break;
        case 922:

            MasterBool.AutoSwitchEnabled = !MasterBool.AutoSwitchEnabled;
            SendFeatuere(922, MasterBool.AutoSwitchEnabled);

            break;
        case 129:

            pAimbotPlayer.Aimkillrotate = !pAimbotPlayer.Aimkillrotate;
            SendFeatuere(129, pAimbotPlayer.Aimkillrotate);

            break;
        case 890:

            pAimbotPlayer.Aimkillrotatev2 = !pAimbotPlayer.Aimkillrotatev2;
            SendFeatuere(890, pAimbotPlayer.Aimkillrotatev2);
            break;
        case 891:

            pAimbotPlayer.AimSilent = !pAimbotPlayer.AimSilent;
            SendFeatuere(891, pAimbotPlayer.AimSilent);
            break;
        case 163:

            pAimbotPlayer.dwonplayerv2 = !pAimbotPlayer.dwonplayerv2;
            SendFeatuere(163, pAimbotPlayer.dwonplayerv2);

            break;


        case 269:

            MasterBool.SpeedTimer = !MasterBool.SpeedTimer;
            SendFeatuere(269, MasterBool.SpeedTimer);

            break;
        case 1154:
            MasterBool.cameraup = !MasterBool.cameraup;
            SendFeatuere(1154, MasterBool.cameraup);
            break;
        case 104:

            pAimbotPlayer.aimbotFOV = value;
            SendFOV(104, value);
            break;



        case 1:
            pEspPlayer.espLine = !pEspPlayer.espLine;
            break;

        case 2:
            pEspPlayer.espBox = !pEspPlayer.espBox;
            break;

        case 3:
            pEspPlayer.espHealth = !pEspPlayer.espHealth;
            break;
        case 9:
            pEspPlayer.espNickName = !pEspPlayer.espNickName;
            break;

        case 4:
            pEspPlayer.espDistance = !pEspPlayer.espDistance;
            break;
        case 5:
            if (value == 0) {
                pEspPlayer.espColor = Color::White();
            } else if (value == 1) {
                pEspPlayer.espColor = Color::Green();
            } else if (value == 2) {
                pEspPlayer.espColor = Color::Blue();
            } else if (value == 3) {
                pEspPlayer.espColor = Color::Red();
            } else if (value == 4) {
                pEspPlayer.espColor = Color::Black();
            } else if (value == 5) {
                pEspPlayer.espColor = Color::Yellow();
            } else if (value == 6) {
                pEspPlayer.espColor = Color::Cyan();
            } else if (value == 7) {
                pEspPlayer.espColor = Color::Magenta();
            } else if (value == 8) {
                pEspPlayer.espColor = Color::Gray();
            } else if (value == 9) {
                pEspPlayer.espColor = Color::Purple();
            }
            break;
        case 6:
            if (value == 0) {
                pEspPlayer.lineType = value;
            } else if (value == 1) {
                pEspPlayer.lineType = value;
            } else if (value == 2) {
                pEspPlayer.lineType = value;
            }
            break;
        case 7:
            if (value == 0) {
                pEspPlayer.boxType = value;
            } else if (value == 1) {
                pEspPlayer.boxType = value;
            } else if (value == 2) {
                pEspPlayer.boxType = value;
            }
            break;
/*
        case 6:
            pEspPlayer.lineType = value;
            break;

        case 7:
            pEspPlayer.boxType = value;
            break;*/

        case 107:
            pMemoryTools.UpPlayer = !pMemoryTools.UpPlayer;
            SendFeatuere(6, pMemoryTools.UpPlayer);
            break;
        case 155:
            MasterBool.speedhackjoy = !MasterBool.speedhackjoy;
            SendFeatuere(155, MasterBool.speedhackjoy);
            break;
        case 199:
            MasterBool.ultraswitch = !MasterBool.ultraswitch;
            SendFeatuere(199, MasterBool.ultraswitch);
            break;
        case 5440:
            MasterBool.fastswitch = !MasterBool.fastswitch;
            SendFeatuere(5440, MasterBool.fastswitch);
            break;
        case 16:
            pEspPlayer.espDrawFov = !pEspPlayer.espDrawFov;
            break;
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_projectvb_Menu_OnDrawLoad(JNIEnv *env, jclass clazz, jobject draw_view, jobject canvas) {
    DrawView draw = DrawView(env, draw_view, canvas);

    if (draw.isValid()) {
        Response response = getData(draw.getWidth(), draw.getHeight());


        if (response.Success) {

            for (int i = 0; i < response.PlayerCount; ++i) {
                PlayerData data = response.Players[i];


                Vector3 HeadLoc = data.headPosition;
                Vector3 PesLoc = data.bottomPlayerPosition;

                if (HeadLoc.Z < -1) continue;

                if (PesLoc.Z < -1) continue;


                float distance = data.distance;
                bool IsCaido = data.isDieing;

                float scale = std::max(0.5f, std::min(1.0f, 500.0f / distance));
                float boxHeight = abs(HeadLoc.Y - PesLoc.Y) * scale;
                float boxWidth = boxHeight * 0.50f;
                Rect PlayerRect(HeadLoc.X - (boxWidth / 2), draw.getHeight() - HeadLoc.Y, boxWidth, boxHeight);


                if (pEspPlayer.espLine) {
                    Vector2 lineStart;
                    Vector2 lineEnd;

                    if (pEspPlayer.lineType == 0) {
                        lineStart = Vector2(draw.getWidth() / 2, 0);
                        lineEnd = Vector2(HeadLoc.X, draw.getHeight() - HeadLoc.Y);
                    } else if (pEspPlayer.lineType == 1) {
                        lineStart = Vector2(draw.getWidth() / 2, draw.getHeight() / 2);
                        lineEnd = Vector2(HeadLoc.X, draw.getHeight() - HeadLoc.Y);
                    } else if (pEspPlayer.lineType == 2) {
                        lineStart = Vector2(draw.getWidth() / 2, draw.getHeight());
                        lineEnd = Vector2(PesLoc.X, draw.getHeight() - PesLoc.Y);
                    }

                    if (IsCaido) {
                        draw.DrawLine(Color::Red(), 1, lineStart, lineEnd);
                    } else {
                        draw.DrawLine(pEspPlayer.espColor, 1, lineStart, lineEnd);
                    }
                }


                if (pEspPlayer.espBox) {
                    if (!IsCaido) {

                        if (IsCaido) {
                            if (pEspPlayer.boxType == 0) {
                                draw.DrawBox(Color::Red(), 1, PlayerRect);
                            } else if (pEspPlayer.boxType == 1) {
                                draw.DrawBox3D(Color::Red(), 1, PlayerRect, 10);
                            } else if (pEspPlayer.boxType == 2) {
                                draw.DrawCornerBox(Color::Red(), 1, PlayerRect, 4, 4);
                            }
                        } else {
                            if (pEspPlayer.boxType == 0) {
                                draw.DrawBox(pEspPlayer.espColor, 1, PlayerRect);
                            } else if (pEspPlayer.boxType == 1) {
                                draw.DrawBox3D(pEspPlayer.espColor, 1, PlayerRect, 10);
                            } else if (pEspPlayer.boxType == 2) {
                                draw.DrawCornerBox(pEspPlayer.espColor, 1, PlayerRect, 4, 4);
                            }
                        }
                    }
                }
                if (pEspPlayer.espNickName) {
                    if (!IsCaido) {

                        if (data.name[0] != '\0') {
                            Vector2 namePos = { PlayerRect.x + PlayerRect.w / 2.0f, PlayerRect.y - 15.0f };
                            draw.DrawText(Color::White(), data.name, namePos, 12);
                        }


                    }
                }

                if (pEspPlayer.espDrawFov) {
                    float centerX = draw.getWidth() / 2.0f;
                    float centerY = draw.getHeight() / 2.0f;

                    if (!IsCaido) {
                        draw.DrawCircle(pEspPlayer.espColor, 1, Vector2(centerX, centerY),  pAimbotPlayer.aimbotFOV);
                    }
                }
                if (pEspPlayer.espDistance) {
                    float centerX = draw.getWidth() / 2.0f;
                    float centerY = draw.getHeight() / 2.0f;

                    if (!IsCaido) {
                        Vector2 namePos(PlayerRect.x + (PlayerRect.w / 2), PlayerRect.y - (5.0f * scale));
                        namePos.X -= (strlen(data.name) * 2.5f * scale);
                        float textSize = 12.0f * scale;
                        Vector2 shadowOffset(1.0f, 1.0f);
                        char distanceText[32];
                        sprintf(distanceText, "%dm", static_cast<int>(data.distance));
                        draw.DrawTextWithShadow(Color(255, 255, 255, 255), distanceText, Vector2(namePos.X, PlayerRect.y + PlayerRect.h + (8.0f * scale)), textSize,shadowOffset,2.0f);


                    }
                }




                if (pEspPlayer.espHealth) {
                    if (!IsCaido) {

                        Vector2 healthBarPos(PlayerRect.x - 5.0f * scale, PlayerRect.y);
                        float healthBarWidth = 2.0f * scale;
                        float healthBarHeight = boxHeight;
                        draw.DrawVerticalHealthBar(healthBarPos, healthBarHeight, 200.0f,data.health);
                    }
                }
                if (pEspPlayer.espLineTracker) {

                    float minDist = 1e9;
                    PlayerData* closest = nullptr;
                    for (int i = 0; i < response.PlayerCount; ++i) {
                        PlayerData* pdata = &response.Players[i];
                        if (pdata->isDieing) continue;
                        if (pdata->distance < minDist) {
                            minDist = pdata->distance;
                            closest = pdata;
                        }
                    }
                    if (closest) {
                        Vector3 HeadLoc = closest->headPosition;
                        Vector2 crosshair(draw.getWidth() / 2, draw.getHeight() / 2);
                        Vector2 enemyHead(HeadLoc.X, draw.getHeight() - HeadLoc.Y);
                        draw.DrawLine(Color::Yellow(), 2, crosshair, enemyHead);
                    }
                }
            }
        }
    }
}
