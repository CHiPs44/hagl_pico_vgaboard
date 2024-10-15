#!/usr/bin/env python3

# MIT License
# Copyright (c) 2021-2024 Christophe "CHiPs44" Petit
# SPDX-License-Identifier: MIT

# Thanks to ChatGPT for the shell code ;-)
# it seems I didn't give it the right prompt...

def convert_font_file(input_file: str, output_file: str):
    # Read the data from the original binary file
    with open(input_file, "rb") as f:
        data = f.read()

    # Check that the file size is correct
    if len(data) != 2048:
        raise ValueError("The input file must be exactly 2048 bytes.")

    # We will rearrange the data into 8 blocks of 256 bytes
    # lines = [data[i:i + 256] for i in range(0, 2048, 256)]
    data2 = bytearray(2048)
    for i in range(0, 255):
        for j in range(0, 7):
            data2[256*j+i] = data[i*8+j]

    # Write the rearranged data into the new binary file
    with open(output_file, "wb") as f_out:
        # for line in lines:
        #     f_out.write(line)
        f_out.write(data2)


# Usage
convert_font_file("BIOS.F08", "BIOS_F08.bin")
