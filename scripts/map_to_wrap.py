#!/usr/bin/env python
# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2019 Intel Corporation
# Copyright (c) 2021, MayaData Inc. All rights reserved.
# Copyright (c) 2021, DataCore Software Corporation. All rights reserved.

from __future__ import print_function
import sys
from os.path import dirname, basename, join, exists

def is_function_line(ln):
    return ln.startswith('\t') and ln.endswith(';\n') and ":" not in ln

def main(args):
    if not args[1].endswith('.map') or \
            not args[2].endswith('.h'):
        return 1

# special case, allow override if an def file already exists alongside map file
    override_file = join(dirname(args[1]), basename(args[2]))
    if exists(override_file):
        with open(override_file) as f_in:
            functions = f_in.readlines()

# generate wrap file from map file.
# This works taking indented lines only which end with a ";" and which don't
# have a colon in them, i.e. the lines defining functions only.
    else:
        with open(args[1]) as f_in:
            functions = [ln[:-2] + '\n' for ln in sorted(f_in.readlines())
                         if is_function_line(ln)]

    wrap = ['/*-\n *  SPDX-License-Identifier: BSD-3-Clause\n']
    wrap = wrap + [' *  Copyright (c) 2020, MayaData Inc. All rights reserved.\n']
    wrap = wrap + [' *  Copyright (c) 2020, DataCore Software Corporation. All rights reserved.\n']
    wrap = wrap + [' */\n\n']

    wrap = wrap + ['#ifndef _WPDK_WRAP_H_\n']
    wrap = wrap + ['#define _WPDK_WRAP_H_\n\n']
    wrap = wrap + ['/*\n *  The MSVC linker does not support the GCC --wrap option used to\n']
    wrap = wrap + [' *  implement mocking. Simulate it using /alternatename.\n */\n']
    wrap = wrap + ['#if defined(_MSC_VER)\n\n']

    for line in functions:
        if line.startswith('\twpdk_'):
            fn = line.replace('\t', '').replace('\n','')
            wrap = wrap + ['#define ' + fn + ' __wrap_' + fn + '\n']
            wrap = wrap + ['#pragma comment(linker, "/alternatename:__wrap_' + fn + '=' + fn + '")\n']
            wrap = wrap + ['#pragma comment(linker, "/alternatename:__real_' + fn + '=' + fn + '")\n\n']

    wrap = wrap + ['#endif /* _MSC_VER */\n\n']
    wrap = wrap + ['/*\n *  Redefine references to __wrap_ and __real_ functions to include\n']
    wrap = wrap + [' *  wpdk in the function name.\n */\n']

    for line in functions:
        if line.startswith('\twpdk_'):
            fn = line.replace('\twpdk_', '').replace('\n','')
            wrap = wrap + ['#define __wrap_' + fn + ' __wrap_wpdk_' + fn + '\n']
            wrap = wrap + ['#define __real_' + fn + ' __real_wpdk_' + fn + '\n\n']
    
    wrap = wrap + ['#endif /* _WPDK_WRAP_H_ */\n']
    with open(args[2], 'w') as f_out:
        f_out.writelines(wrap)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
