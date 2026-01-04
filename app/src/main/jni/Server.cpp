#include <list>
#include <vector>
#include <string.h>
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
#include "Tools/SOCKET/Server.h"
#include "Tools/KittyMemory/KittyInclude.hpp"
#include "Hack/Memory.h"
#include "Hack/Offsets.h"
#include "Hack/class.h"
#include "Hack/il2cpp.h"

#define targetLibName OBFUSCATE("libil2cpp.so")
#include "Tools/Includes/Macros.h"
#include "Tools/Canvas/Color.hpp"
#include "Tools/Substrate/SubstrateHook.h"
#include <sys/mman.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "VASTCLIENT", __VA_ARGS__)




void *getAbsoluteAddress2(const char *libraryName, uintptr_t relativeAddr) {
    void *handle = dlopen(libraryName, RTLD_NOW);
    if (!handle) return nullptr;

    void *baseAddr = dlsym(handle, "base");
    dlclose(handle);

    if (!baseAddr) return nullptr;
    return (void *)((uintptr_t)baseAddr + relativeAddr);
}
#define HOOK_LIB2(offset, ptr, orig) MSHookFunction((void *)getRealOffset((offset)), (void *)ptr, (void **)&orig)

struct {
    float YflyPower = 1.0f;  // 1.0 = neutral
    float flySpeedPower = 1.0f; // 1.0 = neutral speed
} FlyControl;

struct {
    bool enableAimbot = false;

    bool medikitrun = false;
    bool Aimkilltp = false;
    bool AimkillUp = false;
    bool resetguest = false;
    bool Aimkillrotate = false;
    bool shakekillultra = false;
    float aimbotFOV = 0.0f;
    bool Aimkillrotatev2 = false;
    bool aimkillbrutal = false;
    bool POFFNNMOOBM = false;
    bool Aimkilltpv2 = false;
    bool aimbody = false;
    bool upplayerx = false;
    bool teleportcarr = false;
    bool telehack = false;
    int GDKLMFLNNGM = 0;
    bool FOV = true;
    bool ActivateAll = false;
    bool freeze = false;
    bool AimSilent = false;
    bool AimSilent360 = false;
    bool Aimkill = false;
    bool downplayer = false;
    bool Aimkill360 = false;
    bool UpPlayer = false;
    bool speedhackjoy = false;
    bool SpeedTimer = false;
    bool flyhack = false;
    bool downaimkill = false;
    bool ultraswitch = false;
    bool fastswitch = false;
    bool cameraup = false;
    bool AutoSwitchEnabled = false;

} MasterBool;
struct {

    float TFPosX = 0.0f;
    float TFPosY = 0.0f;
    float TFPosZ = 0.0f;

} NT;


#include "dobby.h"
int g_screenWidth, g_screenHeight;

ElfScanner g_il2cppELF;


static int isSpeed = 1;

enum Mode {
    InitMode = 1,
    HackMode = 2,
    StopMode = 98,
    EspMode = 99,
};

struct Request {
    int Mode;
    bool boolean;
    int value;
    int ScreenWidth;
    int ScreenHeight;
};

#define maxplayerCount 54

struct PlayerData {
    Vector3 headPosition;
    Vector3 bottomPlayerPosition;

    float health;
    char name[2000];
    bool isDieing;
    float distance;
};

struct Response {
    bool Success;
    int PlayerCount;
    PlayerData Players[maxplayerCount];
};

SocketServer server;

int InitServer() {
    if (!server.Create()) {
        return -1;
    }
    if (!server.Bind()) {
        return -1;
    }
    if (!server.Listen()) {
        return -1;
    }
    return 0;
}

uintptr_t getLibBase(const char* libName) {
    uintptr_t base = 0;
    FILE* fp = fopen("/proc/self/maps", "rt");
    if (!fp) return 0;

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, libName)) {
            base = strtoul(line, NULL, 16);
            break;
        }
    }
    fclose(fp);
    return base;
}
MemoryPatch patchFunction;
void unlockMemory(uintptr_t address) {
    uintptr_t pageStart = address & ~(getpagesize() - 1);
    mprotect((void*)pageStart, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC);
}

MemoryPatch patchFunction2;
MemoryPatch patchFunction3;


