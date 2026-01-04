#ifndef ANDROID_MOD_MENU_HOOK_H
#define ANDROID_MOD_MENU_HOOK_H



class Vvector3 {
public:
    float X;
    float Y;
    float Z;
    Vvector3() : X(0), Y(0), Z(0) {}
    Vvector3(float x1, float y1, float z1) : X(x1), Y(y1), Z(z1) {}
    Vvector3(const Vvector3 &v);
    ~Vvector3();
};
Vvector3::Vvector3(const Vvector3 &v) : X(v.X), Y(v.Y), Z(v.Z) {}
Vvector3::~Vvector3() {}


static void *Player_GetActiveWeapon(void *LocalPlayer)
{
    void *(*_Player_GetActiveWeapon)(void *LocalPlayer) = (void *(*)(void*)) getRealOffset(pAddress.Player_GetActiveWeapon);
    return _Player_GetActiveWeapon(LocalPlayer);
}

void *GameFacade_CurrentLocalPlayer()
{
    void *(*_CurrentLocalPlayer)() = (void*(*)())getRealOffset(pAddress.GameFacade_CurrentLocalPlayer);
    return _CurrentLocalPlayer();
}

void *GameFacade_CurrentMatch()
{
    void *(*_CurrentMatch)() = (void*(*)())getRealOffset(pAddress.Curent_Match);
    return _CurrentMatch();
}

static Vector3 Transform_INTERNAL_GetPosition(void *player)
{
    Vector3 out = Vector3::Zero();
    void (*_Transform_INTERNAL_GetPosition)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))getRealOffset(pAddress.Transform_INTERNAL_GetPosition);
    _Transform_INTERNAL_GetPosition(player, &out);
    return out;
}

static void Transform_INTERNAL_SetPosition(void *player, Vvector3 inn) {
    void (*_Transform_INTERNAL_SetPosition)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(pAddress.Transform_SetPosition);
    _Transform_INTERNAL_SetPosition(player, inn);
}
static void *Component_get_transform(void *player)
{
    void *(*_Component_GetTransform)(void *nullo) = (void *(*)(void *))getRealOffset(pAddress.Component_GetTransform);
    return _Component_GetTransform(player);
}
static void *Component_get_transform2(void *player)
{
    void *(*_Component_GetTransform2)(void *component) = (void *(*)(void *))getRealOffset(pAddress.Component_GetTransform);
    return _Component_GetTransform2(player);
}
static Vector3 Transform_INTERNAL_GetPosition2(void *player) {
    Vector3 out = Vector3::Zero();
    void (*_Transform_INTERNAL_GetPosition2)(void *transform, Vector3 * out) = (void (*)(void *, Vector3 *))getRealOffset(pAddress.Transform_INTERNAL_GetPosition);
    _Transform_INTERNAL_GetPosition2(player, &out);
    return out;
}

static void *get_main() {
    void *(*nget_main)(void *Instance) = (void *(*)(void *))getRealOffset(pAddress.Camera_main);
    return nget_main(nullptr);
}


Vector3 WorldToScreenPoint(Vector3 pos) {
    auto main = get_main();
    if (main) {
        auto Camera_WorldToScreenPoint = (Vector3 (*)(void *, Vector3))getRealOffset(pAddress.WorldToScreenPoint);
        return Camera_WorldToScreenPoint(main, pos);
    }
    return {0, 0, 0};
}


static Vector3 GetForward(void *player) {
    Vector3 (*NGetForward)(void *players) = (Vector3 (*)(void *))getRealOffset(pAddress.get_forward);
    return NGetForward(player);
}
static Vector3 GetForward2(void *player) {
    Vector3 (*_GetForward2)(void *players) = (Vector3 (*)(void *))getRealOffset(pAddress.get_forward);
    return _GetForward2(player);
}
static int GetHp(void *instance) {
    return ((int (*)(void *)) getRealOffset(pAddress.GetHp))(instance);
}

