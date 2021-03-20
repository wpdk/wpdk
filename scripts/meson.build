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
map_to_wrap_cmd = py3 + files('map_to_wrap.py')

sudo = configure_file(input : 'sudo.sh', output : 'sudo',
		copy: true, install: true, install_dir: 'bin/msys2',
		install_mode: 'rwxr-xr-x')

bashrc = configure_file(input : 'bashrc.sh', output : 'wpdk_bashrc.sh',
		copy: true, install: true, install_dir: 'bin',
		install_mode: 'rwxr-xr-x')
