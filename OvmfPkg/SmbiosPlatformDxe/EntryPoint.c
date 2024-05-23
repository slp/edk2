/** @file
  Find and extract SMBIOS data.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MemoryAllocationLib.h> // FreePool()
#include <OvmfPlatforms.h>               // CLOUDHV_DEVICE_ID
#include <IndustryStandard/SmBios.h>  // SMBIOS_TABLE_3_0_ENTRY_POINT
#include <Library/DebugLib.h>

#include "SmbiosPlatformDxe.h"

/**
  Installs SMBIOS information for OVMF

  @param ImageHandle     Module's image handle
  @param SystemTable     Pointer of EFI_SYSTEM_TABLE

  @retval EFI_SUCCESS    Smbios data successfully installed
  @retval Other          Smbios data was not installed

**/
EFI_STATUS
EFIAPI
SmbiosTablePublishEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  UINT8       *SmbiosTables;

  Status = EFI_NOT_FOUND;

  #define KRUN_SMBIOS_ADDRESS  0x4000F000

  //
  // Add SMBIOS data if found
  //
  DEBUG ((
    DEBUG_VERBOSE,
    "%a: Installing libkrun SMBIOS: addr 0x%lx\n",
    __func__,
    KRUN_SMBIOS_ADDRESS
  ));
  
  SMBIOS_TABLE_3_0_ENTRY_POINT  *KrunTables = (VOID *)KRUN_SMBIOS_ADDRESS;

  if ((KrunTables->AnchorString[0] == '_') &&
      (KrunTables->AnchorString[1] == 'S') &&
      (KrunTables->AnchorString[2] == 'M') &&
      (KrunTables->AnchorString[3] == '3') &&
      (KrunTables->AnchorString[4] == '_'))
  {
    DEBUG ((
      DEBUG_VERBOSE,
      "%a: Found libkrun SMBIOS\n",
      __func__
    ));
    
    SmbiosTables = (UINT8 *)(UINTN)KrunTables->TableAddress;
    Status = InstallAllStructures (SmbiosTables);
  }

  DEBUG ((
    DEBUG_VERBOSE,
    "%a: libkrun SMBIOS install status: %d\n",
    __func__,
    Status
  ));

  return Status;
}
