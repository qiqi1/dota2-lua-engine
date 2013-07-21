#include "DotaPlayerResource.h"

#include "..\utils\utils.h"
#include <tchar.h>
#include <windows.h>

namespace dota {
  DWORD DotaPlayerResource::get_player_name_address_ = 0;
  DotaPlayerResource* DotaPlayerResource::player_resource_ = nullptr;

  DotaPlayerResource* DotaPlayerResource::GetPlayerResource() {
    if (player_resource_ != nullptr) return player_resource_;

    const uint32_t* pattern_address =  utils::FindPattern(
      _T("client.dll"),
      reinterpret_cast<unsigned char*>("\x77\x0F\x8B\x15\x00\x00\x00\x00\x8B\x44\x24\x1C"),
      "xxxx????xxxx",
      0x4);

    player_resource_ = *(dota::DotaPlayerResource**)pattern_address;
    return player_resource_;
  }

  const char* DotaPlayerResource::GetPlayerName(int index) {
    if (get_player_name_address_ == 0) {
      unsigned long GetPlayerNameAddress = (unsigned long)utils::FindPattern(
        _T("client.dll"),
        reinterpret_cast<unsigned char*>("\xE8\x00\x00\x00\x00\x8B\x4D\xFC\x89\x45\xC0"),
        "x????xxxxxx");

      get_player_name_address_ = utils::GetAbsoluteAddress(GetPlayerNameAddress);
    }

    const char* name = nullptr;

    __asm {
      pushad
      mov esi, this
      mov eax, index
      call [get_player_name_address_]
      mov [name], eax
      popad
    }
    return name;
  }
}