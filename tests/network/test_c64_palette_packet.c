/*
C64 Stream - An OBS Studio source plugin for Commodore 64 video and audio streaming
Copyright (C) 2025 Christian Gleissner

Licensed under the GNU General Public License v2.0 or later.
See <https://www.gnu.org/licenses/> for details.
*/
#ifdef NDEBUG
#undef NDEBUG
#endif

#include "c64-protocol.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    uint8_t packet[C64_PALETTE_PACKET_SIZE] = {0};
    uint32_t palette[16];

    packet[4] = 239;
    packet[6] = 0x80;
    packet[7] = 0x01;
    packet[8] = 1;
    packet[9] = 4;
    packet[10] = 1;
    for (size_t i = 0; i < 16; i++) {
        packet[C64_VIDEO_HEADER_SIZE + i * 3] = (uint8_t)i;
        packet[C64_VIDEO_HEADER_SIZE + i * 3 + 1] = (uint8_t)(i + 16);
        packet[C64_VIDEO_HEADER_SIZE + i * 3 + 2] = (uint8_t)(i + 32);
    }

    assert(c64_parse_palette_packet(packet, sizeof(packet), palette));
    assert(palette[0] == 0xFF201000u);
    assert(palette[15] == 0xFF2F1F0Fu);

    packet[10] = 0;
    assert(!c64_parse_palette_packet(packet, sizeof(packet), palette));
    packet[10] = 1;
    assert(!c64_parse_palette_packet(packet, sizeof(packet) - 1, palette));

    puts("test_c64_palette_packet: PASS");
    return 0;
}
