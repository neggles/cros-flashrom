# This file is part of the flashrom project.
#
# Copyright 2020 Google LLC
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

root_includes = include_directories('../subprojects')

srcs = [
  'tests.c',
  'helpers.c',
  'flashrom.c',
  'spi25.c',
  'init_shutdown.c',
]

mocks = [
  '-Wl,--wrap=physunmap',
  '-Wl,--wrap=physmap',
  '-Wl,--wrap=spi_send_command',
  '-Wl,--wrap=sio_write',
  '-Wl,--wrap=sio_read',
  '-Wl,--wrap=open',
  '-Wl,--wrap=open64',
  '-Wl,--wrap=ioctl',
  '-Wl,--wrap=fopen',
  '-Wl,--wrap=fopen64',
  '-Wl,--wrap=rget_io_perms',
  '-Wl,--wrap=test_outb',
  '-Wl,--wrap=test_inb',
  '-Wl,--wrap=test_outw',
  '-Wl,--wrap=test_inw',
  '-Wl,--wrap=test_outl',
  '-Wl,--wrap=test_inl',
  '-Wl,--wrap=usb_dev_get_by_vid_pid_number',
  '-Wl,--wrap=libusb_set_configuration',
  '-Wl,--wrap=libusb_claim_interface',
  '-Wl,--wrap=libusb_control_transfer',
  '-Wl,--wrap=libusb_release_interface',
  '-Wl,--wrap=libusb_close',
  '-Wl,--wrap=libusb_exit',
  '-Wl,--gc-sections',
]

flashrom_tests = executable('flashrom_unit_tests',
  srcs,
  include_directories : root_includes,
  c_args : [
    cargs,
    '-ffunction-sections',
    '-fdata-sections',
    #    '-DSTANDALONE',
  ],
  export_dynamic : true,
  link_args : mocks,
  dependencies : [cmocka_dep, flashrom_test_dep],
)
test('cmocka test flashrom', flashrom_tests)


writeprotect_mocks = [
  '-Wl,--wrap=spi_send_command',
  '-Wl,--wrap=spi_read_status_register',
  '-Wl,--wrap=spi_write_status_register',
  '-Wl,--wrap=w25q_read_status_register_2',
  '-Wl,--wrap=w25q_write_status_register_WREN',
  '-Wl,--wrap=mx25l_read_config_register',
  '-Wl,--gc-sections',
]

writeprotect_srcs = [
  'wp_tests.c',
  'wp_enable_disable.c',
  'wp_chip_info.c',
]

writeprotect_tests = executable('writeprotect_unit_tests',
  writeprotect_srcs,
  include_directories : root_includes,
  c_args : [
    cargs,
    '-ffunction-sections',
    '-fdata-sections',
  ],
  export_dynamic : true,
  link_args : writeprotect_mocks,
  dependencies : [cmocka_dep, writeprotect_test_dep],
)
# b/195978970: writeprotect tests can cause presubmit
# timeouts, disable them for now.
if false
  test('cmocka test writeprotect', writeprotect_tests, timeout: 180)
endif
