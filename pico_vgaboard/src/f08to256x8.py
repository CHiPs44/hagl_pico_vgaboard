#!/usr/bin/env python3

# MIT License
# Copyright (c) 2021-2024 Christophe "CHiPs44" Petit
# SPDX-License-Identifier: MIT

# Thanks to ChatGPT for the "shell" code ;-)
# it seems I didn't give it the right prompt for the effective transposition...

def convert_font_file(source_file: str, destination_file: str):
    # Read the data from the original binary file
    with open(source_file, "rb") as input:
        data = input.read()
    # Check that the file size is correct
    if len(data) != 2048:
        raise ValueError("The input file must be exactly 2048 bytes.")
    # We will rearrange the data into 8 blocks of 256 bytes
    data2 = bytearray(2048)
    for i in range(0, 256):
        for j in range(0, 8):
            data2[256*j+i] = data[i*8+j]
    # Write the rearranged data into the new binary file
    with open(destination_file, "wb") as output:
        output.write(data2)

convert_font_file("BIOS.F08", "BIOS_F08.bin")
