/* Ghidra 12.1.4 pseudocode via REA 6.5.0 - wbdi.dll 3.0.536.320 (sha256 792271a28c3087d8acd1626bbdc35b2190c85058bfc49af1fa00f1ea457285ec)
 * Config reader @ 0x180031e54 excerpt (registry value -> global config). Defaults in .data: EnableS0Idle=1, S0IdleTimeout=5.
 * LOG(level, func, ...) = driver trace macro (FUN_1800c3f70). Excerpt for interoperability analysis. */

  /* ... */
  if (local_18 != (wchar_t *)0x0) {
    wcscpy_s(u_C__ProgramData_Goodix_180244002,0x103,local_18);
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f61 = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\LogOutput\\",L"LogLevel",0,
                           &local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    DAT_18024420a = (undefined1)*local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f62 = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\LogOutput\\",L"LogTarget",0,
                           &local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    _DAT_18024420c = *(undefined4 *)local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f63 = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\LogOutput\\",L"LogPath",0,
                           &local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    wcscpy_s(u_C__ProgramData_Goodix_180244210,0x103,local_18);
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f64 = 1;
  /* ... */
  if (local_18 != (wchar_t *)0x0) {
    DAT_180244422 = (undefined1)*local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f6f = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\",L"RetrySwitch",0,&local_18,0
                          );
  if (local_18 != (wchar_t *)0x0) {
    DAT_180244421 = (undefined1)*local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f6e = 1;
  /* ... */
  if (local_18 != (wchar_t *)0x0) {
    _DAT_18024442c = *(undefined4 *)local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f75 = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\",L"EnableS0Idle",0,&local_18,
                           0);
  if (local_18 != (wchar_t *)0x0) {
    _DAT_180244430 = *(undefined4 *)local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f76 = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"System\\CurrentControlSet\\Control\\Goodix\\FP\\",
                           L"S0IdleTimeout",0,&local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    _DAT_180244434 = *(undefined4 *)local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f77 = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"System\\CurrentControlSet\\Control\\Goodix\\FP\\",
                           L"EnableRemoteWakeup",0,&local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    _DAT_180244438 = *(undefined4 *)local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f78 = 1;
  }
  /* ... */
  if (local_18 != (wchar_t *)0x0) {
    DAT_18024464a = (undefined1)*local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f7c = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\",L"UsbErrorSwitch",0,
                           &local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    DAT_18024464b = (undefined1)*local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f7d = 1;
  }
  local_10 = FUN_180031c44(0xffffffff80000002,L"Software\\Goodix\\FP\\",L"MSStopIdleSwitch",0,
                           &local_18,0);
  if (local_18 != (wchar_t *)0x0) {
    DAT_18024464c = (undefined1)*local_18;
    free(local_18);
    local_18 = (wchar_t *)0x0;
    DAT_180258f7e = 1;
