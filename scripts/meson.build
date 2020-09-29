# SPDX-License-Identifier: BSD-3-Clause
# Copyright(c) 2017-2019 Intel Corporation

# set up map-to-win script using python, either built-in or external
python3 = import('python').find_installation(required: false)
if python3.found()
	py3 = [python3]
else
	py3 = ['meson', 'runpython']
endif

map_to_win_cmd = py3 + files('map_to_win.py')
