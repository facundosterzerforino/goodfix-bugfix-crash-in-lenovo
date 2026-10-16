/* Ghidra 12.1.4 pseudocode via REA 6.5.0 - wbdi.dll 3.0.536.320 (sha256 792271a28c3087d8acd1626bbdc35b2190c85058bfc49af1fa00f1ea457285ec)
 * UsbDevicePrepareHardware excerpt: S0 idle is only assigned when cfg+0x430 (EnableS0Idle) != 0.
 * LOG(level, func, ...) = driver trace macro (FUN_1800c3f70). Excerpt for interoperability analysis. */

  /* ... */
        local_60 = 0;
        local_68 = 0x152;
        LOG(4, "UsbDevicePrepareHardware");
      }
      else {
        lVar1 = FUN_180031de0(0);
        if (*(int *)(lVar1 + 0x430) == 0) {
          local_58 = "s0 idle is disabled";
          local_60 = 0;
          local_68 = 0x15d;
          LOG(8, "UsbDevicePrepareHardware");
        }
        else {
          local_38 = FUN_1800ba500(param_1);
          if (local_38 < 0) {
            local_58 = "EnableS0Idle failed";
            local_60 = 0;
            local_68 = 0x159;
            FUN_1800c3f70(PTR_DAT_180256a20,5,
                          "d:\\project\\thinkpad_gm168\\winfpcode2\\driver_tatin\\wbdi\\_deviceusb.c"