using namespace std;
std::string utf16le_to_utf82(const std::u16string &u16str) {
    if (u16str.empty()) { return std::string(); }
    const char16_t *p = u16str.data();
    std::u16string::size_type len = u16str.length();
    if (p[0] == 0xFEFF) {
        p += 1;
        len -= 1;
    }

    std::string u8str;
    u8str.reserve(len * 3);

    char16_t u16char;
    for (std::u16string::size_type i = 0; i < len; ++i) {

        u16char = p[i];

        if (u16char < 0x0080) {
            u8str.push_back((char) (u16char & 0x00FF));
            continue;
        }
        if (u16char >= 0x0080 && u16char <= 0x07FF) {
            u8str.push_back((char) (((u16char >> 6) & 0x1F) | 0xC0));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
        if (u16char >= 0xD800 && u16char <= 0xDBFF) {
            uint32_t highSur = u16char;
            uint32_t lowSur = p[++i];
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            u8str.push_back((char) ((codePoint >> 18) | 0xF0));
            u8str.push_back((char) (((codePoint >> 12) & 0x3F) | 0x80));
            u8str.push_back((char) (((codePoint >> 06) & 0x3F) | 0x80));
            u8str.push_back((char) ((codePoint & 0x3F) | 0x80));
            continue;
        }
        {
            u8str.push_back((char) (((u16char >> 12) & 0x0F) | 0xE0));
            u8str.push_back((char) (((u16char >> 6) & 0x3F) | 0x80));
            u8str.push_back((char) ((u16char & 0x3F) | 0x80));
            continue;
        }
    }

    return u8str;
}

typedef struct _monoStringlk {
    void *klass;
    void *monitor;
    int length;
    char chars[1];
    int getLength() {
        return length;
    }
    const char *toChars(){
        u16string ss((char16_t *) getChars(), 0, getLength());
        string str = utf16le_to_utf82(ss);
        return str.c_str();
    }
    char *getChars() {
        return chars;
    }
    std::string get_string() {

        return std::string(toChars());
    }
} monoStringlk;


void CreateExtraSensoryPerceptionhackFixed(Response &response)
{

    if (MasterBool.ActivateAll) {
        void *GameFacade = *(void **) getRealOffset(pAddress.GameFacade);
        if (GameFacade != nullptr) {
            void *MatchGame = *(void **) ((uint64_t) GameFacade + pAddress.AcessClass);
            if (MatchGame != nullptr) {
                void *ClassMatchGame = *(void **) ((uint64_t) MatchGame + pAddress.CurrentMatchGame);
                if (ClassMatchGame != nullptr) {
                    void *current_match = *(void **) ((uint64_t) ClassMatchGame + pAddress.m_Match);
                    if (current_match != nullptr) {
                        auto matchStatus = *(uint32_t *) ((uint64_t) current_match + pAddress.LICPHHNNPPF);
                        if (matchStatus == 1) {
                            void *CurrentLocalPlayer = Current_Local_Player();
                            if (CurrentLocalPlayer != nullptr) {
                                System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o *MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o **)((uint64_t) current_match + pAddress.DictionaryEntities);
                                if (MonoPlayer != nullptr) {
                                    COW_GamePlay_Player_array *players = MonoPlayer->valueSlots;
                                    if (players != nullptr) {
                                        int countagempartida = players->max_length;
                                        if (countagempartida != 1)
                                        {
                                            for (int i = 0; i < countagempartida; ++i) {
                                                if (players->m_Items[i] != nullptr && players->m_Items[i] != CurrentLocalPlayer) {
                                                    if (IsStreamerVisible(players->m_Items[i]) &&!IsLocalTeammate(players->m_Items[i]) &&GetHp(players->m_Items[i]) > 0) {

                                                        void *HeadTF = TransformNode(*(void **) ((uintptr_t) players->m_Items[i] +pAddress.HeadTF));
                                                        void *PesTF = TransformNode(*(void **) ((uintptr_t) players->m_Items[i] +pAddress.PesTF));
                                                        void *HeadTFLocal = TransformNode(*(void **) ((uintptr_t) CurrentLocalPlayer +pAddress.HeadTF));
                                                        Vector3 WorldToScreenHead = WorldToScreenPoint(Transform_INTERNAL_GetPosition(HeadTF));

                                                        Vector3 WorldToScreenPes = WorldToScreenPoint(Transform_INTERNAL_GetPosition(PesTF));

                                                        bool Caido2 = IsDieing(players->m_Items[i]);
                                                        float distance = Vector3::Distance(Transform_INTERNAL_GetPosition(HeadTFLocal),Transform_INTERNAL_GetPosition(HeadTF));
                                                        monoStringlk *Nick = *(monoStringlk **) ((uint64_t) players->m_Items[i] + pAddress.OIAJCBLDHKP);
                                                        PlayerData *data = &response.Players[response.PlayerCount];
                                                        if (data != nullptr) {
                                                            data->headPosition = WorldToScreenHead;
                                                            data->bottomPlayerPosition = WorldToScreenPes;
                                                            data->distance = distance;
                                                            data->isDieing = Caido2;
                                                            data->health = GetHp(players->m_Items[i]);
                                                            if (Nick->toChars() != NULL) {
                                                                strcpy(data->name,Nick->toChars());
                                                            }



                                                        }
                                                        ++response.PlayerCount;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        } else {

                            response.PlayerCount = 0;
                        }
                    }
                }
            }
        }

    }
}

void ghoston() {
    uintptr_t libBase = getLibBase("libil2cpp.so");
    if (libBase == 0) {
        return;
    }
    uintptr_t targetOffset = pAddress.GhostModePatch; //private void SyncPlayerState(Player player) { }
    uintptr_t targetAddress = libBase + targetOffset;
    unlockMemory(targetAddress);
    patchFunction = MemoryPatch::createWithHex(targetAddress, "00 00 A0 E3 1E FF 2F E1"); // fast switch
    patchFunction.Modify();
}
void ghostoff() {
    uintptr_t libBase = getLibBase("libil2cpp.so");
    if (libBase == 0) {
        return;
    }
    uintptr_t targetOffset = pAddress.GhostModePatch;
    uintptr_t targetAddress = libBase + targetOffset;
    unlockMemory(targetAddress);
    patchFunction = MemoryPatch::createWithHex(targetAddress, "D5 C5 75 36 8B DC DD 3E"); // fast switch D5 C5 75 36 8B DC DD 3E //09 0B 1A E8 08 0B 0B 01
    patchFunction.Modify();
}

void allfunction(){
    if(MasterBool.ActivateAll){
        if (MasterBool.downaimkill) {
            MasterBool.downplayer = true;
            MasterBool.flyhack = true;

        } else {
            MasterBool.downplayer = false;
            MasterBool.flyhack = false;
        }
    }

    if(MasterBool.ActivateAll){
        if(MasterBool.Aimkilltpv2){
            // ghoston();
        }
        else{
            // ghostoff();
        }
    }
}

void *CreateServer(void *) {
    if (InitServer() == 0) {
        if (server.Accept()) {
            Request request{};
            while (server.receive((void*)&request) > 0) {
                Response response{};

                allfunction();

                if (request.Mode == Mode::InitMode) {
                    response.Success = true;
                } else if (request.Mode == Mode::EspMode) {
                    g_screenWidth = request.ScreenWidth;
                    g_screenHeight = request.ScreenHeight;

                    response.Success = true;
                    CreateExtraSensoryPerceptionhackFixed(response);
                } else if (request.Mode == 3) {
                    MasterBool.ActivateAll = request.boolean;
                    response.Success = true;
                }
                else if (request.Mode == 890) {
                    MasterBool.Aimkillrotatev2 = request.boolean;
                    response.Success = true;
                }
                else if (request.Mode == 1002) {
                    MasterBool.resetguest = request.boolean;
                    response.Success = true;
                }

                else if (request.Mode == 891) {
                    MasterBool.AimSilent = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 269)
                {
                    MasterBool.SpeedTimer = request.boolean;
                    response.Success = true;
                }

                else if (request.Mode == 100)
                {
                    MasterBool.Aimkill = request.boolean;
                    response.Success = true;
                }
                else if (request.Mode == 105)
                {
                    MasterBool.Aimkilltp = request.boolean;
                    response.Success = true;
                }
                else if (request.Mode == 104) {
                    MasterBool.aimbotFOV = request.value;
                    // LOGI(" Aim.maxAngle  %f", MasterBool.aimbotFOV);
                    response.Success = true;
                }else if(request.Mode == 199)
                {
                    MasterBool.ultraswitch = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 155)
                {
                    MasterBool.speedhackjoy = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 161)
                {
                    MasterBool.telehack = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 162)
                {
                    MasterBool.downaimkill = request.boolean;
                    response.Success = true;
                }

                else if (request.Mode == 129) {
                    MasterBool.Aimkillrotate = request.boolean;
                    response.Success = true;
                }
                else if (request.Mode == 111) {
                    MasterBool.AimkillUp = request.boolean;
                    response.Success = true;
                }
                else if (request.Mode == 198){
                    MasterBool.Aimkill360 = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 920){
                    MasterBool.aimkillbrutal = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 922){
                    MasterBool.AutoSwitchEnabled = request.boolean;
                    response.Success = true;
                }

                else if(request.Mode == 153){
                    MasterBool.Aimkillrotate = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 154){
                    MasterBool.Aimkillrotatev2 = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 5440){
                    MasterBool.fastswitch = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 10110){
                    MasterBool.shakekillultra = request.boolean;
                    response.Success = true;
                }
                else if(request.Mode == 1154){
                    MasterBool.cameraup = request.boolean;
                    response.Success = true;
                }


                server.send((void*)& response, sizeof(response));
            }
        }
    }
    return NULL;
}


bool isInsideFOV(int x, int y) {

    float circle_x = g_screenWidth / 2;
    float circle_y = g_screenHeight / 2;
    float rad = MasterBool.aimbotFOV;
    return (x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y) <= rad * rad;
}

Vector3 cSubtract(Vector3 src, Vector3 dst)
{
    Vector3 diff;
    diff.X = src.X - dst.X;
    diff.Y = src.Y - dst.Y;
    diff.Z = src.Z - dst.Z;
    return diff;
}

float cMagnitude(Vector3 vec)
{
    return sqrtf(vec.X*vec.X + vec.Y*vec.Y + vec.Z*vec.Z);
}

float cClamp(float value, float min, float max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}

float CalculateDistance(Vector3 src, Vector3 dst)
{
    Vector3 diff = cSubtract(src, dst);
    return cMagnitude(diff);
}

float Vector3Distance(const Vector3& a, const Vector3& b)
{
    float dx = a.X - b.X;
    float dy = a.Y - b.Y;
    float dz = a.Z - b.Z;
    return sqrtf(dx * dx + dy * dy + dz * dz);
}



//#define lbil2cpp(address, original, backup)  DobbyHook((void *)getRealOffset(address), (void *)original, (void **)&backup)
void* GetClosestEnemy360()
{
    void* closestEnemy = nullptr;
    float closestDistance = 999999.0f;

    void* GameFacade = *(void**) getRealOffset(pAddress.GameFacade);
    if (!GameFacade) return nullptr;

    void* MatchGame = *(void**)((uint64_t)GameFacade + pAddress.AcessClass);
    if (!MatchGame) return nullptr;

    void* ClassMatchGame = *(void**)((uint64_t)MatchGame + pAddress.CurrentMatchGame);
    if (!ClassMatchGame) return nullptr;

    void* current_match = *(void**)((uint64_t)ClassMatchGame + pAddress.m_Match);
    if (!current_match) return nullptr;

    uint32_t matchStatus = *(uint32_t*)((uint64_t)current_match + pAddress.LICPHHNNPPF);
    if (matchStatus != 1) return nullptr; // Not in a live match

    void* LocalPlayer = Current_Local_Player();
    if (!LocalPlayer) return nullptr;

    Vector3 LocalPosition(0.0f, 0.0f, 0.0f);

    void* LocalHead = TransformNode(*(void**)((uint64_t)LocalPlayer + pAddress.HeadTF));
    if (LocalHead) {
        LocalPosition = Transform_INTERNAL_GetPosition(LocalHead);
    } else {
        return nullptr; // Local player head not found
    }

    auto* MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o**)((uint64_t)current_match + pAddress.DictionaryEntities);
    if (!MonoPlayer) return nullptr;

    COW_GamePlay_Player_array* players = MonoPlayer->valueSlots;
    if (!players) return nullptr;

    int playerCount = players->max_length;
    if (playerCount <= 1) return nullptr; // Only you in match

    for (int i = 0; i < playerCount; ++i)
    {
        void* enemy = players->m_Items[i];
        if (!enemy || enemy == LocalPlayer) continue; // Skip null or self

        // Strong checking if enemy is valid
        if (!IsStreamerVisible(enemy)) continue;
        if (IsDieing(enemy)) continue;
        if (IsLocalTeammate(enemy)) continue;
        if (GetHp(enemy) <= 0) continue;

        void* EnemyHead = TransformNode(*(void**)((uint64_t)enemy + pAddress.HeadTF));
        if (!EnemyHead) continue;

        Vector3 EnemyPosition = Transform_INTERNAL_GetPosition(EnemyHead);

        float Distance = Vector3Distance(LocalPosition, EnemyPosition);

        if (Distance < closestDistance)
        {
            closestDistance = Distance;
            closestEnemy = enemy;
        }

    }

    return closestEnemy;
}

void *GetEnemyInsideFOV()
{

    float MaxDist = 9999.0f;
    void *closestEnemy = nullptr;
    void *GameFacade = *(void **) getRealOffset(pAddress.GameFacade);
    if (GameFacade != nullptr) {
        void *MatchGame = *(void **) ((uint64_t) GameFacade + pAddress.AcessClass);
        if (MatchGame != nullptr) {
            void *ClassMatchGame = *(void **) ((uint64_t) MatchGame + pAddress.CurrentMatchGame);
            if (ClassMatchGame != nullptr) {
                void *current_match = *(void **) ((uint64_t) ClassMatchGame + pAddress.m_Match);
                if (current_match != nullptr) {
                    auto matchStatus = *(uint32_t *) ((uint64_t) current_match + pAddress.LICPHHNNPPF);
                    if (matchStatus == 1) {
                        void *CurrentLocalPlayer = Current_Local_Player();
                        if (CurrentLocalPlayer != nullptr) {
                            System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o *MonoPlayer = *(System_Collections_Generic_Dictionary_IHAAMHPPLMG__Player__o **)((uint64_t) current_match + pAddress.DictionaryEntities);
                            if (MonoPlayer != nullptr) {
                                COW_GamePlay_Player_array *players = MonoPlayer->valueSlots;
                                if (players != nullptr) {
                                    int countagempartida = players->max_length;
                                    if (countagempartida != 1) {
                                        for (int i = 0; i < countagempartida; ++i) {
                                            if (players->m_Items[i] != nullptr &&players->m_Items[i] != CurrentLocalPlayer) {
                                                if (IsStreamerVisible(players->m_Items[i]) && !IsDieing(players->m_Items[i]) && !IsLocalTeammate(players->m_Items[i]) && GetHp(players->m_Items[i]) > 0) {
                                                    void *HeadTF = TransformNode(*(void **) ((uint64_t) players->m_Items[i] +pAddress.HeadTF));
                                                    if (HeadTF != nullptr) {
                                                        Vector3 currentLocation = Transform_INTERNAL_GetPosition(HeadTF);
                                                        Vector3 WorldToScreenHead = WorldToScreenPoint(currentLocation);

                                                        if (WorldToScreenHead.Z >= 0.01f) {
                                                            Vector3 v2Middle = Vector3((float) (g_screenWidth / 2),(float) (g_screenHeight / 2));
                                                            Vector3 v2Loc = Vector3(WorldToScreenHead.X,WorldToScreenHead.Y);
                                                            float Distance = (float) CalculateDistance(v2Middle, v2Loc);
                                                            if (isInsideFOV((int) WorldToScreenHead.X,(int) WorldToScreenHead.Y)) {
                                                                if (Distance < MaxDist) {
                                                                    closestEnemy = players->m_Items[i];
                                                                    MaxDist = Distance;
                                                                }
                                                            }
                                                        }

                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return closestEnemy;

}

bool (*Old_Physics$$Raycast)(Vector3, Vector3, UnityEngine_RaycastHit_o *hitInfo, float, int32_t);
bool New_Physics$$Raycast(Vector3 origin, Vector3 direction,UnityEngine_RaycastHit_o *hitInfo, float maxDistance, int32_t layerMask) {
    if (MasterBool.ActivateAll) {
        if (MasterBool.AimSilent) {
            void *closestEnemy = GetEnemyInsideFOV();
            if (closestEnemy != NULL) {

                void *HeadTF = TransformNode(
                        *(void **) ((uint64_t) closestEnemy + pAddress.HeadTF));

                Vector3 enemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);

                direction = enemyHeadPosition - origin;

                auto position = WorldToScreenPoint(enemyHeadPosition);
                position.Y = g_screenHeight - position.Y;
                if (pow(position.X - g_screenWidth / 2, 2) +
                    pow(position.Y - g_screenHeight / 2, 2) >= pow(MasterBool.aimbotFOV, 2)) {
                    direction = enemyHeadPosition - origin;
                }
            }
        }


    }
    return Old_Physics$$Raycast(origin, direction, hitInfo, maxDistance, layerMask);
}






struct COW_GamePlay_PlayerID {
    uint32_t m_Value;
    uint32_t m_ID;
    uint8_t m_TeamID;
    uint8_t m_ShortID;
    uint64_t m_IDMask;
};

struct DamageInfo2_o {
    void *klass;
    void *monitor;
    int32_t DBLBLKADCNP;
    int32_t KENBMOOEHBG;
    monoString* JANPNJIFOJJ;
    bool NNNADMOFPIE;
    COW_GamePlay_PlayerID DHGCIEKPBFA;
    void* GPBDEDFKJNA;
    int32_t PIAMIOFEBKF;
    Vector3 CNEICNJFGLM;
    Vector3 HECJHKEDFEB;
    Vector3 JNLGFLFLBHO;
    uint8_t ACAKHEABPEJ;
    bool MJIHLDJNHLF;
    int32_t LOKIMAEAPCB;
    monoDictionary<uint8_t*, void **> *FHLFLAHCIBN;
};

COW_GamePlay_PlayerID GetplayerID(void *_this)
{
    return ((COW_GamePlay_PlayerID (*)(void *))getRealOffset(pAddress.GetplayerID))(_this);
}
uint32_t CFFPIACECIG(COW_GamePlay_PlayerID IDNEFEOPGIF)
{
    return ((uint32_t (*)(COW_GamePlay_PlayerID))getRealOffset(pAddress.CFFPIACECIG))(IDNEFEOPGIF);
}

monoList<float *> *LCLHHHKFCFP(void *Weapon, void *CAGCICACKCF, void *HFBDJJDICLN, bool LDGHPOPPPNL, DamageInfo2_o *DamageInfo)
{
    return ((monoList<float *> * (*)(void*, void*, void*, bool, DamageInfo2_o*))getRealOffset(pAddress.LCLHHHKFCFP))(Weapon, CAGCICACKCF, HFBDJJDICLN, LDGHPOPPPNL, DamageInfo);
}

int32_t Player_TakeDamage(void *Player, int32_t p_damage, COW_GamePlay_PlayerID PlayerID, DamageInfo2_o *DamageInfo, int32_t WeaponDataID, Vector3 FirePos, Vector3 TargetPos, monoList<float *> *CheckParams, void *p_idk1, int32_t p_idk2)
{
    return ((int32_t (*)(void *, int32_t, COW_GamePlay_PlayerID, DamageInfo2_o *, int32_t, Vector3, Vector3, monoList<float *> *, void *, uint32_t))getRealOffset(pAddress.Player_TakeDamage))(Player, p_damage, PlayerID, DamageInfo, WeaponDataID, FirePos, TargetPos, CheckParams, p_idk1, p_idk2);
}

/*void CheckNotableDamage(void *Pthis, void *localplayer, void *damager)
{
    return ((void (*)(void *, void *, void *))getRealOffset(pAddress.CheckNotableDamage))(Pthis, localplayer, damager);
}*/

#include <cstdint>

void ForceAutoSwitchReliable()
{
    static auto last = std::chrono::steady_clock::now();
    static int currentSlot = -1;
    static int lastSlot = -1;
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if (elapsed < 0) {
        return;
    }
    void* localPlayer = Current_Local_Player();
    if (!localPlayer) {
        return;
    }
    if (MasterBool.AutoSwitchEnabled) {
        do {
            currentSlot = rand() % 3;
        } while (currentSlot == lastSlot);
        SwapWeapon(localPlayer, currentSlot);
        lastSlot = currentSlot;
        last = now;
    }
}


bool (*OriginalFunction)();
bool HookedFunction()
{
    return true;
}
void HideDamageShow() {
    static bool hookApplied = false;
    if (!hookApplied) {
        DobbyHook((void*)getRealOffset(pAddress.HookedFunction), (void*)&HookedFunction, (void **)&OriginalFunction);
        hookApplied = true;
    }
}

void Syns_SwapWeapon(void *LocalPlayer)
{
    void *WeaponOnHand = GetWeaponOnHand(LocalPlayer);
    if (WeaponOnHand != nullptr) {
        void *Class_RUDP_CHANGE_INVENTORY_ON_HAND_c = *(void **)getRealOffset(pAddress.RUDP_CHANGE_INVENTORY_ON_HAND_c);
        if (Class_RUDP_CHANGE_INVENTORY_ON_HAND_c == nullptr) {
            int max = 5;
            SwapWeapon2(LocalPlayer, rand() % max, 1);
        }
        if (Class_RUDP_CHANGE_INVENTORY_ON_HAND_c != nullptr) {
            auto RUDP_CHANGE_INVENTORY_ON_HAND = ((void * (*)(void *))getRealOffset(pAddress.RUDP_CHANGE_INVENTORY_ON_HAND))(Class_RUDP_CHANGE_INVENTORY_ON_HAND_c);
            if (RUDP_CHANGE_INVENTORY_ON_HAND) {
                *(uint32_t*)((uint64_t)RUDP_CHANGE_INVENTORY_ON_HAND + 0xc) = CFFPIACECIG(GetplayerID(LocalPlayer));
                *(uint32_t*)((uint64_t)RUDP_CHANGE_INVENTORY_ON_HAND + 0x10) = *(uint32_t*)((uint64_t)WeaponOnHand + 0x8);
                GameFacade_Send(108, (GCommon_UDPClientMessageBase_o *)RUDP_CHANGE_INVENTORY_ON_HAND, 2, 0);
            }
        }
    }
    return;
}

void StartAimKill(void* targetVivo)
{
    void *LocalPlayer = Current_Local_Player();
    if (LocalPlayer != NULL) {
        void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
        if (weaponOnHand != nullptr) {
            static bool s_Il2CppMethodIntialized;
            if (!s_Il2CppMethodIntialized) {
                SetStartDamage(weaponOnHand, *(void **)((uintptr_t)LocalPlayer + pAddress.HitObjInfoWeapon));
                ((void* (*)(void *))getRealOffset(pAddress.DamageS2c))(LocalPlayer);
                s_Il2CppMethodIntialized = true;
            }
            void* targetEnemy = targetVivo;
            if (targetEnemy != nullptr) {
                void *ObjectPool = *(void **)((uintptr_t)LocalPlayer + pAddress.HitObjInfoWeapon);
                if (ObjectPool != nullptr) {
                    Syns_SwapWeapon(LocalPlayer);
                    ((void* (*)(void *, void *))getRealOffset(pAddress.StartFiring))(LocalPlayer, weaponOnHand);
                    auto WeaponData = *(void **) ((uintptr_t) weaponOnHand + pAddress.WeaponData);
                    auto weaponDataID = *(int *)((uintptr_t)WeaponData + pAddress.WpidfromWD);
                    auto baseDamage = GetDamage(weaponOnHand);
                    auto playerID = GetplayerID(LocalPlayer);
                    auto playerID2 = GetplayerID(targetEnemy);
                    void *HeadTF = TransformNode(*(void **) ((uint64_t)targetEnemy + pAddress.HeadTF));
                    Vector3 m_Head = Transform_INTERNAL_GetPosition(HeadTF);
                    void *HeadTF2 = TransformNode(*(void **) ((uint64_t)LocalPlayer + pAddress.HeadTF));
                    Vector3 m_HeadLocal = Transform_INTERNAL_GetPosition(HeadTF2);
                    Vector3 LocalPlayerPos = CameraPosition(LocalPlayer);
                    auto DamageS2c = (message_CHDLJFJCPFN_o *)((void* (*)(void *))getRealOffset(pAddress.DamageS2c))(LocalPlayer);
                    if (DamageS2c) {
                        auto DamageInfo = (DamageInfo2_o *)((void *(*)(void *))getRealOffset(pAddress.DamageInfo))(*(void **)getRealOffset(pAddress.DamageInfoClass));
                        if (DamageInfo) {
                            *(int *)((uintptr_t)DamageInfo + pAddress.GetDamage) = baseDamage;
                            *(int *)((uintptr_t)DamageInfo + pAddress.EColliderType) = 1;
                            *(void **) ((uintptr_t) DamageInfo + pAddress.WeaponClass) = weaponOnHand;
                            *(int*)((char*)DamageInfo + pAddress.WeaponDataID) = weaponDataID;
                            *(COW_GamePlay_PlayerID *)((char*)DamageInfo + pAddress.GetPlayerId) = playerID;
                            *(void **) ((uint64_t) ObjectPool + pAddress.HitObject) = get_gameObject(get_HeadCollider(targetEnemy));
                            *(void **) ((uint64_t) ObjectPool + pAddress.HitCollider) = get_HeadCollider(targetEnemy);
                            *(int*)((uint64_t)ObjectPool + pAddress.HitGroup) = 1;
                            monoList<float *> *CheckParametros = LCLHHHKFCFP(GetWeaponOnHand(LocalPlayer), GKHECDLGAJA(LocalPlayer, ObjectPool), get_HeadCollider(targetEnemy), false, DamageInfo);
                            Player_TakeDamage(targetEnemy, baseDamage, playerID, DamageInfo, weaponDataID, m_HeadLocal, m_Head, CheckParametros, CheckParametros, 0);
                        }
                    }
                }
            }
        }
    }
    return;
}

/*void Telekillkk(void* targetVivo)
{

    if (MasterBool.telehack)
    {
        void *LocalPlayer = Current_Local_Player();
        if (LocalPlayer != NULL) {
            void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
            if (weaponOnHand != nullptr)
            {
                //LOGI("weaponOnHand");
                static bool s_Il2CppMethodIntialized;
                if (!s_Il2CppMethodIntialized)
                {
                    ((void (*)(void *,void*))getRealOffset(0x1821DCC))(weaponOnHand, *(void**)((uint64_t)LocalPlayer + 0x854));
                    ((void* (*)(void *))getRealOffset(0x13AE504))(LocalPlayer);
                    s_Il2CppMethodIntialized = true;
                }

                void* targetEnemy = targetVivo;

                if (targetEnemy != nullptr)
                {
                    if (MasterBool.telehack && targetEnemy != nullptr && LocalPlayer != nullptr) {
                        // Get transforms
                        void* enemyRoot = *(void**)((uintptr_t)targetEnemy + 0x3B8);  // Root bone
                        if (!enemyRoot) return;

                        void* enemyTransform = *(void**)((uintptr_t)enemyRoot + 0x8);  // TransformNode
                        if (!enemyTransform) return;

                        void* enemyTransformObj = *(void**)((uintptr_t)enemyTransform + 0x8);
                        if (!enemyTransformObj) return;

                        void* enemyMatrix = *(void**)((uintptr_t)enemyTransformObj + 0x20);
                        if (!enemyMatrix) return;

                        void* playerRoot = *(void**)((uintptr_t)LocalPlayer + 0x380);  // Local root bone
                        if (!playerRoot) return;

                        void* playerTransform = *(void**)((uintptr_t)playerRoot + 0x8);
                        if (!playerTransform) return;

                        void* playerTransformObj = *(void**)((uintptr_t)playerTransform + 0x8);
                        if (!playerTransformObj) return;

                        void* playerMatrix = *(void**)((uintptr_t)playerTransformObj + 0x20);
                        if (!playerMatrix) return;

                        Vector3 localPosition = *(Vector3*)((uintptr_t)playerMatrix + 0x80);  // Get real root pos
                        Vector3 enemyPosition = *(Vector3*)((uintptr_t)enemyMatrix + 0x80);

                        float distance = Vector3Distance(enemyPosition, localPosition);
                        if (distance <= 10.0f) {
                            *(Vector3*)((uintptr_t)enemyMatrix + 0x80) = localPosition;  // Teleport
                        }

                        //Speed Hack
                        /*void* timeService = *(void**)((uintptr_t)currentGame + 0x10); // GameTimer
                        if (!timeService) return;

                        // Set higher FixedDeltaTime (faster game)
                        *(float*)((uintptr_t)timeService + 0x24) = 0.065f; // FixedDeltaTime
                    }



                }
            }
        }
    }


    return;
}*/

void ForceAutoSwitchReliable(void* LocalPlayer) {

    if (MasterBool.AutoSwitchEnabled && MasterBool.ActivateAll)
    {

        static int lastSlot = 2;
        static int frameCounter = 0;

        if (++frameCounter < 0) return;  // Faster switching (was 5, now 2)
        frameCounter = 0;

        void* weaponOnHand = GetWeaponOnHand(LocalPlayer);
        if (!weaponOnHand) return;

        int newSlot = (lastSlot == 1) ? 2 : 1;

        SwapWeapon2(LocalPlayer, newSlot, 1);
        lastSlot = newSlot;

    }
}

// Optimized for compatibility with your ForceAutoSwitchReliable
// This is the fixed and optimized version.
void dwonplayerloop() {
    static auto last = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();

    // [ULTRA AGGRESSIVE] 10ms loop (100 updates/sec)
    // Instant lock, no escape for the enemy.
    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if (elapsed < 10) return;
    last = now;

    void* localPlayer = Current_Local_Player();
    if (localPlayer == nullptr) return;

    // Get YOUR position once to use as a stable anchor
    auto myTF = Component_get_transform(localPlayer);
    if (myTF == nullptr) return;
    Vector3 myPos = Transform_INTERNAL_GetPosition(myTF);

    // --- Feature 1: Flyhack (Safety) ---
    if (MasterBool.flyhack) {
        Vector3 safePos = myPos;
        // Use UPPERCASE 'Y' to match your project's Vector3 struct
        safePos.Y -= 1.40f;
        Transform_set_position(myTF, safePos);
    }

    // --- Feature 2: Down Player (The KILLER FIX) ---
    if (MasterBool.downplayer) {
        void *ClosestEnemy = GetClosestEnemy360();

        if (ClosestEnemy != nullptr) {
            auto enemyTransform = Component_get_transform(ClosestEnemy);
            if (enemyTransform != nullptr) {
                Vector3 enemyPos = Transform_INTERNAL_GetPosition(enemyTransform);

                // [THE FIX]
                // ERROR: 'enemyPos.Y -= 1.8f' causes infinite falling.
                // SOLUTION: Align their height to yours. They stop sinking.

                // Set their Y to YOUR Y. This puts you both on the same level
                // underground, creating a perfect line of sight for AutoSwitch.
                enemyPos.Y = myPos.Y;

                Transform_set_position(enemyTransform, enemyPos);
            }
        }
    }
}

void dwonplayerV1() {
    static auto last = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();

    // âœ… 1 switch every 20ms (50 times/second) â€” FAST but not game-breaking
    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if (elapsed < 45) return;
    last = now;

    void* localPlayer = Current_Local_Player();
    if (localPlayer == nullptr) return;

    if (localPlayer != nullptr) {
        void *ClosestEnemy = GetClosestEnemy360();
        if (ClosestEnemy != nullptr) {

            if (MasterBool.flyhack && localPlayer != nullptr) {
                auto tf = Component_get_transform(localPlayer);
                Vector3 currentPos = Transform_INTERNAL_GetPosition(tf);

                currentPos.Y -= 1.35f;  // â¬‡ï¸ Go down from current position
                Transform_set_position(tf, currentPos);
            }
        }
        if (MasterBool.downplayer && ClosestEnemy != nullptr) {
            auto enemyTransform = Component_get_transform(ClosestEnemy);
            if (enemyTransform != nullptr) {
                Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);
                Vector3 newPos = centerPos;

                // Enemy ko thoda neeche shift karna
                newPos.Y -= 1.8f;
                Transform_set_position(enemyTransform, newPos);

            }
        }
    }
}



void SuperFastAutoSwitchLoop() {
    static auto last = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();


    long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    if (elapsed < 70) return;
    last = now;

    void* localPlayer = Current_Local_Player();
    if (localPlayer == nullptr) return;

    ForceAutoSwitchReliable(localPlayer);

}

std::chrono::steady_clock::time_point last_update_time = std::chrono::steady_clock::now();

GCommon_AnimationRuntimeHandle_o *(*GetCurrentRunningHandler)(GCommon_AnimationSystemComponent_o *Instance,int32_t layerIndex);
GCommon_AnimationRuntimeHandle_o *_GetCurrentRunningHandler(GCommon_AnimationSystemComponent_o *Instance,int32_t layerIndex)
{
    if (Instance != nullptr) {
        if (MasterBool.ActivateAll) {

            SuperFastAutoSwitchLoop();
            dwonplayerloop();

            std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update_time).count();
            if (elapsed_time > 1000 /20) {
                void *LocalPlayer = Current_Local_Player();
                void *ClosestEnemy = nullptr;
                ClosestEnemy = GetEnemyInsideFOV();

                if (LocalPlayer != nullptr) {
                    if (ClosestEnemy != nullptr) {
                        void *weaponOnHand = GetWeaponOnHand(LocalPlayer);
                        if (weaponOnHand != nullptr) {
                            if (isEnemyInRangeWeapon(LocalPlayer, ClosestEnemy, weaponOnHand)) {
                                if (isVisible_Aimbot(ClosestEnemy)) {
                                    StartAimKill(ClosestEnemy);
                                }
                            }
                        }
                    }

                }

                if (MasterBool.Aimkilltp && ClosestEnemy != nullptr) {
                    Vector3 gotten = Transform_INTERNAL_GetPosition(
                            Component_get_transform(LocalPlayer));
                    Vector3 enemyPos = Transform_INTERNAL_GetPosition(
                            Component_get_transform(ClosestEnemy));

                    float realDistance = sqrt(pow(enemyPos.X - gotten.X, 2) +
                                              pow(enemyPos.Y - gotten.Y, 2) +
                                              pow(enemyPos.Z - gotten.Z, 2));

                    if (realDistance < 9.0f) {
                        Vector3 coord = gotten;
                        Transform_set_position(
                                Component_get_transform(ClosestEnemy), coord);

                    }
                }
                if (MasterBool.telehack && ClosestEnemy != nullptr) {
                    Vector3 myPos = Transform_INTERNAL_GetPosition(Component_get_transform(LocalPlayer));
                    Vector3 enemyPos = Transform_INTERNAL_GetPosition(Component_get_transform(ClosestEnemy));

                    float distance = sqrt(
                            pow(enemyPos.X - myPos.X, 2) +
                            pow(enemyPos.Y - myPos.Y, 2) +
                            pow(enemyPos.Z - myPos.Z, 2)
                    );

                    if (distance < 200.0f) {
                        // Direction vector from you to enemy
                        Vector3 direction;
                        direction.X = enemyPos.X - myPos.X;
                        direction.Y = enemyPos.Y - myPos.Y;
                        direction.Z = enemyPos.Z - myPos.Z;

                        // Normalize the direction
                        float length = sqrt(direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z);
                        if (length != 0) {
                            direction.X /= length;
                            direction.Y /= length;
                            direction.Z /= length;
                        }

                        // Move you to a position 1 meter away from the enemy
                        Vector3 newMyPos;
                        newMyPos.X = enemyPos.X - direction.X * 1.0f;
                        newMyPos.Y = enemyPos.Y - direction.Y * 1.0f;
                        newMyPos.Z = enemyPos.Z - direction.Z * 1.0f;

                        Transform_set_position(Component_get_transform(LocalPlayer), newMyPos);


                    }
                }
                auto ClosestEnemys = GetEnemyInsideFOV();

                static bool flySet = false;
                static Vector3 lockPos;
                if (MasterBool.flyhack && ClosestEnemys != nullptr) {

                    auto tf = Component_get_transform(LocalPlayer);
                    Vector3 pos = Transform_INTERNAL_GetPosition(tf);

                    if (!flySet) {
                        lockPos = pos;
                        lockPos.Y -= 3.4f; // Ground ke andar le jao//    3.5
                        Transform_set_position(tf, lockPos);
                        flySet = true;
                    }

                    // Sirf Y lock, X/Z mat chedho
                    pos.Y = lockPos.Y;
                    Transform_set_position(tf, pos);

                } else {
                    flySet = false; // Flyhack off

                }
                if (MasterBool.Aimkillrotate && ClosestEnemy != nullptr) {
                    auto enemyTransform = Component_get_transform(ClosestEnemy);

                    // Static angle value to keep rotating every frame
                    static float spinAngle = 0.0f;
                    spinAngle += 122.0f;  // Rotation speed (higher = faster) 15.0f
                    if (spinAngle >= 360.0f) spinAngle = 0.0f;

                    // Center point where enemy will rotate around (its original position)
                    Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);

                    // Radius of circular motion
                    float radius = 4.0f;//5.0f

                    // Create new rotated position using sin/cos
                    Vector3 newPos;
                    newPos.X = centerPos.X + radius * cosf(spinAngle * 3.14159f / 180.0f);
                    newPos.Z = centerPos.Z + radius * sinf(spinAngle * 3.14159f / 180.0f);
                    newPos.Y = centerPos.Y + 0.1f; // Fly slightly up

                    // Apply new position (enemy flies and orbits)
                    Transform_set_position(enemyTransform, newPos);
                }
                if (MasterBool.Aimkillrotatev2 && ClosestEnemy != nullptr) {
                    auto enemyTransform = Component_get_transform(ClosestEnemy);
                    // Static angle value to keep rotating every frame
                    static float spinAngle = 0.0f;
                    spinAngle += 122.0f;  // Rotation speed (higher = faster) 15.0f
                    if (spinAngle >= 360.0f) spinAngle = 0.0f;

                    // Center point where enemy will rotate around (its original position)
                    Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);

                    // Radius of circular motion
                    float radius = 4.0f;//5.0f

                    // Create new rotated position using sin/cos
                    Vector3 newPos;
                    newPos.X = centerPos.X + radius * cosf(spinAngle * 3.14159f / 180.0f);
                    newPos.Z = centerPos.Z + radius * sinf(spinAngle * 3.14159f / 180.0f);
                    newPos.Y = centerPos.Y + 1.0f + 0.04f; // Fly slightly up

                    // Apply new position (enemy flies and orbits)
                    Transform_set_position(enemyTransform, newPos);
                }
                if (MasterBool.shakekillultra && ClosestEnemy != nullptr) {
                    auto enemyTransform = Component_get_transform(ClosestEnemy);
                    static float spinAngle = 0.0f;
                    static float hoverOffset = 0.0f;
                    static float swayOffset = 0.0f;
                    static bool hoverUp = true;
                    static bool swayRight = true;
                    spinAngle += 10.0f; // rotation speed
                    if (spinAngle >= 360.0f) spinAngle -= 360.0f;
                    float hoverSpeed = 0.25f;
                    float hoverLimit = 1.0f;
                    hoverOffset += (hoverUp ? hoverSpeed : -hoverSpeed);
                    if (hoverOffset > hoverLimit) { hoverOffset = hoverLimit; hoverUp = false; }
                    if (hoverOffset < -hoverLimit) { hoverOffset = -hoverLimit; hoverUp = true; }
                    float swaySpeed = 0.6f;
                    float swayLimit = 1.8f;
                    swayOffset += (swayRight ? swaySpeed : -swaySpeed);
                    if (swayOffset > swayLimit) { swayOffset = swayLimit; swayRight = false; }
                    if (swayOffset < -swayLimit) { swayOffset = -swayLimit; swayRight = true; }
                    Vector3 centerPos = Transform_INTERNAL_GetPosition(enemyTransform);
                    float radius = 5.2f;
                    float shakePower = 0.25f;
                    float shakeX = ((rand() % 100) / 100.0f - 0.5f) * shakePower;
                    float shakeZ = ((rand() % 100) / 100.0f - 0.5f) * shakePower;
                    Vector3 newPos;
                    newPos.X = centerPos.X + radius * cosf(spinAngle * 3.14159f / 180.0f) + swayOffset + shakeX;
                    newPos.Z = centerPos.Z + radius * sinf(spinAngle * 3.14159f / 180.0f) + shakeZ;
                    newPos.Y = centerPos.Y + hoverOffset + 0.5f;
                    Transform_set_position(enemyTransform, newPos);
                }

                last_update_time = std::chrono::steady_clock::now();
            }
            return GetCurrentRunningHandler(Instance, layerIndex);
        }

    }
    return GetCurrentRunningHandler(Instance,layerIndex);
}


bool (*FastSwitch)(bool* instance);

bool _FastSwitch(bool* instance) {
    static auto lastSwitch1 = std::chrono::steady_clock::now();

    if (MasterBool.fastswitch && MasterBool.ActivateAll) {
        auto now = std::chrono::steady_clock::now();
        long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSwitch1).count();


        if (elapsed < 35) {
            return true;
        }

        lastSwitch1 = now;
        return false;
    }

    return FastSwitch(instance);
}


bool (*FastWeapon)(bool* instance);

bool _FastWeapon(bool* instance) {
    static auto lastSwitch = std::chrono::steady_clock::now();

    if (MasterBool.ultraswitch && MasterBool.ActivateAll) {
        auto now = std::chrono::steady_clock::now();
        long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSwitch).count();


        if (elapsed < 250) {
            return true;
        }

        lastSwitch = now;
        return false;
    }

    return FastWeapon(instance);
}



bool(*SpeedBypass)(bool* instance);
bool _SpeedBypass(bool* instance) {
    if (MasterBool.speedhackjoy && MasterBool.ActivateAll ) {
        static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return result;
    }
    return SpeedBypass(instance);
}
bool(*SpeedHack)(bool* instance);
bool _SpeedHack(bool* instance) {
    if (MasterBool.speedhackjoy && MasterBool.ActivateAll ) {
        static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return result;
    }
    return SpeedHack(instance);
}
bool (*ResetGuest)(bool* instance);
bool _ResetGuest(bool* instance)
{
    if (MasterBool.resetguest && MasterBool.ActivateAll) {
        static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return *(bool*)result; // Modified to return a boolean
    }
    return ResetGuest(instance);
}
bool(*IsSwimSurf)(bool* instance);
bool _IsSwimSurf(bool* instance) {
    if (MasterBool.cameraup && MasterBool.ActivateAll ) {
        static const unsigned char result[] = { 0x01, 0x00, 0xA0, 0xE3, 0x1E, 0xFF, 0x2F, 0xE1 };
        return result;
    }
    return IsSwimSurf(instance);
}

#define lbil2cpp(address, original, backup)  DobbyHook((void *)getRealOffset(address), (void *)original, (void **)&backup)
#define lbunity(address, original, backup)  DobbyHook((void *)getRealOffsetUnity(address), (void *)original, (void **)&backup)

void *pthreadcreate(void *arg) {

    while (true) {


        if (getRealOffset(0) != 0) {

            //public static bool Raycast(Vector3 origin, Vector3 direction, out RaycastHit hitInfo, float maxDistance, int layerMask) { }
            DobbyHook((void *)getRealOffset(pAddress.Raycast), (void*)New_Physics$$Raycast, (void **)&Old_Physics$$Raycast);
            lbil2cpp(pAddress.FastWeapon, _FastWeapon, FastWeapon);
            lbil2cpp(pAddress.fastswitch, _FastSwitch, FastSwitch);
            lbil2cpp(pAddress.cameraup, _IsSwimSurf, IsSwimSurf);
            //public static EPlayerPhysXState MappingFromPhysXState(Player.EPhysXState state) { }
            lbil2cpp(pAddress.SpeedBypass, _SpeedBypass, SpeedBypass);
            lbil2cpp(pAddress.ResetGuest, _ResetGuest, ResetGuest);
            //public bool IsFoldWingGliding() { }
            lbil2cpp(pAddress.SpeedHack, _SpeedHack, SpeedHack);




            //public AnimationRuntimeHandle GetCurrentRunningHandler(int layerIndex) { }
            HOOK_LIB2(pAddress.GetCurrentRunningHandler, _GetCurrentRunningHandler, GetCurrentRunningHandler);

            pthread_exit(0);
        }

    }
    return NULL;
}
bool hookEnabled = false;


__attribute__((constructor))
void initializer() {

    pthread_t ptid12;
    pthread_create(&ptid12, nullptr, pthreadcreate, nullptr);

    pthread_t ptid1;
    pthread_create(&ptid1, nullptr, CreateServer, nullptr);


}