/* Ghidra 12.1.4 pseudocode via REA 6.5.0 - wbdi.dll 3.0.536.320 (sha256 792271a28c3087d8acd1626bbdc35b2190c85058bfc49af1fa00f1ea457285ec)
 * EnableS0Idle, USB path (_deviceusb.c) @ 0x1800ba500. +0x10 = UserControlOfIdleSettings (1 = IdleDoNotAllowUserControl).
 * LOG(level, func, ...) = driver trace macro (FUN_1800c3f70). Excerpt for interoperability analysis. */

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */

int FUN_1800ba500(undefined8 param_1)

{
  longlong lVar1;
  undefined1 auStack_98 [32];
  undefined4 local_78;
  undefined8 local_70;
  char *local_68;
  char *local_60;
  int local_58;
  int local_48;
  undefined1 local_40 [12];
  uint local_34;
  undefined4 local_30;
  undefined4 local_24;
  ulonglong local_18;
  
  local_18 = DAT_180257048 ^ (ulonglong)auStack_98;
  local_68 = "enter";
  local_70 = 0;
  local_78 = 0x84;
  LOG(8, "EnableS0Idle");
  if (DAT_18024482c == 1) {
    FUN_1800ba0d0(local_40);
    local_24 = 2;
  }
  else {
    FUN_1800ba0d0(local_40);
  }
  local_30 = 1;
  lVar1 = FUN_180031de0(0);
  local_34 = *(int *)(lVar1 + 0x434) * 1000;
  local_60 = (char *)CONCAT44(local_60._4_4_,local_34 / 1000);
  local_68 = "s0 idle timeout: %u sec";
  local_70 = 0;
  local_78 = 0x96;
  LOG(8, "EnableS0Idle");
  local_58 = FUN_1800ba250(param_1,local_40);
  local_48 = local_58;
  if (local_58 < 0) {
    local_60 = "WdfDeviceAssignS0IdleSettings";
    local_68 = "%s failed, status: 0x%x";
    local_70 = 0;
    local_78 = 0xa2;
    LOG(4, "EnableS0Idle");
  }
  else {
    local_68 = "exit";
    local_70 = 0;
    local_78 = 0xa6;
    LOG(8, "EnableS0Idle");
    local_48 = 0;
  }
  return local_48;
}