static bool get_IsSighting(void *player) {
    bool (*_get_IsSighting)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_IsSighting);
    return _get_IsSighting(player);
}
static bool IsFiringPlayer(void *player) {
    bool (*_IsFiringPlayer)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_IsFiringFromPRI);
    return _IsFiringPlayer(player);
}

static void Transform_set_position(void *player, Vector3 inn)
{
    void (*_Transform_SetPosition)(void *transform, Vector3 in) = (void (*)(void *, Vector3))getRealOffset(pAddress.Transform_SetPosition);
    _Transform_SetPosition(player, inn);
}
static void Transform_set_position2(void *player, Vvector3 inn)
{
    void (*_Transform_SetPosition2)(void *transform, Vvector3 in) = (void (*)(void *, Vvector3))getRealOffset(pAddress.Transform_SetPosition);
    _Transform_SetPosition2(player, inn);
}
static Vector3 Transform_get_forward(void *player)
{
    Vector3 (*_GetForward)(void *players) = (Vector3(*)(void *))getRealOffset(pAddress.get_forward);
    return _GetForward(player);
}

static void SetResolution(int32_t width, int32_t height, int32_t fullscreenMode, int32_t preferredRefreshRate)
{
    void (*_SetResolution)(int32_t width, int32_t height, int32_t fullscreenMode, int32_t preferredRefreshRate) = (void (*)(int32_t,int32_t,int32_t,int32_t))getRealOffset(pAddress.SetResolution);
    return _SetResolution(width,height,fullscreenMode,preferredRefreshRate);
}




static bool get_isVisible(void *player) {
    bool (*Nget_isVisible)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_isVisible);
    return Nget_isVisible(player);
}

static bool IsLocalTeammate(void *instance) {
    return ((bool (*)(void *)) getRealOffset(pAddress.IsLocalTeammate))(instance);
}


static Vector3 CameraPosition(void *player)
{
    return Transform_INTERNAL_GetPosition(Component_get_transform(get_main()));
}
void* TransformNode(void *_this)
{
    return ((void* (*)(void *))getRealOffset(pAddress.TransformNode))(_this);
}

static void *Current_Local_Player()
{
    void *(*_Local_Player)(void *players) = (void *(*)(void *))getRealOffset(pAddress.Current_Local_Player);
    return _Local_Player(NULL);
}

static bool IsStreamerVisible(void *player) {
    bool (*_IsStreamerVisible)(void *players) = (bool (*)(void *))getRealOffset(pAddress.get_isVisibleMoita);
    return _IsStreamerVisible(player);
}
static void *IsDieing(void *player) {
    void *(*_IsDieing)(void *players) = (void *(*)(void *))getRealOffset(pAddress.get_IsDieing);
    return _IsDieing(player);
}

void *get_HeadCollider(void *pthis)
{
    return ((void* (*)(void *))getRealOffset(pAddress.get_HeadCollider))(pthis);
}

void *get_gameObject(void *Pthis)
{
    return ((void* (*)(void *))getRealOffset(pAddress.get_gameObject))(Pthis);
}
static bool Physics_Raycast(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) {
    bool (*_Physics_Raycast)(Vector3 camLocation, Vector3 headLocation, unsigned int LayerID, void* collider) = (bool(*)(Vector3, Vector3, unsigned int, void*))getRealOffset(pAddress.Raycast2);//0x1463420
    return _Physics_Raycast(camLocation, headLocation, LayerID, collider);
}

Vector3 GetHeadPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + pAddress.HeadTF));
}

Vector3 GetHipPosition(void* player) {
    return Transform_INTERNAL_GetPosition(*(void**) ((uint64_t) player + pAddress.PesTF));
}
bool isVisible_Aimbot(void * player)
{
    void *hitObj = NULL;
    void *HeadTF = TransformNode(*(void **) ((uint64_t) player + pAddress.HeadTF));
    Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
    Vector3 LocalPlayerPos = CameraPosition(Current_Local_Player());
    return !Physics_Raycast(LocalPlayerPos, EnemyHeadPosition,12, &hitObj);

}
static monoString* get_NickName(void *player)
{
    monoString* (*_get_NickName)(void *players) = (monoString * (*)(void *)) getRealOffset(pAddress.NickName);
    return _get_NickName(player);
}
char get_Chars(monoString *str, int index) {
    char (*_get_Chars)(monoString *str, int index) = (char (*)(monoString *, int))getRealOffset(pAddress.Chars);
    return _get_Chars(str, index);
}

