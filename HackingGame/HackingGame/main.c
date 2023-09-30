#include <Uefi.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/Timer.h>
#include <Library/UefiBootServicesTableLib.h>

#include "model.h"
#include "display.h"

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.



**/
EFI_STATUS EFIAPI
UefiMain(IN
         EFI_HANDLE ImageHandle,
         IN
         EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_INPUT_KEY key;

    hg_game_state_t gState;
    hg_game_state_init(&gState);

    EFI_EVENT events[1];
    events[0] = gST->ConIn->WaitForKey;

    gST->ConOut->EnableCursor(gST->ConOut, 0);

    hg_draw_screen(&gState);

    UINTN eventType;
    hg_cursor_t cursor_loc;

    cursor_loc.x = 0;
    cursor_loc.y = 0;

    do {
        gBS->WaitForEvent(1, events, &eventType);

        if (eventType == 0) {
            gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
            __hg_print_with_colour_at(L" ", EFI_BACKGROUND_BLACK, cursor_loc.x, cursor_loc.y, 0);

            switch (key.ScanCode) {
            case SCAN_UP:
                if (cursor_loc.y - 1 > 0) {
                    cursor_loc.y--;
                }
                break;
            case SCAN_DOWN:
                if (cursor_loc.y + 1 < HG_RES_Y) {
                    cursor_loc.y++;
                }
                break;
            case SCAN_LEFT:
                if (cursor_loc.x - 1 > 0) {
                    cursor_loc.x--;
                }
                break;
            case SCAN_RIGHT:
                if (cursor_loc.x + 1 < HG_RES_X) {
                    cursor_loc.x++;
                }
                break;
            default:
                break;
            }
            __hg_print_with_colour_at(L" ", EFI_BACKGROUND_LIGHTGRAY, cursor_loc.x, cursor_loc.y, 0);
        }
    } while (key.ScanCode != SCAN_END);

    return EFI_SUCCESS;
}
