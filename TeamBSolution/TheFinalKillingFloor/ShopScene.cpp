#include "ShopScene.h"
#include "LGlobal.h"
#include "LInput.h"
#include "LWrite.h"
#include "UIManager.h"
#include "LPlayer.h"
#include "LWeaponMgr.h"
bool ShopScene::Init()
{   
    UIManager::GetInstance().GetUIObject(L"T_Gun1_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.TotalAmmo));
    UIManager::GetInstance().GetUIObject(L"T_Gun2_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.TotalAmmo));
    if(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo <10)
         UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo).insert(0, L"0"));
    else
        UIManager::GetInstance().GetUIObject(L"T_Gun3_MAGAZINE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring(LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.TotalAmmo));
    UIManager::GetInstance().GetUIObject(L"T_Gun1_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60/LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.ShootDelay)));
    UIManager::GetInstance().GetUIObject(L"T_Gun2_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60/LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.ShootDelay)));
    UIManager::GetInstance().GetUIObject(L"T_Gun3_RPM")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)(60/LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.ShootDelay)));

    UIManager::GetInstance().GetUIObject(L"T_Gun1_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::PISTOL]->m_GunSpec.Damage));
    UIManager::GetInstance().GetUIObject(L"T_Gun2_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::ASSAULTRIFLE]->m_GunSpec.Damage));
    UIManager::GetInstance().GetUIObject(L"T_Gun3_DAMAGE")->GetScript<TextToTexture>(L"TextToTexture")->UpdateText(to_wstring((int)LWeaponMgr::GetInstance().m_map[WeaponState::SHOTGUN]->m_GunSpec.Damage));
    UIManager::GetInstance().GetUIObject(L"Shop_Money")->GetScript<DigitDisplay>(L"DigitDisplay")->UpdateNumber(LGlobal::g_PlayerModel->m_Money);
    

    return true;
}

void ShopScene::Render()
{
    UIManager::GetInstance().Render();
}

void ShopScene::Process()
{
    UIManager::GetInstance().Frame();
    if (UIManager::GetInstance().GetUIObject(L"B_Gun1_MAGAZINE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeMagazine(WeaponState::PISTOL);
    else if (UIManager::GetInstance().GetUIObject(L"B_Gun2_MAGAZINE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeMagazine(WeaponState::ASSAULTRIFLE);
    else if (UIManager::GetInstance().GetUIObject(L"B_Gun3_MAGAZINE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeMagazine(WeaponState::SHOTGUN);

    if (UIManager::GetInstance().GetUIObject(L"B_Gun1_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::PISTOL);
    else if (UIManager::GetInstance().GetUIObject(L"B_Gun2_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::ASSAULTRIFLE);
    else if (UIManager::GetInstance().GetUIObject(L"B_Gun3_RPM")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeRPM(WeaponState::SHOTGUN);

    if (UIManager::GetInstance().GetUIObject(L"B_Gun1_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::PISTOL);
    else if (UIManager::GetInstance().GetUIObject(L"B_Gun2_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::ASSAULTRIFLE);
    else if (UIManager::GetInstance().GetUIObject(L"B_Gun3_DAMAGE")->GetScript<ButtonAction>(L"ButtonAction")->state == PICKING_STATE::UP)
        LWeaponMgr::GetInstance().UpgradeDamage(WeaponState::SHOTGUN);
}

void ShopScene::Release()
{

}

ShopScene::ShopScene(LScene* parent) : SceneState(parent)
{

}
ShopScene::~ShopScene() {}