static monoString *U3DStr(const char *str) {
    monoString *(*String_CreateString)(void *_this, const char *str) = (monoString * (*)(void *, const char *))getRealOffset(pAddress.CreateString);
    return String_CreateString(NULL, str);
}
static monoString *U3DStrFormat(float distance) {
    char buffer[128] = {0};
    sprintf(buffer, "[ %.f M VAST CLIENT]", distance);
    return U3DStr(buffer);
}


static int GetDamage(void *pthis)
{
    return ((int (*)(void *))getRealOffset(pAddress.GetDamage2))(pthis);
}

static void *GetWeaponOnHand(void *local) {
    void *(*GetWeaponOnHand)(void *local) = (void *(*)(void *))getRealOffset(pAddress.GetWeaponOnHand);
    return GetWeaponOnHand(local);
}
//public override void SwapWeapon(EEquipSlot InWeaponSlot, bool forceInstant = False) { }
void SwapWeapon(void *Pthis,int32_t prevOrNext)
{
    return ((void (*)(void *,int,bool))getRealOffset(pAddress.swapweapon))(Pthis,prevOrNext,1); //0x1A4E36C
}
//public override void SwapWeapon(int prevOrNext, bool forceInstant = False, List<int> specSlotLst) { }
void SwapWeapon2(void *Pthis,int32_t FANMJANBFIL,bool GDKLMFLNNGM)
{
    return ((void (*)(void *,int,bool,void*))getRealOffset(pAddress.swapweaopon2))(Pthis,FANMJANBFIL,GDKLMFLNNGM,nullptr);
}
bool GameFacade_Send(uint32_t messageID,void *msg,uint8_t sendOption,bool cacheMsgAnyWay)
{
    return ((bool (*)(uint32_t,void *,uint8_t,bool))getRealOffset(pAddress.GameFacade_Send))(messageID,msg,sendOption,cacheMsgAnyWay);
}

static int GetWeaponID(void *pthis)
{
    return ((int (*)(void *))getRealOffset(pAddress.GetWpid))(pthis);
}
void *GKHECDLGAJA(void *pthis, void* a1)
{
    return ((void* (*)(void *,void *))getRealOffset(pAddress.GKHECDLGAJA))(pthis,a1);
}

void SetStartDamage(void *WeaponOnHand, void* ObjectPool)
{
    ((void (*)(void *, void *)) getRealOffset(pAddress.SetStartDamage))(WeaponOnHand,ObjectPool);
}

static float get_time() {
    float (*get_time)(void *nuls) = (float (*)(void *))getRealOffset(pAddress.get_time);
    return get_time(NULL);
}
static float get_Range(void *pthis)
{
    return ((float (*)(void *))getRealOffset(pAddress.get_Range))(pthis);
}
bool isEnemyInRangeWeapon(void *player, void *enemy, void* weapon)
{
    if (player != nullptr && enemy != nullptr && weapon != nullptr)
    {
        void *HeadTF = TransformNode(*(void **) ((uint64_t) enemy + pAddress.HeadTF));
        void *HeadTF2 = TransformNode(*(void **) ((uint64_t) player + pAddress.HeadTF));
        Vector3 EnemyHeadPosition = Transform_INTERNAL_GetPosition(HeadTF);
        Vector3 PlayerHeadPosition = Transform_INTERNAL_GetPosition(HeadTF2);
        float distance = Vector3::Distance(PlayerHeadPosition, EnemyHeadPosition);
        float range = get_Range(weapon);

        if (distance <= range) {
            return true;
        }
    }
    return false;
}

#endif