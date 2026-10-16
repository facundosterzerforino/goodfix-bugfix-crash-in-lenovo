/* Ghidra 12.1.4 pseudocode via REA 6.5.0 - wbdi.dll 3.0.536.320 (sha256 792271a28c3087d8acd1626bbdc35b2190c85058bfc49af1fa00f1ea457285ec)
 * WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT inlined helper @ 0x1800ba0d0.
 * LOG(level, func, ...) = driver trace macro (FUN_1800c3f70). Excerpt for interoperability analysis. */

void FUN_1800ba0d0(undefined4 *param_1,int param_2)

{
  FUN_180128dc0(param_1,0,0x24);
  *param_1 = 0x24;
  param_1[3] = 0;
  param_1[4] = 2;
  param_1[5] = 2;
  param_1[6] = 2;
  param_1[7] = 0;
  param_1[8] = 2;
  param_1[1] = param_2;
  if (param_2 == 1) {
    param_1[2] = 4;
  }
  else if ((1 < param_2) && (param_2 < 4)) {
    param_1[2] = 5;
  }
  return;
}
